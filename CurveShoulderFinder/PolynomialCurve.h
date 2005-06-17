/**
 * @file PolynomialCurve.h
 * @author Austin Hartman
 *
 * $Id: PolynomialCurve.h,v 1.3 2005/06/17 01:42:57 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIALCURVE_H
#define AUSTIN_POLYNOMIALCURVE_H

#include "FittingCurve.h"
#include "PolynomialRegression.h"

template<class T>
class PolynomialCurve : public FittingCurve<T>
{
public:
    PolynomialCurve(const typename FittingCurve<T>::Points& points,
                    const size_t order = m_defaultOrder);

    virtual void setPoints(const typename FittingCurve<T>::Points& points);
    void setPointsAndOrder(const typename FittingCurve<T>::Points& points,
                           const size_t order);
    virtual T operator()(const T& x) const;
    const T& getCoefficient(const size_t coefficient) const;

    virtual ~PolynomialCurve();

    class TooFewPoints
    {};

private:
    void findSolution(const typename FittingCurve<T>::Points& points);
    typename PolynomialRegression<T>::Coefficients m_coefficients;
    static const size_t m_defaultOrder;
};

#include "PolynomialCurve.hpp"

#endif

