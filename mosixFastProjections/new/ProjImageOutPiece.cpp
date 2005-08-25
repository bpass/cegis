
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ProjImageOutPiece.cpp
 * 
 * \brief Defines a piece of a ProjImage.  This is useful in the  
 * MOSIX fast re-projections project as pieces of output images
 * frequently need to be transported across the network.  
 * IMPORTANT: For simplicity this piece is assumed to be contiguous.
 * Therefore one cannot have a piece of a image which contains 
 * the lines 1, 2, 132134, and 33 or something similiar.  It 
 * may only have pieces which contain ranges of scanlines.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "ProjImageOutPiece.h"
#include "GeneralException.h"

namespace USGSMosix {

/******************************************************************************/

ProjImageOutPiece::ProjImageOutPiece()
    : m_scanlines(NULL),
      m_range(-1,-1),
      m_width( 0 ),
      m_spp( 1 )
{
    
}

/******************************************************************************/

ProjImageOutPiece::ProjImageOutPiece( scanlines_t scanlines, 
                                      std::pair<long, long> range,
                                      unsigned long int width,
                                      int spp )
    : m_scanlines ( scanlines ),
      m_range( range ),
      m_width( width ),
      m_spp ( spp )
{
}

/******************************************************************************/

ProjImageOutPiece ProjImageOutPiece::createFromSocket( ClientSocket & socket)
{
    const unsigned int height = range.second - range.first;
    std::pair<long unsigned int, long unsigned int> range(0,0);
    bool bAcceptScanlines = false;
    scanlines_t scanlines = NULL;
    unsigned long int width(0);
    int spp(0);

    client.receive(&range.first,sizeof(range.first));
    client.receive(&range.second, sizeof(range.second));
    client.receive(&width, sizeof(width));
    client.receive(&spp, sizeof(spp));
  
    // sync resulting scanlines if they are there.
    client.receive(&bAcceptScanlines, sizeof(bAcceptScanlines));
    if ( bAcceptScanlines )
    {
        // allocate memory for scanlines
        scanlines = allocScanlines (
                    std::pair<unsigned long, unsigned long>(height,width),spp); 
        
        for ( unsigned int i = 0; i < height; ++i )
        {
            client.receive(scanlines[i] ,spp * width ); 
        }
    }
    
    return ProjImageOutPiece(scanlines, range, width, spp); 
}

/******************************************************************************/

void ProjImageOutPiece::exportToSocket( ClientSocket & socket )const
{
    const unsigned int height = m_range.second - m_range.first; 
    bool bSendScanlines = (m_scanlines == NULL);
    
    socket.appendToBuffer(&m_range.first, sizeof(m_range.first));
    socket.appendToBuffer(&m_range.second, sizeof(m_range.second));
    socket.appendToBuffer(&bSendScanlines, sizeof(bSendScanlines));
    socket.appendToBuffer(&m_width, sizeof(m_width));
    socket.appendToBuffer(&m_spp, sizeof(m_spp));
    
    if ( bSendScalines )
    {
        if ( !(blankscanline = new(std::nothrow) unsigned char[ width*spp ]))
            throw GeneralException("Dynamic Alloc' failed.");
            
        for( unsigned long int h = 0; h < height; ++h )
        {
            if ( m_scanlines[h] != NULL ) 
                socket.appendToBuffer( m_scanlines[h], spp * width ); 
            else
                client.appendToBuffer( blankscanline, spp * width ); 
        }
        delete[] blankscanline;
    }
    
    socket.sendFromBuffer();
}


/******************************************************************************/

void ProjImageOutPiece::putScanline( scanline_t scanline, 
                                     const unsigned int& lineNo )
{
    if ( (m_range.first != -1 ) &&
         lineNo - 1 != m_range.first ||
         lineNo + 1 != m_range.first ||
         lineNo - 1 != m_range.second ||
         lineNo + 1 != m_range.second )
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
 
scanlines_t ProjImageOut::allocScanlines(
    std::pair<unsigned long, unsigned long> hwPixels, int spp )
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

/****************************************************************************/

} // namespace

