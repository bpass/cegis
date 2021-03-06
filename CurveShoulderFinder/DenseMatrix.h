/**
 * @file DenseMatrix.h
 * @author Austin Hartman
 *
 * $Id: DenseMatrix.h,v 1.7 2005/07/01 23:05:37 ahartman Exp $
 */

#ifndef AUSTIN_DENSEMATRIX_H
#define AUSTIN_DENSEMATRIX_H

#include <iostream>
#include <fstream>
#include <iomanip> // for setw
#include <algorithm> // for swap

#include "Matrix.h"



template<class T>
class DenseMatrix;

template<class T>
DenseMatrix<T> operator+(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs);

template<class T>
DenseMatrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);

template<class T>
DenseMatrix<T> operator-(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs);

template<class T>
DenseMatrix<T> operator*(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs);



/**
 * Class to hold a matrix whose values can all be non-zero.
 * This class requires that T be default constructible, copy constructible, and
 * assignable.
 */
template<class T>
class DenseMatrix : public Matrix<T>
{
public:
	/**
	 * Constructor that can be used as a default constructor.  Allows
	 * the input of the initial number of rows and columns, plus the
	 * initial value for the every element of the matrix.
	 * @pre None
	 * @post creates a matrix with the specified number of rows and 
	 * columns with each element equal to initialValue.  If either
	 * rows or cols is 0, creates an empty matrix.
	 */
	DenseMatrix(size_t rows = 0, size_t cols = 0, 
	            const T& initialValue = 0);

	/**
	 * Constructor to create a matrix from a vector.
	 * @pre None
	 * @post If the vector is empty, so is the resulting matrix.
	 * Otherwise, creates a matrix with one column and with its
	 * amount of rows equal to the size of the vector.  The elements
	 * of the matrix are equal to the corresponding elements of the
	 * vector.
	 */
	explicit DenseMatrix(const MyVector<T>& v);

	/**
	 * Copy constructor.
	 * @pre None
	 * @post creates a matrix with the same number of rows and columns
	 * as rhs.  The elements in the matrix are equal to the corresponding
	 * elements of rhs.
	 */
	DenseMatrix(const DenseMatrix<T>& rhs);

	/**
	 * Constructor to construct a dense matrix from any other type of matrix
	 * @pre None
	 * @post creates a matrix with the same number of rows and columns
	 * as rhs.  The elements in the matrix are equal to the corresponding
	 * elements of rhs.
	 */
	explicit DenseMatrix(const Matrix<T>& rhs);

	/**
	 * Destructor.
	 * @pre None
	 * @post the matrix has been destructed
	 */
	virtual ~DenseMatrix();

	/**
	 * Assignment operator.
	 * @pre None
	 * @post This matrix has the same number of rows and columns as rhs.  
	 * The elements in the matrix are equal to the corresponding
	 * elements of rhs.
	 */
	DenseMatrix<T>& operator=(const DenseMatrix<T>& rhs);

	/**
	 * Assignment operator for any matrix type
	 * @pre None
	 * @post This matrix has the same number of rows and columns as rhs.  
	 * The elements in the matrix are equal to the corresponding
	 * elements of rhs.
	 */
	DenseMatrix<T>& operator=(const Matrix<T>& rhs);
	
	/**
	 * Adds the elements of rhs to the corresponding elements in this
	 * matrix.
	 * @pre this and rhs have the same number of rows and columns.
	 * T defines operator+=
	 * @post this matrix has been modified by adding the elements
	 * in rhs to the corresponding elements of this.  Throws 
	 * IncompatibleMatrixSizes if this and rhs have a different number
	 * of rows or columns
	 */
	DenseMatrix<T>& operator+=(const DenseMatrix<T>& rhs);

	/**
	 * Adds the elements of lhs and rhs to create a new matrix.
	 * @pre lhs and rhs have the same number of rows and columns.
	 * T defines operator+=
	 * @post Returns a new matrix that is the sum of lhs and rhs. 
	 * Throws IncompatibleMatrixSizes if lhs and rhs have a different
	 * number of rows or columns.
	 */
	friend DenseMatrix<T> operator+ <>(const DenseMatrix<T>& lhs, 
	                                   const DenseMatrix<T>& rhs);

	/**
	 * Subtracts the elements of rhs from lhs to create a new matrix.
	 * @pre lhs and rhs have the same number of rows and columns.
	 * T defines unary operator- and operator+=
	 * @post Returns a new matrix that is the difference of lhs and rhs. 
	 * Throws IncompatibleMatrixSizes if lhs and rhs have a different
	 * number of rows or columns.
	 */
	friend DenseMatrix<T> operator- <>(const DenseMatrix<T>& lhs, 
	                                   const DenseMatrix<T>& rhs);

	/**
	 * @pre T defines unary operator-
	 * @post returns a new matrix with its elements equal to the
	 * negative of the elements of this matrix
	 */
	DenseMatrix<T> operator-() const;

	/**
	 * @pre this matrix and rhs have the same number of rows and
	 * columns.  T defines unary operator- and operator+=
	 * @post this matrix has been modified by subtracting the elements
	 * in rhs from the corresponding elements of this.  Throws 
	 * IncompatibleMatrixSizes if this and rhs have a different number
	 * of rows or columns
	 */
	DenseMatrix<T>& operator-=(const DenseMatrix<T>& rhs);

	/**
	 * Changes this matrix to be the result of multiplying itself
	 * by rhs.
	 * @pre T defines binary operator* and operator+=.  This matrix
	 * has the same number of columns as the number of rows in rhs.
	 * @post This has the same number of rows as before and the same
	 * number of columns as rhs.  The value of its elements is the result
	 * of multipying this by rhs.  Throws IncompatibleMatrixSizes if the 
	 * number of columns in this matrix differs from the number of rows
	 * in rhs.
	 */
	DenseMatrix<T>& operator*=(const DenseMatrix<T>& rhs);

	/**
	 * Returns a new matrix that is the result of multiplying lhs by rhs.
	 * @pre T defines binary operator* and operator+=.  lhs has the same
	 * number of columns as the number of rows in rhs.
	 * @post returns a new matrix that has the same number of rows as
	 * lhs and the same number of columns as rhs.  The elements in this
	 * matrix are the result of multiplying lhs by rhs.  Throws 
	 * IncompatibleMatrixSizes if the number of columns in lhs differs 
	 * from the number of rows in rhs.
	 */
	friend DenseMatrix<T> operator* <>(const DenseMatrix<T>& lhs, 
	                                   const DenseMatrix<T>& rhs);

	/**
	 * Multiplies this matrix by a vector to create a new vector.
	 * @pre This matrix has the same number of columns as there are
	 * elements in v.  T defines binary operator* and operator+=
	 * @post Returns a new vector with the same number of rows
	 * as this matrix and one column.  The elements in the vector
	 * are the result of multiplying this matrix by v.  Throws
	 * IncompatibleMatrixSizes if the number of columns in this
	 * matrix differs from the number of elements in v.
	 */
	virtual MyVector<T> operator*(const MyVector<T>& v) const;

	/**
	 * @pre None
	 * @post returns the number of rows in this matrix.
	 */
	virtual size_t getNumRows() const;

	/**
	 * @pre None
	 * @post returns the number of columns in this matrix.
	 */
	virtual size_t getNumCols() const;

	/**
	 * Resets the matrix to have the passed-in number of rows and
	 * columns.
	 * @pre None
	 * @post the matrix has newNumRows rows and newNumCols columns.  
	 * The elements in the matrix are all set to be value.  If either
	 * of newNumRows or newNumCols is 0, makes the matrix an empty matrix.
	 */
	void setSize(size_t newNumRows, size_t newNumCols,
		     const T& value = 0);

	/**
	 * Creates a vector from the specified column of the matrix
	 * @pre column < the number of columns
	 * @post Returns a new vector with its number of elements equal
	 * to the number of rows in the matrix.  The values in the vector
	 * are equal to the corresponding values in the specified column
	 * of the matrix.  Throws ColumnRangeError if column >= the number
	 * of columns.
	 */
	MyVector<T> getColumnAsVector(size_t column) const;

	/**
	 * Creates a vector from the specified row of the matrix
	 * @pre row < the number of rows
	 * @post Returns a new vector with its number of elements equal to
	 * the number of columns in the matrix.  The values in the vector
	 * are equal to the corresponding values in the specified row of
	 * the matrix.  Throws RowRangeError if row >= the number of rows.
	 */
	MyVector<T> getRowAsVector(size_t row) const;

	/**
	 * @pre None
	 * @post Creates a new matrix that is the transpose of this matrix.
	 */
	DenseMatrix<T> transpose() const;

	/** Elementary Row Operations */
	/**
	 * @pre row1 and row2 are both < the number of rows in the matrix.
	 * @post Interchanges the corresponding elements of row1 and row2.
	 * No effect if row1 and row2 are the same, but may be inefficient.
	 * Throws RowRangeError if row1 or row2 >= the number of rows in the
	 * matrix.
	 */
	void rowInterchange(size_t row1, size_t row2);

	/**
	 * @pre row is < the number of rows in the matrix.  constant should
	 * be non-zero for the operation to be valid.  T defines operator*=
	 * @post multiplies the elements in row by constant. Throws 
	 * RowRangeError if row >= the number of rows in the matrix.
	 */
	void rowScale(size_t row, const T& constant);

	/**
	 * @pre rowToReplace and otherRow are both < the number of rows
	 * in the matrix.  multiple should be non-zero for the operation
	 * to be valid.  T defines binary operator* and operator+=
	 * @post rowToReplace has been replaced by the sum of itself and
	 * multiple times otherRow.  Throws RowRangeError if rowToReplace or 
	 * otherRow >= the number of rows in the matrix.
	 */
	void rowReplace(size_t rowToReplace, 
		        size_t otherRow, const T& multiple);

protected:
	virtual T& getElement(size_t row, size_t col);
	virtual const T& getElement(size_t row, size_t col) const;

	/**
	 * Output function for the screen.
	 * Outputs the matrix in the form
	 *   [ x00 x01 x02 ... x0n ]
	 *   [ x10 x11 x12 ... x1n ]
	 *   [   ...       ...     ]
	 *   [ xm0 xm1 xm2 ... xmn ]
	 * where m is the number of rows and n is the number of columns 
	 * Uses the os.width() that is first passed in for every element in the
	 * matrix
	 */
	virtual void printOn(std::ostream& os) const;

	/**
	 * Output function for files.
	 * Outputs the matrix in the form
	 *   x00 x01 x02 ... x0n
	 *   x10 x11 x12 ... x1n
	 *     ...       ... 
	 *   xm0 xm1 xm2 ... xmn
	 * where m is the number of rows and n is the number of columns
	 * Inserts a space between elements in the same row but different
	 * columns and inserts an endline between each row.
	 */
	virtual void printOnFile(std::ofstream& ofs) const;

	/**
	 * Reads in matrices in the format specified for the file
	 * output function.  Reads in numRows * numCols elements
	 * in row-major order.
	 */
	virtual void readIn(std::istream& is);


private:
	size_t numRows;
	size_t numCols;
	T* data;

	/**
	 * These are used to quickly get the element that is at
	 * row, column.  row and column are not checked for validity.
	 */
	T& a(size_t row, size_t column);
	const T& a(size_t row, size_t column) const;

	/**
	 * Checks whether the row or column is indeed a valid row
	 * or column.
	 */
	bool validRow(size_t row) const;
	bool validColumn(size_t col) const;
};


#include "DenseMatrix.hpp"

#endif

