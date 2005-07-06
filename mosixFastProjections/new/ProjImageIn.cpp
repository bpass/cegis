
/*! 
 *
 * \author Mark Schisler \n
 *         Chris Bilderback 
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageIn.h 
 * 
 * \brief The ProjImageIn object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <iostream>
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

/*****************************************************************************/

ProjImageIn::ProjImageIn( const ProjImageParams & params,
                          ProjIOLib::ProjectionReader& projReader ) 
    : ProjImageInInterface(),
      ProjImageData(params.getBounds(), NULL, params.getProjection()),
      m_projReader(projReader), 
      m_cache( NULL )
{
  using USGSImageLib::DOQImageIFile;
  using USGSImageLib::GeoTIFFImageIFile;
  using USGSImageLib::LRUCacheManager;
  using std::nothrow;
  using std::bad_alloc;
    
  double scale[3] = {0};                // used to get the scale
  double tp[6] = {0};                   // used to get the tiepoints
  double temp = 0.0f;
  short unsigned int tpnum;             // stores the number of tiepoints
  float xscale = 0.0;                   // scale in x dir
  GeoTIFFImageIFile* ingeo = NULL;      // used to recast as a geotiff
  DOQImageIFile * indoq = NULL;         // used to recast as a doq
  std::string imgFilename = params.getFilename();
  
  // setup the latitude and longitude bounds for the image.
  this->setBounds(params.getBounds()); 
  
  try
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

      ingeo->getPixelScale(scale);
      m_scale.x = scale[0];
      m_scale.y = scale[1]; 
      m_scale.setImgType( GEOTIFF );
      m_scale.setProjSys( m_proj->getProjectionSystem() );

      ingeo->getTiePoints(tp, tpnum);

      this->setLeftBound( tp[3] );
      this->setTopBound( tp[4] );
    
      m_file = ingeo; 
      
      if (indoq != NULL ) delete indoq;
      indoq = NULL;
      
    // if the doq open was successful 
    } else if (indoq->good())
    {
      m_proj = m_projReader.createProjection(indoq);

      if(!m_proj)
        throw GeneralException("Error: Bad Input.");

      indoq->getHorizontalResolution(xscale);
      
      m_scale.x = xscale; 
      m_scale.y = xscale; 
      m_scale.setProjSys(m_proj->getProjectionSystem());
      m_scale.setImgType(DOQ);
                             
      indoq->getXOrigin(temp);
      this->setLeftBound(temp);
      indoq->getYOrigin(temp);
      this->setTopBound(temp);

      m_file = indoq;

      if (ingeo != NULL ) delete ingeo;
      ingeo = NULL;
      
    // use the file extension and parameter file to create the image.
    } else
    {
        this->openImageWithParamFile(params.getFilename());
    }

    // set some common attributes
    m_file->setFileName(imgFilename);

    //check for 16 bit tiffs that can't use cache
    if( (this->getBPS() == 8) && kgCacheSize > 0 ) 
    {
      if(!(m_cache = new(nothrow) LRUCacheManager(
           dynamic_cast<USGSImageLib::ImageIFile*>(m_file), kgCacheSize)))
        throw bad_alloc();
    } 

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

const_scanline_t ProjImageIn::getCachedLine( unsigned int line ) const 
{
    return m_cache->getRawScanline(line);
}

/*****************************************************************************/

ProjImageScale ProjImageIn::calculateScale(const DRect & bounds) 
{
    m_scale.x = bounds.left - bounds.right;
    m_scale.y = bounds.top  - bounds.bottom;

    m_scale.x / this->getWidth();   // divide by width in pixels.
    m_scale.y / this->getHeight();  // divide by height in pixels. 
    
    return m_scale;
}

/*****************************************************************************/

bool ProjImageIn::openImageWithParamFile(const std::string& filename)
{
    bool success = true;
    std::string strExtension, Filename(filename); // const improper in imagelib
    const ProjLib::Projection* proj = m_parameters.getProjection();
    using MiscUtils::cmp_nocase;
    using std::nothrow;

    try 
    {
        if (proj == NULL ) throw GeneralException("No projection found.");
        
        strExtension = getFileExtension(filename);
        calculateScale(m_parameters.getBounds()); 
        m_scale.setProjSys(proj->getProjectionSystem());

        if ( !cmp_nocase(strExtension, "JPG") == 0 || 
             !cmp_nocase(strExtension, "JPEG") == 0 )
        {
            m_scale.setImgType(JPEG);
            if (!(m_file = new(nothrow)USGSImageLib::JPEGImageIFile(Filename)))
                throw std::bad_alloc();
                
        } else if ( !cmp_nocase(strExtension, "PNG") == 0 )
        {
            m_scale.setImgType(PNG);
            if(!(m_file = new(nothrow)USGSImageLib::PNGImageIFile(Filename))) 
                throw std::bad_alloc();
            
        } else
            success = false;
        
    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
        success = false;
        
    } catch ( USGSImageLib::ImageException & ie ) 
    {
        std::cout << "ImageException thrown." << std::endl;
        success = false;
        
    } catch ( ... )
    {
        std::cout << "Uknown error." << std::endl;    
    }

    return success;
}

/*****************************************************************************/

inline const unsigned char * ProjImageIn::getPixel( 
const unsigned int& x, const unsigned int& y ) const 
{
    static unsigned int last_y = y;
    static const unsigned char * row = m_cache->getRawScanline(y);

    if ( last_y == y ) 
    {
        return (row +x);
        
    } else
    {
        row = m_cache->getRawScanline(y);
        last_y = y;
        return (row + x);
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
    outputBounds.left   = this->getMin(xPts);
    outputBounds.right  = this->getMax(xPts);
    outputBounds.bottom = this->getMin(yPts);
    outputBounds.top    = this->getMax(yPts);

    return outputBounds;
}


/*****************************************************************************/

double ProjImageIn::getMin( const std::list<double>& l )const
{
   double _min = *(l.begin());
   
   for( std::list<double>::const_iterator i = ++l.begin(); i != l.end(); ++i)
        if ( *i < _min ) _min = *i;

   return _min;
}

/*****************************************************************************/

double ProjImageIn::getMax( const std::list<double>& l )const
{
    double _max = *(l.begin());

    for( std::list<double>::const_iterator i = ++l.begin(); i != l.end(); ++i) 
        if ( *i > _max ) _max = *i;

    return _max;
}

/*****************************************************************************/

} // namespace

