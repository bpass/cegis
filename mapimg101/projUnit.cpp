// $Id: projUnit.cpp,v 1.3 2005/01/11 17:11:28 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating unit type number to name      //
//       and back. For use with projInfo .info parser  //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#include "projUnit.h"
#include <qstring.h>

projUnit::projUnit()
{
     unitCount = 7;

     unitNames = new QString[unitCount];

     unitNames[0] = "Radians";
     unitNames[1] = "U.S. Feet";
     unitNames[2] = "Meters";
     unitNames[3] = "Seconds of Arc";
     unitNames[4] = "Degrees of Arc";
     unitNames[5] = "International Feet";
     unitNames[6] = "Legislated by State Zone Selection";

}

projUnit::~projUnit()
{
     delete[] unitNames;
}

QString projUnit::projectionUnit( const int unitNumber )
{
     if( unitNumber < 0 || unitNumber >= unitCount )
         return NULL;
     else
         return unitNames[ unitNumber ];
}

QString projUnit::projectionUnit( const QString unitNumber )
{
     return projectionUnit( unitNumber.toInt() );
}
