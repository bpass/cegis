#ifndef _SPINNERTHINGY_H_
#define _SPINNERTHINGY_H_
// $Id: SpinnerThingy.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Christopher B. Bilderback - cbilder@umr.edu
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

/** 
 *A Simple class to draw a spinner and label on
 *a console.  Used as a status spinner.
 *Original author: Greg Martin
 *Documented and updated by Chris Bilderback
 **/

#include <iostream>
#include <string>
#include <sstream>

//Added to be part the miscutils libary
namespace MiscUtils
{

//Consol status spinner class
class SpinnerThingy
{
  char updt[4];        //Holds the snapshots of the spinner
  int status;          //the current status of the spinner
  int lastMsgLength;   //the length of the last message
  
 public:
  //Constructor
  SpinnerThingy();
  
  //Stop spinning and output the done message
  void done(const char * msg) throw();
  
  //Update the spinner and output integer
  void update(int step) throw();
  
  //Update the spinner and output long
  void update(long step) throw();

  //Update the spinner and output double
  void update(double step) throw();
  
  //Update the spinner and output a message
  void update(const char * step) throw();
};
} //namespace
#endif


