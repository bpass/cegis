/**
 * @file DiagonalMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: DiagonalMatrix.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_DIAGONALMATRIX_H
#ifndef AUSTIN_DIAGONALMATRIX_HPP
#define AUSTIN_DIAGONALMATRIX_HPP

template<class T>
T DiagonalMatrix<T>::nonConstZero = 0;

template<class T>
const T DiagonalMatrix<T>::constZero = 0;

template<class T>
DiagonalMatrix<T>::DiagonalMatrix(const size_t& numRowsAndCols,
                                  const T& initialValue)
	: size(numRowsAndCols)
{
        if(size != 0)
	{
		data = new T[size];

		const T* const dataEnd = data + size;
		for(T* i = data; i < dataEnd; ++i)
		{
			*i = initialValue;
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
DiagonalMatrix<T>::DiagonalMatrix(const DiagonalMatrix<T>& rhs)
	: Matrix<T>(rhs), size(rhs.size)
{
        if(size != 0)
	{
		data = new T[size];

		const T* const dataEnd = data + size;
		for(T* i = data, *j = rhs.data; i < dataEnd; ++i, ++j)
		{
                        *i = *j;
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
DiagonalMatrix<T>::~DiagonalMatrix()
{
	delete[] data;
}

template<class T>
DiagonalMatrix<T>&
DiagonalMatrix<T>::operator=(const DiagonalMatrix<T>& rhs)
{
	if(rhs.size != 0)
	{
		T* newData = new T[rhs.size];
		const T* const newDataEnd = newData + rhs.size;

		for(T* i = newData, *j = rhs.data; i < newDataEnd; ++i, ++j)
		{
			*i = *j;
		}

		delete[] data;
		data = newData;
	}
	else
	{
		delete[] data;
		data = NULL;
	}
	size = rhs.size;

	return *this;
}

template<class T>
MyVector<T>
DiagonalMatrix<T>::operator*(const MyVector<T>& v) const
{
	if(size != v.getSize())
	{
		throw typename 
		      Matrix<T>::IncompatibleMatrixSizes(size, size, 
		                                         v.getSize(), 1);
	}

	MyVector<T> rv(size);

	for(size_t i = 0; i < size; ++i)
	{
		// there's only one non-zero element in each row of
		// the matrix, so this operation only needs one loop
		rv[i] = data[i] * v[i];
	}

	return rv;
}

template<class T>
DiagonalMatrix<T>&
DiagonalMatrix<T>::operator+=(const DiagonalMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         rhs.size, rhs.size);
	}

	const T* const dataEnd = data + size;
	for(T* i = data, *j = rhs.data; i < dataEnd; ++i, ++j)
	{
		*i += *j;
	}

	return *this;
}

template<class T>
DiagonalMatrix<T>
operator+(const DiagonalMatrix<T>& lhs, const DiagonalMatrix<T>& rhs)
{
	DiagonalMatrix<T> rv(lhs);
	rv += rhs;
	return rv;
}

template<class T>
DiagonalMatrix<T>
DiagonalMatrix<T>::operator-() const
{
	DiagonalMatrix<T> rv(*this);
	const T* const dataEnd = rv.data + size;
	for(T* i = rv.data; i < dataEnd; ++i)
	{
                *i = -(*i);
	}
	return rv;
}

template<class T>
DiagonalMatrix<T>&
DiagonalMatrix<T>::operator-=(const DiagonalMatrix<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
DiagonalMatrix<T>
operator-(const DiagonalMatrix<T>& lhs, const DiagonalMatrix<T>& rhs)
{
	DiagonalMatrix<T> rv(lhs);
	rv += -rhs;
	return rv;
}

template<class T>
DiagonalMatrix<T>&
DiagonalMatrix<T>::operator*=(const DiagonalMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         rhs.size, rhs.size);
	}

	const T* const dataEnd = data + size;
	for(T* i = data, *j = rhs.data; i < dataEnd; ++i, ++j)
	{
		*i *= *j;
	}
	return *this;
}

template<class T>
DiagonalMatrix<T>
operator*(const DiagonalMatrix<T>& lhs, const DiagonalMatrix<T>& rhs)
{
	//if(lhs.size != rhs.size)
	//{
	//	throw typename
	//	      Matrix<T>::IncompatibleMatrixSizes(lhs.size, lhs.size,
	//	                                         rhs.size, rhs.size);
	//}
        //
	//DiagonalMatrix<T> rv(lhs);
        //
	//const T* const dataEnd = rv.data + size;
	//for(T* i = rv.data, *j = rhs.data; i < dataEnd; ++i, ++j)
	//{
	//	*i *= *j;
	//}
        //
	//return rv;

	DiagonalMatrix<T> rv(lhs);
	rv *= rhs;
	return rv;
}

template<class T>
const size_t&
DiagonalMatrix<T>::getNumRows() const
{
	return size;
}

template<class T>
const size_t&
DiagonalMatrix<T>::getNumCols() const
{
	return size;
}

template<class T>
T&
DiagonalMatrix<T>::getElement(const size_t& row, const size_t& col)
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(row == col)
	{
		return data[row];
	}
	nonConstZero = 0;
	return nonConstZero;
}

template<class T>
const T&
DiagonalMatrix<T>::getElement(const size_t& row, const size_t& col) const
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(row == col)
	{
		return data[row];
	}
	return constZero;
}

template<class T>
void
DiagonalMatrix<T>::printOn(std::ostream& os) const
{
	const std::streamsize width = os.width();
	os.width(0);

	for(size_t i = 0; i < size; ++i)
	{
		os << "[ ";
		for(size_t j = 0; j < size; ++j)
		{
			os << std::setw(width) << getElement(i, j) << ' ';
		}
		os << "]\n";
	}
}

template<class T>
void
DiagonalMatrix<T>::printOnFile(std::ofstream& ofs) const
{
	const T* const dataEnd = data + size;
	for(T* i = data; i < dataEnd; ++i)
	{
		ofs << *i;
	}
	ofs << '\n';
}

template<class T>
void
DiagonalMatrix<T>::readIn(std::istream& is)
{
	const T* const dataEnd = data + size;
	for(T* i = data; i < dataEnd; ++i)
	{
		is >> *i;
	}
}

#endif
#endif

