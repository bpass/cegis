/**
 * @file TridiagonalMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: TridiagonalMatrix.hpp,v 1.3 2005/06/10 21:52:23 ahartman Exp $
 */

template<class T>
T TridiagonalMatrix<T>::nonConstZero = 0;

template<class T>
const T TridiagonalMatrix<T>::constZero = 0;

template<class T>
TridiagonalMatrix<T>::TridiagonalMatrix(const size_t& numRowsAndCols,
					const T& initialValue)
	: size(numRowsAndCols)
{
	if(size != 0)
	{
		// create an array of the proper size to hold the matrix
                data = new T[getArraySize(size)];

		// initialize the data to the given initial value
		const T* const arrayEnd = data + getArraySize(size);
		for(T* i = data; i < arrayEnd; ++i)
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
TridiagonalMatrix<T>::TridiagonalMatrix(const TridiagonalMatrix<T>& rhs)
	: Matrix<T>(rhs), size(rhs.size)
{
	if(size != 0)
	{
		// create an array of the proper size to hold the matrix
		data = new T[getArraySize(size)];

		// initialize the data to be a copy of rhs
		const T* const arrayEnd = data + getArraySize(size);
		for(T* i = data, *j = rhs.data; i < arrayEnd; ++i, ++j)
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
TridiagonalMatrix<T>::~TridiagonalMatrix()
{
	delete[] data;
}

template<class T>
TridiagonalMatrix<T>&
TridiagonalMatrix<T>::operator=(const TridiagonalMatrix<T>& rhs)
{
	if(rhs.size != 0)
	{
		// create an array of the proper size to hold the matrix
		// Also, copy over the data before deleting the old data
		// to ensure that self-assignment works
		T* newData = new T[getArraySize(rhs.size)];

		// set the data to be a copy of rhs
		const T* const arrayEnd = newData + getArraySize(rhs.size);
		for(T* i = newData, *j = rhs.data; i < arrayEnd; ++i, ++j)
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
TridiagonalMatrix<T>::operator*(const MyVector<T>& vec) const
{
        if(size != vec.getSize())
	{
		throw typename Matrix<T>::
		      IncompatibleMatrixSizes(size, size,
		                              vec.getSize(), vec.getSize());
	}

	MyVector<T> rv(size);
	if(size == 1)
	{
		rv[0] = getNonZeroElement(0,0) * vec[0];
	}
	else
	{
		// add the appropriate sums from the first column of the matrix
		rv[0] += getNonZeroElement(0, 0) * vec[0];
		rv[1] += getNonZeroElement(1, 0) * vec[0];

		// add the appropriate sums from the second through n-1 columns
		// of the matrix
		for(size_t j = 1; j < size - 1; ++j)
		{
			for(size_t i = j - 1; i <= j + 1; ++i)
			{
				rv[i] += getNonZeroElement(i, j) * vec[j];
			}
		}

		// add the appropriate sums from the last column of the matrix
		rv[size - 2] += 
			getNonZeroElement(size - 2, size - 1) * vec[size - 1];
		rv[size - 1] += 
			getNonZeroElement(size - 1, size - 1) * vec[size - 1];
	}

	return rv;
}

template<class T>
TridiagonalMatrix<T>&
TridiagonalMatrix<T>::operator+=(const TridiagonalMatrix<T>& rhs)
{
	if(size != rhs.size)
	{
		throw typename Matrix<T>::
		      IncompatibleMatrixSizes(size, size,
		                              rhs.size, rhs.size);
	}

	const T* const dataEnd = data + getArraySize(size);
	for(T* i = data, *j = rhs.data; i < dataEnd; ++i, ++j)
	{
                *i += *j;
	}

	return *this;
}

template<class T>
TridiagonalMatrix<T>
operator+(const TridiagonalMatrix<T>& lhs, const TridiagonalMatrix<T>& rhs)
{
	TridiagonalMatrix<T> returnValue(lhs);
	returnValue += rhs;
	return returnValue;
}

template<class T>
TridiagonalMatrix<T>
TridiagonalMatrix<T>::operator-() const
{
	TridiagonalMatrix<T> returnValue(*this);

	const T* const dataEnd = 
		returnValue.data + getArraySize(returnValue.size);
	for(T* i = returnValue.data; i < dataEnd; ++i)
	{
                *i = -(*i);
	}

	return returnValue;

}

template<class T>
TridiagonalMatrix<T>&
TridiagonalMatrix<T>::operator-=(const TridiagonalMatrix<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
TridiagonalMatrix<T>
operator-(const TridiagonalMatrix<T>& lhs, const TridiagonalMatrix<T>& rhs)
{
	TridiagonalMatrix<T> returnValue(lhs);
	returnValue += -rhs;
	return returnValue;
}

template<class T>
const size_t& 
TridiagonalMatrix<T>::getNumRows() const
{
	return size;
}

template<class T>
const size_t&
TridiagonalMatrix<T>::getNumCols() const
{
	return size;
}

template<class T>
T&
TridiagonalMatrix<T>::getElement(const size_t& row, const size_t& col)
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}

	if((row > col + 1) || (col > row + 1))
	{
		// set this in case the user has changed it
		nonConstZero = 0;
		return nonConstZero;
	}
	else
	{
		return getNonZeroElement(row, col);
	}
}

template<class T>
const T&
TridiagonalMatrix<T>::getElement(const size_t& row, const size_t& col) const
{
	if(row >= size)
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(col >= size)
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}

	if((row > col + 1) || (col > row + 1))
	{
		return constZero;
	}
	else
	{
		return getNonZeroElement(row, col);
	}
}

template<class T>
void
TridiagonalMatrix<T>::printOn(std::ostream& os) const
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
TridiagonalMatrix<T>::printOnFile(std::ofstream& ofs) const
{
	if(size == 1)
	{
		ofs << getNonZeroElement(0, 0) << " \n";
		return;
	}

	// output the non-zero elements, row by row

	// output the first row
	ofs << getNonZeroElement(0, 0) << ' ' 
	    << getNonZeroElement(0, 1) << " \n";

	// output the second through n-1 rows
	for(size_t i = 1; i < size - 1; ++i)
	{
		for(size_t j = i - 1; j <= i + 1; ++j)
		{
			ofs << getNonZeroElement(i, j) << ' ';
		}
		ofs << '\n';
	}

	// output the last row
	ofs << getNonZeroElement(size - 1, size - 2) << ' '
	    << getNonZeroElement(size - 1, size - 1) << " \n";
}

template<class T>
void 
TridiagonalMatrix<T>::readIn(std::istream& is)
{
	if(size == 1)
	{
		is >> getNonZeroElement(0, 0);
	}

	// read in the non-zero elements, row by row

	// read in the first row
	is >> getNonZeroElement(0, 0) >> getNonZeroElement(0, 1);

	// read in the second through n-1 rows
	for(size_t i = 1; i < size - 1; ++i)
	{
		for(size_t j = i - 1; j <= i + 1; ++j)
		{
			is >> getNonZeroElement(i, j);
		}
	}

	// read in the last row
	is >> getNonZeroElement(size - 1, size - 2) 
	   >> getNonZeroElement(size - 1, size - 1);
}

template<class T>
inline
size_t
TridiagonalMatrix<T>::getArraySize(const size_t& size) const
{
	// the size is cast to an int so that the formula works even if
	// size == 1
	return (4 + (static_cast<int>(size) - 2) * 3);
}

template<class T>
inline
T&
TridiagonalMatrix<T>::getNonZeroElement(const size_t& row, const size_t& col)
{
	return data[col + col + row];
}

template<class T>
inline
const T&
TridiagonalMatrix<T>::getNonZeroElement(const size_t& row, 
                                        const size_t& col) const
{
	return data[col + col + row];
}

