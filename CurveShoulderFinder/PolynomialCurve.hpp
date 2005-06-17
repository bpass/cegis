/**
 * @file PolynomialCurve.hpp
 * @author Austin Hartman
 *
 * $Id: PolynomialCurve.hpp,v 1.4 2005/06/17 01:42:57 ahartman Exp $
 */

#ifdef AUSTIN_POLYNOMIALCURVE_H
#ifndef AUSTIN_POLYNOMIALCURVE_HPP
#define AUSTIN_POLYNOMIALCURVE_HPP

#include "DenseMatrix.h"
#include "MyVector.h"
#include "PartialPivotingGaussianSolver.h"

//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <iostream>
#endif

template<class T>
const size_t PolynomialCurve<T>::m_defaultOrder = 2;

template<class T>
PolynomialCurve<T>::
PolynomialCurve(const typename FittingCurve<T>::Points& points,
                const size_t order)
    : m_coefficients(order + 1)
{
    findSolution(points);
}

template<class T>
void
PolynomialCurve<T>::
setPoints(const typename FittingCurve<T>::Points& points)
{
    findSolution(points);
}

template<class T>
void
PolynomialCurve<T>::
setPointsAndOrder(const typename FittingCurve<T>::Points& points,
                  const size_t order)
{
    m_coefficients.resize(order + 1);
    findSolution(points);
}

template<class T>
T
PolynomialCurve<T>::operator()(const T& x) const
{
    const size_t numCoefficients = m_coefficients.size();
    T sum = m_coefficients[0];
    T xPow = 1;

    for(size_t i = 1; i < numCoefficients; ++i)
    {
        xPow *= x;
        sum += m_coefficients[i] * xPow;
    }

    return sum;
}

template<class T>
const T&
PolynomialCurve<T>::
getCoefficient(const size_t coefficient) const
{
    return m_coefficients[coefficient];
}

template<class T>
PolynomialCurve<T>::~PolynomialCurve()
{}

template<class T>
void
PolynomialCurve<T>::
findSolution(const typename FittingCurve<T>::Points& points)
{
    PolynomialRegression<T> polyRegress;
    m_coefficients = polyRegress(points, m_coefficients.size() - 1);
}

#ifdef DEBUG_PRINT
#undef DEBUG_PRINT
#endif

#endif
#endif

