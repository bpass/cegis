/**
 * @file CurvePartitioner.hpp
 * @author Austin Hartman
 *
 * $Id: CurvePartitioner.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_CURVEPARTITIONER_H
#ifndef AUSTIN_CURVEPARTITIONER_HPP
#define AUSTIN_CURVEPARTITIONER_HPP

template<class T>
typename CurvePartitioner<T>::CurveDifferences
CurvePartitioner<T>::getDifferences(const FittingCurve<T>& curve,
                                    const size_t numberOfPartitions,
                                    const T& lowValue,
                                    const T& highValue) const
{
    CurveDifferences rv(numberOfPartitions);

    const T rangeOverN = (highValue - lowValue) / numberOfPartitions;
    
    for(size_t i = 0; i < numberOfPartitions; ++i)
    {
        const T firstX = lowValue + rangeOverN * i;
        const T secondX = lowValue + rangeOverN * (i+1);

        rv[i] = curve(secondX) - curve(firstX);
    }
    
    return rv;
}

#endif
#endif

