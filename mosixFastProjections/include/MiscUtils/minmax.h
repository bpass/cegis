#ifndef _MINMAX_H_
#define _MINMAX_H_
// $Id: minmax.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian Maddox - 27 Dec 1999
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// Definition of the min and max functions

namespace MiscUtils
{
  template<class T> T MAX(const T& x, const T& y) throw()
  {
    if (x >= y)
      return x;
    else
      return y;
  }

  template<class T> T MIN(const T& x, const T& y) throw()
  {
    if (x <= y)
      return x;
    else
      return y;
  }
}


#endif
