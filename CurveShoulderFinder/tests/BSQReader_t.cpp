/**
 * @file BSQReader_t.cpp
 * @author Austin Hartman
 *
 * $Id: BSQReader_t.cpp,v 1.4 2005/08/10 01:01:13 ahartman Exp $
 */

#include "BSQReader.h"

#include <cassert>
#include <cmath>

#define PRINT_VALUES
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
        "./1920newstack.bsq";
    const std::string headerFileName = 
        "./1920newstack.hdr";
    typedef float type;

    // create two BSQReaders using the two different constructors
    BSQReader<type> reader(bsqFileName);
    BSQReader<type> reader2(bsqFileName, headerFileName);

    // create two more BSQReaders using the copy constructor and
    // assignment operator
    BSQReader<type> reader3(reader);
    BSQReader<type> reader4(bsqFileName);
    reader4 = reader2;

    // check if the information read from the header file is correct
    const size_t expectedNumBands = 15;
    const size_t expectedNumRows = 20;
    const size_t expectedNumCols = 19;

    const BSQReader<type>::UTMCoordinateType expectedMinX = 231405.5;
    const BSQReader<type>::UTMCoordinateType expectedMaxX = 265965.5;
    const BSQReader<type>::UTMCoordinateType expectedMinY = 3483721.5;
    const BSQReader<type>::UTMCoordinateType expectedMaxY = 3520201.5;
    
    const BSQReader<type>::UTMCoordinateType expectedPixelWidth = 1920.0;
    const BSQReader<type>::UTMCoordinateType expectedPixelHeight = 1920.0;

    assert(reader.getNumBands() == expectedNumBands);
    assert(reader.getNumRows() == expectedNumRows);
    assert(reader.getNumCols() == expectedNumCols);
    assert(reader2.getNumBands() == expectedNumBands);
    assert(reader2.getNumRows() == expectedNumRows);
    assert(reader2.getNumCols() == expectedNumCols);
    assert(reader3.getNumBands() == expectedNumBands);
    assert(reader3.getNumRows() == expectedNumRows);
    assert(reader3.getNumCols() == expectedNumCols);
    assert(reader4.getNumBands() == expectedNumBands);
    assert(reader4.getNumRows() == expectedNumRows);
    assert(reader4.getNumCols() == expectedNumCols);

    assert(reader.getMinX() == expectedMinX);
    assert(reader.getMaxX() == expectedMaxX);
    assert(reader.getMinY() == expectedMinY);
    assert(reader.getMaxY() == expectedMaxY);
    assert(reader2.getMinX() == expectedMinX);
    assert(reader2.getMaxX() == expectedMaxX);
    assert(reader2.getMinY() == expectedMinY);
    assert(reader2.getMaxY() == expectedMaxY);
    assert(reader3.getMinX() == expectedMinX);
    assert(reader3.getMaxX() == expectedMaxX);
    assert(reader3.getMinY() == expectedMinY);
    assert(reader3.getMaxY() == expectedMaxY);
    assert(reader4.getMinX() == expectedMinX);
    assert(reader4.getMaxX() == expectedMaxX);
    assert(reader4.getMinY() == expectedMinY);
    assert(reader4.getMaxY() == expectedMaxY);

    assert(reader.getPixelWidth() == expectedPixelWidth);
    assert(reader.getPixelHeight() == expectedPixelHeight);
    assert(reader2.getPixelWidth() == expectedPixelWidth);
    assert(reader2.getPixelHeight() == expectedPixelHeight);
    assert(reader3.getPixelWidth() == expectedPixelWidth);
    assert(reader3.getPixelHeight() == expectedPixelHeight);
    assert(reader4.getPixelWidth() == expectedPixelWidth);
    assert(reader4.getPixelHeight() == expectedPixelHeight);

    // check some values in the bsq file to see if they are reported correctly
    // ==== Point 0 ====
    const BSQReader<type>::UTMCoordinateType point0X = expectedMinX;
    const BSQReader<type>::UTMCoordinateType point0Y = expectedMinY;
    const type point0Band0Value = 0.000;
    const type point0Band2Value = 64.000;

#ifdef PRINT_VALUES
    cerr << "POINT 0 VALUES\n";
    cerr << "reader.getValue(point0X, point0Y, 0) = " 
         << reader.getValue(point0X, point0Y, 0) << '\n';
    cerr << "reader.getValue(point0X, point0Y, 2) = " 
         << reader.getValue(point0X, point0Y, 2) << '\n';
    cerr << "reader2.getValue(point0X, point0Y, 0) = " 
         << reader2.getValue(point0X, point0Y, 0) << '\n';
    cerr << "reader2.getValue(point0X, point0Y, 2) = " 
         << reader2.getValue(point0X, point0Y, 2) << '\n';
    cerr << "reader3.getValue(point0X, point0Y, 0) = " 
         << reader3.getValue(point0X, point0Y, 0) << '\n';
    cerr << "reader3.getValue(point0X, point0Y, 2) = " 
         << reader3.getValue(point0X, point0Y, 2) << '\n';
    cerr << "reader4.getValue(point0X, point0Y, 0) = " 
         << reader4.getValue(point0X, point0Y, 0) << '\n';
    cerr << "reader4.getValue(point0X, point0Y, 2) = " 
         << reader4.getValue(point0X, point0Y, 2) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader.getValue(point0X, point0Y, 2), point0Band2Value));
    assert(floatsEqual(reader2.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader2.getValue(point0X, point0Y, 2), point0Band2Value));
    assert(floatsEqual(reader3.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader3.getValue(point0X, point0Y, 2), point0Band2Value));
    assert(floatsEqual(reader4.getValue(point0X, point0Y, 0), point0Band0Value));
    assert(floatsEqual(reader4.getValue(point0X, point0Y, 2), point0Band2Value));

    // ==== Point 1 ====
    const BSQReader<type>::UTMCoordinateType point1X = expectedMinX;
    const BSQReader<type>::UTMCoordinateType point1Y = expectedMaxY;
    const type point1Band0Value = 0.000;
    const type point1Band2Value = 32.000;

#ifdef PRINT_VALUES
    cerr << "POINT 1 VALUES\n";
    cerr << "reader.getValue(point1X, point1Y, 0) = " 
         << reader.getValue(point1X, point1Y, 0) << '\n';
    cerr << "reader.getValue(point1X, point1Y, 2) = " 
         << reader.getValue(point1X, point1Y, 2) << '\n';
    cerr << "reader2.getValue(point1X, point1Y, 0) = " 
         << reader2.getValue(point1X, point1Y, 0) << '\n';
    cerr << "reader2.getValue(point1X, point1Y, 2) = " 
         << reader2.getValue(point1X, point1Y, 2) << '\n';
    cerr << "reader3.getValue(point1X, point1Y, 0) = " 
         << reader3.getValue(point1X, point1Y, 0) << '\n';
    cerr << "reader3.getValue(point1X, point1Y, 2) = " 
         << reader3.getValue(point1X, point1Y, 2) << '\n';
    cerr << "reader4.getValue(point1X, point1Y, 0) = " 
         << reader4.getValue(point1X, point1Y, 0) << '\n';
    cerr << "reader4.getValue(point1X, point1Y, 2) = " 
         << reader4.getValue(point1X, point1Y, 2) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader.getValue(point1X, point1Y, 2), point1Band2Value));
    assert(floatsEqual(reader2.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader2.getValue(point1X, point1Y, 2), point1Band2Value));
    assert(floatsEqual(reader3.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader3.getValue(point1X, point1Y, 2), point1Band2Value));
    assert(floatsEqual(reader4.getValue(point1X, point1Y, 0), point1Band0Value));
    assert(floatsEqual(reader4.getValue(point1X, point1Y, 2), point1Band2Value));

    // ==== Point 2 ====
    const BSQReader<type>::UTMCoordinateType point2X = 250391.940678;
    const BSQReader<type>::UTMCoordinateType point2Y = 3497456.415254; 
    const type point2Band0Value = 77.000;
    const type point2Band3Value = 79.000;
    const type point2Band12Value = 3.000;

#ifdef PRINT_VALUES
    cerr << "POINT 2 VALUES\n";
    cerr << "reader.getValue(point2X, point2Y, 0) = " 
         << reader.getValue(point2X, point2Y, 0) << '\n';
    cerr << "reader.getValue(point2X, point2Y, 3) = " 
         << reader.getValue(point2X, point2Y, 3) << '\n';
    cerr << "reader.getValue(point2X, point2Y, 12) = " 
         << reader.getValue(point2X, point2Y, 12) << '\n';
    cerr << "reader2.getValue(point2X, point2Y, 0) = " 
         << reader2.getValue(point2X, point2Y, 0) << '\n';
    cerr << "reader2.getValue(point2X, point2Y, 3) = " 
         << reader2.getValue(point2X, point2Y, 3) << '\n';
    cerr << "reader2.getValue(point2X, point2Y, 12) = " 
         << reader2.getValue(point2X, point2Y, 12) << '\n';
    cerr << "reader3.getValue(point2X, point2Y, 0) = " 
         << reader3.getValue(point2X, point2Y, 0) << '\n';
    cerr << "reader3.getValue(point2X, point2Y, 3) = " 
         << reader3.getValue(point2X, point2Y, 3) << '\n';
    cerr << "reader3.getValue(point2X, point2Y, 12) = " 
         << reader3.getValue(point2X, point2Y, 12) << '\n';
    cerr << "reader4.getValue(point2X, point2Y, 0) = " 
         << reader4.getValue(point2X, point2Y, 0) << '\n';
    cerr << "reader4.getValue(point2X, point2Y, 3) = " 
         << reader4.getValue(point2X, point2Y, 3) << '\n';
    cerr << "reader4.getValue(point2X, point2Y, 12) = " 
         << reader4.getValue(point2X, point2Y, 12) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader.getValue(point2X, point2Y, 12), point2Band12Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader2.getValue(point2X, point2Y, 12), point2Band12Value));
    assert(floatsEqual(reader3.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader3.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader3.getValue(point2X, point2Y, 12), point2Band12Value));
    assert(floatsEqual(reader4.getValue(point2X, point2Y, 0), point2Band0Value));
    assert(floatsEqual(reader4.getValue(point2X, point2Y, 3), point2Band3Value));
    assert(floatsEqual(reader4.getValue(point2X, point2Y, 12), point2Band12Value));

    // ==== Point 3 ====
    const BSQReader<type>::UTMCoordinateType point3X = 243676.347701;
    const BSQReader<type>::UTMCoordinateType point3Y = 3500067.099730;
    const type point3Band0Value = 57.000;
    const type point3Band4Value = 0.177;
    const type point3Band14Value = 1.000;

#ifdef PRINT_VALUES
    cerr << "POINT 3 VALUES\n";
    cerr << "reader.getValue(point3X, point3Y, 0) = " 
         << reader.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader.getValue(point3X, point3Y, 4) = " 
         << reader.getValue(point3X, point3Y, 4) << '\n';
    cerr << "reader.getValue(point3X, point3Y, 14) = " 
         << reader.getValue(point3X, point3Y, 14) << '\n';
    cerr << "reader2.getValue(point3X, point3Y, 0) = " 
         << reader2.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader2.getValue(point3X, point3Y, 4) = " 
         << reader2.getValue(point3X, point3Y, 4) << '\n';
    cerr << "reader2.getValue(point3X, point3Y, 14) = " 
         << reader2.getValue(point3X, point3Y, 14) << '\n';
    cerr << "reader3.getValue(point3X, point3Y, 0) = " 
         << reader3.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader3.getValue(point3X, point3Y, 4) = " 
         << reader3.getValue(point3X, point3Y, 4) << '\n';
    cerr << "reader3.getValue(point3X, point3Y, 14) = " 
         << reader3.getValue(point3X, point3Y, 14) << '\n';
    cerr << "reader4.getValue(point3X, point3Y, 0) = " 
         << reader4.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader4.getValue(point3X, point3Y, 4) = " 
         << reader4.getValue(point3X, point3Y, 4) << '\n';
    cerr << "reader4.getValue(point3X, point3Y, 14) = " 
         << reader4.getValue(point3X, point3Y, 14) << '\n';
#endif
    assert(floatsEqual(reader.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader.getValue(point3X, point3Y, 4), point3Band4Value));
    assert(floatsEqual(reader.getValue(point3X, point3Y, 14), point3Band14Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 4), point3Band4Value));
    assert(floatsEqual(reader2.getValue(point3X, point3Y, 14), point3Band14Value));
#ifdef PRINT_VALUES
    cerr << "reader3.getValue(point3X, point3Y, 0) = " 
         << reader3.getValue(point3X, point3Y, 0) << '\n';
    cerr << "reader3.getValue(point3X, point3Y, 4) = " 
         << reader3.getValue(point3X, point3Y, 4) << '\n';
    cerr << "reader3.getValue(point3X, point3Y, 14) = " 
         << reader3.getValue(point3X, point3Y, 14) << '\n';
#endif
    assert(floatsEqual(reader3.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader3.getValue(point3X, point3Y, 4), point3Band4Value));
    assert(floatsEqual(reader3.getValue(point3X, point3Y, 14), point3Band14Value));
    assert(floatsEqual(reader4.getValue(point3X, point3Y, 0), point3Band0Value));
    assert(floatsEqual(reader4.getValue(point3X, point3Y, 4), point3Band4Value));
    assert(floatsEqual(reader4.getValue(point3X, point3Y, 14), point3Band14Value));
}

template<class T>
bool
floatsEqual(T v1, T v2)
{
    const T epsilon = .001;
    if(std::abs(v1 - v2) > epsilon)
    {
        return false;
    }
    return true;
}

