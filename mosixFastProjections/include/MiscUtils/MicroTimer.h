#ifndef MICROTIMER_H
#define MICROTIMER_H
// $Id: MicroTimer.h,v 1.1 2005/03/11 23:59:09 mschisler Exp $
// Christopher B. Bilderback - cbilder@umr.edu
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:09 $

/***************************************************************************
 *                                                                         *
 *   This library is free software and is distributed under the MIT open   *
 *   source license.  For more information, consult the file COPYING.      *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
                          MicroTimer.h  -  a cross comptable wrapper around
                                           microsecond timers
                             -------------------
    begin                : Tue Jul 17 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <limits.h>
#else
#include <limits.h>
#include <iostream>
#include <sys/time.h>
#endif

#ifdef _WIN32
namespace MiscUtils
{
#define WRAP_TIME ULONG_MAX //4294967295U //2^32 -1
#else
namespace MiscUtils
{
#define WRAP_TIME INT_MAX
#endif

/**
 *wrapper around high res timers
 **/
class MicroTimer
{
public: 

  /**
   *Default constructor for the class
   *starts the period in windows
   **/
  MicroTimer();

  /**
   *Default destructor for the class
   *stops the period in windows
   **/
  virtual ~MicroTimer();

  /**
   *Returns the current time value in microseconds
   *(probably use start/stop combo instead because
   *it has stuff for time wrapping built in)
   **/
  bool getTime(unsigned long long & intime) const throw();

  /**
   *start Saves the current time for when stop is called
   **/
  bool start() throw();

  /**
   *stop returns the elasped time from when start is called
   **/
  bool stop(unsigned long int & elasped) throw();

protected:

  unsigned long int starttime_sec; //for elasped timing storage
  unsigned long int starttime_usec;//for elasped timing storage
  bool valid;                      //is this a valid timer
  unsigned int minPeriod;          //min period for windows stuff
};


//***************************************************************************
inline bool MicroTimer::getTime(unsigned long long  & intime) const throw()
{
#ifdef _WIN32
  if (valid)
  {
    intime = timeGetTime();
    return true;
  }
  return false;
#else
  timeval temptime;

  if (!gettimeofday(&temptime, 0))
  {
    intime = static_cast<unsigned long long>(temptime.tv_sec)*1000000 + 
      static_cast<unsigned long long>(temptime.tv_usec);
    return true;
  }
  return false;
#endif
}

//***************************************************************************
inline bool MicroTimer::start() throw()
{
#ifdef _WIN32
  if (valid)
  {
    starttime_usec = timeGetTime();
    return true;
  }
#else
  timeval temptime;
  
  if (!gettimeofday(&temptime, 0))
  {
    starttime_sec = static_cast<unsigned long int>(temptime.tv_sec);
    starttime_usec = static_cast<unsigned long int>(temptime.tv_usec);
    return true;
  }
#endif

  return false;

}

//***************************************************************************
inline bool MicroTimer::stop(unsigned long int & elasped) throw()
{
  unsigned long int stoptime_sec(0);
  unsigned long int stoptime_usec(0);

#ifdef _WIN32
  if (valid)
  {
    stoptime_usec = timeGetTime();
    //try to detect a single wrap
    if(starttime_usec >= stoptime_usec)
    {
      starttime_usec = WRAP_TIME - starttime_usec;
      elasped = starttime_usec + stoptime_usec + 1;
    }
    else
      elasped = stoptime_usec - starttime_usec + 1;
    return true;
  }
#else
  timeval temptime;
  
  if (!gettimeofday(&temptime, 0))
  {
    stoptime_sec = static_cast<unsigned long int>(temptime.tv_sec);
    stoptime_usec = static_cast<unsigned long int>(temptime.tv_usec);
    
    elasped = 1000000*(stoptime_sec - starttime_sec) + 
      (stoptime_usec - starttime_usec); 

    return true;
  }
#endif

  return false;
  
}

}//namespace
#endif
