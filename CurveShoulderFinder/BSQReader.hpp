/**
 * @file BSQReader.hpp
 * @author Austin Hartman
 *
 * $Id: BSQReader.hpp,v 1.9 2005/09/19 15:31:25 ahartman Exp $
 */

#ifdef AUSTIN_BSQREADER_H
#ifndef AUSTIN_BSQREADER_HPP
#define AUSTIN_BSQREADER_HPP

#include <cassert>
#include <iostream>
#include <sstream>

template<class DataType>
BSQReader<DataType>::BSQReader(const std::string& bsqFilename)
    : m_bsqFilename(bsqFilename)
{
    // guess the name of the header file based on the name of the
    // bsq file by removing the extension .bsq and replacing it with
    // the extension .hdr
    const std::string::size_type extensionIndex = 
        bsqFilename.rfind(".bsq");
    m_headerFilename = 
        bsqFilename.substr(0, extensionIndex) + ".hdr";

    init(m_bsqFilename, m_headerFilename);
}

template<class DataType>
BSQReader<DataType>::BSQReader(const std::string& bsqFilename,
                               const std::string& headerFilename)
    : m_bsqFilename(bsqFilename),
      m_headerFilename(headerFilename)
{
    init(m_bsqFilename, m_headerFilename);
}

template<class DataType>
DataType
BSQReader<DataType>::getValue(size_t row, size_t col, size_t band) const
{
    --band;

    // check if the specified band, row, and column are valid
    if(band >= m_numBands)
    {
        throw typename BSQReader<DataType>::InvalidBand();
    }
    if(row >= m_numRows)
    {
        throw typename BSQReader<DataType>::InvalidRow();
    }
    if(col >= m_numCols)
    {
        throw typename BSQReader<DataType>::InvalidCol();
    }

    // calculate the offset
    // BSQ is stored with one complete band followed by the next complete band
    // Inside each band, pixels are stored row by row
    const size_t pixelOffset = 
        m_numRows * m_numCols * band + m_numCols * row + col; 

    assert(pixelOffset < m_data.size());

    return m_data[pixelOffset];
}

template<class DataType>
DataType
BSQReader<DataType>::getValue(
    const typename BSQReader<DataType>::UTMCoordinateType& xCoord,
    const typename BSQReader<DataType>::UTMCoordinateType& yCoord,
    size_t band) const
{
    --band;

    // the coordinate is for the middle of the pixel, so we have to take into
    // account that the coordinate corresponds with the same pixel if it's half
    // a pixel width or height off in the x- and y-directions, respectively
    const typename BSQReader<DataType>::UTMCoordinateType halfPixelWidth =
        m_pixelWidth / 2;
    const typename BSQReader<DataType>::UTMCoordinateType halfPixelHeight =
        m_pixelHeight / 2;

    // check if the specified band and coordinates are valid
    if(band >= m_numBands)
    {
        throw typename BSQReader<DataType>::InvalidBand();
    }
    if(xCoord < m_upperLeftXCoordinate - halfPixelWidth || 
       xCoord > m_lowerRightXCoordinate + halfPixelWidth)
    {
        throw typename BSQReader<DataType>::InvalidXCoordinate();
    }
    if(yCoord > m_upperLeftYCoordinate + halfPixelHeight ||
       yCoord < m_lowerRightYCoordinate - halfPixelHeight)
    {
        throw typename BSQReader<DataType>::InvalidYCoordinate();
    }

    const size_t row = 
        static_cast<size_t>
        ((m_upperLeftYCoordinate - yCoord) / m_pixelHeight + .5);
    const size_t col =
        static_cast<size_t>
        ((xCoord - m_upperLeftXCoordinate) / m_pixelWidth + .5);

    return getValue(row, col, band + 1);
}

template<class DataType>
const std::string&
BSQReader<DataType>::getBSQFilename() const
{
    return m_bsqFilename;
}

template<class DataType>
const std::string&
BSQReader<DataType>::getHeaderFilename() const
{
    return m_headerFilename;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMinX() const
{
    return m_upperLeftXCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMaxX() const
{
    return m_lowerRightXCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMinY() const
{
    return m_lowerRightYCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getMaxY() const
{
    return m_upperLeftYCoordinate;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getPixelWidth() const
{
    return m_pixelWidth;
}

template<class DataType>
typename BSQReader<DataType>::UTMCoordinateType
BSQReader<DataType>::getPixelHeight() const
{
    return m_pixelHeight;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumRows() const
{
    return m_numRows;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumCols() const
{
    return m_numCols;
}

template<class DataType>
size_t 
BSQReader<DataType>::getNumBands() const
{
    return m_numBands;
}

template<class DataType>
void
BSQReader<DataType>::init(const std::string& bsqFilename, 
                          const std::string& headerFilename)
{
    readHeaderFile(headerFilename);
    readBSQFile(bsqFilename);
}

template<class DataType>
void 
BSQReader<DataType>::readHeaderFile(const std::string& headerFilename)
{
    std::ifstream headerFile(headerFilename.c_str());
    if(!headerFile)
    {
        throw typename BSQReader<DataType>::FailedOpeningHeaderFile();
    }

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
            valueInterpreter >> m_numBands;
        }
        else if(key == "UL_X_COORDINATE")
        {
            valueInterpreter >> m_upperLeftXCoordinate;
        }
        else if(key == "UL_Y_COORDINATE")
        {
            valueInterpreter >> m_upperLeftYCoordinate;
        }
        else if(key == "LR_X_COORDINATE")
        {
            valueInterpreter >> m_lowerRightXCoordinate;
        }
        else if(key == "LR_Y_COORDINATE")
        {
            valueInterpreter >> m_lowerRightYCoordinate;
        }
        else if(key == "PIXEL_WIDTH")
        {
            valueInterpreter >> m_pixelWidth;
        }
        else if(key == "PIXEL_HEIGHT")
        {
            valueInterpreter >> m_pixelHeight;
        }
        else if(key == "ROWS")
        {
            valueInterpreter >> m_numRows;
        }
        else if(key == "COLS")
        {
            valueInterpreter >> m_numCols;
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
}

template<class DataType>
void
BSQReader<DataType>::readBSQFile(const std::string& bsqFilename)
{
    std::ifstream bsqFile(bsqFilename.c_str(), 
                          std::ios::in | std::ios::binary);
    if(!bsqFile)
    {
        throw typename BSQReader<DataType>::FailedOpeningBSQFile();
    }

    m_data.clear();
    const size_t fileSize = m_numRows * m_numCols * m_numBands;
    m_data.resize(fileSize);

    bsqFile.read(reinterpret_cast<char*>(&m_data[0]), 
                 sizeof(DataType) * fileSize);

    assert(m_data.size() == m_numRows * m_numCols * m_numBands);
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

template<class DataType>
BSQReader<DataType>::FailedOpeningBSQFile::~FailedOpeningBSQFile()
{}

template<class DataType>
BSQReader<DataType>::FailedOpeningHeaderFile::~FailedOpeningHeaderFile()
{}

#endif
#endif

