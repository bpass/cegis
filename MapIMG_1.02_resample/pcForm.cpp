/****************************************************************************
** Form implementation generated from reading ui file 'pcForm.ui'
**
** Created: Thu Oct 7 08:49:17 2004
**      by: The User Interface Compiler ($Id: pcForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "pcForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "pcForm.ui.h"
/*
 *  Constructs a pcForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
pcForm::pcForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "pcForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 320 ) );
    setSizeGripEnabled( TRUE );
    pcFormLayout = new QVBoxLayout( this, 11, 6, "pcFormLayout"); 

    Layout248 = new QVBoxLayout( 0, 0, 6, "Layout248"); 

    Layout246 = new QHBoxLayout( 0, 0, 6, "Layout246"); 

    smajorLabel = new QLabel( this, "smajorLabel" );
    smajorLabel->setTextFormat( QLabel::PlainText );
    Layout246->addWidget( smajorLabel );

    smajorEdit = new QLineEdit( this, "smajorEdit" );
    smajorEdit->setFrameShape( QLineEdit::LineEditPanel );
    smajorEdit->setFrameShadow( QLineEdit::Sunken );
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
    pcFormLayout->addLayout( Layout248 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pcFormLayout->addItem( spacer );

    layout251 = new QVBoxLayout( 0, 0, 6, "layout251"); 

    Layout86 = new QVBoxLayout( 0, 0, 6, "Layout86"); 

    Layout130 = new QHBoxLayout( 0, 0, 6, "Layout130"); 

    centMerLabel_2 = new QLabel( this, "centMerLabel_2" );
    centMerLabel_2->setTextFormat( QLabel::PlainText );
    Layout130->addWidget( centMerLabel_2 );

    centMerButton_2 = new QPushButton( this, "centMerButton_2" );
    centMerButton_2->setEnabled( TRUE );
    centMerButton_2->setMinimumSize( QSize( 20, 20 ) );
    centMerButton_2->setMaximumSize( QSize( 20, 20 ) );
    Layout130->addWidget( centMerButton_2 );
    Layout86->addLayout( Layout130 );

    Layout118 = new QHBoxLayout( 0, 0, 6, "Layout118"); 

    origLatLabel = new QLabel( this, "origLatLabel" );
    origLatLabel->setTextFormat( QLabel::PlainText );
    Layout118->addWidget( origLatLabel );

    origLatButton = new QPushButton( this, "origLatButton" );
    origLatButton->setMinimumSize( QSize( 20, 20 ) );
    origLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout118->addWidget( origLatButton );
    Layout86->addLayout( Layout118 );
    layout251->addLayout( Layout86 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout251->addWidget( zeroAssumed );
    pcFormLayout->addLayout( layout251 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pcFormLayout->addItem( spacer_2 );

    layout355 = new QHBoxLayout( 0, 0, 6, "layout355"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout355->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout355->addWidget( cancelButton );
    pcFormLayout->addLayout( layout355 );
    languageChange();
    resize( QSize(250, 320).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centMerButton_2, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( origLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
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
    setTabOrder( fnEdit, centMerButton_2 );
    setTabOrder( centMerButton_2, origLatButton );
    setTabOrder( origLatButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centMerLabel_2->setBuddy( centMerButton_2 );
    origLatLabel->setBuddy( origLatButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
pcForm::~pcForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void pcForm::languageChange()
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
    centMerLabel_2->setText( tr( "Longitude of Central &Meridian:" ) );
    centMerButton_2->setText( QString::null );
    origLatLabel->setText( tr( "Latitude of Projection &Origin:" ) );
    origLatButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

