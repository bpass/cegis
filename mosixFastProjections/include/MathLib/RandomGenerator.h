// $Id: RandomGenerator.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

// This is the base class for the random number generators.  It also defines
// the interface all descendents should have

#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_


namespace MathLib
{

class RandomGenerator
{
 public:
  RandomGenerator() {};
  virtual ~RandomGenerator() {};

  virtual void reset() throw() =0;                  // reinit the seed values
  virtual void reset(long int& inseed) throw() =0;

  virtual float getRandom() throw() = 0;
};

} // namespace MathLib


#endif // #ifndef _RANDOMGENERATOR_H_

