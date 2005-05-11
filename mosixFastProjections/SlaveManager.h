
#ifndef SLAVEMANAGER_H_
#define SLAVEMANAGER_H_

//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Tue Mar 15 09:21:11 CST 2005
//
// File: SlaveManager.h
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: SlaveManager is responsible for managing work jobs and 
// accepting connections from fork()ed slaved.
//
/////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "SlaveInfo.h"
#include "WorkManager.h"
#include "GobHeader.h"

#define MASTERBACKLOG 20 // how many connection requests master should backlog
                         // setting this too low might starve some slaves

class SlaveManager
{
  public:
    // this object merely copies the pointer to newforeman
    //  therefore, leave newforeman alone until work is done
    SlaveManager(WorkManager * workMan);
    ~SlaveManager();

    //  when this returns, work is complete
    //  finished product can be retrieved from the WorkManager assigned
    //  if the workmanager says work is NOT complete, then something went wrong
    //  that couldn't be coped with, like, something was all *grk ack* and
    //  nobody helped the poor thing, they just stepped over it because they
    //  were in a hurry to get their mocha or whatever
    void Work();

  private:
    // pre : none
    // post: Fork off and execute the slave executable with proper arguments.
    //       In the event of a fork failure, this function will throw a
    //       USGSMosix::GeneralException. 
    void spawnSlaves();

    // pre:  Allocation of child PIDs to std::vector childPIDs.  The children
    //       are assumed to have been running at some point on call of this
    //       function.
    // post: Joins all the slave children (making sure they don't become
    //       zombies).  If for some reason a join fails, a message is 
    //       outputted to std::err to this effect.
    void joinSlaves(); 

    std::vector<int> childPIDs;
    WorkManager * m_workManager; // meaningless to slaves
    WorkUnit * outwork; // for sending workunits
    WorkUnit * inwork; // for receiving workunits
    sockaddr_in serveraddr;
};

#endif

