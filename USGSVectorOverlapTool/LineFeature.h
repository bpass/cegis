/**
 * @file LineFeature.h
 * $Id: LineFeature.h,v 1.1 2004/12/09 19:03:07 ahartman Exp $
 */

#ifndef VO_LINEFEATURE_H
#define VO_LINEFEATURE_H

#include "Feature.h"

class LineFeature : public Feature
{
    public:
        LineFeature(const Points& points);

        virtual ~LineFeature() {}
        virtual const Points& getPoints();
        virtual Type getType(); 
    private:
        Points myPoints;
};

#endif

