/**
 * @file Matrix.hpp
 * @author Austin Hartman
 *
 * $Id: Matrix.hpp,v 1.6 2005/12/07 23:49:04 ahartman Exp $
 */

#ifdef AUSTIN_MATRIX_H
#ifndef AUSTIN_MATRIX_HPP
#define AUSTIN_MATRIX_HPP

/****************************************
********** Matrix functions *************
*****************************************/
template<class T>
inline
Matrix<T>::~Matrix()
{}

template<class T>
inline
typename Matrix<T>::RowProxy
Matrix<T>::operator[](size_t r)
{
	return RowProxy(*this, r);
}

template<class T>
inline
typename Matrix<T>::ConstRowProxy
Matrix<T>::operator[](size_t r) const
{
	return ConstRowProxy(*this, r);
}



/****************************************
********** RowProxy functions ***********
*****************************************/
template<class T>
inline
Matrix<T>::RowProxy::RowProxy(Matrix<T>& mat, size_t r)
	: matrix(mat), row(r)
{}

template<class T>
inline
T&
Matrix<T>::RowProxy::operator[](size_t c) const
{
	return matrix.getElement(row, c);
}



/****************************************
******* ConstRowProxy functions *********
*****************************************/
template<class T>
inline
Matrix<T>::ConstRowProxy::ConstRowProxy(const Matrix<T>& mat, 
                                          size_t r)
	: matrix(mat), row(r) 
{}

template<class T>
inline
const T&
Matrix<T>::ConstRowProxy::operator[](size_t c) const
{
	return matrix.getElement(row, c);
}



/******************************************
**** RowRangeError functions **************
*******************************************/
template<class T>
inline
Matrix<T>::RowRangeError::RowRangeError(size_t r)
	: row(r)
{}

template<class T>
inline
size_t
Matrix<T>::RowRangeError::getRow() const
{
	return row;
}



/******************************************
**** ColumnRangeError functions ***********
*******************************************/
template<class T>
inline 
Matrix<T>::ColumnRangeError::ColumnRangeError(size_t c)
	: column(c)
{}

template<class T>
inline
size_t
Matrix<T>::ColumnRangeError::getColumn() const
{
	return column;
}



/******************************************
**** IncompatibleMatrixSizes functions ****
*******************************************/
template<class T>
Matrix<T>::IncompatibleMatrixSizes::
IncompatibleMatrixSizes(size_t lhsNumRows,
                        size_t lhsNumCols, 
                        size_t rhsNumRows,
                        size_t rhsNumCols)
	: lhsRows(lhsNumRows), lhsCols(lhsNumCols),
	  rhsRows(rhsNumRows), rhsCols(rhsNumCols)
{}

template<class T>
inline
size_t
Matrix<T>::IncompatibleMatrixSizes::getLhsRows() const
{
	return lhsRows;
}

template<class T>
inline
size_t
Matrix<T>::IncompatibleMatrixSizes::getLhsColumns() const
{
	return lhsCols;
}

template<class T>
inline
size_t
Matrix<T>::IncompatibleMatrixSizes::getRhsRows() const
{
	return rhsRows;
}

template<class T>
inline
size_t
Matrix<T>::IncompatibleMatrixSizes::getRhsColumns() const
{
	return rhsCols;
}



/****************************************
******* Global stream operators *********
*****************************************/
template<class T>
inline
std::ostream& 
operator<<(std::ostream& os, const Matrix<T>& m)
{
	m.printOn(os);
	return os;
}

template<class T>
inline
std::ofstream& 
operator<<(std::ofstream& ofs, const Matrix<T>& m)
{
	m.printOnFile(ofs);
	return ofs;
}

template<class T>
inline
std::istream& 
operator>>(std::istream& is, Matrix<T>& m)
{
	m.readIn(is);
	return is;
}

#endif
#endif

