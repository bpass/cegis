// $Id: proj15.cpp,v 1.2 2005/01/06 19:55:35 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for using projection number to obtain param   //
//       names. For use with projInfo .info parser     //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#include "proj15.h"
#include <qstring.h>

proj15::proj15( double* params )
{
     if( params == NULL )
         m_paramValues = new double[15];
     else
         m_paramValues = params;

     setInvalid();
}

proj15::proj15( const int projNumber, double* params )
{
     if( params == NULL )
         m_paramValues = new double[15];
     else
         m_paramValues = params;

     setParamNames( projNumber );
}

proj15::proj15( const QString projectionName, double* params )
{
     if( params == NULL )
         m_paramValues = new double[15];
     else
         m_paramValues = params;

     setParamNames( projectionName );
}

proj15::~proj15()
{
     delete[] m_paramValues;
}

void proj15::setParamNames( const int projNumber )
{
   projName m_name;
   setName( m_name.projectionName( projNumber ) );
   setTrueName( m_projName );
   switch( projNumber )
   {
    case 0:     //"Geographic:"
    case 2:     // "State Plane":
         break;

    case 1:     //"UTM":
         setParamName( 0, lonZ( 0 ) );
         setParamName( 1, latZ( 1 ) );
         break;

    case 3:     //"Albers Equal Area:"
    case 4:     //"Lambert Conformal Conic":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 2, stdPR1() );
         setParamName( 3, stdPR2() );
         setParamName( 4, centMer() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 5:     //"Mercator":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 4, centMer() );
         setParamName( 5, trueScale() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 6:     //"Polar Stereographic":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 4, longPol() );
         setParamName( 5, trueScale() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 7:     //"Polyconic":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 4, centMer() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 8:     //"Equidistant Conic A\\B":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 4, centMer() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         if( getParamValue( 8 ) == 1 )
         {
             setTrueName( "Equidistant Conic B" );
             setParamName( 2, stdPR1() );
             setParamName( 3, stdPR2() );
         }
         else
         {
             setTrueName( "Equidistant Conic A" );
             setParamName( 2, stdPAR() );
         }
         break;

    case 9:     //"Transverse Mercator":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 2, factor() );
         setParamName( 4, centMer() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 10:    //"Stereographic":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 11:    //"Lambert Azimuthal":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 12:    //"Azimuthal":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 13:    //"Gnomonic":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 14:    //"Orthographic":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 15:    //"Gen. Vert. Near Per":
         setParamName( 0, sphere( 0 ) );
         setParamName( 2, height() );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 16:    //"Sinusiodal":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 17:    //"Equirectangular":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 5, trueScale() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 18:    //"Miller Cylindrical":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 19:    //"Van der Grinten":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 20:    //"Hotine Oblique Merc A\\B":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 2, factor() );
         setParamName( 5, originLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         if( getParamValue( 12 ) == 1 )
         {
             setTrueName( "Hotine Oblique Merc B" );
             setParamName( 3, aziAng() );
             setParamName( 4, azmthPt() );
         }
         else
         {
             setTrueName( "Hotine Oblique Merc A" );
             setParamName( 8, long1() );
             setParamName( 9, lat1() );
             setParamName( 10, long2() );
             setParamName( 11, lat2() );
         }
         break;

    case 21:    //"Robinson":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 22:    //"Space Oblique Merc A\\B":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         if( getParamValue( 12 ) == 1 )
         {
             setTrueName( "Space Oblique Merc B" );
             setParamName( 2, satNum() );
             setParamName( 3, path( 3 ) );
         }
         else
         {
             setTrueName( "Space Oblique Merc A" );
             setParamName( 3, incAng() );
             setParamName( 4, ascLong() );
             setParamName( 8, psRev() );
             setParamName( 9, lRat() );
             setParamName( 10, pFlag( 10 ) );
         }
         break;

    case 23:    //"Alaska Conformal":
         setParamName( 0, semiMajor( 0 ) );
         setParamName( 1, semiMinor( 1 ) );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 24:    //"Interrupted Goode":
         setParamName( 0, sphere( 0 ) );
         break;

    case 25:    //"Mollweide":
         setParamName( 0, sphere( 0 ) );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 26:    //"Interrupted Mollweide":
         setParamName( 0, sphere( 0 ) );
         break;

    case 27:    //"Hammer":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 28:    //"Wagner IV":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 29:    //"Wagner VII":
         setParamName( 0, sphere( 0 ) );
         setParamName( 4, centMer() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         break;

    case 30:   //"Oblated Equal Area":
         setParamName( 0, sphere( 0 ) );
         setParamName( 2, shapeM() );
         setParamName( 3, shapeN() );
         setParamName( 4, centLon() );
         setParamName( 5, centerLat() );
         setParamName( 6, FE() );
         setParamName( 7, FN() );
         setParamName( 8, angle() );
         break;
    default:
         setInvalid();
         break;
   }

   return;
}

void proj15::setParamNames( const QString projectionName)
{
     projName m_name;

     setParamNames( m_name.projectionNumber( projectionName ) );
     return;
}

void proj15::setNotUsed()
{
     for( int index = 0; index < 15; index++ )
        setParamName( index, "Not Used" );
    return;
}

void proj15::clearContents()
{
     for( int index = 0; index < 15; index++ )
        setParamName( index, "" );
    return;
}

void proj15::setInvalid()
{
    for( int index = 0; index < 15; index++ )
        setParamName( index, "Invalid projection name" );
    return;
}

QString proj15::lonZ( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) != 0 )
         returnValue = "Longitude of any point in UTM zone";
     else
         returnValue = "Not used, a zone code must be specified";

     return returnValue;
}

QString proj15::latZ( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) != 0 )
         returnValue = "Latitude of any point in UTM zone";
     else
         returnValue = "Not used, a zone code must be specified";

     return returnValue;
}

QString proj15::semiMajor( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) != 0 )
         returnValue = "Semi-major axis of ellipsoid";
     else
         returnValue = "Clarke 1866 in meters is assumed";

     return returnValue;
}

QString proj15::semiMinor( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) < 0 )
         returnValue = "Eccentricity squared of the ellipsoid";
     else if( getParamValue( element ) == 0 )
         returnValue = "A spherical form is assumed";
     else
         returnValue = "Semi-minor axis of ellipsoid";

     return returnValue;
}

QString proj15::sphere( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) != 0 )
         returnValue = "Radius of reference sphere";
     else
         returnValue = "6370997 is used";

     return returnValue;
}

QString proj15::stdPAR()
{
     QString returnValue = "";
     returnValue = "Latitude of the standard parallel";
     return returnValue;
}

QString proj15::stdPR1()
{
     QString returnValue = "";
     returnValue = "Latitude of the first standard parallel";
     return returnValue;
}

QString proj15::stdPR2()
{
     QString returnValue = "";
     returnValue = "Latitude of the second standard parallel";
     return returnValue;
}

QString proj15::centMer()
{
     QString returnValue = "";
     returnValue = "Longitude of the central meridian";
     return returnValue;
}

QString proj15::originLat()
{
     QString returnValue = "";
     returnValue = "Latitude of the porjection origin";
     return returnValue;
}

QString proj15::FE()
{
     QString returnValue = "";
     returnValue = "False easting in the same units as the semi-major axis";
     return returnValue;
}

QString proj15::FN()
{
     QString returnValue = "";
     returnValue = "False northing in the same units as the semi-major axis";
     return returnValue;
}

QString proj15::trueScale()
{
     QString returnValue = "";
     returnValue = "Latitude of true scale";
     return returnValue;
}

QString proj15::longPol()
{
     QString returnValue = "";
     returnValue = "Longitude down below pole of map";
     return returnValue;
}

QString proj15::factor()
{
     QString returnValue = "";

     if( m_projName == "Transverse Mercator" )
         returnValue = "Scale factor at central meridian";
     else if( (m_projName == "Hotine Oblique Merc A\\B" ) ||
              (m_projName == "Hotine Oblique Merc A" ) ||
              (m_projName == "Hotine Oblique Merc B" ) )
         returnValue = "Scale factor at center of projection";
     return returnValue;
}

QString proj15::centLon()
{
     QString returnValue = "";
     returnValue = "Longitude of center of projection";
     return returnValue;
}

QString proj15::centerLat()
{
     QString returnValue = "";
     returnValue = "Latitude of center of projection";
     return returnValue;
}

QString proj15::height()
{
     QString returnValue = "";
     returnValue = "Height of perspective point";
     return returnValue;
}

QString proj15::long1()
{
     QString returnValue = "";
     returnValue = "Longitude of first point on centerline";
     return returnValue;
}

QString proj15::long2()
{
     QString returnValue = "";
     returnValue = "Longitude of second point on centerline";
     return returnValue;
}

QString proj15::lat1()
{
     QString returnValue = "";
     returnValue = "Latitude of first point on centerline";
     return returnValue;
}

QString proj15::lat2()
{
     QString returnValue = "";
     returnValue = "Latitude of second point on centerline";
     return returnValue;
}

QString proj15::aziAng()
{
     QString returnValue = "";
     returnValue = "Azimuth angle east of north of center line";
     return returnValue;
}

QString proj15::azmthPt()
{
     QString returnValue = "";
     returnValue = "Longitude of point on center meridian where azimuth occurs";
     return returnValue;
}

QString proj15::incAng()
{
     QString returnValue = "";
     returnValue = "Inclination of orbit at ascending node, counter-clockwise from equator";
     return returnValue;
}

QString proj15::ascLong()
{
     QString returnValue = "";
     returnValue = "Longitude of ascending orbit at equator";
     return returnValue;
}

QString proj15::psRev()
{
     QString returnValue = "";
     returnValue = "Period of satillite revolution in minutes";
     return returnValue;
}

QString proj15::lRat()
{
     QString returnValue = "";
     returnValue = "Landsat ratio to compensate for confusion at northern end of orbit";
     return returnValue;
}

QString proj15::pFlag( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) == 0 )
         returnValue = "End of path flag for Landsat: start of path";
     else
         returnValue = "End of path flag for Landsat: end of path";

     return returnValue;
}

QString proj15::satNum()
{
     QString returnValue = "";
     returnValue = "Landsat Satillite Number";
     return returnValue;
}

QString proj15::path( const int element )
{
     QString returnValue = "";

     if( getParamValue( element ) >= 1 &&  getParamValue( element ) <= 3 )
         returnValue = "Landsat path number: WRS-1";
     else if( getParamValue( element ) >= 4 &&  getParamValue( element ) <= 6 )
         returnValue = "Landsat path number: WRS-2";
     else
         returnValue = "Landsat path number: undefined";

     return returnValue;
}

QString proj15::shapeM()
{
     QString returnValue = "";
     returnValue = "Oblated Equal Area oval shape parameter m";
     return returnValue;
}

QString proj15::shapeN()
{
     QString returnValue = "";
     returnValue = "Oblated Equal Area oval shape parameter n";
     return returnValue;
}

QString proj15::angle()
{
     QString returnValue = "";
     returnValue = "Oblated Equal Area oval rotation angle";
     return returnValue;
}

float proj15::getParamValue( const int param )
{
     if( param >= 0 && param < 15 )
         return m_paramValues[param];
     else
         return -1;
}

void proj15::setParamName( const int param, const QString Name )
{
     if( param >= 0 && param < 15 )
         m_paramNames[param] = Name;
     return;
}

void proj15::setTrueName( const QString Name )
{
     m_trueName = Name;
     return;
}

void proj15::setName( const QString Name )
{
     m_projName = Name;
     return;
}
