// $Id: projName.h,v 1.2 2005/01/06 19:55:35 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating projection number to name     //
//       and back. For use with projInfo .info parser  //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#ifndef PROJNAME_H
#define PROJNAME_H

#include <qstring.h>

class projName
{
public:
   projName();
   ~projName();
   QString projectionName( const int projNumber );
   QString projectionName( const QString projNumber );
   int     projectionNumber( const QString projName );
   int     maxProjNumber();

private:
   int     projectionCount;
   QString *projectionNames;
};

#endif
