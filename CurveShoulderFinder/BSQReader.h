/**
 * @file BSQReader.h
 * @author Austin Hartman
 *
 * $Id: BSQReader.h,v 1.1 2005/07/06 20:49:26 ahartman Exp $
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
    const DataType& getValue(size_t row, size_t col, size_t band);
    const DataType& getValue(const UTMCoordinateType& xCoord, 
                             const UTMCoordinateType& yCoord,
                             size_t band);
    class UnexpectedHeaderLine
    {};

private:
    std::ifstream bsqFile;
    UTMCoordinateType upperLeftXCoordinate, upperLeftYCoordinate,
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

