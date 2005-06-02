#ifndef AUSTIN_CURVEPARTITIONER_H
#define AUSTIN_CURVEPARTITIONER_H

#include <vector>

#include "FittingCurve.h"

template<class T>
class CurvePartitioner
{
public:
    typedef std::vector<T> CurveDifferences;
    CurveDifferences getDifferences(const FittingCurve<T>& curve, 
                                    const size_t numberOfPartitions, 
                                    const T& lowValue, 
                                    const T& highValue) const;
};

#include "CurvePartitioner.hpp"

#endif
