/*
 * File: Triangle.h
 * Purpose: This file contains the declaration of a class to
 *          represent a triangle in two dimensions.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */


#ifndef TRIANGLE_H
#define TRIANGLE_H


/*
 * The Triangle class represents a triangle as three Points.
 * 
 * Triangle()
 * This constructor takes no arguments and calls the default 
 * constructors for each of its points.
 *
 * Triangle( const Point &a, const Point &b, const Point &c )
 * This constructor takes point references and assigns those
 * points to the a, b, and c points of the triangle.
 *
 * Triangle( const Triangle &old )
 * This constructor creates a copy of the triangle that is used
 * as the function's argument.
 *
 *    const Point &pointA() const {return a;}
 *    const Point &pointB() const {return b;}
 *    const Point &pointC() const {return c;}
 * These three functions provide access to the points
 * of this triangle.  Note that if the points are
 * deallocated then the references returned by this
 * function will be invalid.
 *
 *   void setA( const Point &newA ) {a = newA;} 
 *   void setB( const Point &newB ) {b = newB;}
 *   void setC( const Point &newC ) {c = newC;} 
 * These three functions provide a public method of setting
 * the triangle's vertices. 
 */

#include "Point.h"

class Triangle
{
public:
   Triangle() : a(), b(), c() {}
   Triangle( const Point &a, const Point &b, const Point &c )
      : a(a), b(b), c(c) {}
   Triangle( const Triangle &old )
      : a( old.pointA() ), b( old.pointB() ), c( old.pointC() ) {} 

   const Point &pointA() const {return a;}
   const Point &pointB() const {return b;}
   const Point &pointC() const {return c;}

   void setA( const Point &newA ) {a = newA;} 
   void setB( const Point &newB ) {b = newB;}
   void setC( const Point &newC ) {c = newC;} 

private:
   Point a, b, c;
};


#endif // TRIANGLE_H

