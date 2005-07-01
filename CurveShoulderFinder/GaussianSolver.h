/**
 * @file GaussianSolver.h
 * @author Austin Hartman
 *
 * $Id: GaussianSolver.h,v 1.3 2005/07/01 21:03:55 ahartman Exp $
 */

#ifndef AUSTIN_GAUSSIANSOLVER_H
#define AUSTIN_GAUSSIANSOLVER_H

#include "Matrix.h"
#include "DenseMatrix.h"
#include "MyVector.h"

/**
 * This is an abstract base class to define the interface for various
 * implementations of Gaussian elimination
 */
template<class T>
class GaussianSolver
{
public:
	virtual ~GaussianSolver() = 0;

	class Solution;

	/**
	 * finds the solution for matrix * b = vector.  Returns a class
	 * that contains both matrix in reduced echelon form and b
	 */
	virtual Solution operator()(const Matrix<T>& matrix, 
	                            const MyVector<T>& vector) const = 0;

	/**
	 * class to be used as a return value for the solution of the
	 * Gaussian elimination.  The solution contains the solved
	 * vector and the original matrix in reduced echelon form.
	 */
	class Solution
	{
	public:
		/**
		 * Default constructor.
		 * @pre None
		 * @post The matrix and vector members have been
		 * default-constructed.
		 */
		Solution();

		/**
		 * Initialize the solution with the matrix m and the vector v.
		 * This is useful so that they have the correct size.
		 * @pre None
		 * @post the matrix and vector of this object are copies of
		 * m and v, respectively.
		 */
		Solution(const Matrix<T>& m, const MyVector<T>& v);

		/**
		 * @pre None
		 * @post Returns a reference to the matrix member of this
		 * object.
		 */
		DenseMatrix<T>& matrix();

		/**
		 * @pre None
		 * @post Returns a const-reference to the matrix member of
		 * this object.
		 */
		const DenseMatrix<T>& matrix() const;

		/**
		 * @pre None
		 * @post Returns a reference to the vector member of this
		 * object.
		 */
		MyVector<T>& vector();

		/**
		 * @pre None
		 * @post Returns a const-reference to the vector member of 
		 * this object.
		 */
		const MyVector<T>& vector() const;
	private:
		DenseMatrix<T> mat;
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
		 * @pre None
		 * @post The object has been initialized with the data
		 * passed into the constructor.
		 */
		IncompatibleMatrixAndVector(const size_t& matrixRows,
		                            const size_t& vectorSize);

		/**
		 * @pre None
		 * @post returns the number of rows in the matrix whose
		 * size led to the operation being undefined.
		 */
		const size_t& getMatrixRows() const;

		/**
		 * @pre None
		 * @post returns the number of elements in the vector
		 * whose size led to the operation being undefined.
		 */
		const size_t& getVectorSize() const;
	private:
		size_t matRows;
		size_t vecSize;
	};
};

#include "GaussianSolver.hpp"

#endif

