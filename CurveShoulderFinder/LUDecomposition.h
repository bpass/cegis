/**
 * @file LUDecomposition.h
 * @author Austin Hartman
 *
 * $Id: LUDecomposition.h,v 1.4 2005/06/22 01:38:25 ahartman Exp $
 */

#ifndef AUSTIN_LUDECOMPOSITION_H
#define AUSTIN_LUDECOMPOSITION_H

#include <vector>
#include "Matrix.h"
#include "DenseMatrix.h"
#include "UpperTriangularMatrix.h"
#include "LowerTriangularMatrix.h"

/**
 * A functor to LU decompose a matrix.
 */
template<class T>
class LUDecomposition
{
public:
    class Solution;
    /**
     * The function to find the LU decomposition of a matrix.
     *
     * @param matrix The matrix to decompose.
     * @param tolerance An optional tolerance to detect if the matrix is 
     * singular and thus cannot be LU decomposed.  If it is not specified,
     * the default tolerance will be used.  The tolerance is used by dividing
     * the value of the pivot at each step of decomposing the matrix by the
     * value in the pivot's row that had the largest absolute value in the
     * original matrix.  If that quotient is less than the tolerance, the
     * matrix is judged to be singular.
     *
     * @return An LU decomposition in the form of the Solution class.
     *
     * @exception MatrixIsSingular Thrown if the matrix is determined to be
     * singular.
     */
    Solution operator()(const Matrix<T>& matrix, 
                        const T& tolerance = m_tolerance) const;

    /**
     * Holds a permutated LU decomposition of a matrix.  
     * P()*decomposedMatrix == L()*U().
     */
    class Solution
    {
    public:
        /**
         * Default constructor that initializes L, U, P, and the order vector
         * to be empty.
         */
        Solution();

        /**
         * Constructor to initialize everything as a copy of its parameters.
         *
         * @param L The value of the L lower triangular matrix.
         * @param U The value of the U upper triangular matrix.
         * @param order The order vector whose values at each index give the
         * row where that row is actually contained; i.e., the kth row is
         * contained at the return value of order[k].  For instance, if
         * order[4] == 1, that means the 4th row is actually stored in the
         * first row of the two matrices.
         */
        Solution(const LowerTriangularMatrix<T>& L, 
                 const UpperTriangularMatrix<T>& U,
                 const typename LUDecomposition<T>::OrderType& order);

        //@{
        /**
         * Access the L lower triangular matrix.
         */
        LowerTriangularMatrix<T>& L();
        const LowerTriangularMatrix<T>& L() const;
        //@}

        //@{
        /**
         * Access the U upper triangular matrix.
         */
        UpperTriangularMatrix<T>& U();
        const UpperTriangularMatrix<T>& U() const;
        //@}

        /**
         * Access the permutation matrix.  P()*decomposedMatrix == L()*U().
         */
        DenseMatrix<T> P() const;


        //@{
        /**
         * Access the order vector whose values at each index give the
         * row where that row is actually contained; i.e., the kth row is
         * contained at the return value of order[k].  For instance, if
         * order[4] == 1, that means the 4th row is actually stored in the
         * first row of the two matrices.
         */
        typename LUDecomposition<T>::OrderType& orderVector();
        const typename LUDecomposition<T>::OrderType& orderVector() const;
        //@}

    private:
        LowerTriangularMatrix<T> m_L;
        UpperTriangularMatrix<T> m_U;
        typename LUDecomposition<T>::OrderType m_order;
    };

    typedef std::vector<size_t> OrderType;

    /**
     * Exception class thrown when (surprise!) the matrix is singular.
     */
    class MatrixIsSingular
    {};

private:
    /**
     * The default value for the tolerance that is used to determine if the
     * matrix is singular.
     */
    static const T m_tolerance;
    
    /**
     * A type that holds the scales of the matrix.  The value at each index
     * is the value of the element with the largest absolute value in the
     * corresponding row.
     */
    typedef std::vector<T> ScalesType;

    /**
     * @return a ScalesType where each element contains the element with the
     * largest absolute value in the corresponding row of the matrix
     */
    ScalesType createScales(const Matrix<T>& matrix) const;

    /**
     * Does pivoting on a matrix.
     *
     * @param matrix The matrix to pivot.
     * @param order The order vector adjusted to do the pivoting.  Rather
     * than changing the values in the matrix (which could be a very slow
     * operation), the values are kept in their same spot, but the order
     * vector that tells which row each row in the matrix actually is is
     * adjusted instead.
     * @param scales Tells the value of the elements with the largest absolute
     * value in the original matrix.
     * @param row The row to (possibly) pivot.
     *
     * @pre order.size() >= scales.size() == matrix.getNumRows().  
     * The values in order should all be less than order.size().
     */
    void pivot(const Matrix<T>& matrix, OrderType& order, 
               const ScalesType& scales, const size_t row) const;
};

#include "LUDecomposition.hpp"

#endif

