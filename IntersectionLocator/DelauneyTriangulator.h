/*
 * File: DelauneyTriangulator.h
 * Purpose: This file contains the declaration of an interface to an 
 *          object that encapsulates an algorithm for performing a 
 *          Delauney triangulation.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-12-04
 */


#ifndef DELAUNEYTRIANGULATOR_H
#define DELAUNEYTRIANGULATOR_H

#pragma warning(disable: 4786)

#include <vector>
#include <algorithm>
#include "ogrsf_frmts.h"

#include "Point.h"
#include "Circle.h"
#include "Triangle.h"
#include "ControlPoint.h"

/*
 * Tri is a struct for containing triangles.  Each Tri has a Triangle
 * and ints a, b, c, which represent the indexes of the points in 
 * the triangle from the points vector that is passed to triangulate
 * in the DelauneyTriangulator interface.
 */
struct Tri
{
   int a, b, c;
   Triangle triangle;
};

// A generalization of a triangulation to use for returning Tri's.
typedef std::vector<Tri> Triangulation;


/* 
 * This is an interface for Triangulators.
 *
 * void triangulate( const std::vector<ControlPoint> &points,
 *                   Triangulation &edges )
 * This pure virtual function should be defined in concrete subclasses
 * to take in a set of ControlPoints called points and create a
 * triangulation in edges for those points.
 *
 * void outputTriangles( OGRDataSource *pDS,
 *                       OGRSpatialReference &mToSR,
 *                       const Triangulation &triangles )
 * This is not a virtual function, it is defined for each triangulator
 * so that there is a way to test out new triangulators that you
 * may be designing.  The output isn't pretty, each line can be
 * output several times, but at least it is already written.
 * Pre: pDS should be a writable datasource, mToSR should be
 *      the spatial reference to use in the datasource and
 *      triangles is a Triangulation that has already been 
 *      generated in triangulate.
 * Post: The triangulation is dumped to the pDS file.
 */

class DelauneyTriangulator
{
public:
   virtual ~DelauneyTriangulator() {}

   virtual void triangulate( const std::vector<ControlPoint> &points, 
                             Triangulation &edges ) = 0;

   void outputTriangles( OGRDataSource *pDS, 
                         OGRSpatialReference &mToSR,
                         const Triangulation &triangles );

};


#endif // DELAUNEYTRIANGULATOR_H
