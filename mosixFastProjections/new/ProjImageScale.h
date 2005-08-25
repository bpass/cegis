#ifndef _USGSMOSIX_PROJIMAGESCALE_H_
#define _USGSMOSIX_PROJIMAGESCALE_H_

/*! 
 * \author Mark Schisler 
 *
 * \date   $Date $
 *
 * \version 0.1
 * 
 * \file ProjImageScale.h
 *
 * \brief The ImageScale object is meant to encapsulate the comparison 
 * of different types of image scales. 
 *
 * \note This library is free software and is distributed under the 
 * MIT open source license.  For more information, consult the file 
 * COPYING.
 * 
 */

#include <ProjectionLib/ProjectionTypes.h>
#include <utility>
#include "Globals.h"
#include "Math.h"
#include "SerializableInterface.h"
#include "ClientSocket.h"

namespace USGSMosix 
{
    using ProjLib::UNIT;
    
    class ProjImageScale : public SerializableInterface {
        public:


            ProjImageScale() : x(0.0f), y(0.0f), m_units(ProjLib::UNKNOWN_UNIT)             {
            } 
          
            /// \param xy A pair representing the scale in the horizontal 
            /// and the vertical direction respectively.  Scale being 
            /// the pixel / unit ratio.
            /// \note In this constructor the units are net set.
            ProjImageScale(std::pair<scale_t, scale_t> xy) : x(xy.first), 
                y(xy.second), m_units(ProjLib::UNKNOWN_UNIT)
            {
            } 
            
            /// \param _x The pixel/unit ratio in the horizontal direction.
            /// \param _y The pixel/unit ratio in the vertical direction.
            /// \param unit The units this ratio has been formulated in.
            ProjImageScale( scale_t _x, scale_t _y, UNIT unit ) : x(_x), y(_y), 
                m_units(unit)             
            {
            }   
           
            /// \param socket The socket which has data waiting on it from 
            /// a ProjImageScale object's call to exportToSocket().
            /// \brief Constructs a copy of the object which called 
            /// exportToSocket(). 
            static ProjImageScale createFromSocket( ClientSocket & socket ) 
            {
                scale_t _x, _y;
                UNIT units; 
                
                socket.receive( &_x, sizeof(_x) );
                socket.receive( &_y, sizeof(_y) );
                socket.receive( &units, sizeof(units) );
                
                return ProjImageScale( _x, _y, units );
            }
            
            /// \param socket A socket to which pertinent data should be
            /// outputted for the purposes of creating a copy of the 
            /// current object.
            /// \brief Used to serialize the current object over a socket.
            void exportToSocket( ClientSocket& socket ) const
            {
                socket.appendToBuffer( &x, sizeof(x) );
                socket.appendToBuffer( &y, sizeof(y) );
                socket.appendToBuffer( &m_units, sizeof(m_units) );
                socket.sendFromBuffer();

                return;
            }
            
            // scale in the horizontal direction.
            scale_t x;
            
            // scale in the vertical direction.
            scale_t y;
           
            /// \brief Returns the units that this scale is ini.
            UNIT getUnits() { return m_units; }     
            
            /// \brief Sets the units per pixel.
            void setUnits( UNIT _unit ) { m_units = _unit; }             
            
        private:

            // the units per pixel
            UNIT m_units; 
    };

} // namespace 

#endif // _USGSMOSIX_PROJIMAGESCALE_H_
