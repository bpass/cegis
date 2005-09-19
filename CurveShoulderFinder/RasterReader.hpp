/**
 * @file RasterReader.hpp
 * @author Austin Hartman
 *
 * $Id: RasterReader.hpp,v 1.2 2005/09/19 15:29:15 ahartman Exp $
 */

#ifdef AUSTIN_RASTERREADER_H
#ifndef AUSTIN_RASTERREADER_HPP
#define AUSTIN_RASTERREADER_HPP

template<class DataType>
RasterReader<DataType>::~RasterReader()
{}

template<class DataType>
RasterReader<DataType>::FailedOpeningFile::~FailedOpeningFile()
{}

#endif
#endif

