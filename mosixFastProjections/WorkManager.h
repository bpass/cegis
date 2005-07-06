
#ifndef WORKMANAGER_H_
#define WORKMANAGER_H_

//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Fri Mar 11 22:22:26 CST 2005
//
// File: WorkManager.cpp
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: WorkManager is a class which is designed to 
// check in and out work in units, hence the class WorkUnit.
// WorkManager contains a queue of "WorkUnits" and a Set of returned
// "Results."
//
///////////////////////////////////////////////////////////////////////

#include <deque>
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;
#include "WorkUnit.h"
#include "BigJob.h"

#include "PNGImageOFile.h"
#include "JPEGImageOFile.h"

/* getWorkUnit returns a pointer to what is essentially a static WorkUnit
 *  object, so subsequent calls to getWorkUnit _will_ change the object
 *  returned by a previous call
 */

//TODO: naming conventions and variables

struct WorkUnitBlueprint_t
{
  long int basescanline;
  long int scanlinecount;
};

class WorkManager
{
  public:
    // job is the BigJob to be split up and dispatched as work units
    // do not modify or destroy job until work is done!!!
    // WorkManager does not take its own copy
    WorkManager(BigJob * job);
    ~WorkManager();

    /// Pre : None
    //
    /// Post: This function creates a unit of work at run-time.  
    /// from the cached "blueprints". 
    //
    /// Purpose:for "checking out" a unit of work
    /// returns NULL if there is no work to be gotten
    WorkUnit * getWorkUnit();

    // mmm duh
    // THIS is how completed work units are passed back to the workmanager
    void putWorkResult(WorkUnit * workunit);

    // let the workmanager know if a unit is lost, so it can redispatch it
    // workunits reported lost become first in line for dispatching
    void lostWorkUnit(workunitid_t id);

    // returns true if getWorkUnit will get you anything
    // because it may be possible (in fact, it's certain to happen, given the
    //  constant and finite speed of light) that all workunits are dispatched,
    //  but the job isn't done yet
    //  ...unless the WorkManager is really, really, really tiny
    bool workRemains();

    // returns NULL if the results aren't ready
    BigResult *getBigResult();

    workunitid_t getTotalUnits() { return workunitcount; } 
    
  private:
    // check that id is a workunit that exists
    inline bool idvalid(workunitid_t);

    // compares id with assigned work, etc., to make sure the workunit
    //  is complete and normal and healthy before trying to integrate it
    bool verifyworkunit(WorkUnit * workunit);

    long int m_totalLines; 
    
    USGSImageLib::JPEGImageOFile * m_jpg;
    USGSImageLib::PNGImageOFile * m_png;
    
    Stitcher* m_workStitcher;  ///<  Stitches the image back together
    
    BigJob* m_externJob;  ///< TODO: Should be a reference
    
    WorkUnit * m_baseWorkUnit; ///< This is where the workUnits come from 
                               ///< They are generated at run-time and 
			                   ///< set equal to this pointer.  As to why, 
               			       ///< there doesn't seem to be a reason.
   
    workunitid_t workunitcount; ///< The total number of work units we're using. 
    workunitid_t baseid;    ///< This base id is completely random, but should
                            ///< be a static int, incremented with every
		              		///< new instantiation of the class
    
    // TODO: Erroneous Transmissions?  From Where?
    // REGARDING: 
    //       using goofy numbers for workunit ids makes communications a little
    //       more robust (it's less likely an erroneous transmission will
    //       begin with a valid id, if ids don't start at zero)
    
    // TODO: This deque needs to be for ACTUAL work units and not
    // just ID numbers

    ///< TODO: should integrate with workunitdeque
    WorkUnitBlueprint_t * blueprints;  
    
    deque<workunitid_t> workunitdeque; // queue of work unit IDs pending
    
    set<workunitid_t> resultset; // track which results received and integrated
    BigResult* goal; // TODO: more appropriate name
};

#endif

