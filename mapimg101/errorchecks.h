// $Id: errorchecks.h,v 1.2 2005/01/06 19:55:34 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

///////////////////////////////////////////////////////
// Error checking functions for MapIMG               //
//                                                   //
// Created by: S. Posch - 08/2002                    //
// Modified by: J. Trent - 06/2003                   //
//              Adjusted ppointCheck() bounds for    //
//                       gen. vert.                  //
///////////////////////////////////////////////////////
#include <qstring.h>


// Returns true is string s is a valid double, otherwise false
bool validDouble(QString s);


// Returns true is d between -10 Million and + 10 Million, otherwise false
bool fefnCheck(double d);


// Returns true if scale factor is "normal" (close to 1), otherwise false
bool factorCheck(double d);


// Returns true if between -1 Billion and 1 Billion, otherwise false
bool ppointCheck(double d);


// Returns true if d <= 2, otherwise false
bool shapemCheck(double d);


// Returns true if d >= 2
bool shapenCheck(double d);


// Returns true if between 0 and 1, false otherwise
bool lratCheck(double d);


// Returns true if d == 1,2,3,4,5,or 7, otherwise false
bool satnumCheck(double d);


// Returns true if d is between 0 and 10 Million, otherwise false
bool smajminCheck(double d);


// if l == 4,5,or 7, returns true if d is between 1 and 233
// if l == 1,2,or 3, returns true if d is between 1 and 251
bool pathCheck(int l, int d);


// Returns true if d between -10Mill and 10Mill	
bool psrevCheck(double d);
