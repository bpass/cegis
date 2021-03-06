#ifndef STITCHER_H_
#define STITCHER_H_

//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Unknown
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Thu Mar 17 08:31:32 CST 2005
//
// File               : Stitcher.h 
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: The stitcher's goal is to be a hack, essentially.  It  
// steals the ImageOFile pointer from the projector class after it 
// has been initialized, from there it uses it to write out scanlines
// on a seperate thread.  The idea is that this process will speed up
// execution time.
//
///////////////////////////////////////////////////////////////////////

#include "ImageLib/ImageOFile.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <queue>
#include "StitcherNode.h"


//The stitcher class
class Stitcher
{
 public:
  
  /**
   * This is the constructor for the stitcher class 
   * that takes a pointer to the output file.
   * Immediately starts the interal thread
   * DON'T MESS WITH THE FILE AFTER CREATING
   * THE THREAD UNTIL THE THREAD IS TERMINATED
   * (IMAGELIB is NOT thead safe)
   **/
  Stitcher( USGSImageLib::ImageOFile * inout, unsigned long lines );
  
  /**
   * Destructor: It is up to the user to close the output file
   **/
  virtual ~Stitcher();

  
  /**
   * Add function adds a stitcher node to the stitchers interal
   * queue to be processed.
   * (The stitcher will delete the node when it processes it)
   **/
  void add(StitcherNode * temp) throw();
  
  /**
   * Wait function will wait (block) until the stitcher completely 
   * finishes writing the image.
   **/
  void wait() throw();

  /**
   * run function is where the the thread actually runs and should
   * not ever be called by any outside thread.
   **/
  void run() throw();
  
 private:
  unsigned long m_totalLines;
  unsigned long m_currentRow;
  USGSImageLib::ImageOFile * out;
  boost::mutex scanlineMutex;
  boost::mutex donemutex;
  boost::mutex workmutex;
  boost::condition workcond;
  boost::condition waitcond;

  bool done;                               //the done tag tells the stitcher
                                           //that it is done
  std::priority_queue< StitcherNode *,
                       std::vector<StitcherNode *>, 
                       StitcherCompare > workqueue;    
};


//This is the function that starts a thread that the lives in
//the class
struct thread_start_func
{
  thread_start_func(Stitcher * new_class_instance)
    : class_instance(new_class_instance) {}
  void operator()()
  {
    if(class_instance != NULL)
      class_instance->run();
  }
  Stitcher * class_instance;
};

#endif
