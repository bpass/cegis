#ifndef BIGJOB_H_
#define BIGJOB_H_

/******************************************************************************
 * Last Modified by    : Mark Schisler (glad to not be taking credit for this)
 * Original Programmer : Matt Zykan
 * Date                : Fri Mar 11 18:41:18 CST 2005
 * Filename            : BigJob.h
 ******************************************************************************
 * Purpose             : Inherits Projector.  The BigJob is created by the 
 * Workmanager to setup the initial "Big Job" which will be divided up 
 * among different nodes in the cluster.  These nodes get what's called a 
 * WorkUnit currently, which is essentially the same thing as a "big job," 
 * only with a trivial amount of differences in private data.  As to why
 * BigJob and WorkUnit are not both inheriting off of an interface, or 
 * use Projector as a Composition relation, the world has no idea.
 *****************************************************************************/

#include "Projector.h"
#include "WorkUnit.h"
#include "Stitcher.h"
#include "StitcherNode.h"

class BigJob : public Projector
{
  public:
    BigJob(bool useThread) : m_useThreads(useThread), m_workStitcher(out) {}
    virtual ~BigJob();

    void setnumworkunits(workunitid_t newnumworkunits);
    void insertscanline(unsigned char * scanline, long row);
    workunitid_t getnumworkunits();

  protected:
    bool m_useThreads;
    Stitcher m_workStitcher;
    workunitid_t numworkunits;
};

class BigResult
{
};

#endif

