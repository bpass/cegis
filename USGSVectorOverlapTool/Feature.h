/**
 * @file Feature.h
 * $Id: Feature.h,v 1.1 2004/12/09 18:45:27 ahartman Exp $
 */

#ifndef VO_FEATURE_H
#define VO_FEATURE_H

#include <list>
using std::list;

struct Point
{
    double x, y;
};

typedef list<Point> Points;
enum Type
{
  WKBPoint = 1,
  WKBLineString,
  WKBPolygon,
  WKBMultiPoint,
  WKBMultiLineString,
  WKBMultiPolygon,
  WKBUnknown
};

class Feature
{
    public:
        virtual ~Feature() {}
        virtual const Points& getPoints() = 0;
        virtual Type getType() = 0;        
};

#endif

