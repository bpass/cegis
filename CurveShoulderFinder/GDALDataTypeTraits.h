/**
 * @file GDALDataTypeTraits.h
 * @author Austin Hartman
 *
 * $Id: GDALDataTypeTraits.h,v 1.2 2005/09/19 23:01:01 ahartman Exp $
 */

#ifndef AUSTIN_GDALDATATYPETRAITS_H
#define AUSTIN_GDALDATATYPETRAITS_H

#include <gdal.h>

/**
 * A traits class to allow the conversion from C++ types to the appropriate
 * value of the enum GDALDataType.
 */
//@{
template<class DataType>
struct GDALDataTypeTraits
{
    static const GDALDataType dataType = GDT_Byte;
};

template<>
struct GDALDataTypeTraits<unsigned int>
{
    static const GDALDataType dataType = GDT_UInt32;
};

template<>
struct GDALDataTypeTraits<int>
{
    static const GDALDataType dataType = GDT_Int32;
};

template<>
struct GDALDataTypeTraits<float>
{
    static const GDALDataType dataType = GDT_Float32;
};

template<>
struct GDALDataTypeTraits<double>
{
    static const GDALDataType dataType = GDT_Float64;
};
//@}

#endif
