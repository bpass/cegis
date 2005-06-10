/**
 * @file FittingCurve.h
 * @author Austin Hartman
 *
 * $Id: FittingCurve.h,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

#ifndef AUSTIN_FITTINGCURVE_H
#define AUSTIN_FITTINGCURVE_H

#include <vector>

#include "Point2D.h"

template<class T>
class FittingCurve
{
public:
    typedef std::vector<Point2D<T> > Points;
    virtual void setPoints(const Points& points) = 0;
    virtual T operator()(const T& x) const = 0;
    virtual ~FittingCurve() = 0;
};

#include "FittingCurve.hpp"

#endif

