// part of MSXProject by Matt Zykan

#ifndef BIGJOB_H_
#define BIGJOB_H_

#include "Projector.h"
#include "WorkUnit.h"
#include "Stitcher.h"
#include "StitcherNode.h"

class BigJob : public Projector
{
  public:
    BigJob();
    virtual ~BigJob();

    void setnumworkunits(workunitid_t newnumworkunits);
    void setusethread(bool newusethread);

    void insertscanline(void * scanline, long row);

    workunitid_t getnumworkunits();

  protected:
    workunitid_t numworkunits;
    bool usethread;
};

class BigResult
{
};

#endif

