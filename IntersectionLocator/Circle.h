/*
 * File: Circle.h
 * Purpose: This file contains the declaration of a class for representing
 *          a circle, and for creating circumcircles of Triangles.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */

// Note:
// encloses only returns true if the point is within the circle, not if
// it is on the perimeter.


// Majic numbers for CVS
// $Id: Circle.h,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $


#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"
#include "Triangle.h"

/*
 * Circle is a class to represent a circle... surprise!
 *
 * Circle()
 * Constructor to create a circle with center at (0,0) and a radius
 * of 0.
 *
 * Circle( const Point &center, const double &radius )
 * Constructor that creates a circle with center and radius.
 *
 * Circle( const Circle &old )
 * Copy Constructor for Circle.
 *
 * Circle( const Triangle &triangle )
 * Constructor that creates a circle that circumscribes triangle.
 * For a detailed description of the math used, see Circle.cpp.
 *
 * const Point &center() const
 * const double &radius() const
 * Accessor functions for center and radius.
 *
 * bool encloses( const Point &point )
 * This function returns true if point is within the Circle
 * Post: true if the circle encloses the point.
 */

class Circle
{
public:
   Circle() 
      : mCenter(), mRadius( 0.0 ) {}
   Circle( const Point &center, const double &radius )
      : mCenter(center), mRadius(radius) {}
   Circle( const Circle &old ) 
      : mCenter(old.center()), mRadius(old.radius()) {}

   Circle( const Triangle &triangle );

   const Point &center() const {return mCenter;}
   const double &radius() const {return mRadius;}
   bool encloses( const Point &point )
   { return (mCenter.distanceTo( point ) < mRadius ); }

private:
   Point mCenter;
   double mRadius;
};


#endif CIRCLE_H

