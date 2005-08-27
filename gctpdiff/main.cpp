// $Id: main.cpp,v 1.1 2005/08/27 00:20:32 rbuehler Exp $

#include <qapplication.h>
#include "gctpdiffform.h"

int main( int argc, char ** argv )
{
   QApplication a( argc, argv );
   GctpDiffForm d;
   d.show();

   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
