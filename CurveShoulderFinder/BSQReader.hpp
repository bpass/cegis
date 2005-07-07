/**
 * @file BSQReader.hpp
 * @author Austin Hartman
 *
 * $Id: BSQReader.hpp,v 1.2 2005/07/07 19:07:44 ahartman Exp $
 */

#ifdef AUSTIN_BSQREADER_H
#ifndef AUSTIN_BSQREADER_HPP
#define AUSTIN_BSQREADER_HPP

#include <iostream>
#include <sstream>

template<class DataType>
BSQReader<DataType>::BSQReader(const std::string& bsqFileName)
    : bsqFile(bsqFileName.c_str(), std::ios::in | std::ios::binary)
{
    // guess the name of the header file based on the name of the
    // bsq file by removing the extension .bsq and replacing it with
    // the extension .hdr
    const std::string::size_type extensionIndex = 
        bsqFileName.rfind(".bsq");
    const std::string headerFileName = 
        bsqFileName.substr(0, extensionIndex) + ".hdr";
    readHeaderFile(headerFileName);
}

template<class DataType>
BSQReader<DataType>::BSQReader(const std::string& bsqFileName,
                               const std::string& headerFileName)
    : bsqFile(bsqFileName.c_str(), std::ios::in | std::ios::binary)
{
    readHeaderFile(headerFileName);
}

template<class DataType>
DataType
BSQReader<DataType>::getValue(size_t row, size_t col, size_t band) const
{
    // check if the specified band, row, and column are valid
    if(band >= numBands)
    {
        throw typename BSQReader<DataType>::InvalidBand();
    }
    if(row >= numRows)
    {
        throw typename BSQReader<DataType>::InvalidRow();
    }
    if(col >= numCols)
    {
        throw typename BSQReader<DataType>::InvalidCol();
    }

    // calculate the offset
    // BSQ is stored with one complete band followed by the next complete band
    // Inside each band, pixels are stored row by row
    const size_t pixelOffset = numRows * numCols * band + numCols * row + col; 

    // read the data value at the specified offset
    bsqFile.seekg(pixelOffset * sizeof(DataType));
    DataType value;
    bsqFile.read(reinterpret_cast<char*>(&value), sizeof(DataType));

    return value;
}

template<class DataType>
DataType
BSQReader<DataType>::getValue(
    const typename BSQReader<DataType>::UTMCoordinateType& xCoord,
    const typename BSQReader<DataType>::UTMCoordinateType& yCoord,
    size_t band) const
{
    // the coordinate is for the middle of the pixel, so we have to take into
    // account that the coordinate corresponds with the same pixel if it's half
    // a pixel width or height off in the x- and y-directions, respectively
    const typename BSQReader<DataType>::UTMCoordinateType halfPixelWidth =
        pixelWidth / 2;
    const typename BSQReader<DataType>::UTMCoordinateType halfPixelHeight =
        pixelHeight / 2;

    // check if the specified band and coordinates are valid
    if(band >= numBands)
    {
        throw typename BSQReader<DataType>::InvalidBand();
    }
    if(xCoord < upperLeftXCoordinate - halfPixelWidth || 
       xCoord > lowerRightXCoordinate + halfPixelWidth)
    {
        throw typename BSQReader<DataType>::InvalidXCoordinate();
    }
    if(yCoord > upperLeftYCoordinate + halfPixelHeight ||
       yCoord < lowerRightYCoordinate - halfPixelHeight)
    {
        throw typename BSQReader<DataType>::InvalidYCoordinate();
    }

    const size_t row = 
        static_cast<size_t>
        ((upperLeftYCoordinate - yCoord) / pixelHeight + .5);
    const size_t col =
        static_cast<size_t>
        ((xCoord - upperLeftXCoordinate) / pixelWidth + .5);

    return getValue(row, col, band);
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMinX() const
{
    return upperLeftXCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMaxX() const
{
    return lowerRightXCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMinY() const
{
    return lowerRightYCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMaxY() const
{
    return upperLeftYCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getPixelWidth() const
{
    return pixelWidth;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getPixelHeight() const
{
    return pixelHeight;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumRows() const
{
    return numRows;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumCols() const
{
    return numCols;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumBands() const
{
    return numBands;
}

template<class DataType>
void 
BSQReader<DataType>::readHeaderFile(const std::string& headerFileName)
{
    std::ifstream headerFile(headerFileName.c_str());
    std::string line;

    // read each line of the header file
    while(std::getline(headerFile, line))
    {
        const KeyValuePair keyValuePair = parseHeaderLine(line);
        const std::string& key = keyValuePair.key;
        const std::string& value = keyValuePair.value;
        std::istringstream valueInterpreter(value);

        // get the values from each of the header fields that we're
        // interested in
        if(key == "BANDS")
        {
            valueInterpreter >> numBands;
        }
        else if(key == "UL_X_COORDINATE")
        {
            valueInterpreter >> upperLeftXCoordinate;
        }
        else if(key == "UL_Y_COORDINATE")
        {
            valueInterpreter >> upperLeftYCoordinate;
        }
        else if(key == "LR_X_COORDINATE")
        {
            valueInterpreter >> lowerRightXCoordinate;
        }
        else if(key == "LR_Y_COORDINATE")
        {
            valueInterpreter >> lowerRightYCoordinate;
        }
        else if(key == "PIXEL_WIDTH")
        {
            valueInterpreter >> pixelWidth;
        }
        else if(key == "PIXEL_HEIGHT")
        {
            valueInterpreter >> pixelHeight;
        }
        else if(key == "ROWS")
        {
            valueInterpreter >> numRows;
        }
        else if(key == "COLS")
        {
            valueInterpreter >> numCols;
        }
        else if(key == "PROJECTION_PARAMETERS")
        {
            // skip the projection parameters, which have a different
            // format than the rest of the lines
            const size_t numProjectionParameters = 15;
            for(size_t i = 0; i < numProjectionParameters; ++i)
            {
                getline(headerFile, line);
            }
        }
    }

//    std::cout << std::fixed;
//    std::cout.precision(6);
//    std::cout << __FILE__ << ':' << __LINE__ << '\n';
//    std::cout << "\tnumBands = " << numBands << '\n';
//    std::cout << "\tupperLeftXCoordinate = " << upperLeftXCoordinate << '\n';
//    std::cout << "\tupperLeftYCoordinate = " << upperLeftYCoordinate << '\n';
//    std::cout << "\tlowerRightXCoordinate = " << lowerRightXCoordinate << '\n';
//    std::cout << "\tlowerRightYCoordinate = " << lowerRightYCoordinate << '\n';
//    std::cout << "\tpixelWidth = " << pixelWidth << '\n';
//    std::cout << "\tpixelHeight = " << pixelHeight << '\n';
//    std::cout << "\tnumRows = " << numRows << '\n';
//    std::cout << "\tnumCols = " << numCols << '\n';
}

template<class DataType>
typename BSQReader<DataType>::KeyValuePair
BSQReader<DataType>::parseHeaderLine(const std::string& line) const
{
    KeyValuePair keyValuePair;

    // find the colon which is used to separate the key and value
    const std::string::size_type colonIndex = line.find_first_of(':');

    // if there is no colon, something is wrong
    if(colonIndex == std::string::npos)
    {
        throw typename BSQReader<DataType>::UnexpectedHeaderLine();
    }

    // the key is everything from the start of the line to the colon
    keyValuePair.key = line.substr(0, colonIndex);

    // skip all of the whitespace between the colon and the value
    const std::string::size_type valueIndex = line.find_last_of(" \t") + 1;

    // the value should be the rest of the line
    keyValuePair.value = line.substr(valueIndex);

    return keyValuePair;
}

#endif
#endif

