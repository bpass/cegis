/**
 * @file PolynomialRegression.h
 * @author Austin Hartman
 *
 * $Id: PolynomialRegression.h,v 1.1 2005/06/17 01:42:57 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIALREGRESSION_H
#define AUSTIN_POLYNOMIALREGRESSION_H

#include <vector>
#include "Point2D.h"

template<class T>
class PolynomialRegression
{
public:
    typedef std::vector<Point2D<T> > Points;
    typedef std::vector<T> Coefficients;

    Coefficients operator()(const Points& points,
                            const size_t order) const;

    class TooFewPoints
    {};
private:
    T simplePow(const T& base, const size_t exponent) const;
};

#include "PolynomialRegression.hpp"

#endif

