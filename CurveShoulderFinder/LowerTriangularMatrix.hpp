/**
 * @file LowerTriangularMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: LowerTriangularMatrix.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_LOWERTRIANGULARMATRIX_H
#ifndef AUSTIN_LOWERTRIANGULARMATRIX_HPP
#define AUSTIN_LOWERTRIANGULARMATRIX_HPP

template<class T>
T LowerTriangularMatrix<T>::nonConstZero = 0;

template<class T>
const T LowerTriangularMatrix<T>::constZero = 0;

template<class T>
LowerTriangularMatrix<T>::LowerTriangularMatrix(const size_t& numRowsAndCols,
                                                const T& initialValue)
	: size(numRowsAndCols)
{
        if(size != 0)
	{
		data = create2DArray(size);

		for(size_t col = 0; col < size; ++col)
		{
			const T* const colEnd = data[col] + (size - col);
			for(T* i = data[col]; i < colEnd; ++i)
			{
				*i = initialValue;
			}
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
LowerTriangularMatrix<T>::
LowerTriangularMatrix(const LowerTriangularMatrix<T>& rhs)
	: Matrix<T>(rhs), size(rhs.size)
{
	if(size != 0)
	{
		data = create2DArray(size);

		// initialize all of the values to be the 
		// same as rhs's values
		for(size_t col = 0; col < size; ++col)
		{
                        const T* const colEnd = data[col] + (size - col);
			for(T* i = data[col], *j = rhs.data[col];
			    i < colEnd; ++i, ++j)
			{
				*i = *j;
			}
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
LowerTriangularMatrix<T>::~LowerTriangularMatrix()
{
	destroy2DArray(data, size);
}

template<class T>
LowerTriangularMatrix<T>&
LowerTriangularMatrix<T>::operator=(const LowerTriangularMatrix<T>& rhs)
{
	if(rhs.size != 0)
	{
                T** newData = create2DArray(rhs.size);

		// initialize all of the values to be the 
		// same as rhs's values
		for(size_t col = 0; col < rhs.size; ++col)
		{
			const T* const colEnd = newData[col] + (rhs.size - col);
			for(T* i = newData[col], *j = rhs.data[col]; 
			    i < colEnd; ++i, ++j)
			{
				*i = *j;
			}
		}

		destroy2DArray(data, size);
		data = newData;
	}
	else
	{
		destroy2DArray(data, size);
		data = NULL;
	}

	size = rhs.size;
	return *this;
}

template<class T>
MyVector<T>
LowerTriangularMatrix<T>::operator*(const MyVector<T>& v) const
{
        if(size != v.getSize())
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         v.getSize(), 1);
	}

	MyVector<T> rv(size);
	for(size_t col = 0; col < size; ++col)
	{
                for(size_t row = 0; row < size - col; ++row)
		{
			rv[col + row] += data[col][row] * v[col];
		}
	}

	return rv;
}

template<class T>
LowerTriangularMatrix<T>&
LowerTriangularMatrix<T>::operator+=(const LowerTriangularMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         rhs.size, rhs.size);
	}

	for(size_t col = 0; col < size; ++col)
	{
		const T* const colEnd = data[col] + (size - col);
		for(T* i = data[col], *j = rhs.data[col]; 
		    i < colEnd; ++i, ++j)
		{
			*i += *j;
		}
	}

	return *this;
}

template<class T>
LowerTriangularMatrix<T>
operator+(const LowerTriangularMatrix<T>& lhs, 
          const LowerTriangularMatrix<T>& rhs)
{
	LowerTriangularMatrix<T> rv(lhs);
	rv += rhs;
	return rv;
}

template<class T>
LowerTriangularMatrix<T>
LowerTriangularMatrix<T>::operator-() const
{
	LowerTriangularMatrix<T> rv(*this);
	for(size_t col = 0; col < size; ++col)
	{
		const T* const colEnd = rv.data[col] + (size - col);
		for(T* i = rv.data[col]; i < colEnd; ++i)
		{
			(*i) = -(*i);
		}
	}
	return rv;
}

template<class T>
LowerTriangularMatrix<T>&
LowerTriangularMatrix<T>::operator-=(const LowerTriangularMatrix<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
LowerTriangularMatrix<T>
operator-(const LowerTriangularMatrix<T>& lhs, 
          const LowerTriangularMatrix<T>& rhs)
{
	LowerTriangularMatrix<T> rv(lhs);
	rv += -rhs;
	return rv;
}

template<class T>
LowerTriangularMatrix<T>&
LowerTriangularMatrix<T>::operator*=(const LowerTriangularMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         rhs.size, rhs.size);
	}

	T** newData = create2DArray(size);

	for(size_t col = 0; col < size; ++col)
	{
		for(size_t row = 0; row < size - col; ++ row)
		{
			newData[col][row] = 0;
			for(size_t k = 0; k <= row; ++k)
			{
				newData[col][row] += 
					data[col+k][row-k] * 
					rhs.data[col][k];
			}
		}
	}

	destroy2DArray(data, size);
	data = newData;

	return *this;
}

template<class T>
LowerTriangularMatrix<T>
operator*(const LowerTriangularMatrix<T>& lhs, 
          const LowerTriangularMatrix<T>& rhs)
{
	if(lhs.size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(lhs.size, lhs.size,
		                                         rhs.size, rhs.size);
	}

	LowerTriangularMatrix<T> rv(lhs.size, 0);

	for(size_t col = 0; col < rv.size; ++col)
	{
		for(size_t row = 0; row < rv.size - col; ++row)
		{
			for(size_t k = 0; k <= row; ++k)
			{
				rv.data[col][row] += 
					lhs.data[col+k][row-k] * 
					rhs.data[col][k];
			}
		}
	}

	return rv;
}

template<class T>
const size_t&
LowerTriangularMatrix<T>::getNumRows() const
{
	return size;
}

template<class T>
const size_t&
LowerTriangularMatrix<T>::getNumCols() const
{
	return size;
}

template<class T>
T&
LowerTriangularMatrix<T>::getElement(const size_t& row, const size_t& col)
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(col <= row)
	{
		return data[col][row - col];
	}
	nonConstZero = 0;
	return nonConstZero;
}

template<class T>
const T&
LowerTriangularMatrix<T>::getElement(const size_t& row, const size_t& col) const
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(col <= row)
	{
		return data[col][row - col];
	}
	return constZero;
}

template<class T>
void
LowerTriangularMatrix<T>::printOn(std::ostream& os) const
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
LowerTriangularMatrix<T>::printOnFile(std::ofstream& ofs) const
{
	for(size_t col = 0; col < size; ++col)
	{
		for(size_t row = 0; row < size - col; ++row)
		{
			ofs << data[col][row] << ' ';
		}
		ofs << '\n';
	}
}

template<class T>
void
LowerTriangularMatrix<T>::readIn(std::istream& is)
{
	for(size_t col = 0; col < size; ++col)
	{
		for(size_t row = 0; row < size - col; ++row)
		{
			is >> data[col][row];
		}
	}
}

template<class T>
T**
LowerTriangularMatrix<T>::create2DArray(const size_t& n) const
{
	T** array = new T*[n];
	for(size_t i = 0; i < n; ++i)
	{
		array[i] = new T[n - i];
	}
	return array;
}

template<class T>
void
LowerTriangularMatrix<T>::destroy2DArray(T** array, const size_t& n) const
{
	const T* const * const arrayEnd = array + n;
	for(T** i = array; i < arrayEnd; ++i)
	{
		delete[] *i;
	}
	delete[] array;
}

#endif
#endif

