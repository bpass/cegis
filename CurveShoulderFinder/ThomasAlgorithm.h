/**
 * @file ThomasAlgorithm.h
 * @author Austin Hartman
 *
 * $Id: ThomasAlgorithm.h,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

#ifndef AUSTIN_THOMASALGORITHM_H
#define AUSTIN_THOMASALGORITHM_H

#include "TridiagonalMatrix.h"

template<class T>
class ThomasAlgorithm
{
public:
	class Solution;

	/**
	 * This function solves the system Ax = b for x,
	 * where A is a tridiagonal matrix and b is a vector.
	 * Pre: matrix contains the same number of columns as
	 * vec has elements
	 * Post: returns a Solution, which is described below
	 */
	Solution operator()(const TridiagonalMatrix<T>& matrix, 
	                    const MyVector<T>& vec) const;

	class Solution
	{
	public:
		Solution();

		Solution(const TridiagonalMatrix<T>& m, 
		         const MyVector<T>& v);

		/**
		 * This returns a matrix that has been pseudo-LU decomposed.
		 * It contains the elements of U on the the diagonal and
		 * superdiagonal.  All other elements of U are 0.  It contains
		 * the elements of L on the subdiagonal.  The elements of L
		 * on the main diagonal are all 1, and everything else is 0.
		 */
		TridiagonalMatrix<T>& matrix();
		const TridiagonalMatrix<T>& matrix() const;

		/**
		 * This returns the solved vector for the system
		 */
		MyVector<T>& vector();
		const MyVector<T>& vector() const;
	private:
		TridiagonalMatrix<T> mat;
		MyVector<T> vec;
	};

	/**
	 * Exception class used when the number of columns in the matrix
	 * does not equal the number of elements in the vector.
	 */
	class IncompatibleMatrixAndVector
	{
	public:
		/**
		 * Constructor that takes the number of rows in the matrix
		 * and the number of elements in the vector whose sizes
		 * meant that the operation was undefined.
		 * Pre: None
		 * Post: The object has been initialized with the data
		 * passed into the constructor.
		 */
		IncompatibleMatrixAndVector(const size_t& matrixCols,
		                            const size_t& vectorSize);

		/**
		 * Pre: None
		 * Post: returns the number of cols in the matrix whose
		 * size led to the operation being undefined.
		 */
		const size_t& getMatrixCols() const;

		/**
		 * Pre: None
		 * Post: returns the number of elements in the vector
		 * whose size led to the operation being undefined.
		 */
		const size_t& getVectorSize() const;
	private:
		size_t matCols;
		size_t vecSize;
	};
};

#include "ThomasAlgorithm.hpp"

#endif
