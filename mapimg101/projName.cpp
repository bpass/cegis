// $Id: projName.cpp,v 1.3 2005/01/11 17:11:28 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating projection number to name     //
//       and back. For use with projInfo .info parser  //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#include "projName.h"
#include <qstring.h>

projName::projName()
{
     projectionCount = 31;
     projectionNames = new QString[projectionCount];
     
     projectionNames[0] = "Geographic";
     projectionNames[1] = "UTM";
     projectionNames[2] = "State Plane";
     projectionNames[3] = "Albers Equal Area";
     projectionNames[4] = "Lambert Conformal Conic";
     projectionNames[5] = "Mercator";
     projectionNames[6] = "Polar Stereographic";
     projectionNames[7] = "Polyconic";
     projectionNames[8] = "Equidistant Conic A\\B";
     projectionNames[9] = "Transverse Mercator";
     projectionNames[10] = "Stereographic";
     projectionNames[11] = "Lambert Azimuthal";
     projectionNames[12] = "Azimuthal";
     projectionNames[13] = "Gnomonic";
     projectionNames[14] = "Orthographic";
     projectionNames[15] = "Gen. Vert. Near Per";
     projectionNames[16] = "Sinusiodal";
     projectionNames[17] = "Equirectangular";
     projectionNames[18] = "Miller Cylindrical";
     projectionNames[19] = "Van der Grinten";
     projectionNames[20] = "Hotine Oblique Merc A\\B";
     projectionNames[21] = "Robinson";
     projectionNames[22] = "Space Oblique Merc A\\B";
     projectionNames[23] = "Alaska Conformal";
     projectionNames[24] = "Interrupted Goode";
     projectionNames[25] = "Mollweide";
     projectionNames[26] = "Interrupted Mollweide";
     projectionNames[27] = "Hammer";
     projectionNames[28] = "Wagner IV";
     projectionNames[29] = "Wagner VII";
     projectionNames[30] = "Oblated Equal Area";

}

projName::~projName()
{
     delete[] projectionNames;
}

QString projName::projectionName( const int projNumber )
{
     if( projNumber < 0 || projNumber >= projectionCount )
         return NULL;
     else
         return projectionNames[ projNumber ];
}

QString projName::projectionName( const QString projNumber )
{
     return projectionName( projNumber.toInt() );
}

int projName::maxProjNumber()
{
     return projectionCount;
}

int projName::projectionNumber( const QString projName )
{
     for( int index = 0; index < maxProjNumber(); index++)
          if( projName.lower() == projectionName( index ).lower() )
              return index;

     return -1;
}

