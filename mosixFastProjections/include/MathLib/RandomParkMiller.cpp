// $Id: RandomParkMiller.cpp,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $
// Started: 8 June 1999

// Implementation file for the RandomParkMiller class

#ifndef _RANDOMPARKMILLER_CPP_
#define _RANDOMPARKMILLER_CPP_

#include <stdlib.h>
#include <time.h>
#include "RandomParkMiller.h"

MathLib::RandomParkMiller::RandomParkMiller()
{
  IA = 16807;
  IM = 2147483647;
  AM = 1.0f / IM;
  IQ = 127773;
  IR = 2836;
  MASK = 123459876;
  ans = 0.0;
  k = 0;
  idum = 0;

  // call reset to do the inital sequence initialization
  reset();
}


// **************************************************************************
void MathLib::RandomParkMiller::reset()
  throw()
{
  srand(time(0));
  idum = rand();
}


// **************************************************************************
void MathLib::RandomParkMiller::reset(long int& inseed)
  throw()
{
  idum = inseed;
}

#endif // #ifndef _RANDOMPARKMILLER_CPP_
