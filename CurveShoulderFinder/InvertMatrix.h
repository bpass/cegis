/**
 * @file InvertMatrix.h
 * @author Austin Hartman
 *
 * $Id: InvertMatrix.h,v 1.2 2005/06/15 19:51:46 ahartman Exp $
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

    class MatrixIsSingular
    {};
};

#include "InvertMatrix.hpp"

#endif

