/**
 * @file LineFeature.cpp
 * $Id: LineFeature.cpp,v 1.2 2004/12/10 18:51:28 ahartman Exp $
 */

#include "LineFeature.h"

LineFeature::LineFeature(const Points& points)
    : myPoints(points)
{
}

const Points& LineFeature::getPoints()
{
    return myPoints;
}

Type LineFeature::getType()
{
    return WKBLineString;
}

