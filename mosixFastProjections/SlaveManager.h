//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : 2/18/2005
//
// File: SlaveManager.h
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: SlaveManager is responsible for managing work jobs and 
// accepting connections from fork()ed slaved.
//
/////////////////////////////////////////////////////////////////////// 

#ifndef SLAVEMANAGER_H_
#define SLAVEMANAGER_H_

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
    SlaveManager(WorkManager * newforeman);
    ~SlaveManager();

    // go forth! huzzah!
    // when this returns, work is complete
    // finished product can be retrieved from the WorkManager assigned
    // if the workmanager says work is NOT complete, then something went wrong
    //  that couldn't be coped with, like, something was all *grk ack* and
    //  nobody helped the poor thing, they just stepped over it because they
    //  were in a hurry to get their mocha or whatever
    void Work();

  private:
    // fork off and execute the slave executable with proper arguments
    void spawnslave();

    WorkManager * foreman; // meaningless to slaves
    WorkUnit * outwork; // for sending workunits
    WorkUnit * inwork; // for receiving workunits

    sockaddr_in serveraddr;
};

#endif

