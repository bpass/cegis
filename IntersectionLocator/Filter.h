/**
 * @file Filter.h
 * Purpose: This file contains the declaration of an interface
 *          for a control point filter.  The filter is intended
 *          to pick out the best identified points in the image.
 *          I am using an abstract base class because I expect
 *          to need to use a variety of filters for testing
 *          purposes at least.
 *
 * @author Ryan Stelzleni
 * @date 8-23-04
 *
 * This code was written for the United States Geological Survey.
 */


// Majic numbers for CVS
// $Id: Filter.h,v 1.3 2004/10/22 13:30:26 rstelzleni Exp $


#ifndef FILTER_H
#define FILTER_H

#pragma warning(disable: 4786)

#include <vector>

#include "ControlPoint.h"

/**
 * This class is an interface for control point filters.  The
 * concrete subclasses of this interface should implement
 * the filter function to remove poorly identified control
 * points.  I called the second argument to this function info
 * to keep it general, but usually this will be used as a ratio
 * of points to be removed.
 */

class Filter
{
public:
   virtual ~Filter() {}

   /**
    * Should be implemented to filter out ControlPoints from point.
    * @param point The vector of ControlPoints to be filtered.
    * @param info A generic extra data argument.  Usually this would
    *        be a percentage to remove.
    */
   virtual void filter( std::vector<ControlPoint> &point, 
                        const double &info  ) = 0;
};

#endif // FILTER_H

