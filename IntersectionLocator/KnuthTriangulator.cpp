/**
 * @file KnuthTriangulator.cpp
 *
 * $Id: KnuthTriangulator.cpp,v 1.1 2005/02/09 16:29:06 ahartman Exp $
 */

#include "KnuthTriangulator.h"

KnuthTriangulator::infALocation = -1;
KnuthTriangulator::infBLocation = -2;
KnuthTriangulator::infCLocation = -3;

KnuthTriangulator::~KnuthTriangulator()
{}

KnuthTriangulator::triangulate( const std::vector<ControlPoint>& points,
                                Triangulation& result )
{
   createInfiniteTri( points, edges );
}

void
KnuthTriangulator::createInfiniteTri( const std::vector<ControlPoint>& points,
                                      Triangulation& edges )
{
   double maxX, minX, maxY, minY, x, y, dX, dY;

   // find the maximum and minimum x and y values that occur inside the
   // set of points
   if( points.size() >= 1 )
   {
      minX = minY = maxY = maxX = points[0].point.getX();
   }
   else
   {
      minX = minY = maxY = maxX = 0;
   }

   for( unsigned int i=1; i<points.size(); ++i )
   {
      x = points[i].point.getX();
      y = points[i].point.getY();

      if( minX > x ) minX = x;
      if( maxX < x ) maxX = x;
      if( minY > y ) minY = y;
      if( maxY < y ) maxY = y;
   }

   // Create a buffer triangle around the points
   // Shouldn't cause a problem if min or max is a negative number
   dX = maxX - minX;
   dY = maxY - minY;
   
   halfDX = dX / 2;
   halfDY = dY / 2;
  
   posInfX = minX + dX * 4;
   negInfX = minX - dX;
   posInfY = minY + dY * 4;
   negInfY = minY - dY;
   
   infA.setX( negInfX );
   infA.setY( negInfY );
   
   infB.setX( posInfX );
   infB.setY( negInfY );
  
   infC.setX( negInfX );
   infC.setY( posInfY );

   // insert the infinite triangle into the triangulation
   TriState infTriState;
   infTriState.triangle.setA( infA );
   infTriState.triangle.setB( infB );
   infTriState.triangle.setC( infC );
   infTriState.tri.a = infALocation;
   infTriState.tri.b = infBLocation;
   infTriState.tri.c = infCLocation;
}

Triangulation::iterator KnuthTriangulator::inTriangle( 
                                        const ControlPoint &point,
                                        Triangulation &edges )
{
   // XXX This function is incomplete for now.  It will have to be changed
   // to recursively go through the triangulations until it gets to a 
   // triangle that is in the current triangulation
    
   // using notation like Saalfield 
   // A*y1 + B*x1 + C is the line's equation
   double A, B, C;
   Point dPointA, dPointB, dPointC;
   Triangulation::iterator iter;

   for( iter = edges.begin(); iter != edges.end(); ++iter )
   {
      Triangle T = iter->triangle;
      if( iter->a == -1 )
         dPointA = infA;
      else if( iter->a == -2 )
         dPointA = infB;
      else if( iter->a == -3 )
         dPointA = infC;
      else
         dPointA = T.pointA();

      if( iter->b == -1 )
         dPointB = infA;
      else if( iter->b == -2 )
         dPointB = infB;
      else if( iter->b == -3 )
         dPointB = infC;
      else
         dPointB = T.pointB();

      if( iter->c == -1 )
         dPointC = infA;
      else if( iter->c == -2 )
         dPointC = infB;
      else if( iter->c == -3 )
         dPointC = infC;
      else
         dPointC = T.pointC();
      
      A = dPointB.y() - dPointA.y();
      B = dPointA.x() - dPointB.x();
      C = dPointB.x() * dPointA.y() - dPointA.x() * dPointB.y();

      // Two points are on the same side of the line if and only if
      // A*x + B*y + C has the same sign for each point
      if( ( (A*dPointC.x() + B*dPointC.y() + C) *
            (A*point.point.getX() + B*point.point.getY() + C) )
            > 0 )
      {
         A = dPointC.y() - dPointB.y();
         B = dPointB.x() - dPointC.x();
         C = dPointC.x() * dPointB.y() - dPointB.x() * dPointC.y();

         if( ( (A*dPointA.x() + B*dPointA.y() + C) *
               (A*point.point.getX() + B*point.point.getY() + C) )
               > 0 )
         {
            A = dPointC.y() - dPointA.y();
            B = dPointA.x() - dPointC.x();
            C = dPointC.x() * dPointA.y() - dPointA.x() * dPointC.y();

            if( ( (A*dPointB.x() + B*dPointB.y() + C) *
                  (A*point.point.getX() + B*point.point.getY() + C) )
                  > 0 )
            {
               return iter;
            }
         }
      }
   } // end for loop
   
   // Maybe throw exception here instead of returning the end value
   return edges.end();

} //  end function inTriangle

