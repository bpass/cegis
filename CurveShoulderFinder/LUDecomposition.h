/**
 * @file LUDecomposition.h
 * @author Austin Hartman
 *
 * $Id: LUDecomposition.h,v 1.3 2005/06/13 17:33:08 ahartman Exp $
 */

#ifndef AUSTIN_LUDECOMPOSITION_H
#define AUSTIN_LUDECOMPOSITION_H

#include <vector>
#include "Matrix.h"
#include "DenseMatrix.h"
#include "UpperTriangularMatrix.h"
#include "LowerTriangularMatrix.h"

template<class T>
class LUDecomposition
{
public:
    class Solution;
    Solution operator()(const Matrix<T>& matrix, 
                        const T& tolerance = m_tolerance) const;

    class Solution
    {
    public:
        Solution();
        Solution(const LowerTriangularMatrix<T>& L, 
                 const UpperTriangularMatrix<T>& U,
                 const typename LUDecomposition<T>::OrderType& order);
        LowerTriangularMatrix<T>& L();
        const LowerTriangularMatrix<T>& L() const;
        UpperTriangularMatrix<T>& U();
        const UpperTriangularMatrix<T>& U() const;
        DenseMatrix<T> P() const;
        typename LUDecomposition<T>::OrderType& orderVector();
        const typename LUDecomposition<T>::OrderType& orderVector() const;
    private:
        LowerTriangularMatrix<T> m_L;
        UpperTriangularMatrix<T> m_U;
        typename LUDecomposition<T>::OrderType m_order;
    };

    typedef std::vector<size_t> OrderType;

    class MatrixIsSingular
    {};

private:
    static const T m_tolerance;
    
    typedef std::vector<T> ScalesType;

    /**
     * @return a ScalesType where each element contains the element with the
     * largest absolute value in the corresponding row of the matrix
     */
    ScalesType createScales(const Matrix<T>& matrix) const;

    void pivot(const Matrix<T>& matrix, OrderType& order, 
               const ScalesType& scales, const size_t row) const;
};

#include "LUDecomposition.hpp"

#endif

