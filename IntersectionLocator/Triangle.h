/**
 * @file Triangle.h
 * Purpose: This file contains the declaration of a class to
 *          represent a triangle in two dimensions.
 *
 * @author Ryan Stelzleni
 * @date 8-12-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Triangle.h,v 1.3 2004/11/02 01:20:46 rstelzleni Exp $


#ifndef TRIANGLE_H
#define TRIANGLE_H

 
#include "Point.h"

/**
 * The Triangle class represents a triangle as three Points.
 */
class Triangle
{
public:
   /**
    * This constructor takes no arguments and calls the default 
    * constructors for each of its points.
    */
   Triangle() : a(), b(), c() {}

   /**
    * This constructor takes point references and assigns those
    * points to the a, b, and c points of the triangle.
    * @param a One point on the triangle.
    * @param b Another point on the triangle.
    * @param c The third point on the triangle.
    */
   Triangle( const Point &a, const Point &b, const Point &c )
      : a(a), b(b), c(c) {}

   /**
    * This constructor creates a copy of the triangle that is used
    * as the function's argument.
    * @param old The triangle to copy.
    */
   Triangle( const Triangle &old )
      : a( old.pointA() ), b( old.pointB() ), c( old.pointC() ) {} 

   /**
    * Accessor for point A in the triangle.
    */
   const Point &pointA() const {return a;}

   /**
    * Accessor for point B in the triangle.
    */
   const Point &pointB() const {return b;}

   /**
    * Accessor for point C in the triangle.
    */
   const Point &pointC() const {return c;}

   /**
    * Mutator for point A in the triangle.
    * @param newA The new value for point A.
    */
   void setA( const Point &newA ) {a = newA;} 

   /**
    * Mutator for point B in the triangle.
    * @param newB The new value for point B.
    */
   void setB( const Point &newB ) {b = newB;}

   /**
    * Mutator for point C in the triangle.
    * @param newC The new value for point C.
    */
   void setC( const Point &newC ) {c = newC;} 

private:
   Point a, b, c;
};


#endif // TRIANGLE_H

