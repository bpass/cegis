//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating unit type number to name      //
//       and back. For use with projInfo .info parser  //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#ifndef PROJUNIT_H
#define PROJUNIT_H

#include <qstring.h>

class projUnit
{
public:
   projUnit();
   ~projUnit();
   QString projectionUnit( const int unitNumber );
   QString projectionUnit( const QString unitNumber );

private:
   int unitCount;
   QString *unitNames;
};


#endif
