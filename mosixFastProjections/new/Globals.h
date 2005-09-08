
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
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

#ifndef _USGSMOSIX_GLOBALS_H_
#define _USGSMOSIX_GLOBALS_H_

#include "Macros.h"
#include <MathLib/InterpolatorTypes.h> 
#include <ProjectionLib/ProjectionTypes.h>

namespace USGSMosix
{

///////////////////////////////
// typedefs 
///////////////////////////////

/// The datatype used for holding samples for images. 
typedef unsigned char sample_t; 

/// The datatype used for holding samples for images. 
typedef unsigned char * scanline_t;

/// The datatype used for holding samples for images. 
typedef const unsigned char * const_scanline_t;

/// The datatype used for holding samples for images. 
typedef unsigned char ** scanlines_t;

/// The datatype used for holding samples for images. 
typedef unsigned char *const* const_scanlines_t;

/// The datatype used for holding image scale factors. 
typedef double scale_t;
  
//////////////////////////////
// global constants 
//////////////////////////////

/// The number of alloted retry attempts for connecting to a socket. 
const unsigned int kgConnectRetries = 5;

/// the number of seconds that will elapse between connect retries between
/// sockets 
const unsigned int kgReconnectDelaySecs = 2;

/// Maximum backlogged connections to the server. 
const unsigned int kgMaxBackloggedSlaves = 20;

/// maximum size for the SocketWrapper's buffer. 
const unsigned int kgSocketBuffSize = 1000;

/// Cache size for LRU cache 
const unsigned int kgCacheSize = 2048; 
   
/// Equality Tolerance for some floating point operations 
const double kgEqTol = 0.001f;

/// Default divisions in the width, in the height for the projection
/// mesh that is created.  i.e., if kgMeshDivisions is 4, then the
/// number of distinct areas in the mesh would be 16. 
const unsigned int kgMeshDivisions = 6; 

/// Default type of interpolation that will be used for the MathLib. 
const MathLib::InterpolatorType kgInterpolator = MathLib::CubicSpline;

//////////////////////////////
// enumerations 
//////////////////////////////

/// enumeration of the types of images supported 
enum IMAGETYPE
{
    DOQ = 0, JPEG, TIFF, GEOTIFF, PNG, OTHER = -1
};

/// An enumerated type for defining a projector type. 
enum PROJECTORTYPE 
{
    GEOPROJ = 0, SLAVEPROJ, UNKNOWN = -1
};

} // namespace USGSMosix 

#endif
