/**
 * @file PartialPivotingGaussianSolver.h
 * @author Austin Hartman
 *
 * $Id: PartialPivotingGaussianSolver.h,v 1.2 2005/06/13 18:28:46 ahartman Exp $
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
	 * Pre: T defines unary operator-, operator/, binary operator*,
	 * operator*=, operator+=, and operator>.  The columns of matrix
	 * form a linearly independent set of vectors.
	 * Post: returns the solution as described in GaussianSolver
	 */
	virtual typename GaussianSolver<T>::Solution 
		operator()(const Matrix<T>& matrix, 
		           const MyVector<T>& vector) const;

protected:
	/**
	 * Pre: row and column are valid for mat.  T defines operator>.
	 * Post: swaps row with the row after row that has the largest
	 * element in column.  Also swaps the corresponding elements in
	 * vec.
	 */
	void partialPivot(DenseMatrix<T>& mat, MyVector<T>& vec,
	                  const size_t& row, const size_t& column) const;

	/**
	 * Pre: rowToStart and column are valid rows in mat.  T defines
	 * operator>
	 * Post: searches all rows from rowToStart to the last row in mat
	 * and returns the index of the row that contains the element with
	 * the largest absolute value in column
	 */
	size_t indexLargestElementRow(const Matrix<T>& mat,
	                              const size_t& rowToStart,
	                              const size_t& column) const;
};

#include "PartialPivotingGaussianSolver.hpp"

#endif

