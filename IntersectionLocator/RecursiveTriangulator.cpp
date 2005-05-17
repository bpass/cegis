/*
 * File: RecursiveTriangulator.h
 * Purpose: This file contains the implementation of a class for
 *          recursively performing a O(nlogn) Delauney triangulation.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-18-04
 *
 * This code was written for the United States Geological Survey.
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




// Note:
// Negative values will cause problems in the createInfiniteTriangle
// function.


// Majic numbers for CVS
// $Id: RecursiveTriangulator.cpp,v 1.6 2005/05/17 02:05:02 ahartman Exp $


#include "RecursiveTriangulator.h"
#include <assert.h>



void RecursiveTriangulator::triangulate( 
                          const std::vector<ControlPoint> &points,
                          Triangulation &result )
{
   // Initialization
   createInfiniteTri( points );

   // Perform on each point (this should really be randomized, but
   // I don't think I'll lose anything by just doing it this way.)
   for( unsigned int i=0; i<points.size(); ++i )
      insert( points[i], i, points, result );

   // Get rid of triangles that go to infinity
   cleanTriangulation( result );

   return;
} // end function triangulate



// Note pT doesn't need to be dynamically allocated anymore
void RecursiveTriangulator::insert( const ControlPoint &point,
                                    int index,
                                    const std::vector<ControlPoint> &points,
                                    Triangulation &edges )
{
   // Get the enclosing triangle
   Triangulation::iterator iter = inTriangle( point, edges );
   Tri *pT = new Tri;
   if( iter != edges.end() )
   {
      pT->triangle = iter->triangle;
      pT->a = iter->a;
      pT->b = iter->b;
      pT->c = iter->c;
      // remove the old triangle that is split up now.
      edges.erase( iter );
   }
   else
   {
      // should create a triangle with 0, 0 points
      pT->a = -1;
      pT->b = -2;
      pT->c = -3;
   }
      

   // First create triangle a-c-index
   Tri newTri;
   Point newPoint( points[index].point.getX(), 
                   points[index].point.getY() );
   Triangle newTriangle( pT->triangle.pointA(), 
                         pT->triangle.pointC(), 
                         newPoint );
   newTri.triangle = newTriangle;
   newTri.a = pT->a;
   newTri.b = pT->c;
   newTri.c = index;
   edges.push_back( newTri );

   // Next create triangle a-b-index
   newTriangle.setB( pT->triangle.pointB() );
   newTri.triangle = newTriangle;
   newTri.b = pT->b;
   edges.push_back( newTri );

   // Lastly create triangle c-b-index
   newTriangle.setA( pT->triangle.pointC() );
   newTri.triangle = newTriangle;
   newTri.a = pT->c;
   edges.push_back( newTri );


   // See if the three new interior triangles are valid
   swapTest( index, pT->a, pT->b, points, edges );
   swapTest( index, pT->b, pT->c, points, edges );
   swapTest( index, pT->c, pT->a, points, edges );
   

   delete pT;

   return;
} // end function insert


void RecursiveTriangulator::swapTest( int p, int a, int b, 
                                      const std::vector<ControlPoint> &points,
                                      Triangulation &edges )
{
   //assert( a >= 0 && b >= 0 );
   int i = rightOf(  a, b, points, edges );

   if( circleTest( p, a, b, i, points, edges ) )
   {
      flip( p, a, b, i, points, edges );
      swapTest( p, a, i, points, edges );
      swapTest( p, b, i, points, edges );
   }

   return;
} // end function swapTest


bool RecursiveTriangulator::circleTest( int p, int a, int b, int n,
                                      const std::vector<ControlPoint> &points,
                                      Triangulation &edges )
{
   Point A( points[p].point.getX(), 
            points[p].point.getY() );
   Point B( points[a].point.getX(), 
            points[a].point.getY() );
   Point C( points[b].point.getX(), 
            points[b].point.getY() );
   Point D;
   if( n != -1 && n != -2 && n != -3 )
   {
      D.setX( points[n].point.getX() );
      D.setY( points[n].point.getY() );
   }
   else
   {
      // I don't think it matters what infinity you use here,
      // it shouldn't ever be close enough to hurt
      // i.e. a point at infinity is never in a circumcircle
      return false;
   }

   Circle circle( Triangle( A, B, C ) );

   return circle.encloses( D );
} // end function circleTest

Triangulation::iterator RecursiveTriangulator::inTriangle( 
                                        const ControlPoint &point,
                                        Triangulation &edges )
{
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
      
      A = (dPointB.x() - dPointA.x()) / 2;
      B = (dPointA.y() - dPointB.y()) / 2;
      C = (dPointB.y() * dPointA.x() - 
           dPointA.y() * dPointB.x()) / 2;

      // Two points are on the same side of the line if and only if
      // A*x + B*y + C has the same sign for each point
      if( ( A*dPointC.x() + B*dPointC.y() + C /
            A*point.point.getX() + 
              B*point.point.getY() + C )
            > 0 )
      {
         A = (dPointB.x() - dPointC.x()) / 2;
         B = (dPointC.y() - dPointB.y()) / 2;
         C = (dPointB.y() * dPointC.x() - 
              dPointC.y() * dPointB.x()) / 2;

         if( ( A*dPointA.x() + B*dPointA.y() + C /
               A*point.point.getX() + 
                 B*point.point.getY() + C )
               > 0 )
         {
            A = (dPointA.x() - dPointC.x()) / 2;
            B = (dPointC.y() - dPointA.y()) / 2;
            C = (dPointA.y() * dPointC.x() - 
                 dPointC.y() * dPointA.x()) / 2;

            if( ( A*dPointB.x() + B*dPointB.y() + C /
                  A*point.point.getX() + 
                    B*point.point.getY() + C )
                  > 0 )
            {
               return iter;
            }
         }
      }
   } // end for loop
   
   return edges.end();
} //  end function inTriangle



int RecursiveTriangulator::rightOf( int from, int to, 
                                    const std::vector<ControlPoint> &points,
                                    Triangulation &edges )
{
   // This means right of an edge when traversing the triangle in 
   // counterclockwise order.  The index returned is the index
   // of the point that is in the triangle that shares the edge 
   // passed in.
   for( unsigned int i=0; i<edges.size(); ++i )
   {
      if( from == edges[i].a && to == edges[i].b ||
          to == edges[i].a && from == edges[i].b )
         return edges[i].c;
      else if( from == edges[i].a && to == edges[i].c ||
               to == edges[i].a && from == edges[i].c )
         return edges[i].b;
      else if( from == edges[i].b && to == edges[i].c ||
               to == edges[i].b && from == edges[i].c )
         return edges[i].a;
   }

   return -1;
} // end function rightOf

void RecursiveTriangulator::flip( int p, int a, int b, int n,
                                  const std::vector<ControlPoint> &points,
                                  Triangulation &edges )
{ 
   Triangulation::iterator oldTriOne, oldTriTwo;
   Tri newTri;

   oldTriOne = findTri( a, b, n, edges );
   oldTriTwo = findTri( a, b, p, edges );

   //assert( oldTriOne != edges.end() && oldTriTwo != edges.end() );

   // Create triangle npb
   Triangle newTriangle( oldTriOne->triangle.pointC(), 
                         oldTriTwo->triangle.pointC(),
                         oldTriTwo->triangle.pointB() );
   newTri.triangle = newTriangle;
   newTri.a = n;
   newTri.b = p;
   newTri.c = b;
   edges.push_back( newTri );

   // create triangle npa
   newTri.triangle.setC( oldTriTwo->triangle.pointA() );
   newTri.c = a;
   edges.push_back( newTri );

   // get rid of triangles abn and abp
   edges.erase( oldTriOne );
   edges.erase( oldTriTwo );

   return;
} // end function flip


Triangulation::iterator RecursiveTriangulator::findTri( int a, int b, int c,
                                                        Triangulation &edges )
{
   Triangulation::iterator iter;

   for( iter = edges.begin(); iter != edges.end(); ++iter )
   {
      if( (a == iter->a && b == iter->b && c == iter->c) ||
          (a == iter->a && c == iter->b && b == iter->c) ||
          (b == iter->a && a == iter->b && c == iter->c) ||
          (b == iter->a && c == iter->b && a == iter->c) ||
          (c == iter->a && a == iter->b && b == iter->c) ||
          (c == iter->a && b == iter->b && a == iter->c) )
         return iter;
   }

   return edges.end();
} // end function findTri


// Create vertices for a triangle that completely encloses the points
// set.
void RecursiveTriangulator::createInfiniteTri( 
                      const std::vector<ControlPoint> &points )
{
   double maxX, minX, maxY, minY, x, y, dX, dY;

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

   // create a buffer around the points
   // This causes a problem if the min or max is a negative number
   if( minX < maxX )
   {
      minX *= 0.1;
      maxX *= 10;
      dX = maxX - minX;
   }
   else
   {
      minX *= 10;
      maxX *= 0.1;
      dX = minX - maxX;
   }
   if( minY < maxY )
   {
      minY *= 0.1;
      maxY *= 10;
      dY = maxY - minY;
   }
   else
   {
      minY *= 10;
      maxY *= 0.1;
      dY = minY - maxY;
   }

   infA.setX( minX );
   infA.setY( minY );

   infB.setX( 2*dX );
   infB.setY( minY );

   infC.setX( minX );
   infC.setY( 2*dY );

   return;
} // end function createInfiniteTri


// Predicate for the remove_if in cleanTriangulation
static bool isNoGood( const Tri &T )
{
   return ( T.a < 0 || T.b < 0 || T.c < 0 );
}


void RecursiveTriangulator::cleanTriangulation( Triangulation &edges )
{
   Triangulation::iterator end;

   end = std::remove_if( edges.begin(), edges.end(), isNoGood );
   edges.erase( end, edges.end() );

   return;
} // end function cleanTriangulation
