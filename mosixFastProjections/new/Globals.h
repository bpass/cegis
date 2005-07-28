/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file Globals.h
 * 
 * \brief Global constants, enumerations, and typedefs used by  
 * the MOSIX fast projections project.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#ifndef _USGSMOSIX_GLOBALS
#define _USGSMOSIX_GLOBALS

#include <MathLib/InterpolatorTypes.h> 
#include <ProjectionLib/ProjectionTypes.h>

namespace USGSMosix
{

///////////////////////////////
// typedefs 
///////////////////////////////

typedef unsigned char sample_t; 
    
typedef unsigned char * scanline_t;

typedef const unsigned char * const_scanline_t;

typedef unsigned char ** scanlines_t;

typedef double scale_t;
  
//////////////////////////////
// global constants 
//////////////////////////////

/*! Cache size for LRU cache */
const unsigned int kgCacheSize = 100; 
   
/*! Equality Tolerance for some floating point operations */
const double kgEqTol = 0.001f;

/*! Default divisions in the width, in the height for the projection
 *  mesh that is created.  i.e., if kgMeshDivisions is 4, then the
 *  number of distinct areas in the mesh would be 16. */
const unsigned int kgMeshDivisions = 6; 

/*! Default type of interpolation that will be used for the MathLib. */
const MathLib::InterpolatorType kgInterpolator = MathLib::CubicSpline;

//////////////////////////////
// enumerations 
//////////////////////////////

/*! enumeration of the types of images supported */
enum IMAGETYPE
{
    DOQ = 0, JPEG, TIFF, GEOTIFF, PNG, OTHER = -1
};

} // namespace USGSMosix 

#endif
