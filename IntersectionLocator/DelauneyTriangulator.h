/**
 * @file DelauneyTriangulator.h
 * Purpose: This file contains the declaration of an interface to an 
 *          object that encapsulates an algorithm for performing a 
 *          Delauney triangulation.
 *
 * @author Ryan Stelzleni
 * @date 8-12-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: DelauneyTriangulator.h,v 1.4 2004/11/02 01:20:46 rstelzleni Exp $


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

/**
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

/// A generalization of a triangulation to use for returning Tri's.
typedef std::vector<Tri> Triangulation;


/**
 * This is an interface for Triangulators.
 */
class DelauneyTriangulator
{
public:
   virtual ~DelauneyTriangulator() {}

   /** 
    * This pure virtual function should be defined in concrete subclasses
    * to take in a set of ControlPoints called points and create a
    * triangulation in edges for those points.
    * @param points The vector of control points to triangulate
    * @param result The resulting triangulation, this is an output parameter.
    */
   virtual void triangulate( const std::vector<ControlPoint> &points, 
                             Triangulation &result ) = 0;

    /**
    * This is not a virtual function, it is defined for each triangulator
    * so that there is a way to test out new triangulators that you
    * may be designing.  The output isn't pretty, each line can be
    * output several times, but at least it is already written.
    * Pre: pDS should be a writable datasource, mToSR should be
    *      the spatial reference to use in the datasource and
    *      triangles is a Triangulation that has already been 
    *      generated in triangulate.
    * Post: The triangulation is dumped to the pDS file.
    * @param pDS A writable datasource to output a set of vector lines
    *        to.
    * @param mToSR An OGRSpatialReference to be used for the output file.
    * @param triangles The triangulation to be output.
    */
   void outputTriangles( OGRDataSource *pDS, 
                         OGRSpatialReference &mToSR,
                         const Triangulation &triangles );

};


#endif // DELAUNEYTRIANGULATOR_H
