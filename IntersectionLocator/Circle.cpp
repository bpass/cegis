/*
 * File: Circle.cpp
 * Purpose: This file contains the implementation of functions for
 *          the circle and point classes.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */


// Majic numbers for CVS
// $Id: Circle.cpp,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $


#include "Circle.h"
#include "Triangle.h"
#include "Error.h"

Circle::Circle( const Triangle &triangle )
{
   // This method is based on equations from Mathworld
   // Here is what this is supposed to implement:
   // a = x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2)
   // bx = (x1^2 + y1^2)(y2 - y3) + (x2^2 + y2^2)(y3 - y1) + 
   //      (x3^2 + y3^2)(y1 - y2)
   // by = -[(x1^2 + y1^2)(x2 - x3) + (x2^2 + y2^2)(x3 - x1) + 
   //        (x3^2 + y3^2)(x1 - x2)]
   // x = bx/2a
   // y = by/2a
   // where pointA is point 1, pointB is point 2 and pointC is point 3

   double a, bx, by;
   double x1sy1s, x2sy2s, x3sy3s; // the (x1^2 + y1^2) type terms
   double y2y3, y3y1, y1y2;       // the (y2 - y3) type terms

   y2y3 = triangle.pointB().y() - triangle.pointC().y();
   y3y1 = triangle.pointC().y() - triangle.pointA().y();
   y1y2 = triangle.pointA().y() - triangle.pointB().y();

   x1sy1s = triangle.pointA().x()*triangle.pointA().x() + 
            triangle.pointA().y()*triangle.pointA().y();
   x2sy2s = triangle.pointB().x()*triangle.pointB().x() + 
            triangle.pointB().y()*triangle.pointB().y();
   x3sy3s = triangle.pointC().x()*triangle.pointC().x() + 
            triangle.pointC().y()*triangle.pointC().y();

   a = triangle.pointA().x()*y2y3 + 
       triangle.pointB().x()*y3y1 + 
       triangle.pointC().x()*y1y2;

   bx = x1sy1s*y2y3 + x2sy2s*y3y1 + x3sy3s*y1y2;

   by = x1sy1s*( triangle.pointB().x() - triangle.pointC().x() ) + 
        x2sy2s*( triangle.pointC().x() - triangle.pointA().x() ) + 
        x3sy3s*( triangle.pointA().x() - triangle.pointB().x() );
   by = -by;

   if( a != 0 )
   {
      mCenter.setX( (bx/(2*a)) );
      mCenter.setY( (by/(2*a)) );
   }
   else
   {
      //Error_Print( "Divide by zero error in circumcircle constructor.\n" );
      mCenter.setX( 0.0 );
      mCenter.setY( 0.0 );
   }

   // Next task is to find the radius, Since the triangle's points are
   // on the circle all we need is the distance from the center to a 
   // point on the triangle
   mRadius = mCenter.distanceTo( triangle.pointA() );

} // End Circle( Triangle ) constructor

