/**
 * @file LineFeature.cpp
 * $Id: LineFeature.cpp,v 1.1 2004/12/09 19:03:07 ahartman Exp $
 */

#include "LineFeature.h"

LineFeature::LineFeature(const Points& points)
    : myPoints(points)
{
}

Points LineFeature::getPoints()
{
    return myPoints;
}

Type LineFeature::getType()
{
    return WKBLineString;
}

