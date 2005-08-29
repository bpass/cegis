// $Id: main.cpp,v 1.2 2005/08/29 22:13:47 rbuehler Exp $

#include <qapplication.h>
#include "gctpdiffform.h"

int main( int argc, char ** argv )
{
   QApplication a( argc, argv );
   GctpDiffForm diffForm;
   diffForm.show();
   return a.exec();
}
