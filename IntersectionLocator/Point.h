/*
 * File: Point.h
 * Purpose: This file contains the declaration of a 2 dimensional
 *          point class.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */


// Majic numbers for CVS
// $Id: Point.h,v 1.2 2004/10/18 22:42:51 rstelzleni Exp $



#ifndef POINT_H
#define POINT_H

#include <math.h>  // for sqrt


/*
 * The Point class represents a 2 dimensional point.
 *
 * Point() 
 * This constructor creates a point at (0,0)
 *
 * Point( const double &x, const double &y )
 * This constructor creates a point from two doubles for x and y.
 *
 * Point( const Point &old )
 * This is a copy constructor, it creates a new point that is
 * a copy of an old one.
 *
 * double x() const
 * double y() const
 * Accessor functions for x and y.
 *
 * void setX( const double &newX )
 * void setY( const double &newY )
 * Mutator functions for x and y.
 *
 * double distanceTo( const Point &point ) const
 * This function returns the distance from *this to point.
 */


class Point
{
public:
   Point() : mX(0.0), mY(0.0) {}
   Point( const double &x, const double &y ) : mX(x), mY(y) {}
   Point( const Point &old ) : mX(old.x()), mY(old.y()) {}
   
   double x() const {return mX;}
   double y() const {return mY;}

   void setX( const double &newX ) {mX = newX;}
   void setY( const double &newY ) {mY = newY;}

   double distanceTo( const Point &point ) const
   {
      return ( sqrt( (mX-point.mX)*(mX-point.mX) + 
                     (mY-point.mY)*(mY-point.mY) ) );
   }

private:
   double mX, mY;
};

#endif // POINT_H