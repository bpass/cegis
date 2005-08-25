#ifndef __USGSMOSIX_PROJIMAGEOUTPIECE_H_
#define __USGSMOSIX_PROJIMAGEOUTPIECE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ProjImageOutPiece.h
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

namespace USGSMosix {

    class ProjImageOutPiece : public ProjImageDataOutInterface, 
                              public SerializableInterface  
    {
        public: 
            ProjImageOutPiece();
            
            /// \param scanlines The scanlines for the image.
            /// \param range The range of the scanlines forwarded to 
            /// the image.  (zero based.)
            ProjImageOutPiece( scanlines_t scanlines, 
                               std::pair<long, long> range ); 
           
            virtual ~ProjImageOutPiece() {}
            
            /// \param socket The socket which has information waiting on it
            /// which can construct a ProjImageOutPiece.
            ///
            /// \brief Constructs a ProjImageOutPiece from information waiting
            /// on a socket that was placed there by a call on another
            /// existing object to exportToSocket().  A copy of that object
            /// will be returned from this function.
            static ProjImageOutPiece createFromSocket( ClientSocket & socket);
            
            /// \param socket The socket to which all information needed to
            /// reconstruct this object will be outputted to.
            ///
            /// \brief Puts information out on a socket so that it can 
            /// be grabbed by the createFromSocket() funciton for the purposes
            /// of recreating the exact same state in another object.
            virtual void exportToSocket( ClientSocket & socket)const;
            
            /// \param scanline A pointer to a scanline.  
            /// \param lineNo The line number (zero based) of the forwarded
            /// line.  This line number must be within one of the begining
            /// and end ranges currently used for the object.  Duplicate
            /// lines will also not be accepted.
            /// \brief Adds data for a scanline to the ProjImageOutPiece.
            virtual void putScanline( scanline_t scanline, 
                                      const unsigned int& lineNo );

            /// \param scanline A pointer to a group of scanlines.  
            /// \param beginLine The line number of the begining of the
            /// range of lines that was sent.
            /// \param endLine The line number of the end of the range of lines
            /// that was sent.  
            /// \note beginLine and endLine must be within one of the begining
            /// and end ranges currently used for the object.  Duplicate
            /// lines will also not be accepted.
            /// \brief Adds data for a scanline to the ProjImageOutPiece.
            virtual void putScanlines( scanlines_t scanlines, 
                                       const unsigned int& beginLine,
                                       const unsigned int& endLine ); 
           
            /// \brief Returns the range of scanlines kept in this Piece
            /// of an output image.
            std::pair<unsigned long, unsigned long> getRange()const 
            {
                return m_range;
            } 
            
        private:
            /// \brief allocates dynamic memory for a group of scanlines,
            /// with a certain sample per pixel count, based off the size
            /// of sample_t defined in Globals.h
            static scanlines_t allocScanlines( 
                std::pair<unsigned long, unsigned long> hwPixels, int spp );

            /// The width of the scanlines contained in this piece.
            unsigned long int m_widthPixels;
            
            /// Samples per pixel for this piece of image.
            int m_spp;

            /// The scanline data.
            scanlines_t m_scanlines;

            /// the range of scanlines currently in this piece.
            std::pair<long, long> m_range;
    };



} // namespace 



#endif // __USGSMOSIX_PROJIMAGEOUTPIECE_H_
