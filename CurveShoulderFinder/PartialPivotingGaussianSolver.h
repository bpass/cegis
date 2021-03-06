/**
 * @file PartialPivotingGaussianSolver.h
 * @author Austin Hartman
 *
 * $Id: PartialPivotingGaussianSolver.h,v 1.4 2005/07/01 23:05:37 ahartman Exp $
 */

#ifndef AUSTIN_PARTIALPIVOTINGGAUSSIANSOLVER_H
#define AUSTIN_PARTIALPIVOTINGGAUSSIANSOLVER_H

#include "GaussianSolver.h"

template<class T>
class PartialPivotingGaussianSolver : public GaussianSolver<T>
{
public:
	virtual ~PartialPivotingGaussianSolver();

	/**
	 * Implements the GaussianSolver interface using Gaussian
	 * elimination with partial pivoting.
	 * @pre T defines unary operator-, operator/, binary operator*,
	 * operator*=, operator+=, and operator>.  The columns of matrix
	 * form a linearly independent set of vectors.
	 * @post returns the solution as described in GaussianSolver
	 */
	virtual typename GaussianSolver<T>::Solution 
		operator()(const Matrix<T>& matrix, 
		           const MyVector<T>& vector) const;

protected:
	/**
	 * @pre row and column are valid for mat.  T defines operator>.
	 * @post swaps row with the row after row that has the largest
	 * element in column.  Also swaps the corresponding elements in
	 * vec.
	 */
	void partialPivot(DenseMatrix<T>& mat, MyVector<T>& vec,
	                  size_t row, size_t column) const;

	/**
	 * @pre rowToStart and column are valid rows in mat.  T defines
	 * operator>
	 * @post searches all rows from rowToStart to the last row in mat
	 * and returns the index of the row that contains the element with
	 * the largest absolute value in column
	 */
	size_t indexLargestElementRow(const Matrix<T>& mat,
	                              size_t rowToStart,
	                              size_t column) const;
};

#include "PartialPivotingGaussianSolver.hpp"

#endif

