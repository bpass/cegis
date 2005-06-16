/**
 * @file PolynomialCurve.h
 * @author Austin Hartman
 *
 * $Id: PolynomialCurve.h,v 1.1 2005/06/16 21:23:28 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIALCURVE_H
#define AUSTIN_POLYNOMIALCURVE_H

#include "FittingCurve.h"

template<class T>
class PolynomialCurve : public FittingCurve<T>
{
public:
//    PolynomialCurve(const size_t order = m_defaultOrder);
    PolynomialCurve(const typename FittingCurve<T>::Points& points,
                    const size_t order = m_defaultOrder);

    virtual void setPoints(const typename FittingCurve<T>::Points& points);
//    void setOrder(const size_t order);
    void setPointsAndOrder(const typename FittingCurve<T>::Points& points,
                           const size_t order);
    virtual T operator()(const T& x) const;
    const T& getCoefficient(const size_t coefficient) const;

    virtual ~PolynomialCurve();

    class TooFewPoints
    {};

private:
    void findSolution(const typename FittingCurve<T>::Points& points);
    T simplePow(const T& base, const size_t exponent);
//    size_t m_order;
    std::vector<T> m_coefficients;
    static const size_t m_defaultOrder;
};

#include "PolynomialCurve.hpp"

#endif

