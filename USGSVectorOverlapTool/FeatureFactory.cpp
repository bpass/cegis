/**
 * @file FeatureFactory.cpp
 * $Id: FeatureFactory.cpp,v 1.1 2004/12/10 18:54:30 ahartman Exp $
 */

#include <iostream>

#include "Feature.h"
#include "FeatureFactory.h"

using std::cerr;

namespace
{
    /**
     * This is a basic struct for reading two-dimensional points
     * from a well-known binary stream.
     *
     * This is used instead of Point in case that class needs to change.
     */
    struct Point2D
    {
        double x, y;
    };
}

FeatureFactory::FeatureFactory()
{
}

Feature* FeatureFactory::createFeature(unsigned char* wkbStream, size_t streamSize)
{
    if(!(wkbStream && streamSize))
    {
        return NULL;
    }
    
    unsigned char* current_byte = wkbStream;
    unsigned char* end = wkbStream + streamSize;
    Type current_type;

    // current_byte gets incremented in the functions called below
    while(current_byte < end)
    {
        // XXX this might need to be translated to the correct endian
        uint32_t current_type_int = 
            *(reinterpret_cast<uint32_t*>(current_byte[1]));
        current_type = static_cast<Type>(current_type_int);

        // move the pointer past the endian byte and the 
        // 32-bit type enumeration
        current_byte += sizeof(unsigned char) + sizeof(uint32_t);

        switch(current_type)
        {
            case WKBLineString:
                Points new_points;
                
            default:
                cerr << __FILE__ << ": " << __LINE__
                     << " unsupported well-known binary type "
                     << current_type << '\n';
                return NULL;
        }
    }
} 

void FeatureFactory::getLineFromWKB(unsigned char*& buffer, Points& points)
{
    // read the number of points from the buffer and increment it
    uint32_t numPoints = *(reinterpret_cast<uint32_t*>(buffer));
    buffer += sizeof(uint32_t);

    // get a pointer to the first point in the buffer
    Point2D* currentPoint2D = reinterpret_cast<Point2D*> buffer;

    for(uint32_t i = 0; i < numPoints; i++)
    {
        // create a new Point with the data from the well-known binary
        // stream, add it to the list of points, and increment the
        // well-known binary stream
        Point currentPoint;
        currentPoint.x = currentPoint2D->x;
        currentPoint.y = currentPoint2D->y;
        points->push_back(currentPoint);
        currentPoint2D++;
    }
}

