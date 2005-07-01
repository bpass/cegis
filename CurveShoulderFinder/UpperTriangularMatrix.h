/**
 * @file UpperTriangularMatrix.h
 * @author Austin Hartman
 *
 * $Id: UpperTriangularMatrix.h,v 1.5 2005/07/01 22:52:53 ahartman Exp $
 */

#ifndef AUSTIN_UPPERTRIANGULARMATRIX_H
#define AUSTIN_UPPERTRIANGULARMATRIX_H

#include "Matrix.h"
#include "DenseMatrix.h"

template<class T>
class UpperTriangularMatrix;

template<class T>
UpperTriangularMatrix<T> operator+(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

template<class T>
UpperTriangularMatrix<T> operator-(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

template<class T>
UpperTriangularMatrix<T> operator*(const UpperTriangularMatrix<T>& lhs,
                                   const UpperTriangularMatrix<T>& rhs);

template<class T>
DenseMatrix<T> operator*(const UpperTriangularMatrix<T>& lhs,
                         const Matrix<T>& rhs);

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
	UpperTriangularMatrix(const size_t& numRowsAndCols = 0,
	                      const T& initialValue = 0);

	UpperTriangularMatrix(const UpperTriangularMatrix<T>& rhs);

	virtual ~UpperTriangularMatrix();

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
	 * @pre The number of columns in the matrix must equal the number
	 * of elements in the vector
	 * @post returns a vector that is the result of multiplying this
	 * matrix by the vector v
	 */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	UpperTriangularMatrix<T>& 
		operator+=(const UpperTriangularMatrix<T>& rhs);

	UpperTriangularMatrix<T> operator-() const;

	UpperTriangularMatrix<T>& 
		operator-=(const UpperTriangularMatrix<T>& rhs);

	UpperTriangularMatrix<T>&
		operator*=(const UpperTriangularMatrix<T>& rhs);

	/**
	 * @pre None
	 * @post returns the number of rows in the matrix.
	 */
	virtual size_t getNumRows() const;

	/**
	 * @pre None
	 * @post returns the number of columns in the matrix.
	 */
	virtual size_t getNumCols() const;

protected:
	/**
	 * These getElement functions are used to get the element at
	 * row, column in the matrix.  
	 * They will be automatically called when the user does
	 *   matrix[row][column]
	 */
	virtual T& getElement(const size_t& row, const size_t& col);
	virtual const T& getElement(const size_t& row, const size_t& col) const;

	virtual void printOn(std::ostream& os) const;

	virtual void printOnFile(std::ofstream& ofs) const;

	virtual void readIn(std::istream& is);

private:
	T** data;
	size_t size;

	T** create2DArray(const size_t& n) const;
	void destroy2DArray(T** array, const size_t& n) const;

	static T nonConstZero;
	static const T constZero;
};

#include "UpperTriangularMatrix.hpp"

#endif

