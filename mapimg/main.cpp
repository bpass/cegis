// $Id: main.cpp,v 1.1 2005/01/14 16:18:49 rbuehler Exp $


#include <qapplication.h>
#include "mapimgform.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    mapimgForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
