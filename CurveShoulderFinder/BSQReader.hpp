/**
 * @file BSQReader.hpp
 * @author Austin Hartman
 *
 * $Id: BSQReader.hpp,v 1.1 2005/07/06 20:49:26 ahartman Exp $
 */

#ifdef AUSTIN_BSQREADER_H
#ifndef AUSTIN_BSQREADER_HPP
#define AUSTIN_BSQREADER_HPP

#include <iostream>
#include <sstream>

template<class DataType>
BSQReader<DataType>::BSQReader(const std::string& bsqFileName)
    : bsqFile(bsqFileName.c_str())
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
    : bsqFile(bsqFileName.c_str())
{
    readHeaderFile(headerFileName);
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

//    std::cout << __FILE__ << ':' << __LINE__ << '\n';
//    std::cout << "\tnumBands = " << numBands << '\n';
//    std::cout << "\tupperLeftXCoordinate = " << upperLeftXCoordinate << '\n';
//    std::cout << "\tupperLeftYCoordinate = " << upperLeftYCoordinate << '\n';
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
        throw UnexpectedHeaderLine();
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

