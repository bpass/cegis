/**
 * @file UpperTriangularMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: UpperTriangularMatrix.hpp,v 1.6 2005/12/07 23:49:04 ahartman Exp $
 */

#ifdef AUSTIN_UPPERTRIANGULARMATRIX_H
#ifndef AUSTIN_UPPERTRIANGULARMATRIX_HPP
#define AUSTIN_UPPERTRIANGULARMATRIX_HPP

template<class T>
T UpperTriangularMatrix<T>::nonConstZero = 0;

template<class T>
const T UpperTriangularMatrix<T>::constZero = 0;

template<class T>
UpperTriangularMatrix<T>::UpperTriangularMatrix(size_t numRowsAndCols,
                                                const T& initialValue)
	: size(numRowsAndCols)
{
	if(size != 0)
	{
		// create the two-dimensional array to hold the data
		data = create2DArray(size);

		// initialize all of the values to be the 
		// specified initial value
		for(size_t col = 0; col < size; ++col)
		{
			const T* const colEnd = data[col] + (col + 1);
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
UpperTriangularMatrix<T>::
UpperTriangularMatrix(const UpperTriangularMatrix<T>& rhs)
	: Matrix<T>(rhs), size(rhs.size)
{
	if(size != 0)
	{
		data = create2DArray(size);

		// initialize all of the values to be the 
		// same as rhs's values
		for(size_t col = 0; col < size; ++col)
		{
			const T* const colEnd = data[col] + (col + 1);
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
UpperTriangularMatrix<T>::~UpperTriangularMatrix()
{
	destroy2DArray(data, size);
}

template<class T>
UpperTriangularMatrix<T>&
UpperTriangularMatrix<T>::operator=(const UpperTriangularMatrix<T>& rhs)
{
	if(rhs.size != 0)
	{
                T** newData = create2DArray(rhs.size);

		// initialize all of the values to be the 
		// same as rhs's values
		for(size_t col = 0; col < rhs.size; ++col)
		{
			const T* const colEnd = newData[col] + (col + 1);
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
UpperTriangularMatrix<T>::operator*(const MyVector<T>& v) const
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
                for(size_t row = 0; row <= col; ++row)
		{
			rv[row] += data[col][row] * v[col];
		}
	}

	return rv;
}

template<class T>
UpperTriangularMatrix<T>&
UpperTriangularMatrix<T>::operator+=(const UpperTriangularMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(size, size,
		                                         rhs.size, rhs.size);
	}

	for(size_t col = 0; col < size; ++col)
	{
		const T* const colEnd = data[col] + (col + 1);
		for(T* i = data[col], *j = rhs.data[col]; 
		    i < colEnd; ++i, ++j)
		{
			*i += *j;
		}
	}

	return *this;
}

template<class T>
UpperTriangularMatrix<T>
operator+(const UpperTriangularMatrix<T>& lhs, 
          const UpperTriangularMatrix<T>& rhs)
{
	UpperTriangularMatrix<T> rv(lhs);
	rv += rhs;
	return rv;
}

template<class T>
UpperTriangularMatrix<T>
UpperTriangularMatrix<T>::operator-() const
{
	UpperTriangularMatrix<T> rv(*this);
	for(size_t col = 0; col < size; ++col)
	{
		const T* const colEnd = rv.data[col] + (col + 1);
		for(T* i = rv.data[col]; i < colEnd; ++i)
		{
			(*i) = -(*i);
		}
	}
	return rv;
}

template<class T>
UpperTriangularMatrix<T>&
UpperTriangularMatrix<T>::operator-=(const UpperTriangularMatrix<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
UpperTriangularMatrix<T>
operator-(const UpperTriangularMatrix<T>& lhs, 
          const UpperTriangularMatrix<T>& rhs)
{
	UpperTriangularMatrix<T> rv(lhs);
	rv += -rhs;
	return rv;
}

template<class T>
UpperTriangularMatrix<T>&
UpperTriangularMatrix<T>::operator*=(const UpperTriangularMatrix<T>& rhs)
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
		for(size_t row = 0; row <= col; ++row)
		{
			newData[col][row] = 0;
			for(size_t k = row; k <= col; ++k)
			{
				newData[col][row] += 
					data[k][row] * rhs.data[col][k];
			}
		}
	}

	destroy2DArray(data, size);
	data = newData;
	return *this;
}

template<class T>
UpperTriangularMatrix<T>
operator*(const UpperTriangularMatrix<T>& lhs, 
          const UpperTriangularMatrix<T>& rhs)
{
	if(lhs.size != rhs.size)
	{
		throw typename
		      Matrix<T>::IncompatibleMatrixSizes(lhs.size, lhs.size,
		                                         rhs.size, rhs.size);
	}

	UpperTriangularMatrix<T> rv(lhs.size, 0);

	for(size_t col = 0; col < rv.size; ++col)
	{
		for(size_t row = 0; row <= col; ++row)
		{
			for(size_t k = row; k <= col; ++k)
			{
				rv.data[col][row] += 
					lhs.data[k][row] * rhs.data[col][k];
			}
		}
	}

	return rv;
}

template<class T>
DenseMatrix<T>
operator*(const UpperTriangularMatrix<T>& lhs,
          const Matrix<T>& rhs)
{
	if(lhs.size != rhs.getNumRows())
	{
		throw typename Matrix<T>::
		IncompatibleMatrixSizes(lhs.size, lhs.size,
		                        rhs.getNumRows(), rhs.getNumCols());
	}

	DenseMatrix<T> rv(lhs.size, rhs.getNumCols(), 0);
	for(size_t col = 0; col < rv.getNumCols(); ++col)
	{
		for(size_t row = 0; row < rv.getNumRows(); ++row)
		{
			for(size_t k = row; k < lhs.size; ++k)
			{
				rv[row][col] += lhs.data[k][row] * rhs[k][col];
			}
		}
	}
	return rv;
}

template<class T>
DenseMatrix<T>
operator*(const Matrix<T>& lhs,
	  const UpperTriangularMatrix<T>& rhs)
{
	if(lhs.getNumCols() != rhs.size)
	{
		throw typename Matrix<T>::
		IncompatibleMatrixSizes(lhs.getNumRows(), lhs.getNumCols(),
		                        rhs.size, rhs.size);
	}

	DenseMatrix<T> rv(lhs.getNumRows(), rhs.size, 0);
	for(size_t col = 0; col < rv.getNumCols(); ++col)
	{
		for(size_t row = 0; row < rv.getNumRows(); ++row)
		{
			for(size_t k = 0; k <= col; ++k)
			{
				rv[row][col] += lhs[row][k] * rhs.data[col][k];
			}
		}
	}
	return rv;
}

template<class T>
size_t
UpperTriangularMatrix<T>::getNumRows() const
{
	return size;
}

template<class T>
size_t
UpperTriangularMatrix<T>::getNumCols() const
{
	return size;
}

template<class T>
T&
UpperTriangularMatrix<T>::getElement(size_t row, size_t col)
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(row <= col)
	{
		return data[col][row];
	}
	nonConstZero = 0;
	return nonConstZero;
}

template<class T>
const T&
UpperTriangularMatrix<T>::getElement(size_t row, size_t col) const
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	if(row <= col)
	{
		return data[col][row];
	}
	return constZero;
}

template<class T>
void
UpperTriangularMatrix<T>::printOn(std::ostream& os) const
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
UpperTriangularMatrix<T>::printOnFile(std::ofstream& ofs) const
{
	for(size_t col = 0; col < size; ++col)
	{
		for(size_t row = 0; row <= col; ++row)
		{
			ofs << data[col][row] << ' ';
		}
		ofs << '\n';
	}
}

template<class T>
void
UpperTriangularMatrix<T>::readIn(std::istream& is)
{
	for(size_t col = 0; col < size; ++col)
	{
		for(size_t row = 0; row <= col; ++row)
		{
			is >> data[col][row];
		}
	}
}

template<class T>
T**
UpperTriangularMatrix<T>::create2DArray(size_t n) const
{
	T** rv = new T*[n];
	for(size_t i = 0; i < n; ++i)
	{
		rv[i] = new T[i+1];
	}
	return rv;
}

template<class T>
void
UpperTriangularMatrix<T>::destroy2DArray(T** array, size_t n) const
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

