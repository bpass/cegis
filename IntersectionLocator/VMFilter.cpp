/*
 * File: VMFilter.h
 * Purpose: This file contains the implementation of a class
 *          for a control point filter.  This filter uses the
 *          median vector to locate outlyers.  The class 
 *          defined here implements the Filter interface.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-23-04
 */



// Majic numbers for CVS
// $Id: VMFilter.cpp,v 1.4 2004/11/10 18:35:12 ahartman Exp $


#include "Point.h"
#include "VMFilter.h"

// info should be a ratio of points to remove.
void VMFilter::filter( std::vector<ControlPoint> &point, const double &info )
{
   double aveDist, minDist;
   int median, toRemove, m;
   std::vector<ControlPoint>::iterator iter;
   int n = point.size(), numToRemove;
   std::vector<double> distances( n );
   std::vector<double>::iterator distanceIter;

   minDist = -1;

   for( int i=0; i<n; ++i )
   {
      aveDist = 0;

      // Get the displacement vector for this point
      Point current;
      current.setX( point[i].point.getX() - point[i].origPoint.getX() );
      current.setY( point[i].point.getY() - point[i].origPoint.getY() );

      for( int j=0; j<n; ++j )
      {
         if( j != i )
         {
            // Get the displacement vector for every other point
            Point other;
            other.setX( point[j].point.getX() - point[j].origPoint.getX() );
            other.setY( point[j].point.getY() - point[j].origPoint.getY() );
            
            // Sum the distances.
            aveDist += current.distanceTo( other );
         }
      } // end for j

      // The algorithm searches for the displacement vector with the
      // lowest sum of distances to the other vectors.
      if( aveDist < minDist )
      {
         median = i;
         minDist = aveDist;
      }

      distances[i] = aveDist;
   } // end for i

   // 
   // Now remove the "bad" control points
   //
   numToRemove = static_cast<int>(info * n);
   for( int k=0; k<numToRemove; ++k )
   {
      aveDist = distances[0];
      toRemove = 0;
      for( m=1; m < n; ++m )
      {
         if( distances[m] > aveDist )
         {
            toRemove = m;
            aveDist = distances[m];
         }
      } // end for m
      
      // Find the iterator for the point to remove
      for( distanceIter = distances.begin(), iter = point.begin(); 
           toRemove>0; --toRemove, ++iter, ++distanceIter )
         ;

      // Remove this element
      point.erase( iter );
      distances.erase( distanceIter );
      --n;

   } // end for k

   return;
} // end function filter

