/**
 * @file Circle.h
 * Purpose: This file contains the declaration of a class for representing
 *          a circle, and for creating circumcircles of Triangles.
 *
 * @author Ryan Stelzleni
 * @date 8-12-04
 *
 * This code was written for the United States Geological Survey.
 */

// Note:
// encloses only returns true if the point is within the circle, not if
// it is on the perimeter.


// Majic numbers for CVS
// $Id: Circle.h,v 1.4 2004/11/10 16:52:04 ahartman Exp $


#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"
#include "Triangle.h"


/**
 * Circle is a class to represent a circle... surprise!
 */
class Circle
{
public:
   /**
    * Constructor to create a circle with center at (0,0) and a radius
    * of 0.
    */
   Circle() 
      : mCenter(), mRadius( 0.0 ) {}

   /**
    * Constructor that creates a circle with center and radius.
    * @param center Point that represents the circle's center.
    * @param radius Radius of the circle.
    */
   Circle( const Point &center, const double &radius )
      : mCenter(center), mRadius(radius) {}

   /**
    * Copy Constructor for Circle.
    * @param old The circle to be copied.
    */
   Circle( const Circle &old ) 
      : mCenter(old.center()), mRadius(old.radius()) {}

   /**
    * Constructor that creates a circle that circumscribes triangle.
    * Math used for circumscribing a triangle.
    * This method is based on equations from the website Mathworld.
    * Here is what this is supposed to implement:              <BR>
    * a = x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2)              <BR>
    * bx = (x1^2 + y1^2)(y2 - y3) + (x2^2 + y2^2)(y3 - y1) +    
    *      (x3^2 + y3^2)(y1 - y2)                              <BR>
    * by = -[(x1^2 + y1^2)(x2 - x3) + (x2^2 + y2^2)(x3 - x1) +  
    *        (x3^2 + y3^2)(x1 - x2)]                           <BR>
    * x = bx/2a                                                <BR>
    * y = by/2a                                                <BR>
    * Where x and y are the circle's center.
    * a, bx and by are really just in there to make the math look
    * (somewhat) clearer.
    * To find the radius I just find the distance to any of the 
    * three points on the triangle from the calculated center.
    *
    * @param triangle The triangle to be circumscribed.
    */
   Circle( const Triangle &triangle );

   /**
    * Accessor function for the center Point.
    * @return The circle's center point
    */
   const Point &center() const {return mCenter;}

   /**
    * Accessor function for the circle's radius.
    * @return The circle's radius.
    */
   const double &radius() const {return mRadius;}

   /**
    * This function returns true if the point is within the radius
    * of the circle.  Note that this doesn't include the perimeter of
    * the circle.
    * @param point The point to check for inclusion in the circle.
    * @return true if the point is within the circle.
    */
   bool encloses( const Point &point )
   { return (mCenter.distanceTo( point ) < mRadius ); }

private:
   /** 
    * A Point from file Point.h
    * @see Point.h
    */
   Point mCenter;

   /**
    * The circle's radius.
    */
   double mRadius;
};

#endif

