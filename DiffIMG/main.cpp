#include <qapplication.h>

#include "frmloadimages.h"
#include "frmdifference.h"
#include "frmdimensions.h"


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    FrmLoadImages *main = new FrmLoadImages();

    a.setMainWidget( main );
    main->show();

    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    return a.exec();
}
