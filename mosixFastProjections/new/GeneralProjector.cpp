
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file GeneralProjector.cpp
 * 
 * \brief Implemenation file for GeneralProjector class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"
#include "GeneralProjector.h"
#include "Math.h"
#include "ProjImageOut.h"

namespace USGSMosix
{

// initialization of static members
/******************************************************************************/
    
ProjIOLib::ProjectionReader GeneralProjector::m_projReader;
ProjIOLib::ProjectionWriter GeneralProjector::m_projWriter;

ProjImageFactory 
GeneralProjector::m_imgFactory(m_projReader, m_projWriter); 
   
// member functions
/******************************************************************************/

GeneralProjector::GeneralProjector( 
            ProjImageInInterface & source,
            ProjImageOutInterface & destination 
) 
:   m_imgIn(source),
    m_imgOut(&destination),
    m_pmeshDivisions(kgMeshDivisions), 
    m_pmeshInterp( kgInterpolator ), 
    m_forwardMesh(NULL),
    m_reverseMesh(NULL)
{
    m_fromProj = m_imgIn.getProjection();
    m_toProj = m_imgOut->getProjection(); 
    if (m_toProj == NULL ) 
        throw GeneralException("No to-Projection");
}
    
/******************************************************************************/

GeneralProjector::GeneralProjector( 
            ProjImageParams & params,
            ProjImageInInterface & source
) 
:   m_imgIn(source),
    m_imgOut(NULL),
    m_params(&params),
    m_pmeshDivisions(kgMeshDivisions), 
    m_pmeshInterp( kgInterpolator ), 
    m_forwardMesh(NULL),
    m_reverseMesh(NULL)
{
    m_fromProj = m_imgIn.getProjection();
    m_toProj = m_params->getProjection();
    if (m_toProj == NULL ) 
        throw GeneralException("No to-Projection");
}

/******************************************************************************/

GeneralProjector::~GeneralProjector()
{
    delete m_imgOut;    
}

/******************************************************************************/

void GeneralProjector::setInputSources( ProjImageInInterface & pi )
{
    m_projReader.removeAllProjections();
    m_imgIn = pi;
}

/******************************************************************************/

void GeneralProjector::setPmeshInterpolator( 
    const MathLib::InterpolatorType& t )
{
    m_pmeshInterp = t;
}

/******************************************************************************/

void GeneralProjector::setPmeshDivisions(const int & inpmeshsize) 
{
    m_pmeshDivisions = inpmeshsize;
}

/******************************************************************************/

const ProjLib::Projection* GeneralProjector::getOutputProjection() const 
{
    if ( m_imgOut != NULL ) return m_imgOut->getProjection();    
    else throw GeneralException("m_imgOut is NULL in getOutputProjection()");
}

/******************************************************************************/

MathLib::InterpolatorType GeneralProjector::getPmeshInterpolator() const 
{
    return m_pmeshInterp;
}

/******************************************************************************/

int GeneralProjector::getPmeshDivisions() const 
{
    return m_pmeshDivisions;
}

/******************************************************************************/

DRect GeneralProjector::getoutRect() const
{
    if ( m_imgOut != NULL ) return m_imgOut->getOuterBounds(); 
    else throw GeneralException("m_imgOut is NULL in getoutrect()");
}

/******************************************************************************/

bool GeneralProjector::setupOutput()
{
    if ( m_imgOut == NULL ) 
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
            constructOutImage( *m_forwardMesh);
        
        } else
            throw GeneralException("m_forwardMesh is NULL");
    }
        
    return true;
}

/******************************************************************************/

void GeneralProjector::project()
{
    scanlines_t scanlines;
   
    // get the extents of the new image. 
    setupOutput();
    
    if ( m_imgOut != NULL ) 
    {
        // get scanline data
        scanlines = project(0, m_imgOut->getHeight());
        
        // write out data to image
        m_imgOut->putScanlines( scanlines, static_cast<unsigned int>(0), 
                  static_cast<unsigned int>(m_imgOut->getHeight() - 1) );
                            
        // cleanup space for scanlines    
        for( int i = 0; i < m_imgOut->getHeight(); ++i ) 
            delete scanlines[i];
        delete [] scanlines;
        scanlines = NULL;
                                
    } else
        throw GeneralException("m_imgOut is NULL in project()");
}

/******************************************************************************/

ProjImageOutPiece GeneralProjector::projectPiece( long unsigned int beginLine,
                                                  long unsigned int endLine )
{
    WRITE_DEBUG("GenProjector: projecting piece.");
    scanlines_t scanlines = this->project(beginLine,endLine);
    return ProjImageOutPiece(scanlines,
                             std::pair<unsigned long, unsigned long>(beginLine,
                                                                     endLine),
                             m_imgOut->getWidth(),
                             m_imgOut->getSPP());
}

/******************************************************************************/

ProjImageOutPiece GeneralProjector::projectPiece()
{
    return projectPiece(0, m_imgOut->getHeight());
}

/******************************************************************************/

scanlines_t GeneralProjector::project( long unsigned int beginLine, 
                                       long unsigned int endLine )
{
    static const ProjLib::GeographicProjection geoProj;
    double xSrcScale(0.0f), ySrcScale(0.0f);
    long unsigned int xPixelCount(0), yPixelCount(beginLine);
    int sppCount(0);
    scanlines_t scanlines = NULL;
    const PixelInterface<unsigned char>* tmpPixel = NULL;
    WRITE_DEBUG ( " begin projecting " << std::endl );
    
    try {

        if ( m_imgOut == NULL )
        {
            setupOutput(); 
    
            if ( m_imgOut == NULL ) 
                throw GeneralException( "m_imgOut is NULL in project(int,int)");
        }

        // grab these to start to speed up computation ... 
        // i.e., we don't want to be fetching these variables
        // thousands of times in the double for loop.
        const long unsigned int outWidth(m_imgOut->getWidth());
        const long unsigned int outHeight(m_imgOut->getHeight());
        const ProjImageScale inScale(m_imgIn.getPixelScale());
        const ProjImageScale outScale(m_imgOut->getPixelScale()); 
        const DRect inBounds(m_imgIn.getOuterBounds());
        const DRect outBounds(m_imgOut->getOuterBounds());
        const int spp(m_imgIn.getSPP());

        if ( beginLine > outHeight || endLine > outHeight  )  
            throw GeneralException("Line boundaries out of range."); 

        // allocate the dynamic memory for the resulting scanlines.
        if ( !(scanlines = new(std::nothrow)unsigned char*[endLine-beginLine]))
            throw std::bad_alloc();

        if ( m_imgOut->getPhotometric() != PHOTO_RGB && 
             m_imgOut->getPhotometric() != PHOTO_GREY ) 
            throw GeneralException("Unsupported output format.");
        
        for ( unsigned int i = 0; i < (endLine-beginLine); ++i )
        {
            if (!(scanlines[i] = new(std::nothrow)unsigned char[outWidth*spp]))
                throw std::bad_alloc();
        }

        // setup the reverse projection's mesh            
        m_reverseMesh = & m_imgOut->setupMesh( geoProj, 
                                               m_pmeshDivisions,
                                               m_pmeshInterp ); 

        WRITE_DEBUG ( " begin reverse projection " << std::endl );

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
    
                // xSrcScale is now a latitude after this call
                // ySrcScale is now a longitude after this call
                m_reverseMesh->projectPoint(xSrcScale, ySrcScale);
                
                // get a pixel from these latitude and longitudes
                tmpPixel=m_imgIn.getPixel(ySrcScale, xSrcScale);
                
                if ( tmpPixel == NULL ) 
                {
                   // place a black pixel in each sample (channel)
                   for( sppCount = 0; sppCount < spp; ++sppCount)
                   {
                        scanlines[(yPixelCount - beginLine)]
                                 [ xPixelCount*spp+sppCount ] = 0; 
                   }
                   
                } else // if they are instead in bounds
                { 
                    if ( m_imgOut->getPhotometric() == PHOTO_RGB ) 
                    {
                         tmpPixel->getRGB( 
                                scanlines[(yPixelCount - beginLine)]
                                         [ xPixelCount*spp],
                                scanlines[(yPixelCount - beginLine)]
                                         [ xPixelCount*spp + 1],
                                scanlines[(yPixelCount - beginLine)]
                                         [ xPixelCount*spp + 2]  );
                     
                    } else // i.e., photometric == PHOTO_GREY, which
                           // is one sample per pixel.
                    {
                        tmpPixel->getGrey(scanlines[(yPixelCount - beginLine)]
                                                   [ xPixelCount*spp] );
                    } 

                    delete tmpPixel;
                }
            }
        }

    } catch ( GeneralException & e ) 
    {
        std::cout << "GeneralProjector: " << e.toString() << std::endl;   
    }
    
    return scanlines;    
    
}

/******************************************************************************/

void GeneralProjector::constructOutImage( 
const PmeshLib::ProjectionMesh & mesh )
{
    const long int inHeight = m_imgIn.getHeight();
    const long int inWidth =  m_imgIn.getWidth();
    DRect outputBounds;         
    ProjImageScale outputScale;
    long int outHeight(0), outWidth(0);

    try {
        m_forwardMesh = &mesh;
        
        if ( m_fromProj == NULL || m_toProj == NULL || m_params == NULL )
            throw GeneralException("NULL pointer in constructOutImage()");
        
        outputBounds = m_imgIn.getNewBounds(mesh);
       
        WRITE_DEBUG (  "output left   :" << outputBounds.left << std::endl
                       << "output right  :" << outputBounds.right << std::endl
                       << "output bottom :" << outputBounds.bottom << std::endl
                       << "output top    :" << outputBounds.top << std::endl ) ;
        
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
      
        WRITE_DEBUG ( "height:" << outHeight << std::endl
                      << "width :" << outWidth << std::endl );
       
        WRITE_DEBUG ("Constructing source image: "
                     << "bits per sample: "<< m_imgIn.getBPS() 
                     << "samples per pixel: "<< m_imgIn.getSPP() << std::endl);
       
        // set what we've learned up in the new image object.
        m_imgOut = new (std::nothrow)ProjImageOut( 
                                      *m_params,
                                      m_projWriter,
                                      m_params->getFilename(),
                                      std::pair<long,long>(outHeight,outWidth),
                                      outputScale,
                                      m_imgIn.getPhotometric(), 
                                      m_imgIn.getBPS(), 
                                      m_imgIn.getSPP(),
                                      outputBounds );

        WRITE_DEBUG ( " done creating output" << std::endl );
        
        if (m_imgOut == NULL ) throw std::bad_alloc();

    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
    }

    return;
}

/*****************************************************************************/

GeneralProjector GeneralProjector::createFromSocket( ClientSocket & socket )
{
    PROJECTORTYPE ty = UNKNOWN; // from Globals.h
    int i = 0; // cannot send enumerated types?
    bool bOutImgExists = false;
    
    socket.receive(&i, sizeof(i));
    ty = static_cast<PROJECTORTYPE>(i);
    if ( ty != GEOPROJ ) 
        throw GeneralException("Attempt to create wrong projector.");
    
    ProjImageInInterface * imgIn = m_imgFactory.makeProjImageIn( socket );
    
    if ( imgIn == NULL ) 
        throw GeneralException("Input image creation failed.");

    WRITE_DEBUG ( "input creation succesful" << std::endl );
    socket.receive(&bOutImgExists, sizeof(bOutImgExists));

    if ( bOutImgExists )
    {
       WRITE_DEBUG ( "creating output image" << std::endl );
       ProjImageOutInterface * imgOut = m_imgFactory.makeProjImageOut( socket );
       if ( imgOut == NULL ) 
           throw GeneralException("Output image creation failed.");
       return GeneralProjector( *imgIn, *imgOut );
       
    } else
    {
        WRITE_DEBUG ( "creating parameter file" << std::endl );
        ProjImageParams * params = 
            new ProjImageParams(ProjImageParams::createFromSocket( socket ));
        if ( params == NULL )
            throw GeneralException("Output parms null on creation.");
        return GeneralProjector(*params, *imgIn);
    }
}

/*****************************************************************************/

void GeneralProjector::exportToSocket( ClientSocket & socket )const
{
    PROJECTORTYPE ty = GEOPROJ; // from Globals.h
    int i = static_cast<int>(ty) ; // cannot send enums through socket?
    bool bOutImgExists = (m_imgOut != NULL); 
    
    socket.send(&i, sizeof(i));
    m_imgIn.exportToSocket(socket);
    socket.send(&bOutImgExists, sizeof(bOutImgExists));
    
    if ( bOutImgExists ) 
        m_imgOut->exportToSocket(socket);
    else
    {
        if ( m_params == NULL )
            throw GeneralException("Params do not exist and need to.");
        m_params->exportToSocket(socket);
    }

    
    return;
}

/*****************************************************************************/

} // namespace 
