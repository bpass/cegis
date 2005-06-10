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
	LowerTriangularMatrix(const size_t& numRowsAndCols = 0,
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
	 * Pre: The number of columns in the matrix must equal the number
	 * of elements in the vector
	 * Post: returns a vector that is the result of multiplying this
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
	T** data;
	size_t size;

	T** create2DArray(const size_t& n) const;
	void destroy2DArray(T** array, const size_t& n) const;

	static T nonConstZero;
	static const T constZero;
};

#include "LowerTriangularMatrix.hpp"

#endif
