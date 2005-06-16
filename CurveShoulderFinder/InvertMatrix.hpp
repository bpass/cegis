/**
 * @file InvertMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: InvertMatrix.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_INVERTMATRIX_H
#ifndef AUSTIN_INVERTMATRIX_HPP
#define AUSTIN_INVERTMATRIX_HPP

#include "LUDecomposition.h"
#include "LUDecompositionSolver.h"

template<class T>
DenseMatrix<T>
InvertMatrix<T>::operator()(const Matrix<T>& matrix) const
{
    if(matrix.getNumRows() != matrix.getNumCols())
    {
        throw typename InvertMatrix<T>::MatrixIsNotSquare();
    }

    try
    {
        LUDecomposition<T> luDecomp;
        const typename LUDecomposition<T>::Solution solution = luDecomp(matrix);

        DenseMatrix<T> inverse(matrix.getNumRows(), matrix.getNumRows());
        LUDecompositionSolver<T> luSolver;

        for(size_t j = 0; j < inverse.getNumCols(); ++j)
        {
            // create the b vector whose solution vector will become the
            // column of this iteration
            MyVector<T> b(solution.L().getNumRows());
            // initialize the element corresponding to the current column to 1,
            // since all other elements are initialized to 0 in the MyVector 
            // constructor
            b[j] = 1; 

            // solve the system to get what the current column should be
            const MyVector<T> currentColumn = luSolver(solution, b);

            // copy those entries to the matrix inverse
            for(size_t i = 0; i < currentColumn.getSize(); ++i)
            {
                inverse[i][j] = currentColumn[i];
            }
        }

        return inverse;
    }
    catch(typename LUDecomposition<T>::MatrixIsSingular)
    {
        throw typename InvertMatrix<T>::MatrixIsSingular();
    }
}

#endif
#endif

