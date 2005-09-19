/**
 * @file ImgReader.hpp
 * @author Austin Hartman
 *
 * $Id: ImgReader.hpp,v 1.1 2005/09/19 15:06:21 ahartman Exp $
 */

#ifdef AUSTIN_IMGREADER_H
#ifndef AUSTIN_IMGREADER_HPP
#define AUSTIN_IMGREADER_HPP

#include <cassert>

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
    
    dataset = static_cast<GDALDataSet*>(GDALOpen(filename.c_str(),
                                                 GA_ReadOnly));
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
{
    delete dataset;
}

template<class DataType>
DataType
ImgReader<DataType>::getValue(const UTMCoordinateType& xCoord, 
                              const UTMCoordinateType& yCoord,
                              size_t band) const
{
    GDALRasterBand* rasterBand = dataset->GetRasterBand(band);
    const size_t pixel = /*static_cast<size_t>*/
                         ((xCoord - geoTransform[0]) / geoTransform[1]);
    const size_t line  = /*static_cast<size_t>*/
                         ((yCoord - geoTransform[3]) / geoTransform[5]);
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

#endif
#endif

