#ifndef _IMAGEPROJECTION_H_
#define _IMAGEPROJECTION_H_
// $Id: ImageProjection.h,v 1.1 2005/03/11 23:59:08 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:08 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// This is simply an enumerated list of different projection types, mainly for
// the GeoTIFFImageIFile classes.

namespace USGSImageLib
{
  // These define pretty much everything available through GeoTIFF.  If I 
  // missed something, just add it :)

enum HORIZPROJSYSTEMS
{
  GEOGRAPHIC = 0, UTM, STATEPLANE, MERCATOR, TRANSVERSEMERCATOR,
  OBLIQUEMERCATOR, CYLINDRICAL, MILLERCYLINDRICAL, EQUIDISTANTCYLINDRICAL,
  CASSINI, ALBERS, LAMBERTCONFORMAL, EQUIDISTANT, BIPOLAROBLIQUE,
  POLYCONIC, BONNE, ORTHOGRAPHIC, STEREOGRAPHIC, GNOMONIC, GENERALPERSPECTIVE,
  VERTICALPERSPECTIVE, TILTEDPERSPECTIVE, LAMBERTEQUALAREA, 
  EQIDISTANTAZIMUTHAL, MODIFIEDSTEREOGRAPHICCONFORMAL, SPACEOBLIQUEMERCATOR,
  VANDERGRINTEN, SINUSOIDAL, MOLLWEIDE, ECKERTIV, ECKERTVI, USERDEFINEDPROJSYS,
  UNDEFINEDPROJSYS
};

enum DATUMS
{
  USERDATUM, UNKNOWNDATUM
};

enum ELLIPSOIDS
{
  USERELLIPSOID, UNKNOWNELLIPSOID
};

enum UNITS
{
  USERUNITS, UNKNOWNUNITS
};

} // namespace

#endif
