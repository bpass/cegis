// part of MSXProject by Matt Zykan

#include "BigJob.h"

BigJob::BigJob() : Projector(), numworkunits(2), usethread(false)
{
}

BigJob::~BigJob()
{
}

//////////////////////////////////////////////////

void BigJob::setnumworkunits(workunitid_t newnumworkunits)
{
  numworkunits = newnumworkunits;
}

void BigJob::setusethread(bool newusethread)
{
  usethread = newusethread;
}

//////////////////////////////////////////////////

void BigJob::insertscanline(void * scanline, long row)
{
  /*FILE * dump = fopen("scanlines", "a");
  fprintf(dump, "%4ld:", row);
  for(long i = 0; i < newwidth*spp; ++i)
    fprintf(dump, "%02x", ((char*)scanline)[i]);
  fprintf(dump, "\n");
  fclose(dump);*/
  if(usethread)
  {
    /*
    StitcherNode * temp = new StitcherNode(scanline, size, row);
    TODO feed temp node to stitcher, which should destroy it at its leisure
    */
  }
  else
  {
    if(out == NULL)
      setupOutput(outfile);
    out->putRawScanline(row, scanline);
  }
}

//////////////////////////////////////////////////

workunitid_t BigJob::getnumworkunits()
{
  return numworkunits;
}

