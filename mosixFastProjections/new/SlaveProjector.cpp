/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file SlaveProjector.cpp
 * 
 * \brief Built as a wrapper for an FromMultiGeoProjector object,
 * this class will redirect all output from it usual file ouput, 
 * to being network controlled --i.e., all output will be
 * directed toward a socket via a forwarded descriptor.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "SlaveProjector.h"
#include "FromMultiGeoProjector.h"

namespace USGSMosix {

// initialize static members
/******************************************************************************/

long int SlaveProjector::m_id(0);
ProjectorFactory SlaveProjector::m_projFactory;

// member functions
/******************************************************************************/

SlaveProjector::SlaveProjector( 
ProjImageInInterface & inInterface,
ProjImageOutInterface & imgOutInterface,
std::pair<unsigned long, unsigned long> scanlineRange  ) : 

m_projInterface( new FromMultiGeoProjector(inInterface, imgOutInterface)),
m_scanlineRange( scanlineRange ),
m_noScanlines( 0 ),
m_scanlines( NULL )
{
    ++m_id;     
}

/******************************************************************************/

SlaveProjector::SlaveProjector( ProjectorInterface & interface,
std::pair<unsigned long, unsigned long> scanlineRange,
scanlines_t scanlines ) :

m_projInterface( &interface ),
m_scanlineRange( scanlineRange ),
m_noScanlines( 0 ),
m_scanlines( scanlines )
   
{
    ++m_id; 
}

/******************************************************************************/

SlaveProjector::~SlaveProjector()
{
    delete m_projInterface;    
    cleanupScanlines();
}


/******************************************************************************/

const ProjLib::Projection * SlaveProjector::getOutputProjection()const
{
    return m_projInterface->getOutputProjection(); 
}

/******************************************************************************/

bool SlaveProjector::setupOutput()
{
    return m_projInterface->setupOutput();
}

/******************************************************************************/

scanlines_t SlaveProjector::project( long unsigned int beginLine, 
                                     long unsigned int endLine )
{
    return  m_projInterface->project(beginLine,endLine);
}

/******************************************************************************/

void SlaveProjector::project()
{
    cleanupScanlines();
    
    m_noScanlines = m_scanlineRange.first - m_scanlineRange.second;  
    
    if ( setupOutput() ) 
    {
        // get scanline data
        m_scanlines = project(m_scanlineRange.first,m_scanlineRange.second);

    } else
        throw GeneralException("m_imgOut is NULL");

    return;
}

/******************************************************************************/

void SlaveProjector::cleanupScanlines()
{
    m_noScanlines = 0;
    
    if ( m_scanlines != NULL ) 
    {
        // cleanup space for scanlines    
        for( unsigned long i = 0; i < m_noScanlines; ++i ) 
            delete m_scanlines[i];
        delete [] m_scanlines;
        m_scanlines = NULL;
    }
    return;
}

/******************************************************************************/

void SlaveProjector::exportToSocket( ClientSocket & client ) const
{
    static PROJECTORTYPE ty = SLAVEPROJ; /// from Globals.h
    int spp = this->getProjImageOut()->getSPP();
    unsigned long int width = this->getProjImageOut()->getWidth();
    unsigned long int height = this->getProjImageOut()->getHeight();
    unsigned char * blankscanline = NULL; 
    bool bSendScanlines = false;
   
    // sync our immediate data members
    client.appendToBuffer(&ty, sizeof(ty));
    client.appendToBuffer(&m_scanlineRange.first,sizeof(m_scanlineRange.first));
    client.appendToBuffer( &m_scanlineRange.second,
                           sizeof(m_scanlineRange.first));
    client.sendFromBuffer();

    // sync interface members
    m_projInterface->exportToSocket(client); 
  
    // sync resulting scanlines if they are there.
    bSendScanlines = (m_scanlines != NULL );
    client.send(&bSendScanlines, sizeof(bSendScanlines));
    
    if ( bSendScanlines )
    {
        if ( !(blankscanline = new(std::nothrow) unsigned char[ width*spp ]))
            throw GeneralException("Dynamic Alloc' failed.");
            
        for( unsigned long int h = 0; h < height; ++h )
        {
            if ( m_scanlines[h] != NULL ) 
                client.appendToBuffer( m_scanlines[h], spp * width ); 
            else
                client.appendToBuffer( blankscanline, spp * width ); 
        }
        
        client.sendFromBuffer();
        delete[] blankscanline;
    }

}

/******************************************************************************/

SlaveProjector SlaveProjector::createFromSocket( ClientSocket & client )
{
    PROJECTORTYPE ty = UNKNOWN;
    std::pair<long unsigned int, long unsigned int> range(0,0);
    ProjectorInterface * interface = NULL;
    bool bAcceptScanlines = false;
    scanlines_t scanlines = NULL;
    unsigned long int width(0), height(0);
    int spp(0);

    // sync our immediate data members
    client.receive(&ty, sizeof(ty));
    if ( ty != SLAVEPROJ ) 
        throw GeneralException(
        "SlaveProjector cannot create different projector!");
    
    client.receive(&range.first,sizeof(range.first));
    client.receive(&range.second, sizeof(range.second));
    
    // sync interface members
    interface  = m_projFactory.makeProjector(client);

    if ( interface == NULL )
        throw GeneralException("Creation of projector failed.");
    
    width = interface->getProjImageOut()->getWidth();
    height = interface->getProjImageOut()->getHeight();
    spp = interface->getProjImageOut()->getSPP();
  
    // sync resulting scanlines if they are there.
    client.receive(&bAcceptScanlines, sizeof(bAcceptScanlines));
    if ( bAcceptScanlines )
    {
        // allocate memory for scanlines
        scanlines = allocScanlines (
                    std::pair<unsigned long, unsigned long>(height,width),spp); 
        
        const unsigned int height = range.second - range.first;
        for ( unsigned int i = 0; i < height; ++i )
        {
            client.receive(scanlines[i] ,spp * width ); 
        }
    }
    
    return SlaveProjector(*interface, range, scanlines );
}

/******************************************************************************/

scanlines_t 
SlaveProjector::allocScanlines(std::pair<unsigned long, unsigned long> hwPixels,
                               int spp )
{
    scanlines_t scanlines;

    if ( !(scanlines = new(std::nothrow) scanline_t[hwPixels.first] ) )
        throw GeneralException("Dynamic Alloc' failed.");

    for( unsigned int i = 0; i < hwPixels.first; ++i )
    {
        if ( !(scanlines[i] = new(std::nothrow) sample_t[hwPixels.second*spp]))
            throw GeneralException("Dynamic Alloc' failed.");
    }
    
    return scanlines;
}


} // namespace
