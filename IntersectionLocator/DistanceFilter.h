/**
 * @file DistanceFilter.h
 * Purpose: This file contains the declaration of a class
 *          for a control point filter.  This filter uses the
 *          magnitude of each control point's displacement
 *          vector to filter out control points that seem
 *          worse than others.  This class implements the
 *          Filter interface.
 *
 * @author Ryan Stelzleni
 * @date 8-24-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: DistanceFilter.h,v 1.3 2004/10/22 13:30:26 rstelzleni Exp $


#ifndef DISTANCEFILTER_H
#define DISTANCEFILTER_H

#include <algorithm>

#include "Filter.h"
#include "Point.h"

/**
 * DistanceFilter is a filter that removes info*point.size()
 * control points that are furthest from their original points.
 */

class DistanceFilter : public Filter
{
public:
   virtual ~DistanceFilter() {}

   /**
    * This function will take the given vector of ControlPoint types
    * and will remove info*point.size() number of points that are
    * furthest from the original points.  The result is a filter
    * based on magnitude of displacement only.
    * @param point The set of points to be filtered.
    * @param info The ratio to be removed, for instance, .2 is 20% 
    *        of the points filtered out.  This should be in the range
    *        [0,1].
    */
   virtual void filter( std::vector<ControlPoint> &point, 
                        const double &info );
};


#endif //DISTANCEFILTER_H
