#ifndef _COMPARE_NOCASE_H_
#define _COMPARE_NOCASE_H_
// $Id: compare_nocase.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian G. Maddox - 27 Dec 1999
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// These are the compare_nocase functions.  They take in a C++ std::string or
// a string and a char array and perform a case insensitive comparison,
// returning true if they are equal or false otherwise.  These functions
// are originally from Stroustrup's "The C++ Programming Language", 3rd ed.

#include <string>

namespace MiscUtils
{
  bool compare_nocase(const std::string& s1, const std::string& s2) throw();
  bool compare_nocase(const std::string& s1, const char* s2) throw();
}


#endif // #ifndef _COMPARE_NOCASE_H_
