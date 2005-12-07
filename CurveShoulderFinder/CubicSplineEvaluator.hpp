/**
 * @file CubicSplineEvaluator.hpp
 * @author Austin Hartman
 *
 * $Id: CubicSplineEvaluator.hpp,v 1.4 2005/12/07 23:49:04 ahartman Exp $
 */

#ifdef AUSTIN_CUBICSPLINEEVALUATOR_H
#ifndef AUSTIN_CUBICSPLINEEVALUATOR_HPP
#define AUSTIN_CUBICSPLINEEVALUATOR_HPP

template<class T>
T
CubicSplineEvaluator<T>::operator()
	(const typename CubicSplineSolver<T>::Solution& splines,
	 const T& term) const
{
	// check if term is less than the x value for the first cubic spline
	if(term < splines[0].x())
	{
		return splines[0](term);
	}

	// otherwise, find the cubic spline that applies to the value of term
	// and return the value using it
	for(size_t k = 1; k < splines.size(); ++k)
	{
		if(term < splines[k].x())
		{
			return splines[k-1](term);
		}
	}

	// otherwise, return the value using the last cubic spline
	return splines[splines.size() - 1](term);
}

#endif
#endif

