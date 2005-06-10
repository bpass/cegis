/**
 * @file CurvePartitioner.h
 * @author Austin Hartman
 *
 * $Id: CurvePartitioner.h,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

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
