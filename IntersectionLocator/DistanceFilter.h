/*
 * File: DistanceFilter.h
 * Purpose: This file contains the declaration of a class
 *          for a control point filter.  This filter uses the
 *          magnitude of each control point's displacement
 *          vector to filter out control points that seem
 *          worse than others.  This class implements the
 *          Filter interface.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-24-04
 */


#ifndef DISTANCEFILTER_H
#define DISTANCEFILTER_H

#include <algorithm>

#include "Filter.h"
#include "Point.h"

/*
 * DistanceFilter is a filter that removes info*point.size()
 * control points that are furthest from their original points.
 */

class DistanceFilter : public Filter
{
public:
   virtual ~DistanceFilter() {}

   virtual void filter( std::vector<ControlPoint> &point, 
                        const double &info );
};


#endif //DISTANCEFILTER_H
