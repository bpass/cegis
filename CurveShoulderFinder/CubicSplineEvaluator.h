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
	 * Pre: splines is not empty and has its cubic splines ordered such
	 * that their x() values are in increasing order
	 * Post: returns the evaluation of term using the correct spline
	 * in splines
	 */
	T operator()(const typename CubicSplineSolver<T>::Solution& splines,
	             const T& term) const;
};

#include "CubicSplineEvaluator.hpp"

#endif

