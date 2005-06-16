/**
 * @file PolynomialCurve.hpp
 * @author Austin Hartman
 *
 * $Id: PolynomialCurve.hpp,v 1.2 2005/06/16 23:07:04 ahartman Exp $
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
    : /*m_order(order),*/ m_coefficients(order + 1)
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
//    m_order = order;
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
#ifdef DEBUG_PRINT
    using std::cout;
    using std::setw;
    const size_t width = cout.precision() + 4;
#endif

    const size_t numCoefficients = m_coefficients.size();
    if(points.size() < numCoefficients)
    {
        throw typename PolynomialCurve<T>::TooFewPoints();
    }

    // create the matrix that corresponds to the normal equations and the
    // solution vector
    DenseMatrix<T> A(numCoefficients, numCoefficients);
    MyVector<T> b(numCoefficients);
    for(size_t i = 0; i < numCoefficients; ++i)
    {
        // fill in the matrix
        for(size_t j = 0; j <= i; ++j)
        {
            const size_t power = i + j;
            T sum = 0;
            for(size_t k = 0; k < points.size(); ++k)
            {
                sum += simplePow(points[k].x(), power);
            }
            A[i][j] = sum;
            A[j][i] = sum;
        }

        // fill in the solution vector
        T sum = 0;
        for(size_t k = 0; k < points.size(); ++k)
        {
            sum += points[k].y() * simplePow(points[k].x(), i);
        }
        b[i] = sum;
    }

#ifdef DEBUG_PRINT
    cout << __FILE__ << ':' << __LINE__ << ':' << ": "
         << "A =\n" << setw(width) << A 
         << "b =\n" << setw(width) << DenseMatrix<T>(b);
#endif

    // solve the linear system
    PartialPivotingGaussianSolver<T> solver;
    typename PartialPivotingGaussianSolver<T>::Solution solution = 
        solver(A, b);

    // copy the solution to the m_coefficients vector
    MyVector<T>& x = solution.vector();
    for(size_t i = 0; i < m_coefficients.size(); ++i)
    {
        m_coefficients[i] = x[i];
    }
}

template<class T>
T
PolynomialCurve<T>::
simplePow(const T& base, const size_t exponent)
{
    T rv = 1;
    for(size_t i = 1; i <= exponent; ++i)
    {
        rv *= base;
    }
    return rv;
}

#ifdef DEBUG_PRINT
#undef DEBUG_PRINT
#endif

#endif
#endif

