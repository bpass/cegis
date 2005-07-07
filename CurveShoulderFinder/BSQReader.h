/**
 * @file BSQReader.h
 * @author Austin Hartman
 *
 * $Id: BSQReader.h,v 1.2 2005/07/07 19:07:44 ahartman Exp $
 */

#ifndef AUSTIN_BSQREADER_H
#define AUSTIN_BSQREADER_H

#include <string>
#include <fstream>

template<class DataType>
class BSQReader
{
public:
    typedef double UTMCoordinateType;
    BSQReader(const std::string& bsqFileName);
    BSQReader(const std::string& bsqFileName,
              const std::string& headerFileName);
    DataType getValue(size_t row, size_t col, size_t band) const;
    DataType getValue(const UTMCoordinateType& xCoord, 
                      const UTMCoordinateType& yCoord,
                      size_t band) const;
    UTMCoordinateType getMinX() const;
    UTMCoordinateType getMaxX() const;
    UTMCoordinateType getMinY() const;
    UTMCoordinateType getMaxY() const;
    UTMCoordinateType getPixelWidth() const;
    UTMCoordinateType getPixelHeight() const;
    size_t getNumRows() const;
    size_t getNumCols() const;
    size_t getNumBands() const;

    class UnexpectedHeaderLine
    {};

    class InvalidBand
    {};

    class InvalidRow
    {};

    class InvalidCol
    {};

    class InvalidXCoordinate
    {};

    class InvalidYCoordinate
    {};

private:
    mutable std::ifstream bsqFile;
    UTMCoordinateType upperLeftXCoordinate, upperLeftYCoordinate,
                      lowerRightXCoordinate, lowerRightYCoordinate,
                      pixelWidth, pixelHeight;
    size_t numRows, numCols, numBands;

    struct KeyValuePair
    {
        std::string key;
        std::string value;
    };

    void readHeaderFile(const std::string& headerFileName);
    KeyValuePair parseHeaderLine(const std::string& line) const;
};

#include "BSQReader.hpp"

#endif

