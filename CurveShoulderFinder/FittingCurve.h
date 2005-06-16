/**
 * @file FittingCurve.h
 * @author Austin Hartman
 *
 * $Id: FittingCurve.h,v 1.3 2005/06/16 21:20:00 ahartman Exp $
 */

#ifndef AUSTIN_FITTINGCURVE_H
#define AUSTIN_FITTINGCURVE_H

#include <vector>

#include "Point2D.h"

/**
 * An interface base class for reading values from a curve that fits a 
 * set of points.  Derived classes decide how the curve should fit the
 * points.
 */
template<class T>
class FittingCurve
{
public:
    /**
     * A set of points.
     */
    typedef std::vector<Point2D<T> > Points;

    /**
     * Set the points that the object will generate a curve to fit.
     * @param points The set of points.
     */
    virtual void setPoints(const Points& points) = 0;

    /**
     * A function to get the values along the curve.
     * @param x The value at which to return the value of the curve.
     * @return The value of the curve at the specified value.
     */
    virtual T operator()(const T& x) const = 0;

    /**
     * Pure virtual destructor.
     */
    virtual ~FittingCurve() = 0;
};

#include "FittingCurve.hpp"

#endif

