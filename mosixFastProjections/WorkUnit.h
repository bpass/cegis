// WorkUnit by Matt Zykan
/* defines an atomic unit of work
 *
 * note about export/import
 * the import/export process is not a 100% copy, but it's close
 * a workunit will probably break if you attempt to import from a workunit
 *  that belongs to a different job, but, eh, maybe not
 * it's not intended to do that, so don't
 */

#ifndef WORKUNIT_H_
#define WORKUNIT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include "Pulsar.h"
#include "Projector.h"
using namespace std;

#define MTUGOO 1400 // try to send about this many bytes at a time when
                    //  exporting to socket
                    // TODO figure out exactly how large this can be
                    //  (typical MTU 1500 minus size of various headers)
                    //  TCP - 24 bytes
                    //  IP - 24 bytes
                    //  ethernet - god only knows. does it even count?

typedef unsigned long workunitid_t;

class WorkUnit : public Projector
{
  public:
    WorkUnit(workunitid_t newid);
    WorkUnit(); // id will be 0
    virtual ~WorkUnit();

    workunitid_t getid();
    bool done();
    void reloaded(); // sets workcomplete to false
  
    unsigned char ** getscanlines();
    long int getbasescanline();
    long int getscanlinecount();
    
    void setid(workunitid_t newid);
    void setoutRect(DRect newoutRect);
    void setrange(long int newbasescanline, long int newscanlinecount);
    void setinputfilename(std::string & newinputfilename);
    
    // do what needs doin'
    void Execute();

    // dumps self to socket sock
    // expects importFromSocket on the other end
    // returns true on success
    // if this fails, retrying is unlikely to help, something is broken
    bool exportToSocket(int sock);

    // recieves contents from exportToSocket on other end of sock
    // returns true on success
    // if this fails, the WorkUnit will very likely be corrupted
    bool importFromSocket(int sock);
  
    // systematically and safely dismantles any allocated scanlines
    void clearscanlines();
    
  protected:
    // checks that there is enough memory allocated in **scanlines to hold the
    //  results of work, if there is, the function does nothing, if there is
    //  not, it reallocates the array
    // does not reallocate if **scanlines is too big
    void reallocatescanlines();
    
    bool workcomplete;
    workunitid_t id;

    static workunitid_t instanceCount;  // TODO: for assigning id number 
    std::string inputfilename;
    long int basescanline;
    long int scanlinecount;
    unsigned char ** scanlines;
    long int scanlinesallocated;
};

#endif

