//////////////////////////////////////////////////////////////////////
//
// Original Programmer: Matt Zykan
// 
// Last Modified by   : Mark Schisler
// Last Modified on   : Wed Mar 16 15:43:55 CST 2005
//
// File               : slavemain.cpp
// 
//////////////////////////////////////////////////////////////////////
// 
// Purpose: This is the master process for the fast  projections 
// project.  It is used to spawn off children processes whose purpose 
// it is to take a part in the map projection job through a fork() call 
// making it so that the entire projection can be automatically 
// distributed across a MOSIX grid.
//
///////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifdef _MOSIX
   #include <mos.h>
#endif

#include "WorkManager.h"
#include "SlaveManager.h"
#include "inparms.h"


int main(int argc,char ** argv)
{
  int workunits = 30;
  
  // to keep this process on the machine it started... 
  // (because of input/output
  #ifdef _MOSIX
     msx_lock();
  #endif
  
  BigJob *projJob = new BigJob(false);
  projJob->setnumworkunits((workunitid_t)workunits);

  ///////////////////////////
  
  Projection * outproj = NULL;   //output projection
  inputparm inparms;             //for master setup and parameters

  // this segment of code that follows creates the task
  try
  {
    if (argc == 1) //we are the master with out a flat file so get input
      inparms.getinput();
    else
      if (!inparms.read_parm_file(argv[1]))
      {
        std::cerr << "Unable to read the configuration file." << std::endl;
        return 1;
      }

    std::cerr << inparms.parameterfile << std::endl;
    
    //generate the output projection based on the flat file
    outproj = SetProjection(inparms.parameterfile);
    if (!outproj)
    {
      std::cerr << "Could not create the output projection." << std::endl;
      return 1;
    }

    inparms.numPartitions = 0;
    projJob->setOutputFileName(inparms.outfile_name);
    projJob->setOutputProjection(outproj);
    projJob->setPmeshName(inparms.pmeshname);
    projJob->setPmeshSize(inparms.pmeshsize);
    if (!inparms.samescale)
      projJob->setOutputScale(inparms.newscale);
    else
      projJob->setSameScale(true);
    projJob->setInputFile(inparms.filename);

    //done with the output projection
    delete outproj;
    outproj = NULL;
  }
  catch(ProjectorException & pe)
  {
    std::cerr << pe.getExceptionMessage() << std::endl;
    delete projJob;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  catch(USGSImageLib::ImageException & ie)
  {
    std::string error;
    ie.getString(error);
    std::cerr << error << std::endl;
    delete projJob;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  catch(...)
  {
    std::cout << "Other Exception Caught in mastermain.cpp" << std::endl;
    delete projJob;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  
  ///////////////////////////
  
  WorkManager * workManager = new WorkManager(projJob);
  std::cerr<< "Work manager has been created." << std::endl;

  SlaveManager * slaveManager = new SlaveManager(workManager);
  slaveManager->Work();

  BigResult *projResult = workManager->getBigResult();
  if(projResult != NULL)
    std::cerr << "Results returned." << std::endl;
  else
    std::cerr << "Return of results failed." << std::endl;

  std::cerr << "Workmanager exiting." << std::endl;

  if(workManager != NULL)
    delete workManager;
  if(projJob != NULL)
    delete projJob;

  return 0;
}

