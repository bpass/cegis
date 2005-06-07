#ifndef AUSTIN_TRIDIAGONALMATRIX_H
#define AUSTIN_TRIDIAGONALMATRIX_H

#include <iomanip>

#include "Matrix.h"


template<class T>
class TridiagonalMatrix;

template<class T>
TridiagonalMatrix<T> 
operator+(const TridiagonalMatrix<T>& lhs, const TridiagonalMatrix<T>& rhs);

template<class T>
TridiagonalMatrix<T> 
operator-(const TridiagonalMatrix<T>& lhs, const TridiagonalMatrix<T>& rhs);


template<class T>
class TridiagonalMatrix : public Matrix<T>
{
public:
	/**
	 * Constructor that works as default constructor.  It can also
	 * take an argument for the number of rows and columns (which
	 * are the same in a tridiagonal matrix) and an initial value
	 * for all of the elements.
	 */
	TridiagonalMatrix(const size_t& numRowsAndCols = 0, 
	                  const T& initialValue = 0);

	/**
	 * Copy constructor.
	 */
	TridiagonalMatrix(const TridiagonalMatrix<T>& rhs);

	/**
	 * Destructor.
	 */
	virtual ~TridiagonalMatrix();

	/**
	 * Assignment operator.
	 */
	TridiagonalMatrix<T>& operator=(const TridiagonalMatrix<T>& rhs);

	/**
	 * Multiplies a tridiagonal matrix by a vector.
	 * Pre: The number of columns in this matrix is the same as the
	 * number of elements in vec.
	 * Post: returns a vector that is the result of this multiplication.
	 */
	virtual MyVector<T> operator*(const MyVector<T>& vec) const;

	/**
	 * Adds a tridiagonal matrix to this matrix.
	 * Pre: this and rhs are the same size.  T defines operator+= and
	 * binary operator*.
	 * Post: Has added the elements in rhs to the corresponding elements
	 * in this.  Throws IncompatibleMatrixSizes if this and rhs are not 
	 * the same size.
	 */
	TridiagonalMatrix<T>& operator+=(const TridiagonalMatrix<T>& rhs);

	/**
	 * Adds two tridiagonal matrices together.
	 * Pre: lhs and rhs are the same size.  T defines operator+= and
	 * binary operator*.
	 * Post: Has added the corresponding elements in lhs and rhs together.
	 * Throws IncompatibleMatrixSizes if this and rhs are not 
	 * the same size.
	 */
	friend TridiagonalMatrix<T> 
		operator+ <>(const TridiagonalMatrix<T>& lhs,
		             const TridiagonalMatrix<T>& rhs);

	/**
	 * Subtracts a tridiagonal matrix from another.
	 * Pre: lhs and rhs are the same size.  T defines unary operator-,
	 * operator+=, and binary operator*.
	 * Post: Has subtracted the elements in rhs from the corresponding 
	 * elements in lhs. Throws IncompatibleMatrixSizes if this and rhs 
	 * are not the same size.
	 */
	friend TridiagonalMatrix<T> 
		operator- <>(const TridiagonalMatrix<T>& lhs,
		             const TridiagonalMatrix<T>& rhs);

	/**
	 * Pre: T defines unary operator-
	 * Post: returns a tridiagonal matrix that has had all elements
	 * inverted from this matrix.
	 */
	TridiagonalMatrix<T> operator-() const;

	/**
	 * Subtracts a matrix from this matrix.
	 * Pre: this and rhs are the same size.  T defines unary operator-, 
	 * operator+=, and binary operator*.
	 * Post: Has subtracted the elements in rhs from the corresponding 
	 * elements in this.  Throws IncompatibleMatrixSizes if this and 
	 * rhs are not the same size.
	 */
	TridiagonalMatrix<T>& operator-=(const TridiagonalMatrix<T>& rhs);

	/**
	 * Pre: None
	 * Post: returns the number of rows in the matrix, which must
	 * be the same as the number of columns.
	 */
	virtual const size_t& getNumRows() const;

	/**
	 * Pre: None
	 * Post: returns the number of columns in the matrix, which must
	 * be the same as the number of rows.
	 */
	virtual const size_t& getNumCols() const;

protected:
	/**
	 * Pre: row < getNumRows and col < getNumCols
	 * Returns the element at position row, col.  Throws RowRangeError
	 * or ColumnRangeError if row or column are out of range,
	 * respectively.
	 */
	virtual T& getElement(const size_t& row, const size_t& col);
	virtual const T& getElement(const size_t& row, 
	                            const size_t& col) const;

	/** 
	 * Outputs the matrix in the form
	 *   [ x00 x01 x02 ... x0n ]
	 *   [ x10 x11 x12 ... x1n ]
	 *   [   ...       ...     ]
	 *   [ xn0 xn1 xn2 ... xnn ]
	 * where n is both the number of rows and number of columns minus one 
	 * Uses the os.width() that is first passed in for every element 
	 * in the matrix
	 */
	virtual void printOn(std::ostream& os) const;

	/**
	 * Outputs the matrix in the form
	 *   x00 x01
	 *   x10 x11 x12
	 *   x21 x22 x23
	 *     ...
	 *   x(n-1)(n-2) x(n-1)(n-1) x(n-1)n
	 *   xn(n-1) xnn
	 * where n is the both the number of rows and number of columns minus
	 * one.  I.e., this only outputs the elements of the matrix that could
	 * possibly be nonzero.
	 */
	virtual void printOnFile(std::ofstream& ofs) const;

	/**
	 * Reads in the matrix in the same format that printOnFile output it
	 * in.  The size of the matrix must be set correctly before attempting
	 * to use this function.
	 */
	virtual void readIn(std::istream& is);

private:
	size_t size;
	T* data;

	/**
	 * All the possibly nonzero elements in the matrix are stored in a
	 * single array.  This function gets the size of that array from
	 * the number of rows/columns in the matrix.
	 */
	size_t getArraySize(const size_t& size) const;

	/**
	 * These functions return an element from the matrix that could
	 * possibly be nonzero.
	 */
	T& getNonZeroElement(const size_t& row, const size_t& col);
	const T& getNonZeroElement(const size_t& row, const size_t& col) const;

	/**
	 * These variables are returned in place of an actual stored
	 * object in the array when the user accesses an element that
	 * must be 0 for the matrix to be tridiagonal.
	 */
	static T nonConstZero;
	static const T constZero;
};

#include "TridiagonalMatrix.hpp"

#endif

