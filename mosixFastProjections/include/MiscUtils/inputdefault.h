#ifndef _INPUTDEFAULT_H_
#define _INPUTDEFAULT_H_
// $Id: inputdefault.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Chris Bilderback - 7 March 2002
// Last Modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <string>
#include <sstream>

// These are some functions to get input from the console but allow for
// defaults (ie let the user just press enter).
// Each of these returns true if no default (enter without text) was
// pressed.

namespace MiscUtils
{
  //primatives supported by this must also support stringstream extaction (>>)
  template <typename Type>
    bool indefault(Type & result) throw();
  //put this in so that they can have length checking requirments.
  bool indefault(char result[], int length) throw();

}

//**************************************************************************
template <typename Type>
bool MiscUtils::indefault(Type & result) throw()
{
  std::string inbuff;

  //make sure that any trailing /n are removed from stream
  std::cin.ignore(std::cin.rdbuf()->in_avail());

  //get the line
  std::getline(std::cin, inbuff);
  
  //check the default
  if(!inbuff.size())
  {
    return false;
  }

  std::istringstream ins(inbuff);
  
  ins >> result; //try to extract the answer
  
  //this have the potential to be an infinit loop if
  //they pass a type that does not have extraction overloaded.
  while(ins.fail())
  {
    std::cout << std::endl << "Invalid Input. Try again or " 
              << "hit enter for default: " << std::endl;
    
    std::getline(std::cin, inbuff);
    ins.clear();
    ins.str(inbuff);
    ins >> result;
  }

  return true;
}


//************************************************************************* 
bool MiscUtils::indefault(char result[], int length) throw()
{
  int counter = 0;
  std::string inbuf;

  if (!MiscUtils::indefault(inbuf))   //just read as string
    return false;
  
  while(inbuf.size() >= length)
  {
    std::cout << std::endl  
              << "String entered is too long.  Please try again or hit enter " 
              << "for default." << std::endl;
    
    if(!MiscUtils::indefault(inbuf))
      return false;
  }

  //inbuf is valid now
  for(counter = 0; counter < inbuf.size(); ++counter)
  {
    result[counter] = inbuf[counter];
  }

  return true; //success
}





#endif
