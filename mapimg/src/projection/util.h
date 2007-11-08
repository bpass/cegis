/* File: util.h
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#include <limits.h>
#include <math.h>


namespace trans {

  int sign (double x);
  double phi2z (double eccen, double ts);
  double tsfnz (double eccen, double phi, double sinphi);
  double adjust_lon (double x);
}
