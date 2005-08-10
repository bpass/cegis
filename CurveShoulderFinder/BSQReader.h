/**
 * @file BSQReader.h
 * @author Austin Hartman
 *
 * $Id: BSQReader.h,v 1.4 2005/08/10 01:00:21 ahartman Exp $
 */

#ifndef AUSTIN_BSQREADER_H
#define AUSTIN_BSQREADER_H

#include <string>
#include <fstream>

/**
 * A class used to hold information about a BSQ file and to allow the user to
 * access data from it.
 */
template<class DataType>
class BSQReader
{
public:
    /**
     * Used to indicate when we are referring to types that are UTM 
     * coordinates.
     */
    typedef double UTMCoordinateType;

    /**
     * Constructor to create an object from the specified BSQ file and the 
     * header file whose name is determined from the name of the BSQ file.
     *
     * @param bsqFileName The path to the BSQ file.  The name of the header
     * file is deduced from this by replacing the ".bsq" extension with a
     * ".hdr" extension.
     */
    BSQReader(const std::string& bsqFileName);

    /**
     * Constructor to create an object from the specifed BSQ file and the
     * specified header file.
     *
     * @param bsqFileName The path to the BSQ file.
     * @param headerFileName The path to the header file.
     */
    BSQReader(const std::string& bsqFileName,
              const std::string& headerFileName);

    /**
     * Copy constructor.
     */
    BSQReader(const BSQReader<DataType>& rhs);

    /**
     * Copy assignment operator.
     */
    BSQReader<DataType>& operator=(const BSQReader<DataType>& rhs);

    /**
     * Gets the value at specified row, column, and band in the image, where
     * row and column represent pixel coordinates.  The pixel in the upper
     * left corner of the image is at row 0, col 0, and the pixel in the lower
     * right corner of the image is at row getNumRows(), col getNumCols().
     *
     * @param row The row of the pixel in the image.  Rows start at zero at
     * the top of the image.
     * @param col The column of the pixel in the image.  Columns start at zero
     * at the left of the image.
     * @param band The band of the pixel in the image.
     *
     * @return The value from the image at the specified position and band.
     */
    DataType getValue(size_t row, size_t col, size_t band) const;

    /**
     * Get the value from the BSQ image in the specified band at the 
     * specified UTM coordinates.
     *
     * @param xCoord The x-coordinate of the location we want to get the
     * value from.
     * @param yCoord The y-coordinate of the location we want to get the
     * value from.
     * @param band The band we want to get the value from.
     *
     * @return The value from the image at the specified position and band.
     */
    DataType getValue(const UTMCoordinateType& xCoord, 
                      const UTMCoordinateType& yCoord,
                      size_t band) const;

    /**
     * Gets the minimum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The minimum x-coordinate.
     */
    UTMCoordinateType getMinX() const;

    /**
     * Gets the maximum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The maximum x-coordinate.
     */
    UTMCoordinateType getMaxX() const;

    /**
     * Gets the minimum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The minimum y-coordinate.
     */
    UTMCoordinateType getMinY() const;

    /**
     * Gets the maximum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The maximum y-coordinate.
     */
    UTMCoordinateType getMaxY() const;

    /**
     * Gets the width in UTM coordinates that each pixel in the image covers.
     *
     * @return The width of each pixel.
     */
    UTMCoordinateType getPixelWidth() const;

    /**
     * Gets the height in UTM coordinates that each pixel in the image covers.
     *
     * @return The height of each pixel.
     */
    UTMCoordinateType getPixelHeight() const;
    
    /**
     * Gets the number of rows in the image.
     *
     * @return The number of rows.
     */
    size_t getNumRows() const;
    
    /**
     * Gets the number of columns in the image.
     *
     * @return The number of columns.
     */
    size_t getNumCols() const;
    
    /**
     * Gets the number of bands in the image.
     *
     * @return The number of bands.
     */
    size_t getNumBands() const;

    /**
     * Exception class to be used when the header file contains an unexpected
     * line.
     */
    class UnexpectedHeaderLine
    {};

    /**
     * Exception class to be used when the user requests data from a band not
     * contained in the source image.
     */
    class InvalidBand
    {};

    /**
     * Exception class to be used when the user requests data from a row not
     * in the source image.
     */
    class InvalidRow
    {};

    /**
     * Exception class to be used when the user requests data from a column not
     * in the source image.
     */
    class InvalidCol
    {};

    /**
     * Exception class to be used when the user requests data from an
     * x-coordinate that the source image does not contain.
     */
    class InvalidXCoordinate
    {};

    /**
     * Exception class to be used when the user requests data from an
     * y-coordinate that the source image does not contain.
     */
    class InvalidYCoordinate
    {};

private:
    /**
     * The names of the bsq file and header file that we're reading from.
     */
    std::string m_bsqFilename, m_headerFilename;

    /**
     * The image file that we're reading from.
     *
     * @note This is mutable so the user can treat this object as if it
     * contains the complete contents of the file in memory, even though
     * it is actually only read when a request for a pixel is made.
     */
    mutable std::ifstream m_bsqFile;

    /**
     * Holds some of the data that was read from the header file and contains
     * information about the image file.
     */
    //@{
    UTMCoordinateType m_upperLeftXCoordinate, m_upperLeftYCoordinate,
                      m_lowerRightXCoordinate, m_lowerRightYCoordinate,
                      m_pixelWidth, m_pixelHeight;
    size_t m_numRows, m_numCols, m_numBands;
    //@}

    /**
     * A class used when reading from the header.  Its data members represent
     * what was read from an individual line of the header file.  The key is
     * what came before the colon and the value is what came after.
     */
    struct KeyValuePair
    {
        std::string key;
        std::string value;
    };

    /**
     * Reads the header file and fills in the private data members with the
     * information from it.
     *
     * @param headerFileName The path to the header file to be read.
     */
    void readHeaderFile(const std::string& headerFileName);

    /**
     * Parses a single line from the header file and returns the key and value
     * from it.
     *
     * @param line The line of the header file to be parsed
     */
    KeyValuePair parseHeaderLine(const std::string& line) const;
};

#include "BSQReader.hpp"

#endif

