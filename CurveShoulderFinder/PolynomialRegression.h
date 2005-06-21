/**
 * @file PolynomialRegression.h
 * @author Austin Hartman
 *
 * $Id: PolynomialRegression.h,v 1.2 2005/06/21 23:01:05 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIALREGRESSION_H
#define AUSTIN_POLYNOMIALREGRESSION_H

#include <vector>
#include "Point2D.h"

/**
 * A functor to implement polynomial regression.  
 */
template<class T>
class PolynomialRegression
{
public:
    /**
     * A set of points.
     */ 
    typedef std::vector<Point2D<T> > Points;

    /**
     * The set of coefficients for the polynomial which will be listed in
     * increasing degree starting with the 0-degree term.
     */
    typedef std::vector<T> Coefficients;

    /**
     * Finds a polynomial of the specified order that minimizes the sum
     * of the squares of the residuals.
     *
     * @param points A set of points that the function will fit a polynomial 
     * to.
     * @param order The order of the polynomial that will be fit to the
     * points.  E.g., if order is 3, the polynomial will be of the form
     * a0 + a1*x^1 + a2*x^2 + a3*x^3.
     *
     * @return A set of order + 1 coefficients that correspond to terms of
     * increasing power.  I.e., the value at position 0 is the coefficient
     * for x^0, the value at position 1 is the coefficient for x^1, etc.
     *
     * @exception TooFewPoints Thrown if the number of points is less than
     * order + 1.
     */
    Coefficients operator()(const Points& points,
                            const size_t order) const;

    /**
     * Exception class used when the user does not have specify enough
     * points to fit a polynomial curve of the specified size.
     */
    class TooFewPoints
    {};

private:
    /**
     * A simple function to return base^exponent.
     *
     * @param base The base of the power operation.  It will be multiplied
     * by itself exponent times.
     * @param exponent The exponent referred to above.
     *
     * @return The result of the power operation.
     */
    T simplePow(const T& base, const size_t exponent) const;
};

#include "PolynomialRegression.hpp"

#endif

