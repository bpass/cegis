/****************************************************************************
** Form implementation generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler ($Id: authorform.cpp,v 1.1 2005/01/14 16:18:48 rbuehler Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "authorform.h"
#include "mapimgversion.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a authorForm as a child of 'parent', with the
 *  name 'name'.
 */
authorForm::authorForm( QWidget* parent, const char* name, QSettings *settings )
    : QDialog( parent, name )
{
    set = settings;
    if ( !name )
	    setName( "authorForm" );
    authorFormLayout = new QVBoxLayout( this, 3, 3, "authorFormLayout"); 
    spacer5 = new QSpacerItem( 20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer5 );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setFrameShape( QLabel::WinPanel );
    textLabel1->setFrameShadow( QLabel::Sunken );
    textLabel1->setLineWidth( 2 );
    textLabel1->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    authorFormLayout->addWidget( textLabel1 );
    spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer1 );

    layout1 = new QGridLayout( 0, 1, 1, 0, 3, "layout1"); 

    textLabel4 = new QLabel( this, "textLabel4" );

    layout1->addWidget( textLabel4, 2, 0 );

    textLabel2 = new QLabel( this, "textLabel2" );

    layout1->addWidget( textLabel2, 0, 0 );

    lineEdit1 = new QLineEdit( this, "lineEdit1" );

    layout1->addWidget( lineEdit1, 0, 1 );

    textLabel3 = new QLabel( this, "textLabel3" );

    layout1->addWidget( textLabel3, 1, 0 );

    lineEdit2 = new QLineEdit( this, "lineEdit2" );

    layout1->addWidget( lineEdit2, 1, 1 );

    lineEdit3 = new QLineEdit( this, "lineEdit3" );

    layout1->addWidget( lineEdit3, 2, 1 );
    authorFormLayout->addLayout( layout1 );
    spacer3 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer3 );

    textLabel5 = new QLabel( this, "textLabel5" );
    textLabel5->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    authorFormLayout->addWidget( textLabel5 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
    spacer4 = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer4 );

    pushButton1 = new QPushButton( this, "pushButton1" );
    pushButton1->setMaximumSize( QSize( 80, 32767 ) );
    layout2->addWidget( pushButton1 );
    authorFormLayout->addLayout( layout2 );
    languageChange();
    resize( QSize(259, 181).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    connect( pushButton1, SIGNAL( clicked() ), this, SLOT(close()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
#include <qmessagebox.h>
authorForm::~authorForm()
{
   set->writeEntry( "/USGS/mapimg2/AuthorName", lineEdit1->text() );
   set->writeEntry( "/USGS/mapimg2/AuthorCompany", lineEdit2->text() );
   set->writeEntry( "/USGS/mapimg2/AuthorEmail", lineEdit3->text() );
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void authorForm::languageChange()
{
    setCaption( tr( "Form1" ) );
    textLabel1->setText( tr( "For your covenience mapimg%1 can attach your information to the re-projections that you create." ).arg(MAJOR_VERSION) );
    textLabel4->setText( tr( "Email address" ) );
    textLabel2->setText( tr( "Name:" ) );
    textLabel3->setText( tr( "Company" ) );
    textLabel5->setText( tr( "These fields can be changed at any time by clicking \"Edit Author\" under the options menu." ) );
    pushButton1->setText( tr( "Ok" ) );
    lineEdit1->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorName" ) ) );
    lineEdit2->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorCompany" ) ) );
    lineEdit3->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorEmail" ) ) );
}

