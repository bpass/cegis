/**
 * @file CurvePartitioner.hpp
 * @author Austin Hartman
 *
 * $Id: CurvePartitioner.hpp,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

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

