#ifndef AUSTIN_LUDECOMPOSITION_H
#define AUSTIN_LUDECOMPOSITION_H

#include <vector>
#include "Matrix.h"
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
                 const UpperTriangularMatrix<T>& U);
        LowerTriangularMatrix<T>& L();
        const LowerTriangularMatrix<T>& L() const;
        UpperTriangularMatrix<T>& U();
        const UpperTriangularMatrix<T>& U() const;
    private:
        LowerTriangularMatrix<T> m_L;
        UpperTriangularMatrix<T> m_U;
    };

    class MatrixIsSingular
    {};

private:
//    std::vector<size_t> order; 
//    // holds the order of the rows for pivoting, so that they don't actually
//    // need to be moved
//
//    std::vector<T> scales;
//    // holds the value of the largest element in each row

    static const T m_tolerance;
    
    typedef std::vector<T> ScalesType;
    typedef std::vector<size_t> OrderType;

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

