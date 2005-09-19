/**
 * @file testsUtility.h
 * @author Austin Hartman
 *
 * $Id: testsUtility.h,v 1.2 2005/09/19 21:23:56 ahartman Exp $
 */

#ifndef AUSTIN_TESTSUTILITY_H
#define AUSTIN_TESTSUTILITY_H

#include "Matrix.h"

/**
 * Function to compare matrices element-by-element with a tolerance so
 * that real floating-point comparisons are done.  If the matrices are not
 * the same size or if the absolute value of the difference between any two 
 * corresponding elements is greater than the tolerance, the function 
 * returns false, otherwise true.
 */
template<class T>
bool 
matricesEqual(const Matrix<T>& m1, const Matrix<T>& m2, const T& tolerance);

template<class T>
bool floatsEqual(T v1, T v2);

#include "testsUtility.hpp"

#endif
