// $Id: imgio.cpp,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mapimg I/O functions:  These functions read and write files associated with mapimg
// Written by D. Steinwand
//
// Note: These functions are used by the IMGIO class but not actually members of it.
//
// Updated by Stephan Posch for GUI implementation -- 8/02
// Updated by Jason Trent for templating to allow multiple data types -- 7/03
// Updated again by Jason Trent for object oriented approach with IMGIO class -- 11/04
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <qmessagebox.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qfile.h>
#include "imgio.h"

#include "mapimg.h"


int onLine(double p1[2], double p2[2], double test[2])
{
   float dx, dy, dxt, dyt;

   if( (test[0] == p1[0] && test[1] == p1[1]) ||
      (test[0] == p2[0] && test[1] == p2[1]) )
      return 1;

   if( !((p1[0] <= test[0] && test[0] < p2[0]) || (p2[0] <= test[0] && test[0] < p1[0])) )
      return 0;

   dx = p2[1] - p1[1];
   dy = p2[0]-p1[0];

   dxt = test[1] - p1[1];
   dyt = test[0] - p1[0];

   if(dx*dyt - dy*dxt)
      return 0;
   else
      return 1;
}      //onLine

int inBox(double box[5][2], double test[2])
{
   int intersections = 0;	// # of times test line crosses a polgon line
   int i;			// loop counter
   int j = 3;		// another placeholder

   for(i = 0; i < 4; ++i)
   {
      if(onLine(box[j], box[i], test))
         return 1;
      if( (test[1] < box[j][1] || test[1] < box[i][1]) &&
         ( (box[j][0] <= test[0] && test[0] < box[i][0]) ||
         (box[i][0] <= test[0] && test[0] < box[j][0]) ) )
         intersections++;
      j = i;
   }

   return ( intersections % 2 );
}//inBox


