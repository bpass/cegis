// $Id: logform.cpp,v 1.5 2005/09/28 20:24:28 lwoodard Exp $

//Edited by:lwoodard	date:August 2005	for:qt3 to qt4 porting

/****************************************************************************
** Form implementation generated from reading ui file 'logForm.ui'
**
** Created: Fri Jan 14 15:46:01 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "logform.h"
#include "getprojinfo.h"

#include <QVariant>
#include <QPushButton>
#include <QLayout>
#include <QTooltip>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <qvariant.h>
#include <qpushbutton.h>
#include <QTextEdit>
#include <QWhatsThis>
#include <QFileDialog>

//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>

/*
*  Constructs a logForm as a child of 'parent', with the
*  name 'name' and widget flags set to 'f'.
*
*  The dialog will by default be modeless, unless you set 'modal' to
*  TRUE to construct a modal dialog.
*/
logForm::logForm( QWidget* parent, bool modal, Qt::WFlags fl )
: QDialog( parent, fl )
{
   setModal( modal );
	setObjectName( "logForm" );

   //setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
   setSizePolicy( QSizePolicy::Preferred , QSizePolicy::Preferred );
   setMinimumSize( QSize( 300, 300 ) );
   setMaximumSize( QSize( 600, 600 ) );
   setBaseSize( QSize( 400, 375 ) );
   logFormLayout = new QGridLayout( this ); //, 1, 1, 11, 6, "logFormLayout"); 

   layout3 = new QHBoxLayout( 0 ); 
   layout3->setSpacing( 6 );	
   spacer1 = new QSpacerItem( 210, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
   layout3->addItem( spacer1 );

   saveButton = new QPushButton( this );
   layout3->addWidget( saveButton );

   okButton = new QPushButton( this );
   layout3->addWidget( okButton );

   logFormLayout->addLayout( layout3, 1, 0 );

   logViewer = new QTextEdit( this );

   logFormLayout->addWidget( logViewer, 0, 0 );
   languageChange();
   resize( QSize(361, 365).expandedTo(minimumSizeHint()) );

   // signals and slots connections
   connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
   connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveLog() ) );

   // tab order
   setTabOrder( okButton, saveButton );

   if( QFile::exists( logFile ) )
      loadLog();
   else 
      QMessageBox::warning( this, "Error", "MapIMG Log File Not Found." );
}

/*
*  Destroys the object and frees any allocated resources
*/
logForm::~logForm()
{
   // no need to delete child widgets, Qt does it all for us
}

/*
*  Sets the strings of the subwidgets using the current
*  language.
*/
void logForm::languageChange()
{
	setWindowTitle( tr( "MapIMG Log..." ) );
//   setIconText( QString::null );
   saveButton->setText( tr( "&Save..." ) );
   okButton->setText( tr( "&OK" ) );
}

//save a copy of the log to a file
void logForm::saveLog()
{
   // get the output file to mapimg
   QString fileTypes = "MapIMG Log files (*.txt);;All Files (*.*)";

   QString s = QFileDialog::getSaveFileName(
	   this, "Choose a filename to save under", "" , fileTypes );

   int answer = -1;
   if( s.isNull() ) return;   //cancel pressed

   //add default extension if none is provided
   if( !s.contains( '.' ) ) s.append( ".txt" );

   // if output file exists, ask about overwriting
   if( !s.isNull()  &&  QFile::exists(s) )
   {
      answer = QMessageBox::warning(
         this, "MapIMG -- Overwrite Log File",
         QString( "Overwrite -- Process is Not Reversable\n\'%1\'?" ).
         arg( s ), "&Yes", "&No", QString::null, 1, 1 );
   }

   // if yes, remove generic binary and .info files to prepare for overwrite
   if(answer == 0)
	   remove(s.toAscii());

   // if no overwrite needed
   else if(answer == -1)
   {}

   // otherwise, reset the path
   else
      s = "";

   //if s is defined and not empty
   //save the log
   if( !s.isNull() && s != "")
   {
      QFile logFileOutput( s );
      if ( !logFileOutput.open( QIODevice::WriteOnly ) )
      {
         //error opening the file
         QMessageBox::critical( 0, "MapIMG",
            QString("An internal error occurred while trying to open the designated output file\n\nLog could not be saved."));

         return;
      }

      QTextStream ts( &logFileOutput );
      ts << logViewer->toPlainText();

      logFileOutput.close();
   }

   if( QFile::exists( logFile ) )
      QFile::remove( logFile );

   return;
}


void logForm::refreshLog()
{
   logViewer->repaint();
}


void logForm::loadLog()
{
   QString fullLog = ""; 

   const long MAX_LINE_LENGTH = 500;  //maximum number of characters to read at one time

   if( QFile::exists( logFile ) )
   {
      logViewer->setPlainText( "" );
      logViewer->setReadOnly( true );

      QFile log( logFile );
      log.open( QIODevice::ReadOnly );

      while( !log.atEnd() )
      {
			//if there was not an error reading append
		  if(log.isReadable())
			  fullLog += log.readLine( MAX_LINE_LENGTH );
      }

      logViewer->setPlainText( fullLog );
      log.close();
   }
   return;
}

