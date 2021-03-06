/**
 * @file ImgReader_t.cpp
 * @author Austin Hartman
 *
 * $Id: ImgReader_t.cpp,v 1.4 2005/09/26 17:25:04 ahartman Exp $
 */

#include "ImgReader.h"
#include "testsUtility.h"

#include <algorithm>
#include <cassert>
#include <vector>

//#define PRINT_VALUES

#ifdef PRINT_VALUES
#include <iostream>
#include <iomanip>
#endif

typedef float type;
typedef unsigned int type2;

void readTest(ImgReader<type>& reader);
void copyTest();

int main()
{
    const std::string filename = "./1920newstack.img";

#ifdef PRINT_VALUES
    std::cerr << std::fixed;
    std::cerr.precision(6);
#endif

    // Test to make sure we can read correctly with a single reader
    ImgReader<type> reader(filename);
    readTest(reader);

    // Test to make sure we can create reader templated on a different type
    // XXX It won't work to actually read this file, because it's not of
    // XXX the specified type
    ImgReader<type2> type2Reader(filename);

    // Test to make sure operator= works
    ImgReader<type> reader2 = reader;
    readTest(reader2);

    // Test to make sure copy constructor works
    ImgReader<type> reader3(reader);
    readTest(reader3);

    // Test to make sure we can read correctly with multiple readers pointing
    // to the same img file
    const size_t numReaders = 10;
    const size_t numTimes = 2;
    std::vector<ImgReader<type> > readers(numReaders, reader);
    for(size_t i = 0; i < numTimes; ++i)
    {
        for_each(readers.begin(), readers.end(), readTest);
    }
}

void readTest(ImgReader<type>& reader)
{
    // Test to make sure the number of bands is correct
    assert(reader.getNumBands() == 15);

    // Test to make sure the x and y bounds are correct
#ifdef PRINT_VALUES
    std::cerr << "Min X = " << reader.getMinX() << '\n';
    std::cerr << "Max X = " << reader.getMaxX() << '\n';
    std::cerr << "Min Y = " << reader.getMinY() << '\n';
    std::cerr << "Max Y = " << reader.getMaxY() << '\n';
#endif
    const ImgReader<type>::UTMCoordinateType expectedMinX = 230445.5;
    const ImgReader<type>::UTMCoordinateType expectedMaxX = 266925.5;
    const ImgReader<type>::UTMCoordinateType expectedMinY = 3482761.5;
    const ImgReader<type>::UTMCoordinateType expectedMaxY = 3521161.5;

    assert(floatsEqual(reader.getMinX(), expectedMinX));
    assert(floatsEqual(reader.getMaxX(), expectedMaxX));
    assert(floatsEqual(reader.getMinY(), expectedMinY));
    assert(floatsEqual(reader.getMaxY(), expectedMaxY));

    // Test to make sure we're getting the correct data values
    // Point #1
    const ImgReader<type>::UTMCoordinateType point1x = expectedMinX;
    const ImgReader<type>::UTMCoordinateType point1y = expectedMaxY;
    const float point1band1value = 0.000f;
    const float point1band3value = 32.000f;
    const float point1band15value = 1.000f;
    assert(floatsEqual(reader.getValue(point1x, point1y, 1),
                       point1band1value));
    assert(floatsEqual(reader.getValue(point1x, point1y, 3),
                       point1band3value));
    assert(floatsEqual(reader.getValue(point1x, point1y, 15),
                       point1band15value));
    
    // Point #2
    const ImgReader<type>::UTMCoordinateType point2x = expectedMaxX - .5;
    const ImgReader<type>::UTMCoordinateType point2y = expectedMaxY - .5;
    const float point2band1value = 0.000f;
    const float point2band3value = 128.000f;
    const float point2band7value = 291.000f;
    const float point2band15value = 1.000f;
    assert(floatsEqual(reader.getValue(point2x, point2y, 1),
                       point2band1value));
    assert(floatsEqual(reader.getValue(point2x, point2y, 3),
                       point2band3value));
    assert(floatsEqual(reader.getValue(point2x, point2y, 7),
                       point2band7value));
    assert(floatsEqual(reader.getValue(point2x, point2y, 15),
                       point2band15value));

    // Point #3
    const ImgReader<type>::UTMCoordinateType point3x = 246889.567797;
    const ImgReader<type>::UTMCoordinateType point3y = 3506511.330508;
    const float point3band1value = 34.000f;
    const float point3band2value = 42.000f;
    const float point3band12value = 65.000f;
    const float point3band15value = 6.000f;
    assert(floatsEqual(reader.getValue(point3x, point3y, 1),
                       point3band1value));
    assert(floatsEqual(reader.getValue(point3x, point3y, 2),
                       point3band2value));
    assert(floatsEqual(reader.getValue(point3x, point3y, 12),
                       point3band12value));
    assert(floatsEqual(reader.getValue(point3x, point3y, 15),
                       point3band15value));

    // Point #4
    const ImgReader<type>::UTMCoordinateType point4x = expectedMinX;
    const ImgReader<type>::UTMCoordinateType point4y = expectedMinY + .5;
    const float point4band1value = 0.000f;
    const float point4band3value = 64.000f;
    const float point4band5value = 1.080f;
    const float point4band15value = 1.000f;
    assert(floatsEqual(reader.getValue(point4x, point4y, 1),
                       point4band1value));
    assert(floatsEqual(reader.getValue(point4x, point4y, 3),
                       point4band3value));
    assert(floatsEqual(reader.getValue(point4x, point4y, 5),
                       point4band5value));
    assert(floatsEqual(reader.getValue(point4x, point4y, 15),
                       point4band15value));

    // Point #5
    const ImgReader<type>::UTMCoordinateType point5x = expectedMaxX - .5;
    const ImgReader<type>::UTMCoordinateType point5y = expectedMinY + .5;
    const float point5band1value = 0.000f;
    const float point5band3value = 2.000f;
    const float point5band5value = 1.286f;
    const float point5band15value = 1.000f;
    assert(floatsEqual(reader.getValue(point5x, point5y, 1),
                       point5band1value));
    assert(floatsEqual(reader.getValue(point5x, point5y, 3),
                       point5band3value));
    assert(floatsEqual(reader.getValue(point5x, point5y, 5),
                       point5band5value));
    assert(floatsEqual(reader.getValue(point5x, point5y, 15),
                       point5band15value));

    // Point #6
    const ImgReader<type>::UTMCoordinateType point6x = 260215.669492;
    const ImgReader<type>::UTMCoordinateType point6y = 3493356.076271;
    const float point6band1value = 0.000f;
    const float point6band3value = 1.000f;
    const float point6band4value = 98.000f;
    const float point6band5value = 0.227f;
    const float point6band15value = 1.000f;
    assert(floatsEqual(reader.getValue(point6x, point6y, 1),
                       point6band1value));
    assert(floatsEqual(reader.getValue(point6x, point6y, 3),
                       point6band3value));
    assert(floatsEqual(reader.getValue(point6x, point6y, 4),
                       point6band4value));
    assert(floatsEqual(reader.getValue(point6x, point6y, 5),
                       point6band5value));
    assert(floatsEqual(reader.getValue(point6x, point6y, 15),
                       point6band15value));
}
