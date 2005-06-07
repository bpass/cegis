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
Matrix<T>::operator[](const size_t& r)
{
	return RowProxy(*this, r);
}

template<class T>
inline
typename Matrix<T>::ConstRowProxy
Matrix<T>::operator[](const size_t& r) const
{
	return ConstRowProxy(*this, r);
}



/****************************************
********** RowProxy functions ***********
*****************************************/
template<class T>
inline
Matrix<T>::RowProxy::RowProxy(Matrix<T>& mat, const size_t& r)
	: matrix(mat), row(r)
{}

template<class T>
inline
T&
Matrix<T>::RowProxy::operator[](const size_t& c) const
{
	return matrix.getElement(row, c);
}



/****************************************
******* ConstRowProxy functions *********
*****************************************/
template<class T>
inline
Matrix<T>::ConstRowProxy::ConstRowProxy(const Matrix<T>& mat, 
                                          const size_t& r)
	: matrix(mat), row(r) 
{}

template<class T>
inline
const T&
Matrix<T>::ConstRowProxy::operator[](const size_t& c) const
{
	return matrix.getElement(row, c);
}



/******************************************
**** RowRangeError functions **************
*******************************************/
template<class T>
inline
Matrix<T>::RowRangeError::RowRangeError(const size_t& r)
	: row(r)
{}

template<class T>
inline
const size_t&
Matrix<T>::RowRangeError::getRow() const
{
	return row;
}



/******************************************
**** ColumnRangeError functions ***********
*******************************************/
template<class T>
inline 
Matrix<T>::ColumnRangeError::ColumnRangeError(const size_t& c)
	: column(c)
{}

template<class T>
inline
const size_t&
Matrix<T>::ColumnRangeError::getColumn() const
{
	return column;
}



/******************************************
**** IncompatibleMatrixSizes functions ****
*******************************************/
template<class T>
Matrix<T>::IncompatibleMatrixSizes::
IncompatibleMatrixSizes(const size_t& lhsNumRows,
                        const size_t& lhsNumCols, 
                        const size_t& rhsNumRows,
                        const size_t& rhsNumCols)
	: lhsRows(lhsNumRows), lhsCols(lhsNumCols),
	  rhsRows(rhsNumRows), rhsCols(rhsNumCols)
{}

template<class T>
inline
const size_t&
Matrix<T>::IncompatibleMatrixSizes::getLhsRows() const
{
	return lhsRows;
}

template<class T>
inline
const size_t&
Matrix<T>::IncompatibleMatrixSizes::getLhsColumns() const
{
	return lhsCols;
}

template<class T>
inline
const size_t&
Matrix<T>::IncompatibleMatrixSizes::getRhsRows() const
{
	return rhsRows;
}

template<class T>
inline
const size_t&
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

