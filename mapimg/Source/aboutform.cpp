// $Id: aboutform.cpp,v 1.7 2007/08/31 14:49:31 cvs Exp $

//Edited by:lwoodard	date:August 2005	for:qt3 to qt4 porting

/****************************************************************************
** Form implementation generated from reading ui file 'aboutform.ui'
**
** Created: Thu Jan 6 17:24:09 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
**
****************************************************************************/

#include "aboutform.h"

#include "mapimgversion.h"
#include "mapimgpalette.h"

#include <QVariant>
#include <QPushButton>
#include <QFrame>
#include <QLayout>
#include <QTooltip>
#include <QWhatsThis>
#include <QImage>
#include <QPixmap>

//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

/*
*  Constructs an aboutForm as a child of 'parent', with the
*  name 'name' and widget flags set to 'f'.
*
*  The dialog will by default be modeless, unless you set 'modal' to
*  TRUE to construct a modal dialog.
*/
aboutForm::aboutForm( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
: QDialog( parent, fl )
{
	setObjectName( name );
	setModal( modal );

	if ( objectName().isEmpty() )
		setObjectName( "aboutForm" );

	// Load in the preset colors form mapimgpalette.h
	setPalette( ABOUTFORM_COLOR );
	QPalette palette;
	palette.setColor(foregroundRole(), QColor(ABOUTTEXT_COLOR));
	setPalette(palette);

	aboutFormLayout = new QVBoxLayout( this );
	aboutFormLayout->setSpacing( 4 );

	logoLabel = new QLabel( this );
	logoLabel->setMaximumSize( QSize( 400, 147 ) );
	logoLabel->setFrameShape( QLabel::WinPanel );
	logoLabel->setFrameShadow( QLabel::Sunken );
	logoLabel->setPixmap( QPixmap( ":/Resources/USGS Logo.png" ) );
	logoLabel->setScaledContents( TRUE );
	aboutFormLayout->addWidget( logoLabel );

	titleLabel = new QLabel( this );
	QFont titleLabel_font(  titleLabel->font() );
	titleLabel_font.setFamily( "Arial" );
	titleLabel_font.setPointSize( 22 );
	titleLabel_font.setBold( TRUE );
	titleLabel->setFont( titleLabel_font ); 
	titleLabel->setAlignment( Qt::AlignCenter );
	aboutFormLayout->addWidget( titleLabel );

	descFrame = new QFrame( this );
	descFrame->setFrameShape( QFrame::WinPanel );
	descFrame->setFrameShadow( QFrame::Sunken );
	descFrameLayout = new QVBoxLayout( descFrame ); 
	descFrameLayout->setSpacing( 4 );
	aboutFormLayout->addWidget( descFrame );

	descLabel1 = new QLabel( descFrame );
	QFont descLabel1_font(  descLabel1->font() );
	descLabel1_font.setFamily( "Arial" );
	descLabel1_font.setPointSize( 10 );
	descLabel1->setFont( descLabel1_font ); 
	descLabel1->setTextFormat( Qt::RichText );
	descLabel1->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel1 );
		
	descLabel1_two = new QLabel( descFrame );
	descLabel1_two->setFont( descLabel1_font ); 
	descLabel1_two->setTextFormat( Qt::RichText );
	descLabel1_two->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel1_two );

	descLabel1_three = new QLabel( descFrame );
	descLabel1_three->setFont( descLabel1_font ); 
	descLabel1_three->setTextFormat( Qt::RichText );
	descLabel1_three->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel1_three );

	descLabel1_four = new QLabel( descFrame );
	descLabel1_four->setFont( descLabel1_font ); 
	descLabel1_four->setTextFormat( Qt::RichText );
	descLabel1_four->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel1_four );

	descLabel1_five = new QLabel( descFrame );
	descLabel1_five->setFont( descLabel1_font ); 
	descLabel1_five->setTextFormat( Qt::RichText );
	descLabel1_five->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel1_five );

	descLabel2 = new QLabel( descFrame );
	QFont descLabel2_font(  descLabel2->font() );
	descLabel2_font.setFamily( "Arial" );
	descLabel2_font.setPointSize( 12 );
	descLabel2->setFont( descLabel2_font ); 
	descLabel2->setTextFormat( Qt::RichText );
	descLabel2->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel2 );

	descLabel3 = new QLabel( descFrame );
	descLabel3->setFont( descLabel1_font ); 
	descLabel3->setTextFormat( Qt::RichText );
	descLabel3->setAlignment( Qt::AlignJustify | Qt::AlignTop );
	descFrameLayout->addWidget( descLabel3 );

	okLayout = new QHBoxLayout( 0 );
	okLayout->setSpacing( 4 );
	okLayout->addItem( new QSpacerItem( 30, 10 ) );
	okButton = new QPushButton( this );
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
	setWindowTitle( tr( "About MapIMG" ) );
	titleLabel->setText( QString("MapIMG v%1.%2.%3").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_NUM) );
	descLabel1->setText( tr( "MapIMG is a research-oriented project. Its source and binaries"   ));
		descLabel1_two->setText( tr(	" are provided with no warranty or support. <b>Use at your own risk.</b>"  ));
		descLabel1_three->setText( tr(	" We will, however, provide minimal technical support as time allows."  ));
		descLabel1_four->setText( tr(	" We are also very interested in feedback as well as bug reports. All"  ));
		descLabel1_five->setText( tr(	" questions and comments may be sent to:" ) );
	descLabel2->setText( tr( "<p align=\"center\">mapimg@usgs.gov</p>" ) );
	descLabel3->setText( tr( "Libraries used: Qt, Libtiff, and TinyXML." ) );
	okButton->setText( tr( "Ok" ) );
}
