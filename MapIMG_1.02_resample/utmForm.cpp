/****************************************************************************
** Form implementation generated from reading ui file 'utmForm.ui'
**
** Created: Thu Oct 7 08:49:20 2004
**      by: The User Interface Compiler ($Id: utmForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "utmForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "utmForm.ui.h"
/*
 *  Constructs a utmForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
utmForm::utmForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "utmForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 160 ) );
    setSizeGripEnabled( TRUE );
    utmFormLayout = new QVBoxLayout( this, 11, 6, "utmFormLayout"); 

    Layout26 = new QVBoxLayout( 0, 0, 6, "Layout26"); 

    Layout44 = new QHBoxLayout( 0, 0, 6, "Layout44"); 

    latLabel = new QLabel( this, "latLabel" );
    latLabel->setTextFormat( QLabel::PlainText );
    Layout44->addWidget( latLabel );

    latzButton = new QPushButton( this, "latzButton" );
    latzButton->setMinimumSize( QSize( 20, 20 ) );
    latzButton->setMaximumSize( QSize( 20, 20 ) );
    Layout44->addWidget( latzButton );
    Layout26->addLayout( Layout44 );

    Layout45 = new QHBoxLayout( 0, 0, 6, "Layout45"); 

    lonLabel = new QLabel( this, "lonLabel" );
    lonLabel->setTextFormat( QLabel::PlainText );
    Layout45->addWidget( lonLabel );

    lonzButton = new QPushButton( this, "lonzButton" );
    lonzButton->setMinimumSize( QSize( 20, 20 ) );
    lonzButton->setMaximumSize( QSize( 20, 20 ) );
    Layout45->addWidget( lonzButton );
    Layout26->addLayout( Layout45 );

    Layout25 = new QHBoxLayout( 0, 0, 6, "Layout25"); 

    zonecodeLabel = new QLabel( this, "zonecodeLabel" );
    zonecodeLabel->setTextFormat( QLabel::PlainText );
    Layout25->addWidget( zonecodeLabel );

    zonecodeSpin = new QSpinBox( this, "zonecodeSpin" );
    zonecodeSpin->setMaxValue( 60 );
    zonecodeSpin->setMinValue( 1 );
    Layout25->addWidget( zonecodeSpin );
    Layout26->addLayout( Layout25 );
    utmFormLayout->addLayout( Layout26 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    utmFormLayout->addItem( spacer );

    layout466 = new QHBoxLayout( 0, 0, 6, "layout466"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout466->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout466->addWidget( cancelButton );
    utmFormLayout->addLayout( layout466 );
    languageChange();
    resize( QSize(250, 160).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( latzButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( lonzButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );

    // tab order
    setTabOrder( latzButton, lonzButton );
    setTabOrder( lonzButton, zonecodeSpin );
    setTabOrder( zonecodeSpin, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    latLabel->setBuddy( latzButton );
    lonLabel->setBuddy( lonzButton );
    zonecodeLabel->setBuddy( zonecodeSpin );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
utmForm::~utmForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void utmForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    latLabel->setText( tr( "L&atitude within the Zone:" ) );
    latzButton->setText( QString::null );
    lonLabel->setText( tr( "L&ongitude within the Zone:" ) );
    lonzButton->setText( QString::null );
    zonecodeLabel->setText( tr( "&Zone Code:" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

