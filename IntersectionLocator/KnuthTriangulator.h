/**
 * @file KnuthTriangulator.h
 * 
 * $Id: KnuthTriangulator.h,v 1.2 2005/02/09 17:53:39 ahartman Exp $
 */

#ifndef KNUTHTRIANGULATOR_H
#define KNUTHTRIANGULATOR_H

#include <vector>
#include <list>
#include "DelauneyTriangulator.h"

using std::vector;
using std::list;

class KnuthTriangulator : public DelauneyTriangulator
{
public:
   virtual ~KnuthTriangulator() {}

   virtual void triangulate( const std::vector<ControlPoint>& points,
                             Triangulation& result );
private:
   struct TriState
   {
   public:
      TriState()
          : newTriStates(3), old(false)
      {}

      Tri tri;
      bool old;
      vector<TriState*> newTriStates;
   }
   
   list<TriState> triStates;
   
   Point infA, infB, infC;
   static const int infALocation, infBLocation, infCLocation;

   /**
    * Creates an infinite triangle.
    */
   void KnuthTriangulator::createInfiniteTri( const std::vector<ControlPoint>& points,
                                              Triangulation& edges );

   /**
    * Searches through the triangulations for a triangle that contains the
    * point until it finds one that is in the current triangulation.
    * @param point The point to locate inside the current triangulation.
    * @edges The current and old triangulations.
    */
   Triangulation::iterator inTriangle( const ControlPoint &point,
                                       Triangulation &edges )

   /**
    * Tests whether pointD is inside the circumcircle of the triangle
    * defined by pointA, pointB, and pointC.
    * @return True if it is inside and false if not.
    */
   bool inCircumcircle( const Point& pointA,
                        const Point& pointB,
                        const Point& pointC,
                        const Point& pointD );

   /**
    * Returns x^2 + y^2 where x and y are the coordinates of point.  This
    * is a helper function of inCircumcircle.
    */
   double magnitudeSquared( const Point& point );

   /**
    * Returns the determinant of the matrix defined by the 9 parameters
    * which are listed in row major order.  This is a helper function of 
    * inCircumcircle.
    */
   double det3( double a11, double a12, double a13,
                double a21, double a22, double a23,
                double a31, double a32, double a33 )
};

#endif

