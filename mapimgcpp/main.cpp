// $Id: main.cpp,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


#include <qapplication.h>
#include "mapimgform.h"

/*
The main function for mapimg.
It first checks for any arguments. Then it simply loads up the GUI and waits
for it to exit.
*/
int main( int argc, char ** argv )
{
   QApplication a( argc, argv );
   mapimgForm w;
   w.show();

   /* Load a file requested at command-line or through file association
   Currently, this only works when argv[1] is the .img file.
   It does not work if it is the .img.info or .xml file  */

   if( argc == 2 )
      w.openFile( QString(argv[1]) );

   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}
