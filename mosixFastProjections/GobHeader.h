// part of MSXProject by Matt Zykan
/* header machine for identifying gobs of data coming through sockets
 * closer to commands... but whatever
 * computers are hard
 * I should've been a florist
 * or a comic strip... maker... guy
 * I could study under whoever makes Garfield
 * that human waste of space
 */

#ifndef GOBHEADER_H_
#define GOBHEADER_H_

#include <sys/types.h>
#include "SlaveInfo.h"

// these are for identifying what's in the gob
#define GOB_WORKUNIT    1001
#define GOB_RESULT      1002
#define GOB_NOWORK      1003
#define GOB_DYING       1004
#define GOB_WORKREQUEST 1005
#define GOB_OK          1006
#define GOB_ERROR       0

struct GobHeader
{
  slaveid_t sender; // who's it from
  int contents; // what is it
};

#endif

