// $Id: DatumConvertor.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _DATUMCONVERTOR_H_
#define _DATUMCONVERTOR_H_

#include "ProjectionTypes.h"
#include "ProjectionException.h"

namespace ProjLib
{

class DatumConvertor
{
 public:
  static bool convertDatum( double& lat, double& lon, DATUM from, DATUM to )
    throw();
  static ELLIPSOID getDatumEllipsoid( DATUM d ) throw();

 private:
  // Datum info type
  struct DatumParameters
  {
    DATUM     d_datum;
    ELLIPSOID d_ellipsoid;
    double    d_deltaX;
    double    d_deltaY;
    double    d_deltaZ;
  };

  // Ellipsoid info type
  struct EllipsoidParameters
  {
    ELLIPSOID d_ellipsoid;
    double    d_deltaA; // equatorial radius shift from WGS84
    double    d_deltaF; // flattening shift from WGS84
  };

  static bool convertToWGS84( double& lat, double& lon, double& height,
                              DATUM from ) throw();
  static bool convertFromWGS84( double& lat, double& lon, double& height,
                                DATUM to ) throw();

  static DatumParameters*     getDatumParameters( DATUM d )        throw();
  static EllipsoidParameters* getEllipsoidParameters( ELLIPSOID e ) throw();
  /*static bool convertToECEF( double& lat, double& lon, double& height, DATUM from );
    static bool convertFromECEF( double& lat, double& lon, double& height, DATUM to );*/

  // Conversion parameters array
  static DatumParameters     d_datumParams[];
  static EllipsoidParameters d_ellipsoidParams[];
};

} // namespace ProjLib
#endif
