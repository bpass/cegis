
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
  int workunits = 15;

// to keep this process on the machine it started... 
// (because of input/output)
#ifdef _MOSIX
  msx_lock();
#endif
  
  BigJob *thetask = new BigJob(true);
  thetask->setnumworkunits((workunitid_t)workunits);


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
        std::cout << "Unable to read the configuration file" << std::endl;
        return 1;
      }

    std::cout << inparms.parameterfile << std::endl;
    
    //generate the output projection based on the flat file
    outproj = SetProjection(inparms.parameterfile);
    if (!outproj)
    {
      std::cout << "Could not create the output projection!" << std::endl;
      return 1;
    }

    inparms.numPartitions = 0;
    std::cout << "outfile_name is " << inparms.outfile_name << std::endl;
    thetask->setOutputFileName(inparms.outfile_name);
    thetask->setOutputProjection(outproj);
    thetask->setPmeshName(inparms.pmeshname);
    thetask->setPmeshSize(inparms.pmeshsize);
    if (!inparms.samescale)
      thetask->setOutputScale(inparms.newscale);
    else
      thetask->setSameScale(true);
    thetask->setInputFile(inparms.filename);

    //thetask->project(&progress);

    //done with the output projection
    delete outproj;
    outproj = NULL;
  }
  catch(ProjectorException & pe)
  {
    std::cout << pe.getExceptionMessage() << std::endl;
    delete thetask;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  catch(USGSImageLib::ImageException & ie)
  {
    std::string error;
    ie.getString(error);
    std::cout << error << std::endl;
    delete thetask;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  catch(...)
  {
    std::cout << "!ERROR!" << std::endl;
    delete thetask;
    delete outproj;
    outproj = NULL;
    return 0;
  }
  
  ///////////////////////////
  
  WorkManager * foreman = new WorkManager(thetask);
  printf("work manager has been created\n");

  SlaveManager * drumman = new SlaveManager(foreman);
  drumman->Work();

  BigResult *donut = foreman->getBigResult();
  if(donut != NULL)
    printf("results are back!\n");
  else
    printf("uh, didn't get results for some reason\n");


  printf("killing workmanager\n");
  fflush(stdout);
  if(foreman != NULL)
    delete foreman;
  if(thetask != NULL)
    delete thetask;

  return 0;
}

////////////////////////////////////////////////////////////////

