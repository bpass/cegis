#include "FrodoLib/SpinnerThingy.h"
#include <iostream.h>
#include <stdio.h>
#include <string.h>


SpinnerThingy::SpinnerThingy()
{
   updt[0] = '|';
   updt[1] = '/';
   updt[2] = '-';
   updt[3] = '\\';
   status = 0;
   lastMsgLength = 0;
}

void SpinnerThingy::update(int i)
{
   char foo[80];
   int j,l;
   sprintf(foo,"%c %d",updt[status],i);
   l = lastMsgLength - strlen(foo); 
   lastMsgLength = strlen(foo);
   for (j=0;j<l;j++)
      strcat(foo," ");
   cout << foo << "\r" << flush;
   status++;
   status = status % 4;
}

void SpinnerThingy::update(long i)
{
   char foo[80];
   int j,l;
   sprintf(foo,"%c %d",updt[status],i);
   l = lastMsgLength - strlen(foo);
   lastMsgLength = strlen(foo);
   for (j=0;j<l;j++)
      strcat(foo," ");
   cout << foo << "\r" << flush;
   status++;
   status = status % 4;
}

void SpinnerThingy::update(double d)
{
   char foo[80];
   int j,l;
   sprintf(foo,"%c %lf",updt[status],d);
   l = lastMsgLength - strlen(foo); 
   lastMsgLength = strlen(foo);
   for (j=0;j<l;j++)
      strcat(foo," ");
   cout << foo << "\r" << flush;
   status++;
   status = status % 4;
}

void SpinnerThingy::update(const char* msg)
{
   char foo[80];
   int j,l;
   sprintf(foo,"%c %s",updt[status],msg);
   l = lastMsgLength - strlen(foo); 
   lastMsgLength = strlen(foo);
   for (j=0;j<l;j++)
      strcat(foo," ");
   cout << foo << "\r" << flush;
   status++;
   status = status % 4;
}

void SpinnerThingy::done(const char* msg)
{
   char foo[80]; 
   int j,l;
   sprintf(foo,"%s",msg);
   l = lastMsgLength - strlen(foo);
   for (j=0;j<l;j++)
       strcat(foo," ");
   cout << foo << endl;

   status = 0;
}
