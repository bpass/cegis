/****************************************************************************
** Form implementation generated from reading ui file 'lambForm.ui'
**
** Created: Thu Oct 7 08:49:15 2004
**      by: The User Interface Compiler ($Id: lambForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "lambForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "lambForm.ui.h"
/*
 *  Constructs a lambForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
lambForm::lambForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "lambForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 300, 370 ) );
    setSizeGripEnabled( TRUE );
    lambFormLayout = new QVBoxLayout( this, 11, 6, "lambFormLayout"); 

    Layout248 = new QVBoxLayout( 0, 0, 6, "Layout248"); 

    Layout246 = new QHBoxLayout( 0, 0, 6, "Layout246"); 

    smajorLabel = new QLabel( this, "smajorLabel" );
    smajorLabel->setTextFormat( QLabel::PlainText );
    Layout246->addWidget( smajorLabel );

    smajorEdit = new QLineEdit( this, "smajorEdit" );
    smajorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout246->addWidget( smajorEdit );
    Layout248->addLayout( Layout246 );

    Layout247 = new QHBoxLayout( 0, 0, 6, "Layout247"); 

    sminorLabel = new QLabel( this, "sminorLabel" );
    sminorLabel->setTextFormat( QLabel::PlainText );
    Layout247->addWidget( sminorLabel );

    sminorEdit = new QLineEdit( this, "sminorEdit" );
    sminorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout247->addWidget( sminorEdit );
    Layout248->addLayout( Layout247 );

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout89->addWidget( feEdit );
    Layout248->addLayout( Layout89 );

    Layout90 = new QHBoxLayout( 0, 0, 6, "Layout90"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    Layout90->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setFrameShape( QLineEdit::LineEditPanel );
    fnEdit->setFrameShadow( QLineEdit::Sunken );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout248->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout248->addWidget( fefnLabel );
    lambFormLayout->addLayout( Layout248 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    lambFormLayout->addItem( spacer );

    Layout245 = new QVBoxLayout( 0, 0, 6, "Layout245"); 

    Layout161 = new QHBoxLayout( 0, 0, 6, "Layout161"); 

    stdPar1Label = new QLabel( this, "stdPar1Label" );
    stdPar1Label->setTextFormat( QLabel::PlainText );
    Layout161->addWidget( stdPar1Label );

    stdPar1Button = new QPushButton( this, "stdPar1Button" );
    stdPar1Button->setEnabled( TRUE );
    stdPar1Button->setMinimumSize( QSize( 20, 20 ) );
    stdPar1Button->setMaximumSize( QSize( 20, 20 ) );
    Layout161->addWidget( stdPar1Button );
    Layout245->addLayout( Layout161 );

    Layout162 = new QHBoxLayout( 0, 0, 6, "Layout162"); 

    stdPar2Label = new QLabel( this, "stdPar2Label" );
    stdPar2Label->setTextFormat( QLabel::PlainText );
    Layout162->addWidget( stdPar2Label );

    stdPar2Button = new QPushButton( this, "stdPar2Button" );
    stdPar2Button->setEnabled( TRUE );
    stdPar2Button->setMinimumSize( QSize( 20, 20 ) );
    stdPar2Button->setMaximumSize( QSize( 20, 20 ) );
    Layout162->addWidget( stdPar2Button );
    Layout245->addLayout( Layout162 );

    Layout130 = new QHBoxLayout( 0, 0, 6, "Layout130"); 

    centMerLabel = new QLabel( this, "centMerLabel" );
    centMerLabel->setTextFormat( QLabel::PlainText );
    Layout130->addWidget( centMerLabel );

    centMerButton = new QPushButton( this, "centMerButton" );
    centMerButton->setEnabled( TRUE );
    centMerButton->setMinimumSize( QSize( 20, 20 ) );
    centMerButton->setMaximumSize( QSize( 20, 20 ) );
    Layout130->addWidget( centMerButton );
    Layout245->addLayout( Layout130 );

    Layout118 = new QHBoxLayout( 0, 0, 6, "Layout118"); 

    origLatLabel = new QLabel( this, "origLatLabel" );
    origLatLabel->setTextFormat( QLabel::PlainText );
    Layout118->addWidget( origLatLabel );

    origLatButton = new QPushButton( this, "origLatButton" );
    origLatButton->setMinimumSize( QSize( 20, 20 ) );
    origLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout118->addWidget( origLatButton );
    Layout245->addLayout( Layout118 );

    zeroAssumed_2 = new QLabel( this, "zeroAssumed_2" );
    zeroAssumed_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout245->addWidget( zeroAssumed_2 );
    lambFormLayout->addLayout( Layout245 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    lambFormLayout->addItem( spacer_2 );

    layout311 = new QHBoxLayout( 0, 0, 6, "layout311"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout311->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout311->addWidget( cancelButton );
    lambFormLayout->addLayout( layout311 );
    languageChange();
    resize( QSize(300, 370).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( stdPar1Button, SIGNAL( clicked() ), this, SLOT( getlat1() ) );
    connect( stdPar2Button, SIGNAL( clicked() ), this, SLOT( getlat2() ) );
    connect( centMerButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( origLatButton, SIGNAL( clicked() ), this, SLOT( getlat3() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, stdPar1Button );
    setTabOrder( stdPar1Button, stdPar2Button );
    setTabOrder( stdPar2Button, centMerButton );
    setTabOrder( centMerButton, origLatButton );
    setTabOrder( origLatButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    stdPar1Label->setBuddy( stdPar1Button );
    stdPar2Label->setBuddy( stdPar2Button );
    centMerLabel->setBuddy( centMerButton );
    origLatLabel->setBuddy( origLatButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
lambForm::~lambForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void lambForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    smajorLabel->setText( tr( "Semi-M&ajor Axis:" ) );
    smajorEdit->setText( tr( "0.000000" ) );
    QToolTip::add( smajorEdit, tr( "<B>Semi-Major Axis:</B> value must be between 0 and 10,000 km." ) );
    sminorLabel->setText( tr( "Semi-M&inor Axis:" ) );
    sminorEdit->setText( tr( "0.000000" ) );
    QToolTip::add( sminorEdit, tr( "<B>Semi-Minor Axis:</B> value must be between 0 and 10,000 km." ) );
    feLabel->setText( tr( "False &Easting:" ) );
    feEdit->setText( tr( "0.000000" ) );
    QToolTip::add( feEdit, tr( "<B>False Easting:</B> value must be between -10,000 km and 10,000 km." ) );
    fnLabel->setText( tr( "False &Northing:" ) );
    fnEdit->setText( tr( "0.000000" ) );
    QToolTip::add( fnEdit, tr( "<B>False Northing:</B> value must be between -10,000 km and 10,000 km." ) );
    fefnLabel->setText( tr( "<i>(In meters)</i>" ) );
    stdPar1Label->setText( tr( "Latitude of &1st Standard Parallel:" ) );
    stdPar1Button->setText( QString::null );
    stdPar2Label->setText( tr( "Latitude of &2nd Standard Parallel:" ) );
    stdPar2Button->setText( QString::null );
    centMerLabel->setText( tr( "Longitude of Central &Meridian:" ) );
    centMerButton->setText( QString::null );
    origLatLabel->setText( tr( "Latitude of Projection &Origin:" ) );
    origLatButton->setText( QString::null );
    zeroAssumed_2->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

