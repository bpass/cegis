
#ifndef STITCHER_CPP
#define STITCHER_CPP

#include "Stitcher.h"
#include <iostream>


//*************************************************************
Stitcher::Stitcher( USGSImageLib::ImageOFile * inout, unsigned long lines )
{
  //copy the output file
  out = inout;
  m_currentRow = 0;
  done = false;
  
  // assign the total number of lines to the job
  m_totalLines = lines;
  std::cout << "STITCHER: m_totalLines is " << m_totalLines << std::endl;
  
  // MUST BE LAST: start the thread
  thread_start_func threadthing(this);
  boost::thread threadhead(threadthing);
}
  
//*************************************************************
Stitcher::~Stitcher()
{
  //This is really just for premature termination by the calling thread
  // StitcherNode * tempnode(0);
  
  //check the queue
  /*  while(workqueue.size())
  {
    tempnode = workqueue.top();
    workqueue.pop();
    delete tempnode;
  } */
  
  //create the termination node
  //boost::mutex::scoped_lock workmutexlock(workmutex);
  //if (!(tempnode = new (std::nothrow) StitcherNode(NULL,0, 0)))
  //  throw std::bad_alloc();

  //put it in the queue
  // workqueue.push(tempnode);

  //signal the thread
  //workcond.notify_one();

  // workmutexlock.unlock();

  // lock the done mutex and wait
  boost::mutex::scoped_lock donemutexlock(donemutex);
  if ( !done )
      waitcond.wait(donemutexlock);
  donemutexlock.unlock();
}
  
//***********************************************************
void Stitcher::add(StitcherNode * temp) throw()
{
  // notify the run() execution, there's more in the queue now  
  std::cout << "ADD: signal end wait" << std::endl;
   
  //put the work in the queue
  boost::mutex::scoped_lock workmutexlock(workmutex);
  std::cout << "ADD: acquired lock, adding " << std::endl;
  workqueue.push(temp);
  workmutexlock.unlock();
  std::cout << "ADD: unlock, adding " << std::endl;
  
  workcond.notify_one();
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
  StitcherNode * temp(0);
  long row(0);
  void * data(0);
  bool ldone(false);
  //check the output image
  if (!out)
  {
    std::cout << "STITCHER: out is NULL !" << std::endl;
    waitcond.notify_one();
    boost::mutex::scoped_lock donemutexlock(donemutex);
    done = true;
    donemutexlock.unlock();
    return; //exit
  }

  std::cout << "STITCHER: m_totalLines is " << m_totalLines << std::endl;
  
  while(!ldone)
  { 
    //check the queue
    boost::mutex::scoped_lock workmutexlock(workmutex);
    if ( m_currentRow >= m_totalLines )
    {
          std::cout << "STITCHER: reached last line. " << std::endl;
          std::cout << "STITCHER: m_currentRow is " << m_currentRow <<std::endl;
          std::cout << "STITCHER: m_totalLines is " << m_totalLines <<std::endl;
          workmutexlock.unlock();
          ldone = true;
          
    } else if (workqueue.size() == 0 ) 
    {
      // wait for work to be put in the queue
      std::cout << "STITCHER: waiting on empty" << std::endl;
      workcond.wait(workmutexlock);
      workmutexlock.unlock();
      std::cout << "STITCHER: signaled to continue" << std::endl;
    }
    else 
    {
      temp = workqueue.top();
      if ( static_cast<unsigned int>(temp->getrow()) == m_currentRow )
      {
           m_currentRow++;
           workqueue.pop();
           workmutexlock.unlock();
           row = temp->getrow();
           data = temp->getdata();
          
           //check early termination
           //if (row == INT_MAX )
           //{
           //  std::cout << "STITCHER: int_max encountered " << std::endl;
           //  ldone = true;
           //  delete temp;
           //}
           //else
           //{
           
           std::cout << "STITCHER: waiting for lock on 'out'. " << std::endl;
           boost::mutex::scoped_lock scanlineLock(scanlineMutex);
           std::cout << "STITCHER: writing row " << m_currentRow << std::endl;
            
           try { 
            
                out->putRawScanline(row, data);
                
            } catch ( USGSImageLib::ImageException & e ) 
            {
                std::string s;
                e.getString(s); 
                std::cout << "STITCHER: exception caught: " << s << std::endl;    
            }
             
            scanlineLock.unlock();
            
            delete temp;
            
            //}
      } else       { 
        // std::cout << "STITCHER: top is " << temp->getrow() 
        //           << "waiting for " << m_currentRow << std::endl;
          
        workmutexlock.unlock();
      }
    }
  }

  //termination
  waitcond.notify_one();
  boost::mutex::scoped_lock donemutexlock(donemutex);
  done = true;
  donemutexlock.unlock();
}

#endif

