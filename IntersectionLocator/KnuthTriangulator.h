/**
 * @file KnuthTriangulator.h
 * 
 * $Id: KnuthTriangulator.h,v 1.1 2005/02/09 16:29:06 ahartman Exp $
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
    * @param point The point to locate inside the current triangulation
    * @edges The current and old triangulations
    */
   Triangulation::iterator inTriangle( const ControlPoint &point,
                                       Triangulation &edges )
};

#endif

