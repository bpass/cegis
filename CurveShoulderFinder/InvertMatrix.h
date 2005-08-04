/**
 * @file InvertMatrix.h
 * @author Austin Hartman
 *
 * $Id: InvertMatrix.h,v 1.3 2005/08/04 20:27:49 ahartman Exp $
 */

#ifndef AUSTIN_INVERTMATRIX_H
#define AUSTIN_INVERTMATRIX_H

#include "Matrix.h"
#include "DenseMatrix.h"

/**
 * A functor used to invert a matrix.
 */
template<class T>
class InvertMatrix
{
public:
    /**
     * Inverts the specified matrix.
     *
     * @param matrix The matrix to be inverted.
     *
     * @return The inverted matrix.
     *
     * @exception MatrixIsNotSquare Thrown if the matrix's number of rows and
     * numbers of columns differ.
     * @exception MatrixIsSingular Thrown if the matrix is determined to be
     * singular and thus not invertible.
     */
    DenseMatrix<T> operator()(const Matrix<T>& matrix) const;

    /**
     * Exception class to be used when a square matrix is needed but
     * is not given.
     */
    class MatrixIsNotSquare
    {};

    /**
     * Exception class to be used when the matrix is singular.
     */
    class MatrixIsSingular
    {};
};

#include "InvertMatrix.hpp"

#endif

