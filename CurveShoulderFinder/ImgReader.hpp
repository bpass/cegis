/**
 * @file ImgReader.hpp
 * @author Austin Hartman
 *
 * $Id: ImgReader.hpp,v 1.6 2005/09/21 16:16:29 ahartman Exp $
 */

#ifdef AUSTIN_IMGREADER_H
#ifndef AUSTIN_IMGREADER_HPP
#define AUSTIN_IMGREADER_HPP

#include <cassert>

#include "GDALDataTypeTraits.h"

template<class DataType>
bool ImgReader<DataType>::initialized = false;

template<class DataType>
ImgReader<DataType>::ImgReader(const std::string& filename)
    : geoTransform(6)
{
    if(!initialized)
    {
        GDALAllRegister();
        initialized = true;
    }
    
    dataset.reset(static_cast<GDALDataset*>(GDALOpen(filename.c_str(),
                                                     GA_ReadOnly)));
    if(dataset == NULL)
    {
        throw typename ImgReader<DataType>::FailedOpeningImgFile();
    }

    CPLErr getGeoTransformError = dataset->GetGeoTransform(&geoTransform[0]);
    // XXX do something in case there's an error

    // make sure that our assumption that the image is "north up" is correct
    assert(geoTransform[2] == 0);
    assert(geoTransform[4] == 0);
}

template<class DataType>
ImgReader<DataType>::~ImgReader()
{}

template<class DataType>
DataType
ImgReader<DataType>::getValue(const UTMCoordinateType& xCoord, 
                              const UTMCoordinateType& yCoord,
                              size_t band) const
{
    GDALRasterBand* rasterBand = dataset->GetRasterBand(band);
    const size_t pixel = static_cast<size_t>
                         ((xCoord - geoTransform[0]) / geoTransform[1]);
    const size_t line  = static_cast<size_t>
                         ((yCoord - geoTransform[3]) / geoTransform[5]);
    // XXX maybe do some error checking here to see if xCoord and yCoord are
    // even in the image; GDAL might do this already though

    DataType data; // The memory we'll read into

    CPLErr rasterIOError = 
        rasterBand->RasterIO(GF_Read,
                             pixel, line,
                             1, 1, // read just a single pixel
                             &data,
                             1, 1,
                             GDALDataTypeTraits<DataType>::dataType, 
                             0, 1);

    // XXX do something in case there's an error

    return data;
}

template<class DataType>
typename ImgReader<DataType>::UTMCoordinateType
ImgReader<DataType>::getMinX() const
{
    return geoTransform[0];
}

template<class DataType>
typename ImgReader<DataType>::UTMCoordinateType
ImgReader<DataType>::getMaxX() const
{
    return geoTransform[0] + geoTransform[1] * dataset->GetRasterXSize();
}

template<class DataType>
typename ImgReader<DataType>::UTMCoordinateType
ImgReader<DataType>::getMinY() const
{
    return geoTransform[3] + geoTransform[5] * dataset->GetRasterYSize();
}

template<class DataType>
typename ImgReader<DataType>::UTMCoordinateType
ImgReader<DataType>::getMaxY() const
{
    return geoTransform[3];
}

template<class DataType>
size_t
ImgReader<DataType>::getNumBands() const
{
    return dataset->GetRasterCount();
}

template<class DataType>
ImgReader<DataType>::FailedOpeningImgFile::~FailedOpeningImgFile()
{}

#endif
#endif

