#ifndef _CMP_NOCASE_CPP_
#define _CMP_NOCASE_CPP_
// $Id: cmp_nocase.cpp,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian G. Maddox - 27 Dec 1999
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

#include "cmp_nocase.h"

#include <cctype>


// ***************************************************************************
int MiscUtils::cmp_nocase(const std::string& s, const std::string& s2) throw()
{
  std::string::const_iterator p  = s.begin();
  std::string::const_iterator p2 = s2.begin();
  int temp1, temp2; // for the toupper calls
  size_t s_size, s2_size; // for the size calls

  while ( (p != s.end()) && (p2 != s2.end()) )
  {
    temp1 = std::toupper(*p);
    temp2 = std::toupper(*p2);
    if (temp1 != temp2)
      return ( temp1 < temp2 ? -1 : 1);
    ++p;
    ++p2;
  }

  // If here, return if the size is the same
  s_size  = s.size();
  s2_size = s2.size();
  return ( (s2_size == s_size) ? 0 : (s_size < s2_size) ? -1 : 1);
}

// ***************************************************************************
// Overloaded to allow the user to pass a normal char array in to compare to
// a string.  Note that this simply creates a string and sends it to the above
// function.
int MiscUtils::cmp_nocase(const std::string& s, const char* s2) throw()
{
  std::string temp = std::string(s2); // make the temp string
  return cmp_nocase(s, temp);
}

#endif
