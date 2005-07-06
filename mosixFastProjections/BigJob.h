#ifndef BIGJOB_H_
#define BIGJOB_H_

/******************************************************************************
 * Last Modified by    : Mark Schisler 
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
#include "tiffio.h"
#include "PNGImageOFile.h"

class BigJob : public Projector
{
  public:
    BigJob(bool useThread) : m_jpegOut(false),
                             m_useThreads(useThread)
                             /*, m_workStitcher(out)*/ {}
    virtual ~BigJob();
   
    USGSImageLib::ImageOFile*  getOutputFile() { return out; } 
    
    bool setupOut()  
    { 
        try {
        
           if ( out == NULL )
           {
               
               std::string s; Projector::setupOutput(s);
 
               if ( m_jpegOut == true )
                   dynamic_cast<USGSImageLib::TIFFImageOFile*>
                       (out)->setRowsPerStrip(268435456); // ~unlimited, 2^31
              
               if ( m_jpegOut == true )
                   dynamic_cast<USGSImageLib::TIFFImageOFile*>
                       (out)->setCompression(COMPRESSION_JPEG);
           }
        

        } catch ( ... ) 
        {    
            return false;
        }

        return true;
            
    }
  

    // functions to enable or disable JPEG output from libtiff
    // must be called before setupOut. 
    void enableJPEGOutput() { m_jpegOut = true; } 
    void disableJPEGOutput() { m_jpegOut = false; } 

    
    void setnumworkunits(workunitid_t newnumworkunits);
    void insertscanline(unsigned char * scanline, long row);
    workunitid_t getNumWorkUnits();

  protected:
    bool m_jpegOut; 
    bool m_useThreads;
    /// DEPRECATED : Stitcher m_workStitcher (MS) ;
    workunitid_t numworkunits;
};

class BigResult
{
};

#endif

