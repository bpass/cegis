// $Id: main.cpp,v 1.2 2005/01/27 18:56:14 jtrent Exp $


#include <qapplication.h>
#include "mapimgform.h"

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
