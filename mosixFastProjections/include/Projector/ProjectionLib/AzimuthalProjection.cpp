// $Id: AzimuthalProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <new>
#include <strstream>
#include "AzimuthalProjection.h"
#include "DatumConvertor.h"

// GCTPC includes
extern "C"
{
#include "gctpc/proj.h"
}

using namespace ProjLib;

// ***************************************************************************
AzimuthalProjection::AzimuthalProjection( double centralLongitude,
                                          double centralLatitude,
                                          double falseEasting,
                                          double falseNorthing,
                                          double sphereRadius, DATUM d, UNIT u,
                                          DATUM geoDatum, UNIT geoUnit )
  : Projection( d, u, geoDatum, geoUnit ), d_badDegreeRadius(4.0)
{
  // Initialize all the projection parameters to zero
  for ( int i = 0; i < 15; i++ )
  {
    d_projectionParameters[i] = 0.0;
  }

  setCentralLatitude( centralLatitude );
  setCentralLongitude( centralLongitude );
  setFalseEasting( falseEasting );
  setFalseNorthing( falseNorthing );
  setSphereRadius( sphereRadius );
}


// ***************************************************************************
AzimuthalProjection::AzimuthalProjection( const AzimuthalProjection& p )
  : Projection(p)
{
  if ( this != &p )
  {
    setCentralLatitude(p.getCentralLatitude() );
    setCentralLongitude(p.getCentralLongitude() );
    setFalseEasting(p.getFalseEasting() );
    setFalseNorthing(p.getFalseNorthing() );
    setSphereRadius(p.getSphereRadius() );
    d_badDegreeRadius = p.d_badDegreeRadius;
  }
}


// ***************************************************************************
bool AzimuthalProjection::operator==( const Projection& p ) const throw()
{
  const AzimuthalProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const AzimuthalProjection*>(&p);
    if ( NULL != pRHS )
    {
      if ( d_projectionParameters[5] == pRHS->getCentralLatitude()  &&
           d_projectionParameters[4] == pRHS->getCentralLongitude() &&
           d_projectionParameters[6] == pRHS->getFalseEasting()     &&
           d_projectionParameters[7] == pRHS->getFalseNorthing()    &&
           d_projectionParameters[0] == pRHS->getSphereRadius() )
      {
        return Projection::operator==(p);
      }
    }
  
    return false;
  }
  catch (...)
  {
    return false;
  }
}


// Projection functions
// ***************************************************************************
bool AzimuthalProjection::projectToGeo( double x, double y,
                                       	double& latitude,
                                        double& longitude ) const throw()
{
  double incoor[2];     // input coordinates (x, y)
  long   insys;         // input projection system
  long   inzone;        // input zone
  long   inunit;        // input units
  long   indatum;       // input spheroid code
  long   ipr;           // error message print flag
  char*  efile;         // file to contain output error messages
  long   jpr;           // projection parameter print flag
  char*  pfile;         // file to contain output projection parameter messages
  double outcoor[2];    // output coordinates (x, y)
  long   outsys;        // output projection system
  long   outzone;       // output zone
  double outparm[15];   // output parameter array
  long   outunit;       // output units
  long   outdatum;      // output spheroid code
  char*  fn27;          // file containing NAD 1927 state plane zones
  char*  fn83;          // file containing NAD 1983 state plane zones
  long   iflg;          // error flag

  try
  {
    // Variable initialization
    incoor[0] = x;
    incoor[1] = y;
    insys     = getProjectionSystem();
    inzone    = 0;
    inunit    = d_unit;
    indatum   = DatumConvertor::getDatumEllipsoid(d_datum);
    ipr       = -1;       // Do not print error messages
    efile     = NULL;     // No error file
    jpr       = -1;       // Do not print projection parameters
    pfile     = NULL;     // No projection parameter file
    outsys    = GEO;      // Output in geographic (lat, long)
    outzone   = 0;        // No zones for geographic
    outunit   = d_geoUnit;
    outdatum  = DatumConvertor::getDatumEllipsoid(d_geoDatum);
    fn27      = NULL;     // No NAD27 State Plane parameter file
    fn83      = NULL;     // No NAD83 State Plane parameter file
  
    // Set all the output projection parameters to zero
    for ( int i = 0; i < 15; i++ )
    {
      outparm[i] = 0.0;
    }

    // Do transformation
    gctp( incoor, &insys, &inzone, const_cast<double*>(d_projectionParameters),
          &inunit, &indatum, &ipr, efile, &jpr, pfile, outcoor, &outsys,
          &outzone,  outparm, &outunit, &indatum, fn27, fn83, &iflg );

    // Set latitude and longitude
    longitude = outcoor[0];
    latitude  = outcoor[1];

    // Convert the datums
    return convertDatum( latitude, longitude, d_datum, d_geoDatum);
  }
  catch(...)
  {
    return false;
  }
}


// ***************************************************************************
bool AzimuthalProjection::projectFromGeo( double latitude, double longitude,
					  double& x, double& y ) const throw()
{

  double incoor[2];     // input coordinates (x, y)
  long   insys;         // input projection system
  long   inzone;        // input zone
  long   inunit;        // input units
  long   indatum;       // input spheroid code
  double inparm[15];    // input parameter array
  long   ipr;           // error message print flag
  char*  efile;         // file to contain output error messages
  long   jpr;           // projection parameter print flag
  char*  pfile;         // file to contain output projection parameter messages
  double outcoor[2];    // output coordinates (x, y)
  long   outsys;        // output projection system
  long   outzone;       // output zone
  long   outunit;       // output units
  long   outdatum;      // output spheroid code
  char*  fn27;          // file containing NAD 1927 state plane zones
  char*  fn83;          // file containing NAD 1983 state plane zones
  long   iflg;          // error flag


  try
  {
    // Convert the datums
    if ( !convertDatum( latitude, longitude, d_geoDatum, d_datum) )
    {
      return false;
    }

    // Variable initialization
    incoor[0] = longitude;
    incoor[1] = latitude;
    insys     = GEO;
    inzone    = 0;
    inunit    = d_geoUnit;
    indatum   = DatumConvertor::getDatumEllipsoid(d_geoDatum);
    ipr       = -1;          // Do not print error messages
    efile     = NULL;        // No error file
    jpr       = -1;          // Do not print projection parameters
    pfile     = NULL;        // No projection parameter file
    outsys    = getProjectionSystem();
    outzone   = 0;
    outunit   = d_unit;
    outdatum  = DatumConvertor::getDatumEllipsoid(d_datum);
    fn27      = NULL;        // No NAD27 State Plane parameter file
    fn83      = NULL;        // No NAD83 State Plane parameter file

    // Initialize the projection parameter arrays to all zeroes
    for ( int i = 0; i < 15; i++ )
    {
      inparm[i] = 0.0;
    }
    
    // Do transformation
    gctp( incoor, &insys, &inzone, inparm, &inunit, &outdatum, 
          &ipr, efile, &jpr, pfile, outcoor, &outsys, &outzone, 
          const_cast<double*>(d_projectionParameters), &outunit, 
          &outdatum, fn27, fn83, &iflg );
  
    // Set the coordinates
    x = outcoor[0];
    y = outcoor[1];
    
    return true;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
std::string AzimuthalProjection::toString() const throw()
{
  std::string    ret;
  std::strstream tempstream;
  
  try
  {
    tempstream << Projection::toString() << std::endl
               << std::string("CENTRAL LATITUDE: ")
               << packedDMSToString(d_projectionParameters[5], true)
               << std::endl
               << std::string("CENTRAL LONGITUDE: ")
               << packedDMSToString(d_projectionParameters[4], false )
               << std::endl
               << std::string("FALSE EASTING: ") << d_projectionParameters[6]
               << std::string("FALSE NORTHING: ") << d_projectionParameters[7]
               << std::endl << std::string("SPHERE RADIUS: ")
               << d_projectionParameters[0] << std::ends;

    ret = tempstream.str();
    tempstream.freeze(false);
    return ret;
  }
  catch (...)
  {
    return std::string("");
  }
}


// ***************************************************************************
bool AzimuthalProjection::checkCoordinateRange( double latitude, 
                                                double longitude ) 
  const throw()
{
  double badLat, badLon;
  double top, bottom, left, right;

  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }

    // The out of range areas will be on the opposite side of the globe from
    // the center of the projection
    badLat = -packedDMSToDouble( d_projectionParameters[5] );
    badLon = packedDMSToDouble( d_projectionParameters[4]) + 180;

    if ( badLon > 180.0 ) 
      badLon -= 360.0;

    // The point is out of range if the point is within the bad degree radius 
    // of the bad center
    top    = badLat + d_badDegreeRadius;
    bottom = badLat - d_badDegreeRadius;
    left   = badLon - d_badDegreeRadius;
    right  = badLon + d_badDegreeRadius;
    
    if ( ( latitude > bottom && latitude < top ) &&
         ( longitude > left && longitude < right ) )
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  catch (...)
  {
    return false;
  }
}
