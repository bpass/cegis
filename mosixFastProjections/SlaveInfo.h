/* simple struct containing relevant information about a slave worker */

#ifndef SLAVEINFO_H_
#define SLAVEINFO_H_

#include <sys/types.h>
#include "WorkUnit.h"

typedef pid_t slaveid_t;

// this struct is obsolete in new design
/*struct SlaveInfo
{
  slaveid_t id;
  pid_t pid;
  workunitid_t lastworkunit; // the id of the last WorkUnit this slave had
  long totalworkunits; // total number of workunits dispatched to this slave
};*/

#endif

