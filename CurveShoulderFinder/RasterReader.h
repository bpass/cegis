/**
 * @file RasterReader.h
 * @author Austin Hartman
 *
 * $Id: RasterReader.h,v 1.2 2005/09/19 15:29:15 ahartman Exp $
 */

#ifndef AUSTIN_RASTERREADER_H
#define AUSTIN_RASTERREADER_H

/**
 * An interface base class for the user to access values from raster
 * files when they give UTM coordinates.
 */
template<class DataType>
class RasterReader
{
public:
    /**
     * Used to indicate when we are referring to types that are UTM 
     * coordinates.
     */
    typedef double UTMCoordinateType;

    /**
     * Pure virtual destructor.
     */
    virtual ~RasterReader() = 0;

    /**
     * Get the value from the raster in the specified band at the 
     * specified UTM coordinates.
     *
     * @param xCoord The x-coordinate of the location we want to get the
     * value from.
     * @param yCoord The y-coordinate of the location we want to get the
     * value from.
     * @param band The band we want to get the value from.  Valid bands are 
     * from 1 to getNumBands().
     *
     * @return The value from the image at the specified position and band.
     */
    virtual DataType getValue(const UTMCoordinateType& xCoord, 
                              const UTMCoordinateType& yCoord,
                              size_t band) const = 0;

    /**
     * Gets the minimum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The minimum x-coordinate.
     */
    virtual UTMCoordinateType getMinX() const = 0;

    /**
     * Gets the maximum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The maximum x-coordinate.
     */
    virtual UTMCoordinateType getMaxX() const = 0;

    /**
     * Gets the minimum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The minimum y-coordinate.
     */
    virtual UTMCoordinateType getMinY() const = 0;

    /**
     * Gets the maximum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The maximum y-coordinate.
     */
    virtual UTMCoordinateType getMaxY() const = 0;
    
    /**
     * Gets the number of bands in the image.
     *
     * @return The number of bands.
     */
    virtual size_t getNumBands() const = 0;

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

    /**
     * Exception class to be used when the user requests data from a band not
     * contained in the source image.
     */
    class InvalidBand
    {};

    /**
     * Exception class to be used when opening a file fails
     */
    class FailedOpeningFile
    {
    public:
        virtual ~FailedOpeningFile();
    };
};

#include "RasterReader.hpp"

#endif
