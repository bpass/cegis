#include "SpinnerThingy.h"
#include <iostream.h>

void main(void)
{
   int i;
   SpinnerThingy it;

   cout << "Testing..." << endl;
   for (i=0;i<10000000;i++)
   {
       if ((i % 200000)==0)
          it.update(i);
   }
   it.update("foo");
   for (i=0;i<600000;i++);
   it.done("Done.");
}
