/**
 * @file Point.h
 * Purpose: This file contains the declaration of a 2 dimensional
 *          point class.
 *
 * @author Ryan Stelzleni
 * @date 8-12-04
 *
 * This code was written for the United States Geological Survey.
 */



// Majic numbers for CVS
// $Id: Point.h,v 1.4 2004/11/10 17:10:29 ahartman Exp $



#ifndef POINT_H
#define POINT_H

#include <math.h>  // for sqrt


/**
 * The Point class represents a 2 dimensional point.
 */
class Point
{
public:
   /**
    * This constructor creates a point at (0,0)
    */
   Point() : mX(0.0), mY(0.0) {}

   /**
    * This constructor creates a point from two doubles for x and y.
    * @param x The x coordinate.
    * @param y The y coordinate.
    */
   Point( const double &x, const double &y ) : mX(x), mY(y) {}

   /**
    * This is a copy constructor, it creates a new point that is
    * a copy of an old one.
    * @param old The Point to copy.
    */
   Point( const Point &old ) : mX(old.x()), mY(old.y()) {}
   
   /**
    * An accessor function for x.
    */
   double x() const {return mX;}

   /**
    * An accessor function for y.
    */
   double y() const {return mY;}

   /**
    * Mutator function for x.
    * @param newX The new x value.
    */
   void setX( const double &newX ) {mX = newX;}

   /**
    * Mutator function for y.
    * @param newY The new y value.
    */
   void setY( const double &newY ) {mY = newY;}

   /**
    * This function returns the distance from *this to point.
    * @param point the Point to find the distance to.
    */
   double distanceTo( const Point &point ) const
   {
      return ( sqrt( (mX-point.mX)*(mX-point.mX) + 
                     (mY-point.mY)*(mY-point.mY) ) );
   }

private:
   double mX;  //!< The x coordinate.
   double mY;  //!< The y coordinate.
};

#endif // POINT_H

