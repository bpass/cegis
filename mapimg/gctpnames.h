// $Id: gctpnames.h,v 1.6 2005/03/17 18:57:25 rbuehler Exp $


#ifndef GCTPNAMES_H
#define GCTPNAMES_H

#include <qstringlist.h>

class SmartQStringList : public QStringList
{
public:
   SmartQStringList(const QStringList & I):QStringList(I){}

   const QString &operator[](QStringList::size_type i) const
   {
      if( i < 0 || i > (size()-1) ) 
         return QString::null;
      else
         return (*at(i));
   }
};

namespace
{
   /*
         gctpNames() returns a QStringList containing the names 
      of each of the gctp parameters for the projection defined
      by projNum and variation.
   */
   QStringList gctpNames( uint projNum, char variation )
   {
      QStringList retList;

      switch( projNum )
      {
      case 0:  //Geographic
         retList << "NoWay";
         break;

      case 1:  //UTM
         retList << "Lon/Z" << "Lat/Z" << "NoWay";
         break;

      case 2:  //State Plane
         retList << "Sphere" << "NoWay";
         break;

      case 3:  //Albers Equal Area
         retList << "SMajor" << "SMinor" << "STDPR1" << "STDPR2" << "CentMer" 
            << "OriginLat" << "FE" << "FN";
         break;

      case 4:  //Lambert Conformal Conic
         retList << "SMajor" << "SMinor" << "STDPR1" << "STDPR2" << "CentMer" 
            << "OriginLat" << "FE" << "FN" << "NoWay";
         break;

      case 5:  //Mercator
         retList << "SMajor" << "SMinor" << "" << "" << "CentMer" << "TrueScale"
            << "FE" << "FN";
         break;

      case 6:  //PolarStereographic
         retList << "SMajor" << "SMinor" << "" << "" << "LongPol" << "TrueScale"
            << "NoWay";
         break;

      case 7:  //Polyconic
         retList << "SMajor" << "SMinor" << "" << "" << "CentMer" << "OriginLat"
            << "FE" << "FN";
         break;
         
      case 8:  //Equid. Conic
         if( variation == 'a' )
            retList << "SMajor" << "SMinor" << "STDPAR" << "" << "CentMer" 
               << "OriginLat" << "FE" << "FN" << "one";
         else
            retList << "SMajor" << "SMinor" << "STDPR1" << "STDPR2" << "CentMer" 
               << "OriginLat" << "FE" << "FN" << "zero";
         break;

      case 9: //Transverse Mercator
         retList << "SMajor" << "SMinor" << "FactorM" << "" << "CentMer" << "OriginLat"
            << "FE" << "FN";
         break;

      case 10: //Stereographic
         retList << "Sphere" << "" << "" << "" << "CentLon" << "CenterLat" 
            << "FE" << "FN" << "NoWay";
         break;

      case 11: //Lambert Azimuthal
      case 12: //Azimuthal
      case 14: //Orthographic
         retList << "Sphere" << "" << "" << "" << "CentLon" << "CenterLat" 
            << "FE" << "FN";
         break;

      case 13: //Gnomic
         retList << "Sphere" << "" << "" << "" << "CentLon" << "CenterLat" 
            << "FE" << "FN" << "NoWay"; //Otherwise same as case 10,11,12,14
         break;

      case 15: //Gen. Vert. Near Per
         retList << "Sphere" << "" << "Height" << "" << "CentLon" << "CenterLat" 
            << "FE" << "FN";
         break;

      case 16: //Sinusoidal
      case 18: //Miller Cylindrical
      case 21: //Robinson
      case 25: //Mollweide
      case 27: //Hammer
      case 28: //Wagner IV
      case 29: //Wagner VII
         retList << "Sphere" << "" << "" << "" << "CentMer" << "" << "FE" << "FN";
         break;

      case 17: //Equirectangular
         retList << "Sphere" << "" << "" << "" << "CentMer" << "TrueScale" 
            << "FE" << "FN";
         break;

      case 19: //Van der Grinten
         retList << "Sphere" << "" << "" << "" << "CentMer" << "OriginLat" 
            << "FE" << "FN";
         break;

      case 20: //Hotin Oblique Merc
         if( variation == 'a' )
            retList << "SMajor" << "SMinor" << "FactorH" << "" << "" << "OriginLat" 
               << "FE" << "FN" << "Long1" << "Lat1" << "Long2" << "Lat2" << "zero";
         else
            retList << "SMajor" << "SMinor" << "FactorH" << "AziAng" << "AzmthPt" 
               << "OriginLat" << "FE" << "FN" << "" << "" << "" << "" << "one";
         break;

      case 22: //Space Oblique Merc
         if( variation == 'a' )
            retList << "SMajor" << "SMinor" << "" << "IncAng" << "AscLong" << "" 
               << "FE" << "FN" << "PSRev" << "LRat" << "PFlag" << "" << "zero";
         else
            retList << "SMajor" << "SMinor" << "Satnum" << "Path" << "" << "" 
               << "FE" << "FN" << "" << "" << "" << "" << "one";
         break;

      case 23: //Alaska Conformal
         retList << "SMajor" << "SMinor" << "" << "" << "" << "" << "FE" << "FN" << "NoWay";
         break;

      case 24: //Interrupted Goode
      case 26: //Interrupted Mollweide
         retList << "Sphere";
         break;

      case 30:
         retList << "Sphere" << "" << "Shapem" << "Shapen" << "CentLon" 
            << "CenterLat" << "FE" << "FN" << "Angle";
         break;
      }

      //Fill the list up to 15.
      while( retList.count() < 15 )
         retList << "";

      return retList;
   }

   QString nameMeanings( QString &gctpName )
   {
      if( gctpName == "" )
         return "Unused";
      else if( gctpName == "Angle" )
         return "Oval Rotation Angle:";
      else if( gctpName == "AscLong" )	
         return "Longitude of Ascending Orbit:";
      else if( gctpName == "AziAng" )	
         return "Azimuth Angle East of North:";
      else if( gctpName == "AzmthPt" )		
         return "Longitude where Azimuth Occurs:";
      else if( gctpName == "CenterLat" )	
         return "Latitude of Projection Center:";
      else if( gctpName == "CentLon" )		
         return "Longitude of Projection Center:";
      else if( gctpName == "CentMer" )	
         return "Longitude of Central Meridian:";
      else if( gctpName == "FactorH" )	
         return "Scale Factor at Center of Projection:";
      else if( gctpName == "FactorM" )		
         return "Scale Factor at Central Meridian:";
      else if( gctpName == "FE" )		
         return "False Easting:";
      else if( gctpName == "FN" )	
         return "False Northing:";
      else if( gctpName == "Height" )	
         return "Height of Perspective:";
      else if( gctpName == "IncAng" )	
         return "Inclination of Orbit:";
      else if( gctpName == "Lat/Z" )	
         return "Latitude within Zone:";
      else if( gctpName == "Lat1" )		
         return "Latitude of 1st Point on Center Line:";
      else if( gctpName == "Lat2" )
         return "Latitude of 2nd Point on Center Line:";
      else if( gctpName == "Lon/Z" )	
         return "Longitude within Zone:";
      else if( gctpName == "Long1" )	
         return "Longitude of 1st Point on Center Line:";
      else if( gctpName == "Long2" )	
         return "Longitude of 2nd Point on Center Line:";
      else if( gctpName == "LongPol" )	
         return "Longitude down below Pole of Map:";
      else if( gctpName == "LRat" )	
         return "Landsat Ratio:";
      else if( gctpName == "OriginLat" )	
         return "Latitude of Projection Origin:";
      else if( gctpName == "Path" )		
         return "Landsat Path Number:";
      else if( gctpName == "PFlag" )	
         return "End of Path Flag:";
      else if( gctpName == "PSRev" )	
         return "Period of Satellite Revolution:";
      else if( gctpName == "Satnum" )	
         return "Landsat Satellite Number:";
      else if( gctpName == "Shapem" )		
         return "Oval Shape Parameter m:";
      else if( gctpName == "Shapen" )	
         return "Oval Shape Parameter n:";
      else if( gctpName == "SMajor" )		
         return "Semi-Major Axis:";
      else if( gctpName == "SMinor" )	
         return "Semi-Minor Axis:";
      else if( gctpName == "Sphere" )	
         return "Radius of the Reference Sphere:";
      else if( gctpName == "STDPAR" )	
         return "Latitude of Standard Parallel:";
      else if( gctpName == "STDPR1" )	
         return "Latitude of 1st Standard Parallel:";
      else if( gctpName == "STDPR2" )	
         return "Latitude of 2nd Standard Parallel:";
      else if( gctpName == "TrueScale" )		
         return "Latitude of True Scale:";
      else if( gctpName == "one" )	
         return "One";
      else if( gctpName == "zero" )
         return "Zero";

      return "Unused";
   }

   uint combo2proj( uint i )
   {
      if( i < 10 ) return i - 1;
      if( i < 23 ) return i - 2;
      if( i < 26 ) return i - 3;
      return i - 4;
   }

   const SmartQStringList projNames(QStringList::split( ',', 
      "Geographic,Universal Transverse Mercator,State Plane Coordinates,"
      "Albers Conical Equal Area,Lambert Conformal Conic,Mercator,"
      "Polar Stereographic,Polyconic,Equidistant Conic,Transverse Mercator,"
      "Stereographic,Lambert Azimuthal Equal Area,Azimuthal Equidistant,Gnomonic,"
      "Orthographic,General Vertical Near-Side Perspective,Sinusoidal,"
      "Equirectangular,Miller Cylindrical,Van der Grinten,"
      "Hotine Oblique Mercator,Robinson,Space Oblique Mercator,"
      "Modified Stereohraphic Conformal--Alaska,Interrupted Goode Homolsine,"
      "Mollweide,Interrupted Mollweide,Hammer,Wagner IV,Wagner VII,"
      "Oblated Equal Area" ));

   const QStringList unitNames = QStringList::split( ',',
      "Radians,U.S. Feet,Meters,Seconds of Arc,Degrees of Arc,"
      "International Feet,State Zone Table,Unknown" );

   const QStringList spheroidNames = QStringList::split( ',',
      "Clarke 1866,Clarke 1880,Bessel,International 1967,International 1909,"
      "WGS 72,Everest,WGS 66,GRS 1980,Airy,Modified Everest,Modified Airy,"
      "WGS 84,Southeast Asia,Australlian National,Krassovsky,Hough,Mercury 1960,"
      "Modified Mercury 1968,Sphere of Radius 6370997 meters,Unknown" );

   const QStringList pixelSizes = QStringList::split( ',',
      "5 Degrees,1 Degree,30 Minutes,5 Minutes,30 Arc Seconds,Meters..." );

   const QStringList pixelValues = QStringList::split( ',',
      "555974.548395,111194.909679,55597.454840,9266.242473,926.624247" );

   const QStringList dataTypes = QStringList::split( ',',
      "Signed 64 Bit IEEE Float,"
      "Signed 32 Bit IEEE Float,Unsigned 32 Bit Integer,Signed 32 Bit Integer,"
      "Unsigned 16 Bit Integer,Signed 16 Bit Integer,"
      "Unsigned 8 Bit Integer,Signed 8 Bit Integer" );
}

#endif

