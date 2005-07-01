/**
 * @file CubicSplineEvaluator.h
 * @author Austin Hartman
 *
 * $Id: CubicSplineEvaluator.h,v 1.3 2005/07/01 21:03:55 ahartman Exp $
 */

#ifndef AUSTIN_CUBICSPLINEEVALUATOR_H
#define AUSTIN_CUBICSPLINEEVALUATOR_H

#include "CubicSplineSolver.h"

template<class T>
class CubicSplineEvaluator
{
public:
	/**
	 * This is used to return the value of a term using the correct 
	 * cubic spline when given a set of them.
	 * @pre splines is not empty and has its cubic splines ordered such
	 * that their x() values are in increasing order
	 * @post returns the evaluation of term using the correct spline
	 * in splines
	 */
	T operator()(const typename CubicSplineSolver<T>::Solution& splines,
	             const T& term) const;
};

#include "CubicSplineEvaluator.hpp"

#endif

