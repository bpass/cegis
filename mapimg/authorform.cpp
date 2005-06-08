// $Id: authorform.cpp,v 1.10 2005/06/08 18:00:54 rbuehler Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "authorform.h"
#include "mapimgversion.h"
#include "mapimgpalette.h"

#include <qvariant.h>
#include <qgroupbox.h>
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

   setPalette( AUTHORFORM_COLOR );

   authorFormLayout = new QVBoxLayout( this, 5, 3, "authorFormLayout"); 

   descrLabel = new QLabel( this, "descrLabel" );
   descrLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
   authorFormLayout->addWidget( descrLabel );

   inputBox = new QGroupBox( this, "inputBox" );
   layout1 = new QGridLayout( inputBox, 1, 1, 10, 3, "layout1"); 

   nameLabel = new QLabel( inputBox, "nameLabel" );
   layout1->addWidget( nameLabel, 0, 0 );
   nameEdit = new QLineEdit( inputBox, "nameEdit" );
   layout1->addWidget( nameEdit, 0, 1 );

   companyLabel = new QLabel( inputBox, "companyLabel" );
   layout1->addWidget( companyLabel, 1, 0 );
   companyEdit = new QLineEdit( inputBox, "companyEdit" );
   layout1->addWidget( companyEdit, 1, 1 );

   emailLabel = new QLabel( inputBox, "emailLabel" );
   layout1->addWidget( emailLabel, 2, 0 );
   emailEdit = new QLineEdit( inputBox, "emailEdit" );
   layout1->addWidget( emailEdit, 2, 1 );

   authorFormLayout->addWidget( inputBox );

   spacer3 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
   authorFormLayout->addItem( spacer3 );

   infoLabel = new QLabel( this, "infoLabel" );
   infoLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
   authorFormLayout->addWidget( infoLabel );

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
   resize( QSize(265, 200).expandedTo(minimumSizeHint()) );
   clearWState( WState_Polished );

   connect( okButton, SIGNAL( clicked() ), this, SLOT(okClicked()) );
   connect( cancelButton, SIGNAL( clicked() ), this, SLOT(close()) );

   nameEdit->setFocus();
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
   descrLabel->setText( tr( "For your covenience MapIMG can attach your information to the re-projections that you create." ).arg(MAJOR_VERSION) );
   nameLabel->setText( tr( "Name:" ) );
   companyLabel->setText( tr( "Company:" ) );
   emailLabel->setText( tr( "Email address:" ) );
   infoLabel->setText( tr( "These fields can be changed at any time by clicking \"Edit Author\" under the Options menu." ) );
   okButton->setText( tr( "Ok" ) );
   cancelButton->setText( tr( "Cancel" ) );
   nameEdit->setText( tr( set->readEntry( "/USGS/MapIMG/AuthorName" ) ) );
   companyEdit->setText( tr( set->readEntry( "/USGS/MapIMG/AuthorCompany" ) ) );
   emailEdit->setText( tr( set->readEntry( "/USGS/MapIMG/AuthorEmail" ) ) );
}

// okClicked() saves the changes made by the user before exitting.
void authorForm::okClicked()
{
   set->writeEntry( "/USGS/MapIMG/AuthorName", nameEdit->text() );
   set->writeEntry( "/USGS/MapIMG/AuthorCompany", companyEdit->text() );
   set->writeEntry( "/USGS/MapIMG/AuthorEmail", emailEdit->text() );

   close();
}
