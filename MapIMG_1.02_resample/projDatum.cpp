//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

///////////////////////////////////////////////////////
// Class for translating datum number to name        //
//       For use with projInfo .info parser          //
//                                                   //
// Created by: J. Trent - 07/2003                    //
///////////////////////////////////////////////////////

#include "projDatum.h"
#include <qstring.h>


projDatum::projDatum()
{
     datumCount = 20;

     datumNames = new QString[datumCount];

     datumNames[0] = "Clarke 1866 (default)";
     datumNames[1] = "Clarke 1880";
     datumNames[2] = "Bessel";
     datumNames[3] = "International 1967";
     datumNames[4] = "International 1909";
     datumNames[5] = "WGS 72";
     datumNames[6] = "Everest";
     datumNames[7] = "WGS 66";
     datumNames[8] = "GRS 1980";
     datumNames[9] = "Airy";
     datumNames[10] = "Modified Everest";
     datumNames[11] = "Modified Airy";
     datumNames[12] = "WGS 84";
     datumNames[13] = "Southeast Asia";
     datumNames[14] = "Austalian National";
     datumNames[15] = "Krassovsky";
     datumNames[16] = "Hough";
     datumNames[17] = "Mercury 1960";
     datumNames[18] = "Modified Mercury 1968";
     datumNames[19] = "Sphere of Radius 6370997 meters";

}

projDatum::~projDatum()
{
     delete[] datumNames;
}

QString projDatum::projectionDatum( const int datumNumber )
{
     if( datumNumber < 0 || datumNumber >= getDatumCount() )
         return NULL;
     else
         return datumNames[ datumNumber ];
}


QString projDatum::projectionDatum( const QString datumNumber )
{
     return projectionDatum( datumNumber.toInt() );
}

