/****************************************************************************
** Form implementation generated from reading ui file 'stereoForm.ui'
**
** Created: Thu Oct 7 08:49:19 2004
**      by: The User Interface Compiler ($Id: stereoForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "stereoForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "stereoForm.ui.h"
/*
 *  Constructs a stereoForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
stereoForm::stereoForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "stereoForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 300 ) );
    setSizeGripEnabled( TRUE );
    stereoFormLayout = new QVBoxLayout( this, 11, 6, "stereoFormLayout"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    stereoFormLayout->addLayout( Layout58 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    stereoFormLayout->addItem( spacer );

    Layout151 = new QVBoxLayout( 0, 0, 6, "Layout151"); 

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout89->addWidget( feEdit );
    Layout151->addLayout( Layout89 );

    Layout90 = new QHBoxLayout( 0, 0, 6, "Layout90"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    Layout90->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout151->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout151->addWidget( fefnLabel );
    stereoFormLayout->addLayout( Layout151 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    stereoFormLayout->addItem( spacer_2 );

    Layout150 = new QVBoxLayout( 0, 0, 6, "Layout150"); 

    Layout114 = new QHBoxLayout( 0, 0, 6, "Layout114"); 

    centLatLabel = new QLabel( this, "centLatLabel" );
    centLatLabel->setTextFormat( QLabel::PlainText );
    Layout114->addWidget( centLatLabel );

    centLatButton = new QPushButton( this, "centLatButton" );
    centLatButton->setMinimumSize( QSize( 20, 20 ) );
    centLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout114->addWidget( centLatButton );
    Layout150->addLayout( Layout114 );

    Layout131 = new QHBoxLayout( 0, 0, 6, "Layout131"); 

    centLonLabel = new QLabel( this, "centLonLabel" );
    centLonLabel->setTextFormat( QLabel::PlainText );
    Layout131->addWidget( centLonLabel );

    centLonButton = new QPushButton( this, "centLonButton" );
    centLonButton->setMinimumSize( QSize( 20, 20 ) );
    centLonButton->setMaximumSize( QSize( 20, 20 ) );
    Layout131->addWidget( centLonButton );
    Layout150->addLayout( Layout131 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout150->addWidget( zeroAssumed );
    stereoFormLayout->addLayout( Layout150 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    stereoFormLayout->addItem( spacer_3 );

    layout451 = new QHBoxLayout( 0, 0, 6, "layout451"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout451->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout451->addWidget( cancelButton );
    stereoFormLayout->addLayout( layout451 );
    languageChange();
    resize( QSize(350, 300).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( centLonButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( sphereEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, centLatButton );
    setTabOrder( centLatButton, centLonButton );
    setTabOrder( centLonButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centLatLabel->setBuddy( centLatButton );
    centLonLabel->setBuddy( centLonButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
stereoForm::~stereoForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void stereoForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    sphereText->setText( tr( "&Radius of the Reference Sphere:" ) );
    sphereEdit->setText( tr( "6370997.000000 " ) );
    QToolTip::add( sphereEdit, tr( "<B>Sphere Radius:</B> value must be a valid double." ) );
    feLabel->setText( tr( "False &Easting:" ) );
    feEdit->setText( tr( "0.000000" ) );
    QToolTip::add( feEdit, tr( "<B>False Easting:</B> value must be between -10,000 km and 10,000 km." ) );
    fnLabel->setText( tr( "False &Northing:" ) );
    fnEdit->setText( tr( "0.000000" ) );
    QToolTip::add( fnEdit, tr( "<B>False Northing:</B> value must be between -10,000 km and 10,000 km." ) );
    fefnLabel->setText( tr( "<i>(In meters)</i>" ) );
    centLatLabel->setText( tr( "Latitude of Projection &Center:" ) );
    centLatButton->setText( QString::null );
    centLonLabel->setText( tr( "Longitude of &Projection Center:" ) );
    centLonButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

