//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Fri Mar 11 22:22:34 CST 2005
//
// File               : WorkManager.cpp
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: WorkManager is a class which is designed to 
// check in and out work in units, hence the class WorkUnit.
// WorkManager contains a queue of "WorkUnits" and a Set of returned
// "Results."
//
///////////////////////////////////////////////////////////////////////

#include "WorkManager.h"

// TODO: use std::cerr instead of printf()

WorkManager::WorkManager(BigJob* job)
{
  workunitdeque.clear(); 
  resultset.clear(); 

  // TODO: variables need to be declared at the top of the
  // function
  m_externJob = job;
  workunitcount = job->getnumworkunits();
      
  // TODO:  You need to have a static integer count here
  // not randomness
  srand(time(NULL));
  baseid = rand() >> 12; 
  
  // figure out how to split up the work
  m_externJob->dogetExtents();
  blueprints = new WorkUnitBlueprint_t[workunitcount];
  long int lines = m_externJob->getnewheight();

  printf("setup for %d workunits spanning %ld lines\n",
                      (unsigned int)workunitcount, lines);

  if ( lines > 0 ) 
  { 
      // Enqueue the IDs to be worked on
      for(workunitid_t i = 0; i < workunitcount; ++i)
        workunitdeque.push_back(i + baseid); 
        // fill the queue with all workunits

      // spread the love evenly among the workunits
      // this is where different work distribution methods may be applied
      long int linesleft = lines;
      for(workunitid_t i = 0; i < workunitcount; ++i)
      {
        blueprints[i].basescanline = lines - linesleft;
        blueprints[i].scanlinecount = linesleft / (workunitcount - i);
        linesleft -= blueprints[i].scanlinecount;
      }
      printf("workunits average %ld lines each\n", lines / workunitcount);
      
      // create the BigResult
      goal = new BigResult;

      // create and prepare the model workunit
      m_baseWorkUnit = new WorkUnit(0);
      std::string inputfile = m_externJob->getInputFile();
      m_baseWorkUnit->setInputFile(inputfile);
      m_baseWorkUnit->setnewheight(m_externJob->getnewheight());
      m_baseWorkUnit->setnewwidth(m_externJob->getnewwidth());

      if(m_externJob->getsamescale())
        m_baseWorkUnit->setSameScale(true);
      else
        m_baseWorkUnit->setOutputScale(m_externJob->getOutputScale());

      m_baseWorkUnit->setoutRect(m_externJob->getoutRect());
      m_baseWorkUnit->setPmeshSize(m_externJob->getPmeshSize());
      m_baseWorkUnit->setPmeshName(m_externJob->getPmeshName());
      m_baseWorkUnit->setOutputProjection(m_externJob->getOutputProjection());
  } 

  return; 
}

////////////////////////////////////////////////////////////////////////

WorkManager::~WorkManager()
{
  if(blueprints != NULL)
    delete[] blueprints;
  if(m_baseWorkUnit != NULL)
    delete m_baseWorkUnit;
  if(goal != NULL)
    delete goal;
}

////////////////////////////////////////////////////////////////////////

WorkUnit *WorkManager::getWorkUnit()
{
  if(workunitdeque.empty())
    return NULL;
  else
  {
    workunitid_t id = workunitdeque.front();

    std::cout << "workmanager: giving workunit #: " << id << std::endl;
    
    // set up workunit (set scanline range, and anything else)
    m_baseWorkUnit->setrange(blueprints[id - baseid].basescanline,
        blueprints[id - baseid].scanlinecount);
    m_baseWorkUnit->setid(id);
    m_baseWorkUnit->reloaded();
    workunitdeque.pop_front();
    return m_baseWorkUnit;
  }
}

////////////////////////////////////////////////////////////////////////

void WorkManager::putWorkResult(WorkUnit* workunit)
{
  if(verifyworkunit(workunit) && resultset.count(workunit->getid()) == 0)
  {
    // integrate the result
    unsigned char ** wad = workunit->getscanlines();
    long int base = workunit->getbasescanline();
    long int count = workunit->getscanlinecount();
   
    for(long int i = 0; i < count; ++i)
      m_externJob->insertscanline(wad[i], i + base);
    /// TODO: pass these results onto the stitcher instead of back
    /// to the job itself.
    
    resultset.insert(workunit->getid()); // *rrrring* we got one!
    //workunit->clearscanlines(); this is dumb, probably
  }
  else
  {
    printf("rejecting [%d]\n", (unsigned int)workunit->getid());
    if(idvalid(workunit->getid()))
      lostWorkUnit(workunit->getid());
  }
}

////////////////////////////////////////////////////////////////////////

void WorkManager::lostWorkUnit(workunitid_t id)
{
  if(idvalid(id))
    if(resultset.count(id) == 0) // only redispatch if neccessary
      workunitdeque.push_front(id);
}

////////////////////////////////////////////////////////////////////////

bool WorkManager::workRemains()
{
  return !workunitdeque.empty();
}

////////////////////////////////////////////////////////////////////////

BigResult *WorkManager::getBigResult()
{
  if((workunitid_t)resultset.size() == workunitcount)
    return goal;
  else
    return NULL;
}

//********************************************************************
// private

//////////////////////////////////////////////////

bool WorkManager::idvalid(workunitid_t id)
{
  return id < baseid + workunitcount &&
    id >= baseid;
}

//////////////////////////////////////////////////

bool WorkManager::verifyworkunit(WorkUnit * workunit)
{
  if(workunit == NULL)
  {
    std::cout << "WorkUnit == NULL" << std::endl;
    return false;
  }
  workunitid_t workunitid = workunit->getid();
  if(!idvalid(workunitid))
  {
    std::cout << "WorkUnit ID invalid" << std::endl;
    return false;
  }
  
  long int scanlinecount = workunit->getscanlinecount();
  long int basescanline = workunit->getbasescanline();
  unsigned char ** remainingScanLines = workunit->getscanlines();
  if(basescanline != blueprints[workunitid - baseid].basescanline ||
      scanlinecount != blueprints[workunitid - baseid].scanlinecount)
  {
    std::cout << "Scanline Count Error" << std::endl;
    return false;
  }
  
  // if the current work unit is done and ...
  if(workunit->done())
  {
    std::cout << "WorkUnit Done" << std::endl;
    // the workunit doesn't have any scanlines in it
    if(remainingScanLines == NULL)
      return false;
    
    // the scanline list exists, but has 
    // no remaining scanlines in it
    for(long int i = 0; i < scanlinecount; ++i)
      if(remainingScanLines[i] == NULL)
        return false;

    // else return true
  }
  else
  { std::cout << "WorkUnit returning false, not done." << std::endl;
    return false;
  }
  std::cout << "WorkUnit returning true" << std::endl;
  return true; // now you are a man
}

