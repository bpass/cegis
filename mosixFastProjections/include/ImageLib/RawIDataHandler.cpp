#ifndef _RAWIDATAHANDLER_CPP_
#define _RAWIDATAHANDLER_CPP_
// $Id: RawIDataHandler.cpp,v 1.1 2005/03/11 23:59:08 mschisler Exp $
// Brian Maddox - bmaddox@usgs.gov
// 21 May 1998
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:08 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

// Base class for raw data input

#include "RawIDataHandler.h"

using namespace USGSImageLib;

// ***************************************************************************
RawIDataHandler::RawIDataHandler(std::ifstream* instream, long int inposition,
                                 long int inwidth, long int inheight,
                                 int inphotometric) throw(ImageException)
  : RawDataHandler(inwidth, inheight, inphotometric),
    dataposition(inposition),
    Stream(instream)
{
}

// ***************************************************************************
RawIDataHandler::~RawIDataHandler()
{
}


#endif // #ifndef RAWIDATAHANDLER_CPP
