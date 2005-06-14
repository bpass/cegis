/**
 * @file DenseMatrix.hpp
 * @author Austin Hartman
 *
 * $Id: DenseMatrix.hpp,v 1.3 2005/06/14 23:44:34 ahartman Exp $
 */

/******************************************
**** DenseMatrix functions ****************
*******************************************/
template<class T>
DenseMatrix<T>::DenseMatrix(const size_t& rows, const size_t& cols, 
                            const T& initialValue)
	: numRows(rows), numCols(cols)
{
	if(numRows == 0 || numCols == 0)
	{
		numRows = 0; 
		numCols = 0;
		data = NULL;
	}
	else
	{
		data = new(std::nothrow) T[numRows * numCols];

		for(size_t i = 0; i < numRows * numCols; ++i)
		{
			data[i] = initialValue;
		}
	}
}

template<class T>
DenseMatrix<T>::DenseMatrix(const MyVector<T>& v)
	: numRows(v.getSize()), numCols((numRows != 0)?1:0)
{
	if(numRows == 0)
	{
		data = NULL;
	}
	else
	{
		data = new T[numRows];
		for(size_t i = 0; i < numRows; ++i)
		{
			data[i] = v[i];
		}
	}
}

template<class T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix<T>& rhs)
	: Matrix<T>(rhs), numRows(rhs.numRows), numCols(rhs.numCols)
{
	if(numRows == 0)
	{
		data = NULL;
	}
	else
	{
                data = new T[numRows * numCols];
		for(size_t i = 0; i < numRows * numCols; ++i)
		{
			data[i] = rhs.data[i];
		}
	}
}

template<class T>
DenseMatrix<T>::DenseMatrix(const Matrix<T>& rhs)
	: Matrix<T>(rhs), numRows(rhs.getNumRows()), numCols(rhs.getNumCols())
{
	if(numRows == 0)
	{
		data = NULL;
		numCols = 0;
	}
	else if(numCols == 0)
	{
		data = NULL;
		numRows = 0;
	}
	else
	{
		data = new T[numRows * numCols];
		T* iter = data;
		for(size_t j = 0; j < numCols; ++j)
		{
			for(size_t i = 0; i < numRows; ++i)
			{
				*iter = rhs[i][j];
				++iter;
			}
		}
	}
}

template<class T>
DenseMatrix<T>::~DenseMatrix()
{
	delete[] data;
}

template<class T>
DenseMatrix<T>&
DenseMatrix<T>::operator=(const DenseMatrix<T>& rhs)
{
	// if the other matrix has no elements
	if(rhs.numRows == 0)
	{
		delete[] data;
		data = NULL;
		numRows = 0;
		numCols = 0;
	}
	else
	{
                // ensure that self-assignment works without problems by
		// making a copy of the old array first before deleting
		// the old one
		T* newData = new T[rhs.numRows * rhs.numCols];
		for(size_t i = 0; i < rhs.numRows * rhs.numCols; ++i)
		{
			newData[i] = rhs.data[i];
		}
		delete[] data;
		data = newData;
		numRows = rhs.numRows;
		numCols = rhs.numCols;
	}

	return *this;
}

template<class T>
DenseMatrix<T>&
DenseMatrix<T>::operator=(const Matrix<T>& rhs)
{
	const size_t newNumRows = rhs.getNumRows();
	const size_t newNumCols = rhs.getNumCols();
	if(newNumRows == 0 || newNumCols == 0)
	{
		delete[] data;
		data = NULL;
		numRows = 0;
		numCols = 0;
	}
	else
	{
		T* newData = new T[newNumRows * newNumCols];
		T* iter = newData;
		for(size_t j = 0; j < newNumCols; ++j)
		{
			for(size_t i = 0; i < newNumRows; ++i)
			{
				*iter = rhs[i][j];
				++iter;
			}
		}
		delete[] data;
		data = newData;
		numRows = newNumRows;
		numCols = newNumCols;
	}
	return *this;
}


template<class T>
DenseMatrix<T>&
DenseMatrix<T>::operator+=(const DenseMatrix<T>& rhs)
{
	if(numRows != rhs.numRows || numCols != rhs.numCols)
	{
		throw typename Matrix<T>::IncompatibleMatrixSizes
			(numRows, numCols, rhs.numRows, rhs.numCols);
	}
	for(size_t i = 0; i < numRows * numCols; ++i)
	{
		data[i] += rhs.data[i];
	}
	return *this;
}

template<class T>
DenseMatrix<T>
operator+(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs)
{
	DenseMatrix<T> returnValue(lhs);
	returnValue += rhs;
	return returnValue;
}

template<class T>
DenseMatrix<T>
operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    return DenseMatrix<T>(lhs) + DenseMatrix<T>(rhs);
}

template<class T>
DenseMatrix<T>
DenseMatrix<T>::operator-() const
{
	DenseMatrix<T> returnValue(*this);
	for(size_t i = 0; i < returnValue.numRows * returnValue.numCols; ++i)
	{
		returnValue.data[i] = -returnValue.data[i];
	}
	return returnValue;
}

template<class T>
DenseMatrix<T>&
DenseMatrix<T>::operator-=(const DenseMatrix<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
DenseMatrix<T>
operator-(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs)
{
	DenseMatrix<T> returnValue(lhs);
	returnValue -= rhs;
	return returnValue;
}


template<class T>
DenseMatrix<T>&
DenseMatrix<T>::operator*=(const DenseMatrix<T>& rhs)
{
	if(numCols != rhs.numRows)
	{
		throw typename Matrix<T>::IncompatibleMatrixSizes
			(numRows, numCols, rhs.numRows, rhs.numCols);
	}

	// if numRows == 0, this matrix is already equal to the result,
	// which would also be an empty matrix
	if(numRows != 0)
	{
		T* newData = new T[numRows * rhs.numCols];

		// copy over the result into newData
		// start with the column variable first, because matrices
		// are stored in column major order
		for(size_t j = 0; j < rhs.numCols; ++j)
		{
			for(size_t i = 0; i < numRows; ++i)
			{
				// get an easier-to-read name for the
				// current element
				T& b = newData[i + j * numRows];

				// set the current element to be the
				// dot product of the ith row of the first
				// matrix and the jth column of the second
				// matrix
				b = 0;
				for(size_t k = 0; k < numCols; ++k)
				{
					b += a(i, k) * rhs.a(k, j);
				}
			}
		}

		delete[] data;
		data = newData;
		numCols = rhs.numCols;
		// numRows is already correct
	}
	return *this;
}

template<class T>
DenseMatrix<T>
operator*(const DenseMatrix<T>& lhs, const DenseMatrix<T>& rhs)
{
	if(lhs.numCols != rhs.numRows)
	{
		throw typename Matrix<T>::IncompatibleMatrixSizes
			(lhs.numRows, lhs.numCols,
			 rhs.numRows, rhs.numCols);
	}

	// create a matrix with every element set to 0
	DenseMatrix<T> rv(lhs.numRows, rhs.numCols, 0);

	// copy over the result into newData
	// start with the column variable first, because matrices
	// are stored in column major order
	for(size_t j = 0; j < rv.numCols; ++j)
	{
		for(size_t i = 0; i < rv.numRows; ++i)
		{
			// set the current element to be the
			// dot product of the ith row of the first
			// matrix and the jth column of the second
			// matrix
			for(size_t k = 0; k < lhs.numCols; ++k)
			{
				rv.a(i, j) += lhs.a(i, k) * rhs.a(k, j);
			}
		}
	}
	return rv;
}


template<class T>
T&
DenseMatrix<T>::getElement(const size_t& row, const size_t& col)
{
	if(!validRow(row))
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(!validColumn(col))
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	return a(row, col);
}

template<class T>
const T&
DenseMatrix<T>::getElement(const size_t& row, const size_t& col) const
{
	if(!validRow(row))
	{
		throw typename Matrix<T>::RowRangeError(row);
	}
	if(!validColumn(col))
	{
		throw typename Matrix<T>::ColumnRangeError(col);
	}
	return a(row, col);
}


template<class T>
MyVector<T>
DenseMatrix<T>::operator*(const MyVector<T>& v) const
{
	if(numCols != v.getSize())
	{
		throw typename Matrix<T>::IncompatibleMatrixSizes
			(numRows, numCols, v.getSize(), 1);
		// vectors always have one column
	}

	// again, go with the column number in the outer loop because
	// matrices are stored in column major order
	MyVector<T> result(numRows);
	for(size_t j = 0; j < numCols; ++j)
	{                            
		for(size_t i = 0; i < numRows; ++i)
		{
			result[i] += a(i, j) * v[j];
		}
	}
	return result;
}


template<class T>
void
DenseMatrix<T>::printOn(std::ostream& os) const
{
	std::streamsize width = os.width();
	os.width(0);
	for(size_t i = 0; i < numRows; ++i)
	{
		os << "[ ";
		for(size_t j = 0; j < numCols; ++j)
		{
			os << std::setw(width) << a(i, j) << ' ';
		}
		os << "]\n";
	}
}

template<class T>
void
DenseMatrix<T>::printOnFile(std::ofstream& ofs) const
{
        for(size_t i = 0; i < numRows; ++i)
	{
		for(size_t j = 0; j < numCols; ++j)
		{
			ofs << a(i, j) << ' ';
		}
		ofs << '\n';
	}
}

template<class T>
void
DenseMatrix<T>::readIn(std::istream& is)
{
	for(size_t i = 0; i < numRows; ++i)
	{
		for(size_t j = 0; j < numCols; ++j)
		{
			is >> a(i, j);
		}
	}
}


template<class T>
const size_t&
DenseMatrix<T>::getNumRows() const
{
	return numRows;
}

template<class T>
const size_t&
DenseMatrix<T>::getNumCols() const
{
	return numCols;
}

template<class T>
void
DenseMatrix<T>::setSize(const size_t& newNumRows, const size_t& newNumCols,
                        const T& value)
{
	delete[] data;
	if(newNumRows == 0 || newNumCols == 0)
	{
		data = NULL;
		numRows = 0;
		numCols = 0;
	}
	else
	{
		numRows = newNumRows;
		numCols = newNumCols;
		data = new T[numRows * numCols];

		for(size_t j = 0; j < numCols; ++j)
		{
			for(size_t i = 0; i < numRows; ++i)
			{
				a(i, j) = value;
			}
		}
	}
}


template<class T>
MyVector<T>
DenseMatrix<T>::getColumnAsVector(const size_t& column) const
{
	if(!validColumn(column))
	{
		throw typename Matrix<T>::ColumnRangeError(column);
	}

	MyVector<T> vec(numRows);
	for(size_t i = 0; i < numRows; ++i)
	{
		vec[i] = a(i, column);
	}
	return vec;
}

template<class T>
MyVector<T>
DenseMatrix<T>::getRowAsVector(const size_t& row) const
{
	if(!validRow(row))
	{
		throw typename Matrix<T>::RowRangeError(row);
	}

	MyVector<T> vec(numCols);
        for(size_t j = 0; j < numCols; ++j)
	{
		vec[j] = a(row, j);
	}
	return vec;
}


template<class T>
DenseMatrix<T>
DenseMatrix<T>::transpose() const
{
	DenseMatrix<T> rv(numCols, numRows);
	for(size_t j = 0; j < numRows; ++j)
	{
		for(size_t i = 0; i < numCols; ++i)
		{
			rv.a(i, j) = a(j, i);
		}
	}
	return rv;
}


template<class T>
void
DenseMatrix<T>::rowInterchange(const size_t& row1, const size_t& row2)
{
	if(!validRow(row1))
	{
                throw typename Matrix<T>::RowRangeError(row1);
	}
	if(!validRow(row2))
	{
		throw typename Matrix<T>::RowRangeError(row2);
	}

	for(size_t j = 0; j < numCols; ++j)
	{
		std::swap(a(row1, j), a(row2, j));
	}
}

template<class T>
void
DenseMatrix<T>::rowScale(const size_t& row, const T& constant)
{
	if(!validRow(row))
	{
		throw typename Matrix<T>::RowRangeError(row);
	}

	for(size_t j = 0; j < numCols; ++j)
	{
		a(row, j) *= constant;
	}
}

template<class T>
void
DenseMatrix<T>::rowReplace(const size_t& rowToReplace,
			   const size_t& otherRow, const T& multiple)
{
	if(!validRow(rowToReplace))
	{
		throw typename Matrix<T>::RowRangeError(rowToReplace);
	}
	if(!validRow(otherRow))
	{
		throw typename Matrix<T>::RowRangeError(otherRow);
	}

	for(size_t j = 0; j < numCols; ++j)
	{
		a(rowToReplace, j) += a(otherRow, j) * multiple;
	}
}


template<class T>
inline
T&
DenseMatrix<T>::a(const size_t& row, const size_t& column)
{
	return data[row + column * numRows];
}

template<class T>
inline
const T&
DenseMatrix<T>::a(const size_t& row, const size_t& column) const
{
	return data[row + column * numRows];
}


template<class T>
inline
bool
DenseMatrix<T>::validRow(const size_t& row) const
{
	return row < numRows;
}

template<class T>
inline
bool
DenseMatrix<T>::validColumn(const size_t& col) const
{
	return col < numCols;
}

