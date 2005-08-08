/**
 * @file DiagonalMatrix.h
 * @author Austin Hartman
 *
 * $Id: DiagonalMatrix.h,v 1.5 2005/08/08 20:04:15 ahartman Exp $
 */

#ifndef AUSTIN_DIAGONALMATRIX_H
#define AUSTIN_DIAGONALMATRIX_H

#include <iostream>
#include <iomanip>

#include "Matrix.h"

template<class T>
class DiagonalMatrix;

/**
 * Adds two diagonal matrices.
 *
 * @param lhs The left diagonal matrix.
 * @param rhs The right diagonal matrix.
 *
 * @return The sum of the two diagonal matrices, which is itself a diagonal
 * matrix.
 */
template<class T>
DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& lhs, 
                            const DiagonalMatrix<T>& rhs);

/**
 * Subtracts one diagonal matrix from another.
 *
 * @param lhs The diagonal matrix on the left of the expression.
 * @param rhs The diagonal matrix that will be subtracted from lhs.
 *
 * @return The difference of the two diagonal matrices, which is itself a 
 * diagonal matrix.
 */
template<class T>
DiagonalMatrix<T> operator-(const DiagonalMatrix<T>& lhs, 
                            const DiagonalMatrix<T>& rhs);

/**
 * Finds the result of the matrix multiplication of two diagonal matrices.
 *
 * @param lhs The diagonal matrix on the left of the operation.
 * @param rhs The diagonal matrix on the right of the operation.
 *
 * @return The result of the matrix multiplication, which is itself a diagonal
 * matrix.
 */
template<class T>
DiagonalMatrix<T> operator*(const DiagonalMatrix<T>& lhs, 
                            const DiagonalMatrix<T>& rhs);

/**
 * A diagonal matrix may only have non-zero elements on its main diagonal.
 */
template<class T>
class DiagonalMatrix : public Matrix<T>
{
public:
    /**
     * Creates a diagonal matrix of the specified size with the elements
     * on the main diagonal set to the specified value.
     *
     * @param numRowsAndCols The number of rows and columns in the matrix.
     * These values must be equal because diagonal matrices are always square.
     * @param initialValue The initial value for the elements on the main
     * diagonal.
     */
	DiagonalMatrix(size_t numRowsAndCols = 0,
	               const T& initialValue = 0);

    /**
     * Copy constructor.
     */
	DiagonalMatrix(const DiagonalMatrix<T>& rhs);

    /**
     * Destructor.
     */
	virtual ~DiagonalMatrix();

    /**
     * Copy assignment operator.  This is the only way to change the size of
     * the matrix after it has been initialized.
     */
	DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator+ <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator- <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator* <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

    /**
	 * Multiplies a matrix by a vector.
     * @param v The vector to multiply this matrix by.  The number of elements
     * in the vector must equal the number of columns in this matrix.
	 * @return a vector that is the result of multiplying this
	 * matrix by the vector v
     * @exception Matrix<T>::IncompatibleMatrixSizes Thrown if the sizes of
     * the matrix and vector are not the same.
     */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

    /**
     * Adds a diagonal matrix to this matrix.
     * @param rhs The matrix to add to this matrix.  It must be the same
     * size as this matrix.
     * @return This matrix.
     * @exception Matrix<T>::IncompatibleMatrixSizes Thrown if the sizes of
     * the matrices are not the same.
     */
	DiagonalMatrix<T>& operator+=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T> operator-() const;

	DiagonalMatrix<T>& operator-=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T>& operator*=(const DiagonalMatrix<T>& rhs);

	virtual size_t getNumRows() const;
	virtual size_t getNumCols() const;

protected:
	virtual T& getElement(size_t row, size_t col);
	virtual const T& getElement(size_t row, size_t col) const;

	virtual void printOn(std::ostream& os) const;

	virtual void printOnFile(std::ofstream& ofs) const;

	virtual void readIn(std::istream& is);

private:
	size_t size;
	T* data;

	static T nonConstZero;
	static const T constZero;
};

#include "DiagonalMatrix.hpp"

#endif

