// $Id: projRowCol.h,v 1.3 2005/01/11 17:11:28 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for separating and returing rows and columns  //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////


#ifndef PROJROWCOL_H
#define PROJROWCOL_H

#include <qstring.h>

class projRowCol
{
public:
  static int projectionRow( const QString rowcolInfo )
                       { return projectionRowCol( rowcolInfo, 0 ); }
  static int projectionCol( const QString rowcolInfo )
                       { return projectionRowCol( rowcolInfo, 1 ); }

private:
  static int projectionRowCol( const QString rowcolInfo, int element = -1 )
             {
             	int breakPoint = rowcolInfo.find( ' ', 0, false );
             	if( element == -1 || breakPoint == -1 ) return -1;
             	else if( element == 0 ) return rowcolInfo.left( breakPoint ).toInt();
             	else if( element == 1 ) return rowcolInfo.right( rowcolInfo.length() - breakPoint ).toInt();
                else return -1;
             }
};

#endif
