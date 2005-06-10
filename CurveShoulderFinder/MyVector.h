/**
 * @file MyVector.h
 * @author Austin Hartman
 *
 * $Id: MyVector.h,v 1.3 2005/06/10 21:52:23 ahartman Exp $
 */

#ifndef AUSTIN_MYVECTOR_H
#define AUSTIN_MYVECTOR_H

#include <iostream>
#include <string>



template<class T>
class MyVector;

template<class T>
MyVector<T> 
operator+(const MyVector<T>& lhs, const MyVector<T>& rhs);

template<class T>
MyVector<T> 
operator-(const MyVector<T>& lhs, const MyVector<T>& rhs);

template<class T>
T 
operator*(const MyVector<T>& lhs, const MyVector<T>& rhs);

template<class T>
std::ostream& 
operator<<(std::ostream& os, const MyVector<T>& v);

template<class T>
std::istream& 
operator>>(std::istream& is, MyVector<T>& v);



/**
 * All functions require that T be assignable.
 */
template<class T>
class MyVector
{
public:
	/**
	 * Default constructor.  Creates an empty vector (getSize() == 0)
	 */
	MyVector();

	/**
	 * Constructor to create a vector of the specified size.    
	 * Pre: T must be assignable to 0.
	 * Post: All elements are initialized to 0.
	 */
	explicit MyVector(size_t newSize);

	/**
	 * Copy constructor.
	 * Pre: None
	 * Post: Creates a vector that is a copy of rhs such that its
	 * size is the same, as are all of its elements.
	 */
	MyVector(const MyVector<T>& rhs);

	/**
	 * Destructor.
	 * Pre: None
	 * Post: The object is destructed and memory is freed.
	 */
	~MyVector();

	/**
	 * Assignment operator.
	 * Pre: None
	 * Post: Sets this vector to be a copy of rhs, such that its size
	 * is the same, as are all of its elements.
	 */
	const MyVector<T>& operator=(const MyVector<T>& rhs);

	/**
	 * Adds the terms in rhs to the terms in this vector.
	 * Pre: T defines operator+=, and this and rhs are the same size.
	 * Post: The terms in rhs have been added to this vector, or a 
	 * MyVector::IncompatibleVectorSizes exception is thrown if lhs and
	 * rhs are not the same size.
	 */
	const MyVector<T>& operator+=(const MyVector<T>& rhs);

	/**
	 * Adds the terms in lhs and rhs to form a new vector.
	 * Pre: T defines operator+=, and lhs and rhs are the same size.
	 * Post: Returns the new vector that is the same size as lhs and
	 * rhs and whose terms are the sum of the terms in lhs and rhs, 
	 * or a MyVector::IncompatibleVectorSizes exception is thrown if lhs 
	 * and rhs are not the same size.
	 */
	friend MyVector<T> operator+ <>(const MyVector<T>& lhs, 
	                                const MyVector<T>& rhs);

	/**
	 * Subtracts the terms in rhs from this vector.
	 * Pre: T defines operator+= and unary operator-, and this and
	 * rhs are the same size.
	 * Post: The terms in rhs have been subtracted from this vector, 
	 * or a MyVector::IncompatibleVectorSizes exception is thrown if 
	 * lhs and rhs are not the same size.
	 */
	const MyVector<T>& operator-=(const MyVector<T>& rhs);

	/**
	 * Subtracts the terms in rhs from lhs to form a new vector.
	 * Pre: T defines operator+= and unary operator-, and lhs and rhs
	 * are the same size.
	 * Post: Returns the new vector that is the same size as lhs and
	 * rhs and whose terms are the result of subtracting rhs from lhs, 
	 * or a MyVector::IncompatibleVectorSizes exception is thrown if 
	 * lhs and rhs are not the same size.
	 */
	friend MyVector<T> operator- <>(const MyVector<T>& lhs,
	                                const MyVector<T>& rhs);

	/**
	 * Returns a new vector whose terms are the opposite of the terms
	 * in this.
	 * Pre: T defines unary operator-.
	 * Post: Returns a new vector whose terms are the terms in this set
	 * equal to themselves with unary operator- applied
	 */ 
	MyVector<T> operator-() const;

	/**
	 * Finds the dot product of two vectors.
	 * Pre: T is constructable from 0, T defines binary operator*
	 * and operator+=, and lhs and rhs are the same size.
	 * Post: Returns a new vector that is the result of multiplying 
	 * the corresponding terms in lhs and rhs and summing them together, 
	 * or a MyVector::IncompatibleVectorSizes exception is thrown if lhs 
	 * and rhs are not the same size.
	 */
	friend T operator* <>(const MyVector<T>& lhs,
	                      const MyVector<T>& rhs);

	/**
	 * Multiplies a vector by a scalar.
	 * Pre: T defines operator*=
	 * Post: Returns a new vector that is the result of multiplying this 
	 * vector by a scalar.
	 */
	MyVector<T> operator*(const T& scalar) const;

	/**
	 * Divides a vector by a scalar.
	 * Pre: T defines operator*= and operator/
	 * Post: Returns a new vector that is the result of dividing this
	 * vector by a scalar.
	 */
	MyVector<T> operator/(const T& scalar) const;

	/**
	 * Pre: None
	 * Post: Returns the size of the vector, which is greater than or
	 * equal to 0.
	 */
	size_t getSize() const;

	/**
	 * Sets the new size of the vector.
	 * Pre: T is assignable to 0.
	 * Post: The size of this vector has been set to newSize.  Any old
	 * terms that fit into a vector with the new size have been copied.
	 * If the new size was greater than the old size, the new terms have
	 * been initialized to 0.
	 */
	void setSize(size_t newSize);

	/**
	 * Returns the term at position index.
	 * Pre: index < getSize()
	 * Post: returns the term at the requested position or throws an
	 * exception if the position is invalid.
	 */
	T& operator[](size_t index);

	/**
	 * Returns the term at position index.
	 * Pre: index < getSize()
	 * Post: returns the term at the requested position or throws an
	 * exception if the position is invalid.
	 */
	const T& operator[](size_t index) const;

	/**
	 * Returns the magnitude of the vector.
	 * Pre: T is constructible from 0, has operator+= and binary operator*,
	 * and works as a parameter to std::sqrt.
	 * Post: Returns the magnitude of the vector, which is defined as
	 * the square root of the sum of the squares of the components
	 * of the vector.
	 */
	T getMagnitude() const;

	/**
	 * Outputs the terms of the vector in the form
	 * [ a1 a2 ... an ].
	 * Pre: None
	 * Post: v has been outputted to os in the format described above
	 */
	friend std::ostream& operator<< <>(std::ostream& os, const MyVector<T>& v);

	/**
	 * Reads in the terms for v from is.
	 * Pre: v's size is equal to the number of terms to read in.
	 * Post: v has been set to the terms read in from is.
	 */
	friend std::istream& operator>> <>(std::istream& is, MyVector<T>& v);

	/**
	 * An exception class to be used when the user tries to perform an
	 * operation that requires two vectors to be of the same size.
	 */
	class IncompatibleVectorSizes
	{
	public:
		/**
		 * Constructor that takes in two different sizes to indicate
		 * the sizes of the incompatible vectors and an optional 
		 * message
		 */
		IncompatibleVectorSizes(size_t size1, size_t size2,
		                        std::string message = "");

		/**
		 * Get the size of the first vector that was incompatible with
		 * the size of the second vector.
		 */
		size_t getSize1() const;

		/**
		 * Get the size of the second vector that was incompatible with
		 * the size of the first vector.
		 */
		size_t getSize2() const;

		/**
		 * Get the optional message that was said about the exception.
		 */
		const std::string& getMessage() const;
	private:
		/**
		 * Holds the different sizes of the incompatible vectors
		 */
		size_t s1, s2;

		/**
		 * Holds the optional message
		 */
        std::string msg;
	};

	/**
	 * An exception class to be used when the user tries to access
	 * a term not contained in the vector.
	 */
	class RangeError
	{
	public:
		/**
		 * @param n The subscript that would have accessed beyond the
		 * the end of the vector.
		 */
		RangeError(size_t n);

		/**
		 * @return The subscript that was too large and would not have
		 * accessed an element in the vector.
		 */
		size_t badSubscript() const;
	private:
		/**
		 * Holds the bad subscript.
		 */
		size_t subscript;
	};

private:
	/**
	 * Holds the size of the vector.
	 */
	size_t size;

	/**
	 * A pointer to the dynamic array containing the vector's data.
	 */
	T* data;
};

/**
 * Multiplies a vector by a scalar.
 * Pre: T defines operator*=
 * Post: Returns a new vector that is the result of multiplying v 
 * by a scalar.
 */
template<class T>
MyVector<T> operator*(const T& scalar, const MyVector<T>& v);

#include "MyVector.hpp"

#endif
