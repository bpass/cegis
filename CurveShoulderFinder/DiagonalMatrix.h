/**
 * @file DiagonalMatrix.h
 * @author Austin Hartman
 *
 * $Id: DiagonalMatrix.h,v 1.3 2005/06/22 01:37:57 ahartman Exp $
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

template<class T>
class DiagonalMatrix : public Matrix<T>
{
public:
	DiagonalMatrix(const size_t& numRowsAndCols = 0,
	               const T& initialValue = 0);

	DiagonalMatrix(const DiagonalMatrix<T>& rhs);

	virtual ~DiagonalMatrix();

	DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator+ <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator- <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

	friend DiagonalMatrix<T> operator* <>(const DiagonalMatrix<T>& lhs, 
	                                      const DiagonalMatrix<T>& rhs);

	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	DiagonalMatrix<T>& operator+=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T> operator-() const;

	DiagonalMatrix<T>& operator-=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T>& operator*=(const DiagonalMatrix<T>& rhs);

	virtual size_t getNumRows() const;
	virtual size_t getNumCols() const;

protected:
	virtual T& getElement(const size_t& row, const size_t& col);
	virtual const T& getElement(const size_t& row, const size_t& col) const;

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

