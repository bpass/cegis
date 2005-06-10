/**
 * @file CubicSplineSolver.h
 * @author Austin Hartman
 *
 * $Id: CubicSplineSolver.h,v 1.3 2005/06/10 21:52:23 ahartman Exp $
 */

#ifndef AUSTIN_CUBICSPLINESOLVER_H
#define AUSTIN_CUBICSPLINESOLVER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "MyVector.h"
#include "TridiagonalMatrix.h"
#include "ThomasAlgorithm.h"

/**
 * This is a class to represent a cubic spline.  Its a, b, c, and d
 * functions return the coefficients for the cubic polynomial.
 * The x function returns the starting x-coordinate for the polynomial
 * operator() evaluates the input term using the polynomial
 */
template<class T>
class CubicSpline
{
public:
	T& a();
	const T& a() const;
	T& b();
	const T& b() const;
	T& c();
	const T& c() const;
	T& d();
	const T& d() const;

	T& x();
	const T& x() const;
        
	T operator()(const T& term) const;
private:
	T a_;
	T b_;
	T c_;
	T d_;

	T x_;
};

template<class T>
class CubicSplineSolver
{
public:
	// using a std::vector because MyVector is for numeric types
	// and requires operator=(int)
	typedef std::vector<CubicSpline<T> > Solution;

	/**
	 * Builds a set of cubic splines using the input xCoords and
	 * yCoords
	 * Pre: xCoords contains x-coordinates in increasing order
	 * Post: returns a set of cubic splines for the system
	 */
	Solution operator()(const MyVector<T>& xCoords, 
	                    const MyVector<T>& yCoords) const;

};

/**
 * Outputs a cubic spline in the form:
 *   a + b*('x' - x) + c*('x' - x)^2 + d*('x' - x)^3
 * where 'x' refers to the literal character x
 * 
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const CubicSpline<T>& spline);

#include "CubicSplineSolver.hpp"

#endif

