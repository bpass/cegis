/**
 * @file ImgReader.h
 * @author Austin Hartman
 *
 * $Id: ImgReader.h,v 1.3 2005/09/19 17:29:03 ahartman Exp $
 */

#ifndef AUSTIN_IMGREADER_H
#define AUSTIN_IMGREADER_H

#include <string>
#include <vector>

#include <gdal_priv.h>

#include "RasterReader.h"

template<class DataType>
class ImgReader : public RasterReader<DataType>
{
public:
    /**
     * Used to indicate when we are referring to types that are UTM 
     * coordinates.
     */
    typedef typename RasterReader<DataType>::UTMCoordinateType 
        UTMCoordinateType;

    /**
     * Constructor to create an img reader from a filename.
     */
    ImgReader(const std::string& filename);

    /**
     * Destructor.
     */
    virtual ~ImgReader();

    /**
     * Get the value from the img file in the specified band at the 
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
                              size_t band) const;

    /**
     * Gets the minimum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The minimum x-coordinate.
     */
    virtual UTMCoordinateType getMinX() const;

    /**
     * Gets the maximum value of the x-coordinate that the image contains
     * data for.
     *
     * @return The maximum x-coordinate.
     */
    virtual UTMCoordinateType getMaxX() const;

    /**
     * Gets the minimum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The minimum y-coordinate.
     */
    virtual UTMCoordinateType getMinY() const;

    /**
     * Gets the maximum value of the y-coordinate that the image contains
     * data for.
     *
     * @return The maximum y-coordinate.
     */
    virtual UTMCoordinateType getMaxY() const;
    
    /**
     * Gets the number of bands in the image.
     *
     * @return The number of bands.
     */
    virtual size_t getNumBands() const;

    /**
     * Exception class to be used when opening the img file fails.
     */
    class FailedOpeningImgFile : 
        public RasterReader<DataType>::FailedOpeningFile
    {
    public:
        virtual ~FailedOpeningImgFile();
    };

private:
    static bool initialized;
    GDALDataset* dataset;
    std::vector<double> geoTransform;
};

#include "ImgReader.hpp"

#endif

