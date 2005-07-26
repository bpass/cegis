#ifndef _USGSMOSIX_PROJIMAGESCALE_H_
#define _USGSMOSIX_PROJIMAGESCALE_H_

/*! 
 * \author Mark Schisler 
 *
 * \date   $Date $
 *
 * \version 0.1
 * 
 * \file ImageScale.h
 *
 * \brief The ImageScale object is meant to encapsulate the comparison 
 * of different types of image scales. 
 *
 * \note This library is free software and is distributed under the 
 * MIT open source license.  For more information, consult the file 
 * COPYING.
 * 
 */

#include "Globals.h"
#include "Math.h"
#include <ProjectionLib/ProjectionTypes.h>
#include <utility>

namespace USGSMosix 
{
    using ProjLib::UNIT;
    
    class ProjImageScale
    {
        public:
            ProjImageScale()
                : x(0.0f), y(0.0f),
                  m_units(ProjLib::UNKNOWN_UNIT) {} 
           
            ProjImageScale(std::pair<scale_t, scale_t> xy)
                : x(xy.first), y(xy.second) {} 
            
            ProjImageScale( scale_t _x, 
                            scale_t _y,
                            UNIT unit )
                : x(_x), y(_y), m_units(unit) {}  
            
            scale_t x;
            scale_t y;
            
            UNIT getUnits() { return m_units; }     
            void setUnits( UNIT _unit) { m_units = _unit; }             
        private:
            UNIT m_units; // per pixel 
    };

} // namespace 

#endif // _USGSMOSIX_PROJIMAGESCALE_H_
