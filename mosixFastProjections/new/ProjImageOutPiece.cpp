
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/20 23:21:15 $
 *
 * \version 0.1
 * 
 * \file ProjImageOutPiece.cpp
 * 
 * \brief Implementation file for ProjImageOutPiece class.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Macros.h"
#include <iostream>
#include "ProjImageOutPiece.h"
#include "GeneralException.h"

namespace USGSMosix {

/******************************************************************************/

ProjImageOutPiece::ProjImageOutPiece()
    : m_scanlines(NULL),
      m_widthPixels( 0 ),
      m_spp( 1 ),
      m_range(-1, -1),
      m_id(0)
          
{
    
}

/******************************************************************************/

ProjImageOutPiece::ProjImageOutPiece( scanlines_t scanlines, 
                                      std::pair<long, long> range,
                                      unsigned long int width,
                                      int spp,
                                      unsigned long int id )
    : m_scanlines ( scanlines ),
      m_widthPixels( width ),
      m_spp ( spp ),
      m_range( range ),
      m_id(id)
{   
    WRITE_DEBUG ( "Creating ProjImageOutPiece with ID : " << m_id << std::endl );
    WRITE_DEBUG ( "Has spp : " << m_spp << std::endl );
}

/******************************************************************************/

ProjImageOutPiece::ProjImageOutPiece( const ProjImageOutPiece & copyOf )
    : ProjImageDataOutInterface(),
      SerializableInterface(),
      m_widthPixels( copyOf.m_widthPixels ),
      m_spp ( copyOf.m_spp ),
      m_range ( copyOf.m_range ),
      m_id ( copyOf.m_id )
{
    WRITE_DEBUG ( "CALLED ProjImageOut Copy Constructor" << std::endl );
    WRITE_DEBUG ( "Has spp : " << m_spp << std::endl );
    const unsigned int height = m_range.second - m_range.first;
    m_scanlines = allocScanlines(std::pair<unsigned long, unsigned long>
                                 (height,m_widthPixels),m_spp); 
    
    for( unsigned int i = 0; i < height; ++i )
    {
        for ( unsigned int j = 0; j < (m_widthPixels * m_spp ); ++j )
        {
            m_scanlines[i][j] = copyOf.m_scanlines[i][j];
        }
    }

}

/******************************************************************************/

ProjImageOutPiece::~ProjImageOutPiece()
{
    cleanupScanlines();
}

/******************************************************************************/

ProjImageOutPiece ProjImageOutPiece::createFromSocket( ClientSocket & client )
{
    bool bAcceptScanlines = false;
    scanlines_t scanlines = NULL;
    std::pair<long, long> range(0,0); 
    unsigned long int width(0);
    int spp(0);
    unsigned long int id(0);
    
    client.receive(&id, sizeof(id));
    WRITE_DEBUG ( "getting ID" << id << std::endl );
    client.receive(&range.first,sizeof(range.first));
    WRITE_DEBUG ( "begin range: " << range.first << std::endl );
    client.receive(&range.second, sizeof(range.second));
    WRITE_DEBUG ( "end range: " << range.second << std::endl );
    client.receive(&width, sizeof(width));
    WRITE_DEBUG ( "width:" << width << std::endl );
    client.receive(&spp, sizeof(spp));
    WRITE_DEBUG ( "spp:" << spp << std::endl );
    // sync resulting scanlines if they are there.
    client.receive(&bAcceptScanlines, sizeof(bAcceptScanlines));
    WRITE_DEBUG ( "accept scanlines" << bAcceptScanlines << std::endl );
    if ( bAcceptScanlines )
    {
        const unsigned int height = range.second - range.first;
        const size_t rcvSize = spp * width * sizeof(sample_t);
        WRITE_DEBUG ( "send size" << rcvSize << std::endl; )
        // allocate memory for scanlines
        scanlines = allocScanlines (
                    std::pair<unsigned long, unsigned long>(height,width),spp); 
        
        for ( unsigned int i = 0; i < height; ++i )
        {
            client.receive(scanlines[i] , rcvSize); 
        }
        WRITE_DEBUG ( "Received " << height << " scanlines." );
    } else
        WRITE_DEBUG ( "no scanlines received." << std::endl );
   
   
    return ProjImageOutPiece(scanlines, range, width, spp, id); 
}

/******************************************************************************/

void ProjImageOutPiece::exportToSocket( ClientSocket & socket )const
{
    const unsigned int height = m_range.second - m_range.first; 
    bool bSendScanlines = (m_scanlines != NULL);
    scanline_t blankscanline = NULL;
   
    socket.appendToBuffer(&m_id, sizeof(m_id));
    WRITE_DEBUG ( "sending ID" << m_id << std::endl );
    socket.appendToBuffer(&m_range.first, sizeof(m_range.first));
    WRITE_DEBUG ( "sending begin range: " << m_range.first << std::endl );
    socket.appendToBuffer(&m_range.second, sizeof(m_range.second));
    WRITE_DEBUG ( "sending end range: " << m_range.second << std::endl );
    socket.appendToBuffer(&m_widthPixels, sizeof(m_widthPixels));
    WRITE_DEBUG ( "sending width (pixels): " << m_widthPixels << std::endl );
    socket.appendToBuffer(&m_spp, sizeof(m_spp));
    WRITE_DEBUG ( "sending samples per pixel: " << m_spp << std::endl );
    socket.appendToBuffer(&bSendScanlines, sizeof(bSendScanlines));
    WRITE_DEBUG ( "sending scanline flag: " << bSendScanlines << std::endl );
    socket.sendFromBuffer();
    if ( bSendScanlines )
    {
        const size_t sendSize = m_spp * m_widthPixels * sizeof(sample_t);
       
        WRITE_DEBUG ( "send size" << sendSize << std::endl; )
        
        WRITE_DEBUG ( "sending scanlines" << std::endl );
        if ( !(blankscanline = new(std::nothrow)sample_t[m_widthPixels*m_spp]))
            throw GeneralException("Dynamic Alloc' failed.");
            
        for( unsigned long int h = 0; h < height; ++h )
        {
            if ( m_scanlines[h] != NULL ) 
                socket.send( m_scanlines[h], sendSize ); 
            else
            {
                WRITE_DEBUG ( "sending blankline :" << h << std::endl; )
                socket.send( blankscanline, sendSize ); 
                                     
            }
        }
        delete[] blankscanline;
    }
}


/******************************************************************************/

void ProjImageOutPiece::putScanline( scanline_t, 
                                     const unsigned int& lineNo )
{
    if ( (m_range.first != -1 ) &&
         static_cast<int>(lineNo + 1) != m_range.first ||
         static_cast<int>(lineNo - 1) != m_range.second )
        throw GeneralException(
        "Error: Inserting line which would make ProjImageOut non-contiguous.");

    // TODO.....
}

/******************************************************************************/

void ProjImageOutPiece::putScanlines( scanlines_t scanlines, 
                                      const unsigned int& beginLine,
                                      const unsigned int& endLine )
{
    // TODO... make be able to accept lines and make a contiguous 
    // segment.
    
    m_range.first = beginLine; 
    m_range.second = endLine; 
    m_scanlines = scanlines;
}

/******************************************************************************/
 
scanlines_t ProjImageOutPiece::allocScanlines(
    std::pair<unsigned long, unsigned long> hwPixels, int spp )
{
    scanlines_t scanlines;
    const size_t makeSize = hwPixels.second * spp; 
    if ( !(scanlines = new(std::nothrow) scanline_t[hwPixels.first] ) )
        throw GeneralException("Dynamic Alloc' failed.");

    WRITE_DEBUG ( "making this many scanlines: " << hwPixels.first << std::endl; )
    WRITE_DEBUG ( "with this width: " << makeSize << std::endl; )
    for( unsigned int i = 0; i < hwPixels.first; ++i )
    {
        if ( !(scanlines[i] = new(std::nothrow) sample_t[makeSize]))
            throw GeneralException("Dynamic Alloc' failed.");
    }
    return scanlines;
}

/****************************************************************************/

void ProjImageOutPiece::cleanupScanlines()
{
    const long int h = m_range.second - m_range.first;
    if ( m_scanlines != NULL )
    {
        for ( long int i = 0; i < h; ++i )
        {
            if ( m_scanlines[i] != NULL )
            {
                delete [] m_scanlines[i];
                m_scanlines[i] = NULL;
            }
        }
        delete [] m_scanlines;
        m_scanlines = NULL;
    }
}

/****************************************************************************/

} // namespace

