// WorkUnit by Matt Zykan

#include "WorkUnit.h"

WorkUnit::WorkUnit(workunitid_t newid) : Projector()
{
  id = newid;
  workcomplete = false;
  scanlines = NULL;
  scanlinesallocated = 0;
}

WorkUnit::WorkUnit() : Projector()
{
  id = 0;
  workcomplete = false;
  scanlines = NULL;
  scanlinesallocated = 0;
}

//////////////////////////////////////////////////

WorkUnit::~WorkUnit()
{
  clearscanlines();
}

//////////////////////////////////////////////////

workunitid_t WorkUnit::getid()
{
  return id;
}

//////////////////////////////////////////////////

bool WorkUnit::done()
{
  return workcomplete;
}

//////////////////////////////////////////////////

void WorkUnit::reloaded()
{
  workcomplete = false;
}

//////////////////////////////////////////////////

unsigned char ** WorkUnit::getscanlines()
{
  return scanlines;
}

long int WorkUnit::getbasescanline()
{
  return basescanline;
}

long int WorkUnit::getscanlinecount()
{
  return scanlinecount;
}

//////////////////////////////////////////////////

void WorkUnit::setid(workunitid_t newid)
{
  id = newid;
}

void WorkUnit::setoutRect(DRect newoutRect)
{
  outRect = newoutRect;
}

void WorkUnit::setrange(long int newbasescanline, long int newscanlinecount)
{
  basescanline = newbasescanline;
  scanlinecount = newscanlinecount;
}

void WorkUnit::setinputfilename(std::string & newinputfilename)
{
  inputfilename = newinputfilename;
}

//////////////////////////////////////////////////

void WorkUnit::Execute()
{
  if(workcomplete)
    return;

  try
  {
    if(toprojection != NULL)
      delete toprojection;
    toprojection = SetProjection(Params);
    setupInput(inputfilename);
    dogetExtents();
  }
  catch(ProjectorException & pe)
  {
    std::cout << pe.getExceptionMessage() << std::endl;
    return;
  }
  catch(USGSImageLib::ImageException & ie)
  {
    std::string error;
    ie.getString(error);
    std::cout << error << std::endl;
    return;
  }
  catch(...)
  {
    std::cout << "!ERROR!" << std::endl;
    return;
  }

  reallocatescanlines();
  double xscaleinv = 1.0 / oldscale.x;
  double yscaleinv = 1.0 / oldscale.y;
  double x, y;
  int _x, _y;
  unsigned char * scanline = NULL;
  const unsigned char * inscanline = NULL;
  PmeshLib::ProjectionMesh * pmesh;
  long int xcounter, ycounter;
  int sppcounter;

  try
  {
    pmesh = setupReversePmesh();
    //printf("{%ld", basescanline); fflush(stdout);
    for(ycounter = basescanline;
        ycounter < basescanline + scanlinecount;
        ++ycounter)
    {
      //printf("."); fflush(stdout);
      scanline = scanlines[ycounter - basescanline];
      for(xcounter = 0; xcounter < newwidth; ++xcounter)
      {
        x = outRect.left + newscale.x * xcounter;
        y = outRect.top - newscale.y * ycounter;

        if(pmesh)
          pmesh->projectPoint(x, y);
        else
        {
          toprojection->projectToGeo(x, y, y, x);
          fromprojection->projectFromGeo(y, x, x, y);
        }
        _x = static_cast<long int>((x - inRect.left) * (xscaleinv) + 0.5);
        _y = static_cast<long int>((inRect.top - y) * (yscaleinv) + 0.5);
        if((_x >= oldwidth) || (_x < 0) || (_y >= oldheight) || (_y < 0))
          for(sppcounter = 0; sppcounter < spp; ++sppcounter)
            scanline[xcounter * spp + sppcounter] = 0;
        else
        {
          inscanline = cache->getRawScanline(_y);
          for(sppcounter = 0; sppcounter < spp; ++sppcounter)
            scanline[xcounter * spp + sppcounter] = 
              inscanline[_x * spp + sppcounter];
        }
      }
    }
  }
  catch(ProjectorException & pe)
  {
    std::cout << pe.getExceptionMessage() << std::endl;
    return;
  }
  catch(USGSImageLib::ImageException & ie)
  {
    std::string error;
    ie.getString(error);
    std::cout << error << std::endl;
    return;
  }
  catch(...)
  {
    std::cout << "!ERROR!" << std::endl;
    return;
  }

  //printf("%ld}", basescanline + scanlinecount - 1); fflush(stdout);
  if(pmesh)
  {
    delete pmesh;
    pmesh = NULL;
  }

  // close any open files, generally clean up everything
  if(cache != NULL)
  {
    delete cache; // is this even used...?
    cache = NULL;
  }
  if(infile != NULL)
  {
    delete infile;
    infile = NULL;
  }

  workcomplete = true;
}

//////////////////////////////////////////////////

bool WorkUnit::exportToSocket(int sock)
{
  Pulsar outbox(sock, MTUGOO);

  outbox.feed(&id, sizeof(id));
  outbox.feed(&workcomplete, sizeof(workcomplete));

  outbox.feed(&basescanline, sizeof(basescanline));
  outbox.feed(&scanlinecount, sizeof(scanlinecount));

  unsigned int len = infilename.length();
  outbox.feed(&len, sizeof(len));
  char * filenametemp = new char[len + 1];
  strncpy(filenametemp, infilename.c_str(), len);
  filenametemp[len] = '\0';
  outbox.feed(filenametemp, len);

  outbox.feed(&newheight, sizeof(newheight));
  outbox.feed(&newwidth, sizeof(newwidth));
  outbox.feed(&spp, sizeof(spp));
  outbox.feed(&samescale, sizeof(samescale));
  outbox.feed(&newscale, sizeof(newscale));
  outbox.feed(&outRect, sizeof(outRect));
  outbox.feed(&pmeshsize, sizeof(pmeshsize));
  outbox.feed(&pmeshname, sizeof(pmeshname));

  // send output projection
  // the idea here is to capitalize on the assumption that ProjectionParams
  //  is a static hunk of data, if this doesn't work, well, iunno
  // maybe there will be trouble with enumerations...? meh, doubt it
  outbox.feed(&Params, sizeof(Params));

  bool sendscanlines = workcomplete && newwidth * spp > 0 && scanlines != NULL;
  outbox.feed(&sendscanlines, sizeof(sendscanlines));
  if(sendscanlines)
  {
    unsigned char * blankscanline = new unsigned char[newwidth * spp];
    for(long int i = 0; i < scanlinecount; ++i)
      if(scanlines[i] != NULL)
        outbox.feed(scanlines[i], newwidth * spp);
      else
        outbox.feed(blankscanline, newwidth * spp);
    delete[] blankscanline;
  }

  // ha ha check out this next part
  // what the hell is going on around here WHO ARE YOU PEOPLE
  if(!outbox.flush())
    return false;
  return true;
}

//////////////////////////////////////////////////

bool WorkUnit::importFromSocket(int sock)
{
  Pulsar inbox(sock, 0);

  inbox.get(&id, sizeof(id));
  inbox.get(&workcomplete, sizeof(workcomplete));

  inbox.get(&basescanline, sizeof(basescanline));
  inbox.get(&scanlinecount, sizeof(scanlinecount));

  unsigned int len;
  inbox.get(&len, sizeof(len));
  char * filenametemp = new char[len + 1];
  inbox.get(filenametemp, len);
  filenametemp[len] = '\0';
  inputfilename = filenametemp;
  delete[] filenametemp;

  inbox.get(&newheight, sizeof(newheight));
  inbox.get(&newwidth, sizeof(newwidth));
  inbox.get(&spp, sizeof(spp));
  inbox.get(&samescale, sizeof(samescale));
  inbox.get(&newscale, sizeof(newscale));
  inbox.get(&outRect, sizeof(outRect));
  inbox.get(&pmeshsize, sizeof(pmeshsize));
  inbox.get(&pmeshname, sizeof(pmeshname));

  ProjectionParams newparams;
  inbox.get(&newparams, sizeof(newparams));
  setOutputProjection(newparams);

  bool receivescanlines = false;
  inbox.get(&receivescanlines, sizeof(receivescanlines));
  if(receivescanlines)
  {
    reallocatescanlines();
    for(long int i = 0; i < scanlinecount; ++i)
      inbox.get(scanlines[i], newwidth * spp);
    //printf("{got %ld}", scanlinecount); fflush(stdout);
  }

  return true;
}

//////////////////////////////////////////////////

void WorkUnit::clearscanlines()
{
  if(scanlines != NULL)
  {
    for(long int i = 0; i < scanlinesallocated; ++i)
      if(scanlines[i] != NULL)
      {
        delete[] scanlines[i];
        // scanlines[i] = NULL; extraneous
      }
    delete[] scanlines;
    scanlines = NULL;
  }
  scanlinesallocated = 0;
}

//***************************************************************
// private

void WorkUnit::reallocatescanlines()
{
  if(scanlinecount > scanlinesallocated)
  {
    clearscanlines();
    long int i;
    try
    {
      scanlines = new unsigned char * [scanlinecount];
      for(i = 0; i < scanlinecount; ++i)
        scanlines[i] = new unsigned char[newwidth * spp];
    }
    catch(...)
    {
      printf("cannot allocate scanline %ld of %ld\n", i, scanlinecount);
      clearscanlines();
      exit(1);
    }
    scanlinesallocated = scanlinecount;
  }
}

