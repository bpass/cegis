//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : 2/18/2005
//
// File: SlaveManager.cpp
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: SlaveManager is responsible for managing work jobs and 
// accepting connections from fork()ed slaves.
//
/////////////////////////////////////////////////////////////////////// 

#include "SlaveManager.h"

SlaveManager::SlaveManager(WorkManager * newforeman)
{
  foreman = newforeman;
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
      throw("socket() failed");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(0);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(serveraddr.sin_zero), '\0', 8);
    socklen_t addrlen = sizeof(sockaddr_in);
    if(bind(serversock, (sockaddr*)&serveraddr, addrlen) != 0)
      throw("bind() failed");
    getsockname(serversock, (sockaddr*)&serveraddr, &addrlen);
    if(listen(serversock, MASTERBACKLOG) < 0)
      throw("listen() failed");

    // at this point socket setup is complete
    printf("listening on port %d\n", ntohs(serveraddr.sin_port));

    // TODO: spawn slaves here with fork()

    // Service requests at the socket until all work is done
    while(foreman->getBigResult() == NULL) 
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
     	  outwork = foreman->getWorkUnit();
          
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
        foreman->putWorkResult(inwork);
      }
      
    }
  // TODO: make msg a std::string
  } catch(const char * msg)
  {
    printf("exception: %s\n", msg);
  }
  if(inwork != NULL)
  {
    delete inwork;
    inwork = NULL;
  }
  if(serversock != -1)
    close(serversock);
}

//************************************************************************
// private

// isn't used for some reason, probably needs to be developed ... MS
void SlaveManager::spawnslave()
{
  char ** arg;
  // construct argument table
  if(fork() == 0)
    if(fork() == 0)
    {
      // launch slave
      (void)arg;
    }
    else
      exit(0);
}

