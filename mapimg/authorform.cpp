// $Id: authorform.cpp,v 1.4 2005/01/31 17:24:02 jtrent Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
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
authorForm::authorForm( QSettings *settings, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    set = settings;
    if ( !name )
	    setName( "authorForm" );
    authorFormLayout = new QVBoxLayout( this, 3, 3, "authorFormLayout"); 
    spacer5 = new QSpacerItem( 20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer5 );

    descrLabel = new QLabel( this, "descrLabel" );
    descrLabel->setFrameShape( QLabel::WinPanel );
    descrLabel->setFrameShadow( QLabel::Sunken );
    descrLabel->setLineWidth( 2 );
    descrLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    authorFormLayout->addWidget( descrLabel );
    spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer1 );

    layout1 = new QGridLayout( 0, 1, 1, 0, 3, "layout1"); 

    emailLabel = new QLabel( this, "emailLabel" );

    layout1->addWidget( emailLabel, 2, 0 );

    nameLabel = new QLabel( this, "nameLabel" );

    layout1->addWidget( nameLabel, 0, 0 );

    nameEdit = new QLineEdit( this, "nameEdit" );

    layout1->addWidget( nameEdit, 0, 1 );

    companyLabel = new QLabel( this, "companyLabel" );

    layout1->addWidget( companyLabel, 1, 0 );

    companyEdit = new QLineEdit( this, "companyEdit" );

    layout1->addWidget( companyEdit, 1, 1 );

    emailEdit = new QLineEdit( this, "emailEdit" );

    layout1->addWidget( emailEdit, 2, 1 );
    authorFormLayout->addLayout( layout1 );
    spacer3 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    authorFormLayout->addItem( spacer3 );

    textLabel5 = new QLabel( this, "textLabel5" );
    textLabel5->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    authorFormLayout->addWidget( textLabel5 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
    spacer4 = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer4 );

    okButton = new QPushButton( this, "okButton" );
    okButton->setMaximumSize( QSize( 80, 32767 ) );
    cancelButton = new QPushButton( this, "cancelButton" );
    cancelButton->setMaximumSize( QSize( 80, 32767 ) );
    layout2->addWidget( okButton );
    layout2->addWidget( cancelButton );
    authorFormLayout->addLayout( layout2 );
    languageChange();
    resize( QSize(265, 174).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    connect( okButton, SIGNAL( clicked() ), this, SLOT(okClicked()) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT(close()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
authorForm::~authorForm()
{}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void authorForm::languageChange()
{
    setCaption( tr( "Author Properties" ) );
    descrLabel->setText( tr( "For your covenience mapimg%1 can attach your information to the re-projections that you create." ).arg(MAJOR_VERSION) );
    emailLabel->setText( tr( "Email address" ) );
    nameLabel->setText( tr( "Name:" ) );
    companyLabel->setText( tr( "Company" ) );
    textLabel5->setText( tr( "These fields can be changed at any time by clicking \"Edit Author\" under the options menu." ) );
    okButton->setText( tr( "Ok" ) );
    cancelButton->setText( tr( "Cancel" ) );
    nameEdit->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorName" ) ) );
    companyEdit->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorCompany" ) ) );
    emailEdit->setText( tr( set->readEntry( "/USGS/mapimg2/AuthorEmail" ) ) );
}

void authorForm::okClicked()
{
   set->writeEntry( "/USGS/mapimg2/AuthorName", nameEdit->text() );
   set->writeEntry( "/USGS/mapimg2/AuthorCompany", companyEdit->text() );
   set->writeEntry( "/USGS/mapimg2/AuthorEmail", emailEdit->text() );

   close();
}
