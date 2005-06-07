#include <cmath> // for sqrt

template<class T>
MyVector<T>::MyVector()
	: size(0), data(NULL)
{}

template<class T>
MyVector<T>::MyVector(size_t newSize)
        : size(newSize) 
{
	if(size != 0)
	{
		data = new T[newSize];
                
		// set every term equal to 0
		for(size_t i = 0; i < size; ++i)
		{
			data[i] = 0;
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
MyVector<T>::MyVector(const MyVector<T>& rhs)
	: size(rhs.size) 
{
	if(size != 0)
	{         
		data = new T[rhs.size];

		// copy every term element by element
		for(size_t i = 0; i < size; ++i)
		{
			data[i] = rhs.data[i];
		}
	}
	else
	{
		data = NULL;
	}
}

template<class T>
MyVector<T>::~MyVector()
{
	delete[] data;
}

template<class T>
const MyVector<T>&
MyVector<T>::operator=(const MyVector<T>& rhs)
{
	if(rhs.size != 0)
	{
		// copy all the data from rhs
		T* newData = new T[rhs.size];
		for(size_t i = 0; i < rhs.size; ++i)
		{
			newData[i] = rhs.data[i];
		}

		// delete the old data
		// this is done after the data is copied in case this == &rhs
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
const MyVector<T>&
MyVector<T>::operator+=(const MyVector<T>& rhs)
{
	// if the sizes are incompatible, throw an exception that tells the
	// user from where it was thrown
	if(size != rhs.size)
	{
		std::string message = __FILE__;
		message += ':';
		message += __LINE__;
		message += ':';
		message += __FUNCTION__; 
		throw IncompatibleVectorSizes(size, rhs.size, message);
	}

	// add all the terms in rhs to the corresponding terms in this
	for(size_t i = 0; i < size; ++i)
	{
		data[i] += rhs.data[i];
	}
	return *this;
}

template<class T>
MyVector<T>
operator+(const MyVector<T>& lhs, const MyVector<T>& rhs)
{
	MyVector<T> returnValue(lhs);
	returnValue += rhs;
	return returnValue;
}

template<class T>
const MyVector<T>&
MyVector<T>::operator-=(const MyVector<T>& rhs)
{
	*this += -rhs;
	return *this;
}

template<class T>
MyVector<T>
operator-(const MyVector<T>& lhs, const MyVector<T>& rhs)
{
	MyVector<T> returnValue(lhs);
	returnValue += -rhs;
	return returnValue;
}

template<class T>
MyVector<T>
MyVector<T>::operator-() const
{
	// set all the terms in the new vector to the opposite of
	// what they currently are
	MyVector<T> returnValue(*this);
	for(size_t i = 0; i < returnValue.size; ++i)
	{
		returnValue[i] = -returnValue[i];
	}
	return returnValue;
}

template<class T>
T
operator*(const MyVector<T>& lhs, const MyVector<T>& rhs)
{
	// if the sizes are incompatible, throw an exception that tells the
	// user from where it was thrown
	if(lhs.size != rhs.size)
	{
		std::string message = __FILE__;
		message += ':';
		message += __LINE__;
		message += ':';
		message += __FUNCTION__; 
		throw MyVector<T>::IncompatibleVectorSizes(lhs.size, 
		                                           rhs.size, 
		                                           message);
	}

	// sum up the results of multiplying the corresponding terms in each
	// vector
	T returnValue(0);
	for(size_t i = 0; i < lhs.size; ++i)
	{
		returnValue += lhs.data[i] * rhs.data[i];
	}
	return returnValue;
}

template<class T>
MyVector<T>
MyVector<T>::operator*(const T& scalar) const
{
	// multiply each term in the vector by a scalar
        MyVector<T> returnValue(*this);
	for(size_t i = 0; i < returnValue.size; ++i)
	{
		returnValue[i] *= scalar;
	}
	return returnValue;
}

template<class T>
inline 
MyVector<T> 
operator*(const T& scalar, const MyVector<T>& v)
{      
	// do the same thing as the member function since scalar
	// multiplication is commutative
	return v*scalar;
}

template<class T>
inline
MyVector<T>
MyVector<T>::operator/(const T& scalar) const
{
        return (*this) * (1/scalar);
}

template<class T>
inline
size_t
MyVector<T>::getSize() const
{
	return size;
}

template<class T>
void
MyVector<T>::setSize(size_t newSize)
{
	if(newSize != 0)
	{
		T* newData = new T[newSize];

		// copy the old data
		size_t copySize = std::min(size, newSize);
		for(size_t i = 0; i < copySize; ++i)
		{
			newData[i] = data[i];
		}

		delete[] data;
		data = newData;

		// set all the uninitialized elements to 0
		for(size_t i = size; i < newSize; ++i)
		{
			data[i] = 0;
		}
	}
	else
	{
		delete[] data;
		data = NULL;
	}
	size = newSize;
}

template<class T>
T&
MyVector<T>::operator[](size_t index)
{
	if(index >= size)
	{
		throw RangeError(index);
	}

	return data[index];
}

template<class T>
const T&
MyVector<T>::operator[](size_t index) const
{
	if(index >= size)
	{
		throw RangeError(index);
	}

	return data[index];
}

template<class T>
T
MyVector<T>::getMagnitude() const
{
	// calculate the sum of the squares of the components of the vector
	T returnValue(0);
        for(size_t i = 0; i < size; ++i)
	{
		returnValue += data[i] * data[i];
	}
	// return the square root of that value
	returnValue = std::sqrt(returnValue);
	return returnValue;
}

template<class T>
std::ostream&
operator<<(std::ostream& os, const MyVector<T>& v)
{
	os << '<';

	// check this to make sure vectors with a size of 0 print
	// without segfaulting
	if(v.size > 0)
	{
		// output the first size - 1 elements
		for(size_t i = 0; i < v.size - 1; ++i)
		{
			os << v.data[i] << ", ";
		}
		// output the last element
		os << v.data[v.size - 1];
	}
	os << ">";

	return os;
}

template<class T>
std::istream&
operator>>(std::istream& is, MyVector<T>& v)
{
	for(size_t i = 0; i < v.size; ++i)
	{
		is >> v.data[i];
	}

	return is;
}



template<class T>
inline
MyVector<T>::IncompatibleVectorSizes::
IncompatibleVectorSizes(size_t size1, size_t size2, std::string message)
	: s1(size1), s2(size2), msg(message)
{}

template<class T>
inline
size_t
MyVector<T>::IncompatibleVectorSizes::getSize1() const
{
	return s1;
}

template<class T>
inline
size_t
MyVector<T>::IncompatibleVectorSizes::getSize2() const
{
	return s2;
}

template<class T>
inline
const std::string&
MyVector<T>::IncompatibleVectorSizes::getMessage() const
{
	return msg;
}



template<class T>
inline
MyVector<T>::RangeError::RangeError(size_t n)
	: subscript(n)
{}

template<class T>
inline
size_t
MyVector<T>::RangeError::badSubscript() const
{
	return subscript;
}

