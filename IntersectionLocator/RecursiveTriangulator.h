/*
 * File: RecursiveTriangulator.h
 * Purpose: This file contains the declaration of a class for
 *          recursively performing a O(nlogn) Delauney triangulation.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-18-04
 */

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////                                                                         
// Important note:
//
// This class doesn't work at all at the moment.  I might fix it
// in the future, but the QuarticTriangulator is doing the task
// so this is on the scrap heap
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// NOTE
// This does not work.  It returns all triangles with one vertice
// at 0, 0.




#ifndef RECURSIVETRIANGULATOR_H
#define RECURSIVETRIANGULATOR_H

#include <algorithm>

#include "DelauneyTriangulator.h"

struct RDTEdge
{
   RDTEdge( int from, int to ) : from(from), to(to) {}
   int from, to;
};


class RecursiveTriangulator : public DelauneyTriangulator
{
public:
   virtual ~RecursiveTriangulator() {}

   virtual void triangulate( const std::vector<ControlPoint> &points,
                             Triangulation &edges );

private:
   void insert( const ControlPoint &point, int index, 
                const std::vector<ControlPoint> &points,
                Triangulation &edges );
   void swapTest( int p, int a, int b, 
                  const std::vector<ControlPoint> &points,
                  Triangulation &edges );
   Triangulation::iterator inTriangle( const ControlPoint &point, 
                    Triangulation &edges );
   int rightOf( int from, int to, 
                const std::vector<ControlPoint> &points,
                Triangulation &edges );
   bool circleTest( int p, int a, int b, int n, 
                    const std::vector<ControlPoint> &points,
                    Triangulation &edges );
   void flip( int p, int a, int b, int n, 
              const std::vector<ControlPoint> &points,
              Triangulation &edges );
   Triangulation::iterator findTri( int a, int b, int c,
                                    Triangulation &edges );
   void createInfiniteTri( const std::vector<ControlPoint> &point );
   void cleanTriangulation( Triangulation &edges );

   Point infA, infB, infC;
};

#endif //RECURSIVETRIANGULATOR_H
