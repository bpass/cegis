#ifndef _IMAGEFILE_H_
#define _IMAGEFILE_H_
// $Id: ImageFile.h,v 1.1 2005/03/11 23:59:08 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:08 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// Defines the abstract class ImageFile

#include <new>
#include "ImageData.h"

namespace USGSImageLib
{
class ImageFile : public ImageData
{
public:

  ImageFile() throw(ImageException, std::bad_alloc);
  ImageFile(std::string& fn, long int width, long int height)
    throw(ImageException, std::bad_alloc);  // create
  ImageFile(std::string& fn)
    throw(ImageException, std::bad_alloc);  // open existing
  virtual ~ImageFile();

  void setFileName(std::string& inname)          throw();
  void getFileName(std::string& _filename) const throw();

  // Bounds checking
  bool validateRow(long int& row)           const throw();
  bool validateXY(long int& x, long int& y) const throw();

  bool normalizeCoords(long int& x1, long int& y1, long int& x2, long int& y2)
    const throw();

private:
  std::string filename;

};

} // namespace
#endif
