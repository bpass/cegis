/*
 * File: QuarticTriangulator.h
 * Purpose: This file contains the declaration of a class for
 *          performing a O(n^4) Delauney triangulation.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-16-04
 */


#ifndef QUARTICTRIANGULATOR_H
#define QUARTICTRIANGULATOR_H

#pragma warning(disable: 4786)

#include "DelauneyTriangulator.h"


/*
 * The QuarticTriangulator class implements the DelauneyTriangulator
 * interface to triangulate a set of ControlPoints.  The triangulation
 * is output through the Triangulation &result.
 *
 * void triangulate( const std::vector<ControlPoint> &points,
 *                   Triangulation &result );
 * This function performs the triangulation.
 * Pre: points should already contain identified control points.
 * Post: result will contain a Triangulation (defined in 
 *       DelauneyTriangulator.h) of the control points.
 */

class QuarticTriangulator : public DelauneyTriangulator
{
public:
   virtual ~QuarticTriangulator() {}

   virtual void triangulate( const std::vector<ControlPoint> &points, 
                             Triangulation &result );
};


#endif //QUARTICTRIANGULATOR_H
