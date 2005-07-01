/**
 * @file LowerTriangularMatrix.h
 * @author Austin Hartman
 *
 * $Id: LowerTriangularMatrix.h,v 1.5 2005/07/01 23:05:37 ahartman Exp $
 */

#ifndef AUSTIN_LOWERTRIANGULARMATRIX_H
#define AUSTIN_LOWERTRIANGULARMATRIX_H

#include "Matrix.h"

template<class T>
class LowerTriangularMatrix;

template<class T>
LowerTriangularMatrix<T> operator+ (const LowerTriangularMatrix<T>& lhs,
                                    const LowerTriangularMatrix<T>& rhs);

template<class T>
LowerTriangularMatrix<T> operator- (const LowerTriangularMatrix<T>& lhs,
                                    const LowerTriangularMatrix<T>& rhs);

template<class T>
LowerTriangularMatrix<T> operator* (const LowerTriangularMatrix<T>& lhs,
                                    const LowerTriangularMatrix<T>& rhs);


template<class T>
class LowerTriangularMatrix : public Matrix<T>
{
public:
	LowerTriangularMatrix(size_t numRowsAndCols = 0,
	                      const T& initialValue = 0);

	LowerTriangularMatrix(const LowerTriangularMatrix<T>& rhs);

	virtual ~LowerTriangularMatrix();

	LowerTriangularMatrix<T>& operator=(const LowerTriangularMatrix<T>& rhs);

	friend LowerTriangularMatrix<T> 
		operator+ <>(const LowerTriangularMatrix<T>& lhs,
		             const LowerTriangularMatrix<T>& rhs);

	friend LowerTriangularMatrix<T> 
		operator- <>(const LowerTriangularMatrix<T>& lhs,
		             const LowerTriangularMatrix<T>& rhs);

	friend LowerTriangularMatrix<T>
		operator* <>(const LowerTriangularMatrix<T>& lhs,
		             const LowerTriangularMatrix<T>& rhs);

	/**
	 * Multiplies a matrix by a vector.
	 * @pre The number of columns in the matrix must equal the number
	 * of elements in the vector
	 * @post returns a vector that is the result of multiplying this
	 * matrix by the vector v
	 */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	LowerTriangularMatrix<T>& 
		operator+=(const LowerTriangularMatrix<T>& rhs);

	LowerTriangularMatrix<T> operator-() const;

	LowerTriangularMatrix<T>& 
		operator-=(const LowerTriangularMatrix<T>& rhs);

	LowerTriangularMatrix<T>&
		operator*=(const LowerTriangularMatrix<T>& rhs);

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
	virtual T& getElement(size_t row, size_t col);
	virtual const T& getElement(size_t row, size_t col) const;

	virtual void printOn(std::ostream& os) const;
	virtual void printOnFile(std::ofstream& ofs) const;
	virtual void readIn(std::istream& is);

private:
	T** data;
	size_t size;

	T** create2DArray(size_t n) const;
	void destroy2DArray(T** array, size_t n) const;

	static T nonConstZero;
	static const T constZero;
};

#include "LowerTriangularMatrix.hpp"

#endif
