/**
 * @file BSQReader_t.cpp
 * @author Austin Hartman
 *
 * $Id: BSQReader_t.cpp,v 1.2 2005/07/07 18:58:55 ahartman Exp $
 */

#include "BSQReader.h"

#include <cassert>
#include <cmath>

//#define PRINT_VALUES
#ifdef PRINT_VALUES
#include <iostream>
#endif

template<class T>
bool floatsEqual(T v1, T v2);

int main()
{
#ifdef PRINT_VALUES
    using std::cerr;
#endif

    const std::string bsqFileName = 
        "./littler30_nitro.bsq";
    const std::string headerFileName = 
        "./littler30_nitro.hdr";
    typedef float type;

    // create two BSQReaders using the two different constructors
    BSQReader<type> reader(bsqFileName);
    BSQReader<type> reader2(bsqFileName, headerFileName);

    // check if the information read from the header file is correct
    const size_t expectedNumBands = 6;
    const size_t expectedNumRows = 1274;
    const size_t expectedNumCols = 1155;

    const BSQReader<type>::UTMCoordinateType expectedMinX = 231405.5;
    const BSQReader<type>::UTMCoordinateType expectedMaxX = 266025.5;
    const BSQReader<type>::UTMCoordinateType expectedMinY = 3482011.5;
    const BSQReader<type>::UTMCoordinateType expectedMaxY = 3520201.5;
    
    const BSQReader<type>::UTMCoordinateType expectedPixelWidth = 30.0;
    const BSQReader<type>::UTMCoordinateType expectedPixelHeight = 30.0;

    assert(reader.getNumBands() == expectedNumBands);
    assert(reader.getNumRows() == expectedNumRows);
    assert(reader.getNumCols() == expectedNumCols);
    assert(reader2.getNumBands() == expectedNumBands);
    assert(reader2.getNumRows() == expectedNumRows);
    assert(reader2.getNumCols() == expectedNumCols);

    assert(reader.getMinX() == expectedMinX);
    assert(reader.getMaxX() == expectedMaxX);
    assert(reader.getMinY() == expectedMinY);
    assert(reader.getMaxY() == expectedMaxY);
    assert(reader2.getMinX() == expectedMinX);
    assert(reader2.getMaxX() == expectedMaxX);
    assert(reader2.getMinY() == expectedMinY);
    assert(reader2.getMaxY() == expectedMaxY);

    assert(reader.getPixelWidth() == expectedPixelWidth);
    assert(reader.getPixelHeight() == expectedPixelHeight);
    assert(reader2.getPixelWidth() == expectedPixelWidth);
    assert(reader2.getPixelHeight() == expectedPixelHeight);

    // check some values in the bsq file to see if they are reported correctly
    // ==== Point 0 ====
    const BSQReader<type>::UTMCoordinateType point0X = expectedMinX;
    const BSQReader<type>::UTMCoordinateType point0Y = expectedMaxY;
    const type point0Band0Value = 0.000;
    const type point0Band3Value = 0.000;

#ifdef PRINT_VALUES
    cerr << "reader.getValue(point0X, point0Y, 0) = " 
         << reader.getValue(point0X, point0Y, 0) << '\n';
    cerr << "reader.getValue(point0X, point0Y, 3) = " 
         << reader.getValue(point0X, point0Y, 3) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader.getValue(point0X, point0Y, 3), point0Band3Value));
    assert(floatsEqual(reader2.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader2.getValue(point0X, point0Y, 3), point0Band3Value));

    // ==== Point 1 ====
    const BSQReader<type>::UTMCoordinateType point1X = expectedMinX;
    const BSQReader<type>::UTMCoordinateType point1Y = expectedMaxY;
    const type point1Band0Value = 0.000;
    const type point1Band3Value = 0.000;

#ifdef PRINT_VALUES
    cerr << "reader.getValue(point1X, point1Y, 0) = " 
         << reader.getValue(point1X, point1Y, 0) << '\n';
    cerr << "reader.getValue(point1X, point1Y, 3) = " 
         << reader.getValue(point1X, point1Y, 3) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader.getValue(point1X, point1Y, 3), point1Band3Value));
    assert(floatsEqual(reader2.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader2.getValue(point1X, point1Y, 3), point1Band3Value));

    // ==== Point 2 ====
    const BSQReader<type>::UTMCoordinateType point2X = 239533.389454;
    const BSQReader<type>::UTMCoordinateType point2Y = 3503942.107063; 
    const type point2Band0Value = 0.220;
    const type point2Band3Value = 1.220;
    const type point2Band5Value = 0.290;

#ifdef PRINT_VALUES
    cerr << "reader.getValue(point2X, point2Y, 0) = " 
         << reader.getValue(point2X, point2Y, 0) << '\n';
    cerr << "reader.getValue(point2X, point2Y, 3) = " 
         << reader.getValue(point2X, point2Y, 3) << '\n';
    cerr << "reader.getValue(point2X, point2Y, 5) = " 
         << reader.getValue(point2X, point2Y, 5) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader.getValue(point2X, point2Y, 5), point2Band5Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 5), point2Band5Value));

    // ==== Point 2 ====
    const BSQReader<type>::UTMCoordinateType point3X = 243676.347701;
    const BSQReader<type>::UTMCoordinateType point3Y = 3500067.099730;
    const type point3Band0Value = 11019.810;
    const type point3Band3Value = 0.590;
    const type point3Band5Value = 0.000;

#ifdef PRINT_VALUES
    cerr << "reader.getValue(point3X, point3Y, 0) = " 
         << reader.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader.getValue(point3X, point3Y, 3) = " 
         << reader.getValue(point3X, point3Y, 3) << '\n';
    cerr << "reader.getValue(point3X, point3Y, 5) = " 
         << reader.getValue(point3X, point3Y, 5) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader.getValue(point3X, point3Y, 3), point3Band3Value));
    assert(floatsEqual(reader.getValue(point3X, point3Y, 5), point3Band5Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 3), point3Band3Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 5), point3Band5Value));
}

template<class T>
bool
floatsEqual(T v1, T v2)
{
    const T epsilon = .00001;
    if(std::abs(v1 - v2) > epsilon)
    {
        return false;
    }
    return true;
}

