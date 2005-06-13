/**
 * @file InvertMatrix.h
 * @author Austin Hartman
 *
 * $Id: InvertMatrix.h,v 1.1 2005/06/13 20:21:02 ahartman Exp $
 */

#ifndef AUSTIN_INVERTMATRIX_H
#define AUSTIN_INVERTMATRIX_H

#include "Matrix.h"
#include "DenseMatrix.h"

template<class T>
class InvertMatrix
{
public:
    DenseMatrix<T> operator()(const Matrix<T>& matrix) const;

    class MatrixIsNotSquare
    {};
};

#include "InvertMatrix.hpp"

#endif

