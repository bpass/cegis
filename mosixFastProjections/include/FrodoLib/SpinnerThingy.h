#ifndef _SPINNERTHINGY_H_
#define _SPINNERTHINGY_H_


class SpinnerThingy
{
    char updt[4];
    int status;
    int lastMsgLength;

  public:

    SpinnerThingy();
    void done(const char *);
    void update(int);
    void update(long);
    void update(double);
    void update(const char* msg);
};

#endif
