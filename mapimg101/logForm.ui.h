/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qtextstream.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "qembed_images.h"

#include "getprojinfo.h"

//save a copy of the log to a file
void logForm::saveLog()
{
    // get the output file to MapIMG
    QString fileTypes = "MapIMG Log files (*.txt);;All Files (*.*)";

    QString s = QFileDialog::getSaveFileName(
                    "",
                    fileTypes,
                    this,
                    "save log dialog"
                    "Choose a filename to save under" );

    int answer = -1;
    if( s == NULL ) return;   //cancel pressed

    //add default extension if none is provided
     if( (s.find( '.' ) == -1 ) ) s.append( ".txt" );

    // if output file exists, ask about overwriting
    if( s  &&  QFile::exists(s) )
    {
                answer = QMessageBox::warning(
                               this, "MapIMG -- Overwrite Log File",
                                QString( "Overwrite -- Process is Not Reversable\n\'%1\'?" ).
                                    arg( s ),
                                "&Yes", "&No", QString::null, 1, 1 );
    }

    // if yes, remove generic binary and .info files to prepare for overwrite
    if(answer == 0)
    {
		remove(s);
    }
    // if no overwrite needed
    else if(answer == -1)
    {
    }

    // otherwise, reset the path
    else
    {
    	        s = "";
    }

    //if s is defined and not empty
    //save the log
    if( s && s != "")
    {
        QFile logFileOutput( s );
	if ( !logFileOutput.open( IO_WriteOnly ) )
	{
             //error opening the file
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated output file\n\nLog could not be saved."));


	    return;
	}

	QTextStream ts( &logFileOutput );
	ts << logViewer->text();

        logFileOutput.close();
    }

    if( QFile::exists( logFile ) )
    {
    	QFile::remove( logFile );
    }


    return;
}



//things to do when created
void logForm::init()
{
   this->setIcon( qembed_findImage( "usgs" ) );

   if( QFile::exists( logFile ) )
   {
       loadLog();
   }
   else
   {
       QMessageBox::warning( this, "Error", "MapIMG Log File Not Found." );
   }

   return;
}



void logForm::refreshLog()
{
       logViewer->repaint(true);
}


void logForm::loadLog()
{
   QString fullLog = "", tempLine = "";
   const Q_LONG MAX_LINE_LENGTH = 500;  //maximum number of characters to read at one time

   if( QFile::exists( logFile ) )
   {
       logViewer->setText( "" );
       logViewer->setReadOnly( true );

       QFile log( logFile );
       log.open( IO_ReadOnly );

       while( !log.atEnd() )
       {
           if( log.readLine( tempLine, MAX_LINE_LENGTH ) != -1 )    //if there was not an error reading append
       	       fullLog += tempLine;
       }

       logViewer->setText( fullLog );
       log.close();
   }
   return;
}
