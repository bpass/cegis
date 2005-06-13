#ifndef AUSTIN_DIAGONALMATRIX_H
#define AUSTIN_DIAGONALMATRIX_H

#include <iostream>
#include <iomanip>

#include "Matrix.h"

template<class T>
class DiagonalMatrix;

template<class T>
DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& lhs, 
                            const DiagonalMatrix<T>& rhs);

template<class T>
DiagonalMatrix<T> operator-(const DiagonalMatrix<T>& lhs, 
                            const DiagonalMatrix<T>& rhs);

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

	/**
	 * Multiplies a matrix by a vector.
	 * Pre: The number of columns in the matrix must equal the number
	 * of elements in the vector
	 * Post: returns a vector that is the result of multiplying this
	 * matrix by the vector v
	 */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	DiagonalMatrix<T>& operator+=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T> operator-() const;

	DiagonalMatrix<T>& operator-=(const DiagonalMatrix<T>& rhs);

	DiagonalMatrix<T>& operator*=(const DiagonalMatrix<T>& rhs);

	/**
	 * Pre: None
	 * Post: returns the number of columns in the matrix.
	 */
	virtual const size_t& getNumCols() const;

	/**
	 * Pre: None
	 * Post: returns the number of rows in the matrix.
	 */
	virtual const size_t& getNumRows() const;

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
	size_t size;
	T* data;

	static T nonConstZero;
	static const T constZero;
};

#include "DiagonalMatrix.hpp"

#endif

