// $Id: aboutform.cpp,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'aboutform.ui'
**
** Created: Thu Jan 6 17:24:09 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "aboutform.h"

#include "mapimgversion.h"
#include "mapimgimages.h"
#include "mapimgpalette.h"

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
*  Constructs an aboutForm as a child of 'parent', with the
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

   // Load in the preset colors form mapimgpalette.h
   setPalette( ABOUTFORM_COLOR );
   setPaletteForegroundColor( ABOUTTEXT_COLOR );

   aboutFormLayout = new QVBoxLayout( this, 4, 3, "aboutFormLayout");

   logoLabel = new QLabel( this, "logoLabel" );
   logoLabel->setMaximumSize( QSize( 400, 147 ) );
   logoLabel->setFrameShape( QLabel::WinPanel );
   logoLabel->setFrameShadow( QLabel::Sunken );
   logoLabel->setPixmap( mapimgImage( "USGS Logo" ) );
   logoLabel->setScaledContents( TRUE );
   aboutFormLayout->addWidget( logoLabel );

   titleLabel = new QLabel( this, "titleLabel" );
   QFont titleLabel_font(  titleLabel->font() );
   titleLabel_font.setFamily( "Arial" );
   titleLabel_font.setPointSize( 22 );
   titleLabel_font.setBold( TRUE );
   titleLabel->setFont( titleLabel_font ); 
   titleLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
   aboutFormLayout->addWidget( titleLabel );

   descFrame = new QFrame( this, "descFrame" );
   descFrame->setFrameShape( QFrame::WinPanel );
   descFrame->setFrameShadow( QFrame::Sunken );
   descFrameLayout = new QVBoxLayout( descFrame, 4, 0, "discFrameLayout"); 

   descLabel1 = new QLabel( descFrame, "descLabel1" );
   QFont descLabel1_font(  descLabel1->font() );
   descLabel1_font.setFamily( "Arial" );
   descLabel1_font.setPointSize( 10 );
   descLabel1->setFont( descLabel1_font ); 
   descLabel1->setTextFormat( QLabel::RichText );
   descLabel1->setAlignment( int( QLabel::WordBreak | QLabel::AlignJustify | QLabel::AlignTop ) );
   descFrameLayout->addWidget( descLabel1 );

   descLabel2 = new QLabel( descFrame, "descLabel2" );
   QFont descLabel2_font(  descLabel2->font() );
   descLabel2_font.setFamily( "Arial" );
   descLabel2_font.setPointSize( 12 );
   descLabel2->setFont( descLabel2_font ); 
   descLabel2->setTextFormat( QLabel::RichText );
   descLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignJustify | QLabel::AlignTop ) );
   descFrameLayout->addWidget( descLabel2 );
   aboutFormLayout->addWidget( descFrame );

   descLabel3 = new QLabel( descFrame, "descLabel3" );
   descLabel3->setFont( descLabel1_font ); 
   descLabel3->setTextFormat( QLabel::RichText );
   descLabel3->setAlignment( int( QLabel::WordBreak | QLabel::AlignJustify | QLabel::AlignTop ) );
   descFrameLayout->addWidget( descLabel3 );

   okLayout = new QHBoxLayout( this, 4, 3, "okLayout" );
   okLayout->addItem( new QSpacerItem( 30, 10 ) );
   okButton = new QPushButton( this, "okButton" );
   okButton->setMaximumWidth( 80 );
   okLayout->addWidget( okButton );

   aboutFormLayout->addItem( okLayout );

   connect( okButton, SIGNAL( clicked() ), this, SLOT( close() ) );

   languageChange();
}

/*
*  Sets the strings of the subwidgets using the current
*  language.
*/
void aboutForm::languageChange()
{
   setCaption( tr( "About mapimg2" ) );
   titleLabel->setText( QString("mapimg v%1.%2.%3").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_NUM) );
   descLabel1->setText( tr( "MapImage is a research-oriented project. Its source and binaries"
      " are provided with no warranty or support. <b>Use at your own risk.</b>"
      " We will, however, provide minimal technical support as time allows."
      " We are also very interested in feedback as well as bug reports. All"
      " questions and comments may be sent to:" ) );
   descLabel2->setText( tr( "<p align=\"center\">mapimg@usgs.gov</p>" ) );
   descLabel3->setText( tr( "Libraries used: Qt and TinyXML." ) );
   okButton->setText( tr( "Ok" ) );
}
