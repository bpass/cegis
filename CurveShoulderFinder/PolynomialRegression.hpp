/**
 * @file PolynomialRegression.hpp
 * @author Austin Hartman
 *
 * $Id: PolynomialRegression.hpp,v 1.1 2005/06/17 01:42:57 ahartman Exp $
 */

#ifdef AUSTIN_POLYNOMIALREGRESSION_H
#ifndef AUSTIN_POLYNOMIALREGRESSION_HPP
#define AUSTIN_POLYNOMIALREGRESSION_HPP

#include "DenseMatrix.h"
#include "MyVector.h"
#include "PartialPivotingGaussianSolver.h"

//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <iostream>
#endif

template<class T>
typename PolynomialRegression<T>::Coefficients
PolynomialRegression<T>::
operator()(const typename PolynomialRegression<T>::Points& points,
           const size_t order) const
{
#ifdef DEBUG_PRINT
    using std::cout;
    using std::setw;
    const size_t width = cout.precision() + 4;
#endif

    const size_t numCoefficients = order + 1;
    const size_t numPoints = points.size();

    if(numPoints < numCoefficients)
    {
        throw typename PolynomialRegression<T>::TooFewPoints();
    }

    // create the matrix and solution vector that correspond to the normal
    // equations
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
    typename PolynomialRegression<T>::Coefficients coefficients(x.getSize());
    for(size_t i = 0; i < coefficients.size(); ++i)
    {
        coefficients[i] = x[i];
    }

    return coefficients;
}

template<class T>
T
PolynomialRegression<T>::
simplePow(const T& base, const size_t exponent) const
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

