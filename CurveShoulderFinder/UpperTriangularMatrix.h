/**
 * @file UpperTriangularMatrix.h
 * @author Austin Hartman
 *
 * $Id: UpperTriangularMatrix.h,v 1.7 2005/07/02 00:01:21 ahartman Exp $
 */

#ifndef AUSTIN_UPPERTRIANGULARMATRIX_H
#define AUSTIN_UPPERTRIANGULARMATRIX_H

#include "Matrix.h"
#include "DenseMatrix.h"

template<class T>
class UpperTriangularMatrix;

/**
 * Adds two upper triangular matrices together.
 * @pre lhs and rhs are the same size.
 * @param lhs The matrix on the left hand side of the operation.
 * @param rhs The matrix on the right hand side of the operation.
 * @return A matrix that is the result of adding lhs and rhs together.
 */
template<class T>
UpperTriangularMatrix<T> operator+(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

/**
 * Subtracts an upper triangular matrix from another.
 * @pre lhs and rhs are the same size.
 * @param lhs The matrix on the left hand side of the operation.
 * @param rhs The matrix on the right hand side of the operation.
 * @return A matrix that is the result of rhs from lhs.
 */
template<class T>
UpperTriangularMatrix<T> operator-(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

/**
 * Multiplies two upper triangular matrices together.
 * @pre lhs and rhs are the same size.
 * @param lhs The matrix on the left hand side of the operation.
 * @param rhs The matrix on the right hand side of the operation.
 * @return A matrix that is the result of multiplying lhs and rhs together.
 */
template<class T>
UpperTriangularMatrix<T> operator*(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

/**
 * Multiplies an upper triangular matrix by any other type of matrix.
 * @pre The number of columns in lhs equals the number of rows in rhs.
 * @param lhs The matrix on the left hand side of the operation.
 * @param rhs The matrix on the right hand side of the operation.
 * @return A matrix that is the result of multiplying lhs and rhs together.
 * It must be a DenseMatrix because there's no guarantee that any elements
 * of the resulting matrix will be non-zero.
 */
template<class T>
DenseMatrix<T> operator*(const UpperTriangularMatrix<T>& lhs,
                         const Matrix<T>& rhs);

/**
 * Multiplies any type of matrix by an upper triangular matrix.
 * @pre The number of columns in lhs equals the number of rows in rhs.
 * @param lhs The matrix on the left hand side of the operation.
 * @param rhs The matrix on the right hand side of the operation.
 * @return A matrix that is the result of multiplying lhs and rhs together.
 * It must be a DenseMatrix because there's no guarantee that any elements
 * of the resulting matrix will be non-zero.
 */
template<class T>
DenseMatrix<T> operator*(const Matrix<T>& lhs,
                         const UpperTriangularMatrix<T>& rhs);

/**
 * A class to store an upper triangular matrix.  An upper triangular matrix
 * is always square and is only allowed to have non-zero values for the 
 * elements on and above the main diagonal. Below the main diagonal, every 
 * element is zero.
 */
template<class T>
class UpperTriangularMatrix : public Matrix<T>
{
public:
	/**
	 * Constructor to initialize the size of the matrix and to set the 
	 * initial value of all the elements that can be non-zero.
	 *
	 * @param numRowsAndCols The number of rows and columns that the matrix
	 * will have.  There is only one parameter for both values because the 
	 * matrix must be square.
	 * @param initialValue The initial value for every element that can
	 * possibly be non-zero.
	 */
	UpperTriangularMatrix(size_t numRowsAndCols = 0,
	                      const T& initialValue = 0);

	/**
	 * Copy constructor.
	 */
	UpperTriangularMatrix(const UpperTriangularMatrix<T>& rhs);

	/**
	 * Destructor.
	 */
	virtual ~UpperTriangularMatrix();

	/**
	 * Assignment operator.  As of now, this is the only way to change the
	 * size of a matrix that's already been constructed.
	 */
	UpperTriangularMatrix<T>& operator=(const UpperTriangularMatrix<T>& rhs);

	friend UpperTriangularMatrix<T> 
		operator+ <>(const UpperTriangularMatrix<T>& lhs,
		             const UpperTriangularMatrix<T>& rhs);

	friend UpperTriangularMatrix<T> 
		operator- <>(const UpperTriangularMatrix<T>& lhs,
		             const UpperTriangularMatrix<T>& rhs);

	friend UpperTriangularMatrix<T>
		operator* <>(const UpperTriangularMatrix<T>& lhs,
		             const UpperTriangularMatrix<T>& rhs);

	friend DenseMatrix<T>
		operator* <>(const UpperTriangularMatrix<T>& lhs,
		             const Matrix<T>& rhs);

	friend DenseMatrix<T>
		operator* <>(const Matrix<T>& lhs,
		             const UpperTriangularMatrix<T>& rhs);

	/**
	 * Multiplies a matrix by a vector.
	 * @param v The vector to multiply by. The number of columns in the 
	 * matrix must equal the number of elements in the vector.
	 * @return A vector that is the result of multiplying this
	 * matrix by the vector v.
	 */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	/**
	 * Adds another UpperTriangularMatrix to this matrix.
	 * @param rhs The matrix to add to this matrix.  It must be the same
	 * size as this matrix.
	 * @return This matrix.
	 */
	UpperTriangularMatrix<T>& 
		operator+=(const UpperTriangularMatrix<T>& rhs);

	/**
	 * Get the negation of this matrix.
	 * @return A matrix whose elements have all had unary operator-()
	 * applied to the elements of this matrix.
	 */
	UpperTriangularMatrix<T> operator-() const;

	/**
	 * Subtracts another UpperTriangularMatrix from this matrix.
	 * @param rhs The matrix to subtract from this matrix.  It must be the 
	 * same size as this matrix.
	 * @return This matrix.
	 */
	UpperTriangularMatrix<T>& 
		operator-=(const UpperTriangularMatrix<T>& rhs);

	/**
	 * Multiplies this matrix by another UpperTriangularMatrix.
	 * @param rhs The matrix to multiply this matrix by.  It must be the
	 * same size as this matrix.
	 * @return This matrix.
	 */
	UpperTriangularMatrix<T>&
		operator*=(const UpperTriangularMatrix<T>& rhs);

	/**
	 * @return The number of rows in the matrix.
	 */
	virtual size_t getNumRows() const;

	/**
	 * @return The number of columns in the matrix.
	 */
	virtual size_t getNumCols() const;

protected:
	/**
	 * These getElement functions are used to get the element at
	 * row, column in the matrix.  
	 * They will be automatically called when the user does
	 *   matrix[row][column]
	 */
	virtual T& getElement(size_t row, size_t col);
	virtual const T& getElement(size_t row, size_t col) const;

	virtual void printOn(std::ostream& os) const;

	virtual void printOnFile(std::ofstream& ofs) const;

	virtual void readIn(std::istream& is);

private:
	/**
	 * All of the elements of the matrix are contained in a two-dimensional
	 * array.  The elements are stored by column and the arrays are exactly
	 * large enough to hold the values that can be non-zero.  For example, 
	 * data[0] contains the first column of the matrix, and it points to an 
	 * array of size 1.  data[1] contains the second column of the matrix, 
	 * and it points to an array of size 2.
	 */
	T** data;

	/**
	 * The size of the matrix, which represents both the number of rows and
	 * the number of columns.  The matrix is square, so only one variable is
	 * needed to store both values.
	 */
	size_t size;

	/**
	 * Creates a two-dimensional array with n columns.  The data format is
	 * that specified in the documentation for the variable data.  The
	 * elements are default constructed.
	 */
	T** create2DArray(size_t n) const;

	/**
	 * Deletes a two-dimensional array that has n columns.  Should be used
	 * on everything that create2DArray returns.
	 */
	void destroy2DArray(T** array, size_t n) const;

	/**
	 * This is returned by the non-const getElement() for the elements that 
	 * must be zero.  This is used to decrease the amount of memory used by 
	 * objects of this class.  They don't need to allocate space in memory 
	 * for each individual element that must be zero, they can just use 
	 * one element that is shared by each instance of the class.  This 
	 * variable must be reset by getElement() during every call to it, 
	 * because there is nothing stopping a user from changing it, since 
	 * it's returned by non-const reference.
	 */
	static T nonConstZero;

	/**
	 * This is returned by the const getElement() for the elements that must
	 * be zero.  This is used to decrease the amount of memory used by objects
	 * of this class.  They don't need to allocate space in memory for each
	 * individual element that must be zero, they can just use one element
	 * that is shared by each instance of the class.  There is no way for a 
	 * user to change this variable since it's const, therefore its value
	 * never needs to be altered.
	 */
	static const T constZero;
};

#include "UpperTriangularMatrix.hpp"

#endif

