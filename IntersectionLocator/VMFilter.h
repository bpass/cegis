/*
 * File: VMFilter.h
 * Purpose: This file contains the declaration of a class
 *          for a control point filter.  This filter uses the
 *          median vector to locate outlyers.  The class 
 *          defined here implements the Filter interface.
 *
 * Programmer: Ryan Stelzleni
 * Date: 8-23-04
 */


#ifndef VMFILTER_H
#define VMFILTER_H

#include "Filter.h"



/*
 * The VMFilter class represents a Vector Median Filter for
 * removing bad data points.  This filter locates the median
 * vector from a set of displacement vectors for the intersections
 * and removes info*point.size() vectors that are furthest 
 * from the median.
 *
 * void filter( std::vector<ControlPoint> &point,
 *              const double &info )
 * This is the function that performs the filtration method
 * described above.  Note that info should be a value in the
 * range [0,1], where 0 would remove no points, 1 would remove
 * all points, and 0.5 would remove 50% of the points.  The
 * function doesn't check to see if info is valid and if passed
 * and info outside the range then the behavior can be thought
 * of as undefined.
 * Pre: point should contain identified ControlPoints and
 *      info should be in the range [0,1].
 * Post: The info*points.size() control points with displacement
 *       furthest from the vector median are removed.
 */

class VMFilter : public Filter
{
public:
   virtual ~VMFilter() {}

   virtual void filter( std::vector<ControlPoint> &point, 
                        const double &info );
};

#endif //VMFILTER_H
