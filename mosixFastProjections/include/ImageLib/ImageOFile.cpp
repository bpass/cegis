#ifndef _IMAGEOFILE_CPP_
#define _IMAGE_FILE_CPP_
// $Id: ImageOFile.cpp,v 1.1 2005/03/11 23:59:08 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:08 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

#include "ImageOFile.h"

using namespace USGSImageLib;

// ***************************************************************************
ImageOFile::ImageOFile()
  throw (ImageException, std::bad_alloc)
  : ImageFile()
{
}

// ***************************************************************************
ImageOFile::ImageOFile(std::string& infile, long int width, long height)
  throw (ImageException, std::bad_alloc)
  : ImageFile (infile, width, height)
{
}

// ***************************************************************************
ImageOFile::~ImageOFile()
{
}

#endif
