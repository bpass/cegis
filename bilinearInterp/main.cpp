#include <qapplication.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include "InterpDlg.h"
int main( int argc, char ** argv )
{
	
    QApplication a( argc, argv );
	
	InterpDlg dlg;
	dlg.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	return a.exec();
}
