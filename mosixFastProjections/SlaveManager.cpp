
//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Tue Mar 15 09:21:20 CST 2005
//
// File: SlaveManager.cpp
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: SlaveManager is responsible for managing work jobs and 
// accepting connections from fork()ed slaves.
//
/////////////////////////////////////////////////////////////////////// 

#include <sstream>
#include "SlaveManager.h"
#include "GeneralException.h"

SlaveManager::SlaveManager(WorkManager * workMan)
{
  m_workManager = workMan;
  inwork = new WorkUnit;
  outwork = NULL;
}

//////////////////////////////////////////////////

SlaveManager::~SlaveManager()
{
  if(inwork != NULL)
    delete inwork;
  
  // NOTE: outwork is not our responsibility, 
  // it belongs to the Work Manager, foreman
  
}

//////////////////////////////////////////////////

void SlaveManager::Work()
{
  int serversock = -1;      // master will listen on this
  int sock = -1;            // for transient per-connection sockets
  sockaddr_in slaveaddr;    // blank address card of connecting sockets
  GobHeader gobin, gobout;  // get "gobs" of data
  gobout.sender = getpid(); // get the process ID of this process
  bool gotresults = false;

  // TODO: get rid of printfs and use std::cerr
  
  try
  {
    // set up listening socket
    
    // TODO: modularize this code	  
    serversock = socket(AF_INET, SOCK_STREAM, 0);
    
    if(serversock < 0) 
        throw USGSMosix::GeneralException("socket() failed");
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(0);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(serveraddr.sin_zero), '\0', 8);
    socklen_t addrlen = sizeof(sockaddr_in);
    if(bind(serversock, (sockaddr*)&serveraddr, addrlen) != 0)
        throw USGSMosix::GeneralException("bind() failed");
    
    getsockname(serversock, (sockaddr*)&serveraddr, &addrlen);
    
    if(listen(serversock, MASTERBACKLOG) < 0)
      throw USGSMosix::GeneralException("listen() failed");

    // at this point socket setup is complete
    printf("listening on port %d\n", ntohs(serveraddr.sin_port));

    // spawn the slave processes to do the work for us    
    spawnSlaves();

    // Service requests at the socket until all work is done
    while(m_workManager->getBigResult() == NULL) 
    {
      
      // Assume results haven't come yet
      gotresults = false;
      
      printf("..."); fflush(stdout);

      // accept sets up for new connections (blocking call)
      // TODO: what if accept returns negative?
      sock = accept(serversock, (sockaddr*)&slaveaddr, &addrlen);
      printf("connect "); fflush(stdout);
      
      // receive from the connecting socket
      recv(sock, &gobin, sizeof(gobin), MSG_WAITALL);
      
      // print address info
      printf("%d from %s | ",
          gobin.sender,
          inet_ntoa(slaveaddr.sin_addr));
      fflush(stdout);
     
      // if what's coming in is the result
      if(gobin.contents == GOB_RESULT)
      {
	    // tell the connecting socket that the work
      	// was received.
        printf("in:"); fflush(stdout);
        gobout.contents = GOB_OK;
        send(sock, &gobout, sizeof(gobout), 0);
        inwork->importFromSocket(sock);

	    // receive the next packet from the socket
        printf("[%d]ok ", (unsigned int)inwork->getid());
        gotresults = true;
        recv(sock, &gobin, sizeof(gobin), MSG_WAITALL);
      }
      switch(gobin.contents)
      {
   	    // request work
        case GOB_WORKREQUEST:

	      // gets work to be done from the work manager
     	  outwork = m_workManager->getWorkUnit();
          
    	  // If there is Remaining work
	      if(outwork != NULL)
          {
            printf("out:"); fflush(stdout);
            gobout.contents = GOB_WORKUNIT;
            send(sock, &gobout, sizeof(gobout), 0);
            outwork->exportToSocket(sock);
            printf("[%d]ok\n", (unsigned int)outwork->getid()); fflush(stdout);
          }
	  
	      // else, tell the connecting worker, there is none left
          else
          {
            gobout.contents = GOB_NOWORK;
            send(sock, &gobout, sizeof(gobout), 0);
            printf("no work remains\n"); fflush(stdout);
          }
          break;

      	// there is a problem with the request
        default:
          printf("ERROR: slave sent gob %d\n", gobin.contents); fflush(stdout);
          gobout.contents = GOB_ERROR;
          send(sock, &gobout, sizeof(gobout), 0);
      }
      close(sock);
      
      if(gotresults)
      {
        // hands off the result of work being performed 
        // to the work manager
        m_workManager->putWorkResult(inwork);
      }
    }

    joinSlaves();  // joins the slaves spawned off
    
    
  } catch( USGSMosix::Exception& e)
  {
    std::cerr << e.toString() << std::endl;
  }
  
  if(inwork != NULL)
  {
    delete inwork;
    inwork = NULL;
  }
  if(serversock != -1)
    close(serversock);
}

void SlaveManager::joinSlaves() 
{
    int status = 0; 
    
    // for each slave process
    for( unsigned int i = 0; i < childPIDs.size(); ++i )
    {
        // Make a blocking call to wait, so as to wait
        // for all children to return.
        if ( waitpid(childPIDs[i], &status,0) <= 0 )
            std::cerr << "Error joining slave" << i << std::endl;
    }

    return;
}


void SlaveManager::spawnSlaves()
{
  std::stringstream strStream;
  std::string path, arg1, arg2, arg3;
  int slaveStatus(0); 
 
  childPIDs.resize(m_workManager->getTotalUnits());
  
  // for each slave
  for( unsigned int i = 0; i < m_workManager->getTotalUnits(); ++i )
  {
    if( (childPIDs[i] = fork()) == 0 ) // child's context
    {
        path = "/home/mschisler/MOSIX/slave";
        arg1 = "slave";
        arg2 = "localhost";

        // give the slave the address of master
        strStream << ntohs(serveraddr.sin_port);
        arg3 = strStream.str();
    
        // execute slave
        slaveStatus = execl(path.c_str(),
                            arg1.c_str(), 
                            arg2.c_str(), 
                            arg3.c_str(), 
                            (char*)NULL);

        // after slave code is done.
        exit(slaveStatus);
        
    } else if ( childPIDs[i] == -1 ) // parent's context, failure
    {
        strStream << "Fork Failure in Slave Spawning; i = " << i;
        std::string failMsg = strStream.str();
        throw USGSMosix::GeneralException(failMsg);
    }
  }

  return;
}

