#ifndef _IMAGE_DATA_CPP_
#define _IMAGE_DATA_CPP_
// $Id: ImageData.cpp,v 1.1 2005/03/11 23:59:08 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:08 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// Implements the class ImageData

#include "ImageData.h"

using namespace USGSImageLib;

// ***************************************************************************
ImageData::ImageData(void)
  throw(ImageException, std::bad_alloc)
  : Width(0), Height(0), bitsPerSample(0), samplesPerPixel(0), Photometric(0),
    State(goodbit), _hasPalette(false)
{
}

// ***************************************************************************
ImageData::ImageData(long int width, long height) 
  throw(ImageException, std::bad_alloc)
  : Width(width), Height(height), bitsPerSample(0), samplesPerPixel(0),
    Photometric(0), State(goodbit),  _hasPalette(false)
{
}

// ***************************************************************************
ImageData::~ImageData()
{
}


#endif
