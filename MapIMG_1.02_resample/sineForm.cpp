/****************************************************************************
** Form implementation generated from reading ui file 'sineForm.ui'
**
** Created: Thu Oct 7 08:49:18 2004
**      by: The User Interface Compiler ($Id: sineForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "sineForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sineForm.ui.h"
/*
 *  Constructs a sineForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
sineForm::sineForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "sineForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 270 ) );
    setSizeGripEnabled( TRUE );
    sineFormLayout = new QVBoxLayout( this, 11, 6, "sineFormLayout"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    sineFormLayout->addLayout( Layout58 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    sineFormLayout->addItem( spacer );

    Layout151 = new QVBoxLayout( 0, 0, 6, "Layout151"); 

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setFrameShape( QLineEdit::LineEditPanel );
    feEdit->setFrameShadow( QLineEdit::Sunken );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout89->addWidget( feEdit );
    Layout151->addLayout( Layout89 );

    Layout90 = new QHBoxLayout( 0, 0, 6, "Layout90"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    Layout90->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setFrameShape( QLineEdit::LineEditPanel );
    fnEdit->setFrameShadow( QLineEdit::Sunken );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout151->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout151->addWidget( fefnLabel );
    sineFormLayout->addLayout( Layout151 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    sineFormLayout->addItem( spacer_2 );

    Layout61 = new QVBoxLayout( 0, 0, 6, "Layout61"); 

    Layout130 = new QHBoxLayout( 0, 0, 6, "Layout130"); 

    centMerLabel = new QLabel( this, "centMerLabel" );
    centMerLabel->setTextFormat( QLabel::PlainText );
    Layout130->addWidget( centMerLabel );

    centMerButton = new QPushButton( this, "centMerButton" );
    centMerButton->setEnabled( TRUE );
    centMerButton->setMinimumSize( QSize( 20, 20 ) );
    centMerButton->setMaximumSize( QSize( 20, 20 ) );
    Layout130->addWidget( centMerButton );
    Layout61->addLayout( Layout130 );

    zeroAssumed_2 = new QLabel( this, "zeroAssumed_2" );
    zeroAssumed_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout61->addWidget( zeroAssumed_2 );
    sineFormLayout->addLayout( Layout61 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    sineFormLayout->addItem( spacer_3 );

    layout388 = new QHBoxLayout( 0, 0, 6, "layout388"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout388->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout388->addWidget( cancelButton );
    sineFormLayout->addLayout( layout388 );
    languageChange();
    resize( QSize(350, 270).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centMerButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( sphereEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, centMerButton );
    setTabOrder( centMerButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centMerLabel->setBuddy( centMerButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
sineForm::~sineForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void sineForm::languageChange()
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
    centMerLabel->setText( tr( "Longitude of Central &Meridian:" ) );
    centMerButton->setText( QString::null );
    zeroAssumed_2->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

