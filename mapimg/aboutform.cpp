// $Id: aboutform.cpp,v 1.2 2005/01/31 03:09:04 rbuehler Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'aboutform.ui'
**
** Created: Thu Jan 6 17:24:09 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "aboutform.h"

#include "mapimgversion.h"
#include "mapimgimages.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a abourForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
aboutForm::aboutForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
   if ( !name )
	   setName( "aboutForm" );

   setMinimumSize( QSize( 407, 314 ) );
   setMaximumSize( QSize( 407, 314 ) );
   setPalette( QColor( 180, 173, 165 ) );

   aboutFormLayout = new QVBoxLayout( this, 4, 3, "aboutFormLayout"); 

   logoLabel = new QLabel( this, "logoLabel" );
   logoLabel->setMaximumSize( QSize( 400, 147 ) );
   logoLabel->setFrameShape( QLabel::WinPanel );
   logoLabel->setFrameShadow( QLabel::Sunken );
   logoLabel->setPixmap( mapimgImage( "USGS Logo" ) );
   logoLabel->setScaledContents( TRUE );
   aboutFormLayout->addWidget( logoLabel );

   titleLabel = new QLabel( this, "titleLabel" );
   titleLabel->setPaletteForegroundColor( QColor( 0, 102, 51 ) );
   QFont titleLabel_font(  titleLabel->font() );
   titleLabel_font.setFamily( "Arial" );
   titleLabel_font.setPointSize( 22 );
   titleLabel_font.setBold( TRUE );
   titleLabel->setFont( titleLabel_font ); 
   titleLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
   aboutFormLayout->addWidget( titleLabel );

   discFrame = new QFrame( this, "discFrame" );
   discFrame->setFrameShape( QFrame::WinPanel );
   discFrame->setFrameShadow( QFrame::Sunken );
   discFrameLayout = new QVBoxLayout( discFrame, 4, 0, "discFrameLayout"); 

   discLabel1 = new QLabel( discFrame, "discLabel1" );
   discLabel1->setPaletteForegroundColor( QColor( 0, 102, 51 ) );
   QFont discLabel1_font(  discLabel1->font() );
   discLabel1_font.setFamily( "Arial" );
   discLabel1_font.setPointSize( 10 );
   discLabel1->setFont( discLabel1_font ); 
   discLabel1->setTextFormat( QLabel::RichText );
   discLabel1->setAlignment( int( QLabel::WordBreak | QLabel::AlignJustify | QLabel::AlignTop ) );
   discFrameLayout->addWidget( discLabel1 );

   discLabel2 = new QLabel( discFrame, "discLabel2" );
   discLabel2->setPaletteForegroundColor( QColor( 0, 102, 51 ) );
   QFont discLabel2_font(  discLabel2->font() );
   discLabel2_font.setFamily( "Arial" );
   discLabel2_font.setPointSize( 12 );
   discLabel2->setFont( discLabel2_font ); 
   discLabel2->setTextFormat( QLabel::RichText );
   discLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignJustify | QLabel::AlignTop ) );
   discFrameLayout->addWidget( discLabel2 );
   aboutFormLayout->addWidget( discFrame );

   languageChange();
   //resize( QSize(407, 314).expandedTo(minimumSizeHint()) );
   //clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
aboutForm::~aboutForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void aboutForm::languageChange()
{
    setCaption( tr( "About mapimg2" ) );
    titleLabel->setText( QString("mapimg v%1.%2.%3").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_NUM) );
    discLabel1->setText( tr( "MapImage is a research-oriented project. Its source and binaries are provided with no warranty or support. <b>Use at your own risk.</b> We will, however, provide minimal technical support as time allows. We are also very interested in feedback as well as bug reports. All questions and comments may be sent to:" ) );
    discLabel2->setText( tr( "<p align=\"center\">mapimg@usgs.gov</p>" ) );
}

void aboutForm::mousePressEvent(QMouseEvent *e)
{
   QWidget::mousePressEvent(e);

   accept();
}
