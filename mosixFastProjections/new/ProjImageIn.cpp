
/*! 
 *
 * \author Mark Schisler \n
 *         Chris Bilderback 
 *
 * \date $Date: 2005/10/20 23:21:15 $
 *
 * \version 0.1
 * 
 * \file ProjImageIn.h 
 * 
 * \brief Implementation file for ProjImageIn class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <iostream>
#include <ProjectionLib/GeographicProjection.h>
#include <ProjectionMesh/ProjectionMesh.h>
#include <ImageLib/GeoTIFFImageIFile.h>
#include <ImageLib/DOQImageIFile.h>
#include <ImageLib/JPEGImageIFile.h>
#include <ImageLib/PNGImageIFile.h>
#include <ImageLib/LRUCacheManager.h>
#include <MiscUtils/cmp_nocase.h>
#include "ProjImageIn.h"
#include "GeneralException.h"

namespace USGSMosix 
{

// static data
/*****************************************************************************/

const ProjLib::GeographicProjection ProjImageIn::m_geoProjection;
ProjIOLib::ProjectionReader ProjImageIn::m_projReader;

// member functions
/*****************************************************************************/

ProjImageIn::ProjImageIn( const ProjImageParams & params ) 
    : ProjImageData(params.getBounds(), NULL, params.getProjection()),
      ProjImageInInterface(),
      m_params(params),
      m_cache( NULL ),
      m_spp(1),
      m_lastLine(-1, NULL),
      m_geoMesh( NULL ),
      m_geoBounds(0.0,0.0,0.0,0.0),
      m_haveGeoBounds(false)
{
  using USGSImageLib::DOQImageIFile;
  using USGSImageLib::GeoTIFFImageIFile;
  using USGSImageLib::LRUCacheManager;
  using std::nothrow;
  using std::bad_alloc;
  using MiscUtils::cmp_nocase;
    
  double scale[3] = {0};                // used to get the scale
  double tp[6] = {0};                   // used to get the tiepoints
  double temp = 0.0f;
  short unsigned int tpnum;             // stores the number of tiepoints
  float xscale = 0.0;                   // scale in x dir
  GeoTIFFImageIFile* ingeo = NULL;      // used to recast as a geotiff
  DOQImageIFile * indoq = NULL;         // used to recast as a doq
  std::string imgFilename = params.getFilename();
  std::string strExtension;
 
  WRITE_DEBUG ( "Creating ProjImageIn" << std::endl );
  
  strExtension = getFileExtension(imgFilename);
  
  // setup the latitude and longitude bounds for the image.
  this->setBounds(params.getBounds()); 
  
  try
  {
 
    if ( !cmp_nocase( strExtension, "TIFF" ) ||
         !cmp_nocase( strExtension, "TIF" ) ) 
    {
        // try to open the file as a doq
        if(!(indoq = new(nothrow)DOQImageIFile(imgFilename)))
          throw bad_alloc();

        // try to open the file as a geotiff
        if(!(ingeo = new(nothrow)GeoTIFFImageIFile(imgFilename, false)))
          throw bad_alloc();
       
            // if the geo open was successful 
            if(ingeo->good())
            { 
              //create the from projection
              m_proj = m_projReader.createProjection(ingeo);

              if(!m_proj)
                throw GeneralException("Error: Bad Input.");

              // GeoTIFFs store data at a certain resolution, in 
              // units per pixel.  The units can be determined
              // from the parameter file, and hence, by the 
              // projection object created from the parameter file.
              ingeo->getPixelScale(scale); 
              m_scale.x = scale[0];
              m_scale.y = scale[1]; 
              m_scale.setUnits( m_proj->getUnit() );

              ingeo->getTiePoints(tp, tpnum);

              this->setLeftBound( tp[3] );
              this->setTopBound( tp[4] );
            
              this->setImageFile(ingeo); 
              
              if (indoq != NULL ) delete indoq;
              indoq = NULL;
              
            // if the doq open was successful 
            } else if (indoq->good())
            {
              m_proj = m_projReader.createProjection(indoq);

              if(!m_proj)
                throw GeneralException("Error: Bad Input.");

              indoq->getHorizontalResolution(xscale);
              
              // DOQ's store image files at 1 meter per pixel resolution
              // by definition, so x and y should be 1 in this case.
              m_scale.x = xscale; // 1 meter per pixel 
              m_scale.y = xscale; // 1 meter per pixel
              m_scale.setUnits( m_proj->getUnit() );
                                     
              indoq->getXOrigin(temp);
              this->setLeftBound(temp);
              indoq->getYOrigin(temp);
              this->setTopBound(temp);

              this->setImageFile(indoq);

              if (ingeo != NULL ) delete ingeo;
              ingeo = NULL;
              
            // use the file extension and parameter file to create the image.
            } else
                throw GeneralException("Unable to open TIFF.");
    } else
    {
        this->openImageWithParamFile(params.getFilename());
    }

    // set some common attributes
    this->setFilename(imgFilename);

    //check for 16 bit tiffs that can't use cache
    if( (this->getBPS() == 8) && kgCacheSize > 0 ) 
    {
      if(!(m_cache = new(nothrow) LRUCacheManager(
         dynamic_cast<USGSImageLib::ImageIFile*>(getImageFile()), kgCacheSize)))
        throw bad_alloc();
    } 

    // save a copy for faster access time.
    m_spp = this->getSPP();
    WRITE_DEBUG (  "got spp " << m_spp << std::endl );
    
    //set the dimensions
    this->setRightBound(this->getLeftBound() + m_scale.x * this->getWidth());
    this->setBottomBound(this->getTopBound() - m_scale.y * this->getHeight());

  }
  catch( GeneralException & ge ) 
  {
    std::cout << ge.toString() << std::endl;
  }
}

/*****************************************************************************/

ProjImageIn::ProjImageIn( const ProjImageIn & copyOf )
    : ProjImageDataInterface(),
    ProjImageData(copyOf.getOuterBounds(), copyOf.getImageFile(), 
                  copyOf.getProjection()),
    SerializableInterface(),
    ProjImageInInterface(),
    m_params( copyOf.m_params ),
    m_cache(new USGSImageLib::CacheManager(*copyOf.m_cache)),
    m_spp(copyOf.m_spp),
    m_lastLine(copyOf.m_lastLine),
    m_geoMesh(copyOf.m_geoMesh),
    m_geoBounds(copyOf.m_geoBounds),
    m_haveGeoBounds(copyOf.m_haveGeoBounds)
{
    WRITE_DEBUG( "PROJIMAGEIN CONSTRUCTOR CALLED" << std::endl );
}

/*****************************************************************************/

ProjImageIn::~ProjImageIn()
{
    delete m_cache;
}

/*****************************************************************************/

const_scanline_t ProjImageIn::getCachedLine( unsigned int line ) const 
{
    return m_cache->getRawScanline(line);
}

/*****************************************************************************/

ProjImageScale ProjImageIn::calculateScale(const DRect & bounds) 
{
    m_scale.x = Math<scale_t>::abs(bounds.left - bounds.right);
    m_scale.y = Math<scale_t>::abs(bounds.top  - bounds.bottom);

    m_scale.x /= this->getWidth();   // divide by width in pixels.
    m_scale.y /= this->getHeight();  // divide by height in pixels. 

    return m_scale;
}

/*****************************************************************************/

bool ProjImageIn::openImageWithParamFile(const std::string& filename)
{
    bool success = false;
    std::string strExtension, Filename(filename); // const improper in imagelib
    USGSImageLib::ImageFile* file = NULL;
    using MiscUtils::cmp_nocase;
    using std::nothrow;

    try 
    {
        if (m_proj == NULL ) throw GeneralException("No projection found.");
        
        strExtension = getFileExtension(filename);

        if ( !cmp_nocase(strExtension, "JPG") == 0 || 
             !cmp_nocase(strExtension, "JPEG") == 0 )
        {
            if (!(file = new(nothrow)USGSImageLib::JPEGImageIFile(Filename)))
                throw std::bad_alloc();
           
            WRITE_DEBUG ( "opened jpg:" << filename << std::endl
                          << getOuterBounds() << std::endl );
           
            this->setImageFile(file);
            success = true;
                
        } else if ( !cmp_nocase(strExtension, "PNG") == 0 )
        {
            if(!(file = new(nothrow)USGSImageLib::PNGImageIFile(Filename))) 
                throw std::bad_alloc();
            
            WRITE_DEBUG ( " opened png:" << std::endl
                          << getOuterBounds() << filename << std::endl );

            this->setImageFile(file);
            success = true;
            
        } else
        {
            WRITE_DEBUG ( "failure" << std::endl );
        }
        
        calculateScale(getOuterBounds()); 
        m_scale.setUnits( m_proj->getUnit() );
       
    } catch ( GeneralException & ge ) 
    {
        WRITE_DEBUG ( ge.toString() << std::endl );
        
    } catch ( USGSImageLib::ImageException & ie ) 
    {
        WRITE_DEBUG ( "ImageException thrown." << std::endl );
        
    } catch ( ... )
    {
        WRITE_DEBUG ( "Uknown error." << std::endl );
    }

    return success;
} 

/*****************************************************************************/

inline const PixelInterface<sample_t> * ProjImageIn::getPixel( 
const unsigned int& x, const unsigned int& y ) const 
{
    if ( static_cast<unsigned int>(m_lastLine.first) != y ) 
    {
        m_lastLine.second = m_cache->getRawScanline(y);
        m_lastLine.first = y;
    }
    
    if ( getPhotometric() == PHOTO_RGB ) 
        return new PixelRGB<sample_t>(m_lastLine.second + (x * m_spp) );
            
    else if ( getPhotometric() == PHOTO_GREY ) 
        return new PixelGrey<sample_t>(m_lastLine.second + (x * m_spp) );
    else
        throw GeneralException("Unsupported image coloring system.");
}

/*****************************************************************************/

const PixelInterface<sample_t> *
ProjImageIn::getPixel( const double& latitude, const double& longitude ) const
{
    static long int xSrcPixel(0), ySrcPixel(0); 
    static double xSrcScale(0.0), ySrcScale(0.0);

    // if we haven't already, set up the geographic mesh.
    if ( !m_geoMesh )
    {
        m_geoMesh = &this->setupReverseMesh(m_geoProjection,
                                            getGeographicBounds());
    }
    
    xSrcScale = longitude;
    ySrcScale = latitude;
    
    // project latitude and longitude to scale of image.
    m_geoMesh->projectPoint(xSrcScale, ySrcScale);
  
    // get pixel coordinates using source boundaries. 
    xSrcPixel  = Math<long int>::ceil( (xSrcScale - getLeftBound()) 
                 / m_scale.x ); 
    ySrcPixel  = Math<long int>::ceil( (getTopBound() - ySrcScale) 
                 / m_scale.y );
                
    // if the resulting pixel coordinates are out of bounds
    if ( xSrcPixel < 0  || xSrcPixel >= getWidth() ||
         ySrcPixel < 0  || ySrcPixel >= getHeight() ) 
    {
        return NULL;
        
    } else // if they are instead in bounds
    {
        // get the pixel using the getPixel function
        return getPixel(static_cast<unsigned int>(xSrcPixel), 
                        static_cast<unsigned int>(ySrcPixel));
    }
}


/*****************************************************************************/

DRect ProjImageIn::getNewBounds(const PmeshLib::ProjectionMesh & mesh)const
{
    const long int inHeight = this->getHeight();
    const long int inWidth =  this->getWidth();
    DRect sourceBounds = this->getOuterBounds();
    std::list<double> xPts, yPts;
    double xSrcScale(0), ySrcScale(0);
    long int xPixelCount(0), yPixelCount(0);
    DRect outputBounds;

    WRITE_DEBUG ( "sourceBounds" << sourceBounds << std::endl );
    
    for( xPixelCount = 0; xPixelCount < inWidth; ++xPixelCount )
    {
        // reproject the top boundary
        xSrcScale = sourceBounds.left + m_scale.x * xPixelCount;
        ySrcScale = sourceBounds.top;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters
       
        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale);

        // reproject the bottom boundary
        xSrcScale = sourceBounds.left + m_scale.x * xPixelCount;
        ySrcScale = sourceBounds.bottom;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters
        
        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 
    }
    
    for( yPixelCount = 0; yPixelCount < inHeight; ++yPixelCount ) 
    {
        // reproject the left line
        ySrcScale = sourceBounds.top - m_scale.y * yPixelCount;
        xSrcScale = sourceBounds.left;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters

        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 

        // reproject the right line
        ySrcScale = sourceBounds.top - m_scale.y * yPixelCount;
        xSrcScale = sourceBounds.right;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters

        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 
    }
   
    // calculate the new image's bounds in terms of the 
    // old map's measurement system.
    outputBounds.left   = Math<double>::getMin(xPts);
    outputBounds.right  = Math<double>::getMax(xPts);
    outputBounds.bottom = Math<double>::getMin(yPts);
    outputBounds.top    = Math<double>::getMax(yPts);

    WRITE_DEBUG (  "outputBounds" << outputBounds << std::endl );
    
    return outputBounds;
}

/*****************************************************************************/

DRect ProjImageIn::getGeographicBounds()const
{
    // since this function is pretty expensive, we use a bool to have this
    // function keep state so we only have to generate the data for it once.
    if ( !m_haveGeoBounds  )
    {
        m_haveGeoBounds = true; 
        
        if ( m_proj->getProjectionSystem() != GEO )
        {
            WRITE_DEBUG ( "generating geographic bounds. " << std::endl );
            return ( m_geoBounds = 
                    this->getNewBounds(this->setupMesh(m_geoProjection)));
        
        // in this case, no need to create a mesh, return the geographic 
        // bounds directly.
        } else
        {
            WRITE_DEBUG ( "returning geographic bounds directly."<<std::endl);
            return ( m_geoBounds = getOuterBounds() );
        }
            
    } else
        return m_geoBounds;
}

/*****************************************************************************/

ProjImageIn ProjImageIn::createFromSocket( ClientSocket & socket )
{
    unsigned int strLen(0);
    char * pszParamfilename = NULL;
    std::string strParamfilename("");
    int i = 0; 
    
    socket.receive(&i, sizeof(i));
    // in this case what is trying to be constructed here is a list, and 
    // not an individual image. 
    if ( i != -1 )
    {
        WRITE_DEBUG ( "i = " << i << std::endl; )
        throw GeneralException("Error, not an individual image in transit.");
    }

    socket.receive(&strLen,sizeof(strLen));
    
    if ( strLen <= 0 )
        throw GeneralException("Cannot make array of size less than zero.");
    WRITE_DEBUG ( "string length in: "<< strLen << std::endl; );
    if ( ( pszParamfilename = new (std::nothrow)char[strLen + 1] ) == NULL ) 
        throw GeneralException("Failed Dynamic Allocation.");
    socket.receive(pszParamfilename, strLen);
    pszParamfilename[strLen] = '\0';
    strParamfilename = pszParamfilename;
    delete pszParamfilename;    
    
    WRITE_DEBUG ( "filename: " << strParamfilename << std::endl );
    
    return ProjImageIn(ProjImageParams(strParamfilename,ProjImageParams::INPUT));
}
 
/*****************************************************************************/

void ProjImageIn::exportToSocket( ClientSocket & socket )const
{
    std::string paramFilename = m_params.getParamFilename();
    unsigned int length = paramFilename.length();
    int i = -1; 

    // put information in buffer
    socket.appendToBuffer(&i, sizeof(i));
    socket.appendToBuffer(&length, sizeof(length));
    WRITE_DEBUG ( "string length out: "<< length << std::endl; )
    WRITE_DEBUG ( "exporting filename: " << paramFilename << std::endl; )
    
    socket.appendToBuffer(paramFilename.c_str(), 
                          paramFilename.length());
    
    // send
    socket.sendFromBuffer();
}

/*****************************************************************************/

} // namespace  
