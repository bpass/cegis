// $Id: projDatum.h,v 1.2 2005/01/06 19:55:35 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

///////////////////////////////////////////////////////
// Class for translating datum number to name        //
//       For use with projInfo .info parser          //
//                                                   //
// Created by: J. Trent - 07/2003                    //
///////////////////////////////////////////////////////

#ifndef PROJDATUM_H
#define PROJDATUM_H

#include <qstring.h>

class projDatum
{
public:
   projDatum();
   ~projDatum();
   QString projectionDatum( const int datumNumber );
   QString projectionDatum( const QString datumNumber );
   int getDatumCount() { return datumCount; }

private:

   int datumCount;
   QString *datumNames;
};

#endif
