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

#include "BigJob.h"

BigJob::~BigJob() {}

//////////////////////////////////////////////////

void BigJob::setnumworkunits(workunitid_t newnumworkunits)
{
  numworkunits = newnumworkunits;
}

//////////////////////////////////////////////////

void BigJob::insertscanline(unsigned char * scanline, long row)
{
  if(m_useThreads)
  {
    if ( out == NULL ) 
        setupOutput(outfile);
      
    //    StitcherNode * temp = new StitcherNode(scanline,
    //                                           sizeof(scanline), 
    //                                           row);
    //    m_workStitcher.add(temp);
  }
  else
  {
     if(out == NULL)
       setupOutput(outfile);
     out->putRawScanline(row, scanline);
     std::cout << "outputing scanline w/o stitcher: " << row << std::endl;
  }
}

//////////////////////////////////////////////////

workunitid_t BigJob::getNumWorkUnits()
{
  return numworkunits;
}


