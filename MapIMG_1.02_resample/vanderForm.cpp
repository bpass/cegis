/****************************************************************************
** Form implementation generated from reading ui file 'vanderForm.ui'
**
** Created: Thu Oct 7 08:49:20 2004
**      by: The User Interface Compiler ($Id: vanderForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "vanderForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "vanderForm.ui.h"
/*
 *  Constructs a vanderForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
vanderForm::vanderForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "vanderForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 320 ) );
    setSizeGripEnabled( TRUE );
    vanderFormLayout = new QVBoxLayout( this, 11, 6, "vanderFormLayout"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    sphereText->setAlignment( int( QLabel::AlignVCenter ) );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    vanderFormLayout->addLayout( Layout58 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vanderFormLayout->addItem( spacer );

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
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout151->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout151->addWidget( fefnLabel );
    vanderFormLayout->addLayout( Layout151 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vanderFormLayout->addItem( spacer_2 );

    Layout78 = new QVBoxLayout( 0, 0, 6, "Layout78"); 

    Layout130 = new QHBoxLayout( 0, 0, 6, "Layout130"); 

    centMerLabel = new QLabel( this, "centMerLabel" );
    centMerLabel->setTextFormat( QLabel::PlainText );
    Layout130->addWidget( centMerLabel );

    centMerButton = new QPushButton( this, "centMerButton" );
    centMerButton->setEnabled( TRUE );
    centMerButton->setMinimumSize( QSize( 20, 20 ) );
    centMerButton->setMaximumSize( QSize( 20, 20 ) );
    Layout130->addWidget( centMerButton );
    Layout78->addLayout( Layout130 );

    Layout118 = new QHBoxLayout( 0, 0, 6, "Layout118"); 

    origLatLabel = new QLabel( this, "origLatLabel" );
    origLatLabel->setTextFormat( QLabel::PlainText );
    Layout118->addWidget( origLatLabel );

    origLatButton = new QPushButton( this, "origLatButton" );
    origLatButton->setMinimumSize( QSize( 20, 20 ) );
    origLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout118->addWidget( origLatButton );
    Layout78->addLayout( Layout118 );

    zeroAssumed_2 = new QLabel( this, "zeroAssumed_2" );
    zeroAssumed_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout78->addWidget( zeroAssumed_2 );
    vanderFormLayout->addLayout( Layout78 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vanderFormLayout->addItem( spacer_3 );

    layout474 = new QHBoxLayout( 0, 0, 6, "layout474"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout474->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout474->addWidget( cancelButton );
    vanderFormLayout->addLayout( layout474 );
    languageChange();
    resize( QSize(350, 320).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centMerButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( origLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( sphereEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, centMerButton );
    setTabOrder( centMerButton, origLatButton );
    setTabOrder( origLatButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centMerLabel->setBuddy( centMerButton );
    origLatLabel->setBuddy( origLatButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
vanderForm::~vanderForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void vanderForm::languageChange()
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
    origLatLabel->setText( tr( "Latitude of Projection &Origin:" ) );
    origLatButton->setText( QString::null );
    zeroAssumed_2->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

