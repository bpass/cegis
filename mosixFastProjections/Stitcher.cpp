
#ifndef STITCHER_CPP
#define STITCHER_CPP

#include "Stitcher.h"


//*************************************************************
Stitcher::Stitcher(USGSImageLib::ImageOFile * inout)
{
  
  //copy the output file
  out = inout;
  
  done = false;
  //start the thread
  thread_start_func threadthing(this);
  boost::thread threadhead(threadthing);
  threadhead.join();
}
  
//*************************************************************
Stitcher::~Stitcher()
{
  //This is really just for premature termination by the calling thread
  StitcherNode * tempnode(0);
  
  //lock the que
  boost::mutex::scoped_lock workmutexlock(workmutex);
  
  //check the queue
  while(workqueue.size())
  {
    tempnode = workqueue.front();
    workqueue.pop();
    delete tempnode;
  }
  
  //create the termination node
  if (!(tempnode = new (std::nothrow) StitcherNode(NULL,0, 0)))
    throw std::bad_alloc();

  //put it in the queue
  workqueue.push(tempnode);

  //signal the thread
  workcond.notify_one();
  workmutexlock.unlock();

  //lock the done mutex and wait
  boost::mutex::scoped_lock donemutexlock(donemutex);
  while(!done)
    waitcond.wait(donemutexlock);
  donemutexlock.unlock();
}
  
//***********************************************************
void Stitcher::add(StitcherNode * temp) throw()
{
  //aquire the workmutex
  boost::mutex::scoped_lock workmutexlock(workmutex);
  //put the work in the queue
  workqueue.push(temp);
  //signal the thread
  workcond.notify_one();
  workmutexlock.unlock();
}

//**********************************************************
void Stitcher::wait() throw()
{
  //try to aquire the done mutex
  boost::mutex::scoped_lock donemutexlock(donemutex);
  while(!done)
    waitcond.wait(donemutexlock);
  donemutexlock.unlock();
}

//**************************************************************
void Stitcher::run() throw()
{
  long int counter(0), newheight(0), newwidth(0);
  int spp(0), bps(0);
  StitcherNode * temp(0);
  long row(0);
  void * data(0);
  bool ldone(false);
  //check the output image
  if (!out)
  {
    boost::mutex::scoped_lock donemutexlock(donemutex);
    done = true;
    waitcond.notify_one();
    donemutexlock.unlock();
    return; //exit
  }

  //get the image info
  out->getHeight(newheight);
  out->getWidth(newwidth);
  out->getSamplesPerPixel(spp);
  out->getBitsPerSample(bps);
  //loop
  while(!ldone)
  {
    //check the queue
    boost::mutex::scoped_lock workmutexlock(workmutex);
    if (!workqueue.size()) // if the size is 0
    {
      // wait for work to be put in the queue
      workcond.wait(workmutexlock);
      workmutexlock.unlock();
    }
    else // get to work!
    {
      temp = workqueue.front();
      workqueue.pop();
      workmutexlock.unlock();
      row = temp->getrow();
      data = temp->getdata();
      
      //check early termination
      if (row < 0)
      {
        ldone = true;
        delete temp;
      }
      else
      {
        out->putRawScanline(row, data);
        delete temp;
        // LEGACY?: check termination
        //   if (stop == newheight-1)
        //       ldone = true;
      }      
    }
  }

  //termination
  boost::mutex::scoped_lock donemutexlock(donemutex);
  done = true;
  waitcond.notify_one();
  donemutexlock.unlock();
}

#endif

