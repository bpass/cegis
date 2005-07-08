
/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file FromMultiGeoProjector.cpp
 * 
 * \brief The FromMultiGeoProjector class is an object which takes 
 * input from several images in a Geographic projection, and projects 
 * this input into a single image of a desired projection.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"
#include "FromMultiGeoProjector.h"
#include "Math.h"

namespace USGSMosix
{

/******************************************************************************/

FromMultiGeoProjector::FromMultiGeoProjector( 
            ProjIOLib::ProjectionWriter & projWriter,
            ProjIOLib::ProjectionReader & projReader,
            ProjImageParams & params,
            ProjImageInInterface & source
) 
:   m_imgIn(source),
    m_projWriter(projWriter),
    m_projReader(projReader),
    m_params(params),
    m_pmeshDivisions(4), 
    m_pmeshInterp( MathLib::CubicSpline ), 
    m_forwardMesh(NULL),
    m_reverseMesh(NULL)
{
    m_fromProj = m_imgIn.getProjection();
    m_toProj = m_params.getProjection(); 
    if (m_toProj == NULL ) 
        throw GeneralException("No to Projection");
}

/******************************************************************************/

FromMultiGeoProjector::~FromMultiGeoProjector()
{
    if ( m_imgOut ) delete m_imgOut;    
}

/******************************************************************************/

void FromMultiGeoProjector::setInputSources( ProjImageInInterface & pi )
{
    m_projReader.removeAllProjections();
    m_imgIn = pi;
}

/******************************************************************************/

void FromMultiGeoProjector::setPmeshInterpolator( 
    const MathLib::InterpolatorType& t )
{
    m_pmeshInterp = t;
}

/******************************************************************************/

void FromMultiGeoProjector::setPmeshDivisions(const int & inpmeshsize) 
{
    m_pmeshDivisions = inpmeshsize;
}

/******************************************************************************/

const ProjLib::Projection* FromMultiGeoProjector::getOutputProjection() const 
{
    if ( m_imgOut != NULL ) return m_imgOut->getProjection();    
    else throw GeneralException("m_imgOut is NULL");
}

/******************************************************************************/

MathLib::InterpolatorType FromMultiGeoProjector::getPmeshInterpolator() const 
{
    return m_pmeshInterp;
}

/******************************************************************************/

int FromMultiGeoProjector::getPmeshDivisions() const 
{
    return m_pmeshDivisions;
}

/******************************************************************************/

DRect FromMultiGeoProjector::getoutRect() const
{
    if ( m_imgOut != NULL ) return m_imgOut->getOuterBounds(); 
    else throw GeneralException("m_imgOut is NULL");
}

/******************************************************************************/

void FromMultiGeoProjector::getNewExtents()
{
    // set up the mesh for the forward projection using for the bounds
    // of said image, the outermost edges of the input list of images.
    m_forwardMesh = & m_imgIn.setupMesh( *(m_toProj), 
                                          m_pmeshDivisions,
                                          m_pmeshInterp ); 
    
    if( m_forwardMesh != NULL ) 
    {
        // get the boundaries for the potential image, the one that we 
        // are projecting onto.
        getImageBounds( *m_forwardMesh);
    
        // delete this mesh as we're now done with it.
        if ( m_forwardMesh != NULL ) 
        {   
            delete m_forwardMesh;
            m_forwardMesh = NULL;
        }

    } else
        throw GeneralException("m_forwardMesh is NULL");

    return;
}

/******************************************************************************/

void FromMultiGeoProjector::project()
{
    scanlines_t scanlines;
   
    // get the extents of the new image. 
    getNewExtents();
    
    if ( m_imgOut != NULL ) 
    {
        // get scanline data
        scanlines = project(0, m_imgOut->getHeight());
        
        // write out data to image
        m_imgOut->putScanlines( scanlines, 
                                m_imgOut->getHeight() );
                            
        // cleanup space for scanlines    
        for( int i = 0; i < m_imgOut->getHeight(); ++i ) 
            delete scanlines[i];
        delete [] scanlines;
        scanlines = NULL;
                                
    } else
        throw GeneralException("m_imgOut is NULL");
}

/******************************************************************************/

scanlines_t FromMultiGeoProjector::project( long unsigned int beginLine, 
                                            long unsigned int endLine )
{
    double xSrcScale(0.0f), ySrcScale(0.0f);
    long unsigned int xPixelCount(0), yPixelCount(beginLine);
    long int xSrcPixel(0), ySrcPixel(0);
    int sppCount(0);
    scanlines_t scanlines = NULL;
    const unsigned char * tmpPixel = NULL;
    
    try {

        if ( m_imgOut == NULL )
            throw GeneralException("m_imgOut is NULL");

        // grab these to start to speed up computation ... 
        // i.e., we don't want to be fetching these variables
        // thousands of times in the double for loop.
        const long unsigned int inWidth(m_imgIn.getWidth());
        const long unsigned int inHeight(m_imgIn.getHeight());
        const long unsigned int outWidth(m_imgOut->getWidth());
        const long unsigned int outHeight(m_imgOut->getHeight());
        const ProjImageScale inScale(m_imgIn.getPixelScale());
        const ProjImageScale outScale(m_imgOut->getPixelScale()); 
        const DRect inBounds(m_imgIn.getOuterBounds());
        const DRect outBounds(m_imgOut->getOuterBounds());
        const int spp(m_imgIn.getSPP());

        if ( !Math<long int>::inInclusiveRange(beginLine, 0, outHeight ) ||
             !Math<long int>::inInclusiveRange(endLine, 0, outHeight ) )
            throw GeneralException("Line boundaries out of range."); 

        // allocate the dynamic memory for the resulting scanlines.
        if ( !(scanlines = new(std::nothrow)unsigned char*[endLine-beginLine]))
            throw std::bad_alloc();

        for ( unsigned int i = 0; i < (endLine-beginLine); ++i )
        {
            if (!(scanlines[i] = new(std::nothrow)unsigned char[outWidth*spp]))
                throw std::bad_alloc();
        }

        // setup the reverse projection's mesh            
        m_reverseMesh = & m_imgOut->setupMesh( *(m_fromProj), 
                                               m_pmeshDivisions,
                                               m_pmeshInterp ); 
        
        // This is a reverse projection, so in other words,  we're iterating 
        // over the new image's x/y pixels with the double for loops, using 
        // first source image 'to scale' coordinates and then x/y pixel 
        // coordinates from the old or source image of what we need to place 
        // in every newly formed pixel.  Then we use that coodinate to copy 
        // that data over to our new image.
        for( yPixelCount = 0; yPixelCount < (endLine-beginLine); ++yPixelCount )
        {
            for( xPixelCount = 0; xPixelCount < outWidth; ++xPixelCount ) 
            {
                // grab the 'to scale' x/y coordinates of where we are
                // on this image.
                xSrcScale = outBounds.left + outScale.x * xPixelCount;
                ySrcScale = outBounds.top - outScale.y * yPixelCount;
                
                // reverse project those points to get the 'to scale'
                // location on our old image of the pixels we need 
                // to place on this one, considering the projection 
                // we're doing. So, what's returned from here is to scale 
                // of our input image. 
                reverseProjectPoint(xSrcScale, ySrcScale);    
                
                // take those x,y coordinates to scale and then 
                // translate them to the x,y pixel coordinates 
                // of the input image.
                xSrcPixel  = Math<long int>::ceil( (xSrcScale - inBounds.left) 
                             / inScale.x ); 
                ySrcPixel  = Math<long int>::ceil( (inBounds.top - ySrcScale) 
                             / inScale.y );
                
                // if the resulting pixel coordinates are out of bounds
                if (!Math<long int>::inInclusiveRange(xSrcPixel,0,inWidth-1) ||
                    !Math<long int>::inInclusiveRange(ySrcPixel,0,inHeight-1) )
                {
                   // place a black pixel in each sample (channel)
                   for( sppCount = 0; sppCount < spp; ++sppCount)
                   {
                        scanlines[(yPixelCount - beginLine)]
                                 [ xPixelCount*spp+sppCount ] = 0; 
                   }
                   
                } else // if they are instead in bounds
                {
                   tmpPixel = m_imgIn.getPixel(xSrcPixel, ySrcPixel);
                    
                   // get the pixel from the group of source images.  
                   for( sppCount = 0; sppCount < spp; ++sppCount)
                   {
                       scanlines[(yPixelCount - beginLine)]
                                [ xPixelCount*spp + sppCount ] = 
                       tmpPixel[sppCount]; 
                   }
                }
            }
        }
    } catch ( GeneralException & e ) 
    {
        std::cout << "FromMultiGeoProjector: " << e.toString() << std::endl;   
    }
    
    return scanlines;    
    
}

/******************************************************************************/

void FromMultiGeoProjector::getImageBounds( 
const PmeshLib::ProjectionMesh & translatingMesh )
{
    const long int inHeight = m_imgIn.getHeight();
    const long int inWidth =  m_imgIn.getWidth();
    DRect outputBounds;         
    ProjImageScale outputScale;
    long int outHeight(0), outWidth(0);

    try {
        m_forwardMesh = &translatingMesh;
        
        if ( m_fromProj == NULL || m_toProj == NULL )
            throw GeneralException("m_imgOut is NULL");
        outputBounds = m_imgIn.getNewBounds(translatingMesh);
        
        // calculate the pixel to "true scale" ratio, keeping the 
        // same number of pixels in each direction so as to keep
        // the size of the image constant.
        outputScale.x = (outputBounds.right - outputBounds.left)   / inWidth;
        outputScale.y = (outputBounds.top   - outputBounds.bottom) / inHeight;
        
        // in case we were off by a pixel, recalculate the outWidth and 
        // outHeight and round up with how many we need.
        outWidth  = Math<long int>::ceil((outputBounds.right-outputBounds.left) 
                    / (outputScale.x) ); 
        
        outHeight = Math<long int>::ceil((outputBounds.top-outputBounds.bottom) 
                    / (outputScale.y) );
       
        // set what we've learned up in the new image object.
        m_imgOut = new(std::nothrow)ProjImageOut( 
                                     *m_toProj,
                                     m_projWriter,
                                     m_params.getFilename(),
                                     std::pair<long,long>(outHeight,outWidth),
                                     outputScale,
                                     m_imgIn.getPhotometric(), 
                                     m_imgIn.getBPS(), 
                                     m_imgIn.getSPP(),
                                     outputBounds );
        
        if (m_imgOut == NULL ) throw std::bad_alloc();

    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
    }

    return;
}

/*****************************************************************************/

} // namespace 