#ifndef _CMP_NOCASE_H_
#define _CMP_NOCASE_H_
// $Id: cmp_nocase.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian G. Maddox - 27 Dec 1999
// Last Modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

#include <string>

// These are the cmp_nocase functions from Stroustup's "The C++ Programming
// Language" 3rd edition.  They function as C's strcmp and strncmp do.

namespace MiscUtils
{

  int cmp_nocase(const std::string& s, const std::string& s1) throw();
  int cmp_nocase(const std::string& s, const char* s1) throw();

}

#endif // #ifndef _CMP_NOCASE_H_
