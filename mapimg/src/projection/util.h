/* File: util.h
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify if under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

namespace trans {

  int sign(double x);
  double phi2z (double eccen, double ts);
  double adjust_lon (double x);
}
