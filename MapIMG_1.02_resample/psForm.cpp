/****************************************************************************
** Form implementation generated from reading ui file 'psForm.ui'
**
** Created: Thu Oct 7 08:49:18 2004
**      by: The User Interface Compiler ($Id: psForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "psForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "psForm.ui.h"
/*
 *  Constructs a psForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
psForm::psForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "psForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 270, 300 ) );
    setSizeGripEnabled( TRUE );
    psFormLayout = new QVBoxLayout( this, 11, 6, "psFormLayout"); 

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
    sminorEdit->setFrameShape( QLineEdit::LineEditPanel );
    sminorEdit->setFrameShadow( QLineEdit::Sunken );
    sminorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout247->addWidget( sminorEdit );
    Layout248->addLayout( Layout247 );

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setFrameShape( QLineEdit::LineEditPanel );
    feEdit->setFrameShadow( QLineEdit::Sunken );
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
    psFormLayout->addLayout( Layout248 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    psFormLayout->addItem( spacer );

    Layout82 = new QVBoxLayout( 0, 0, 6, "Layout82"); 

    Layout73 = new QHBoxLayout( 0, 0, 6, "Layout73"); 

    longPolLabel = new QLabel( this, "longPolLabel" );
    longPolLabel->setTextFormat( QLabel::PlainText );
    Layout73->addWidget( longPolLabel );

    longPolButton = new QPushButton( this, "longPolButton" );
    longPolButton->setEnabled( TRUE );
    longPolButton->setMinimumSize( QSize( 20, 20 ) );
    longPolButton->setMaximumSize( QSize( 20, 20 ) );
    Layout73->addWidget( longPolButton );
    Layout82->addLayout( Layout73 );

    Layout81 = new QHBoxLayout( 0, 0, 6, "Layout81"); 

    trueScaleLabel = new QLabel( this, "trueScaleLabel" );
    trueScaleLabel->setTextFormat( QLabel::PlainText );
    Layout81->addWidget( trueScaleLabel );

    trueScaleButton = new QPushButton( this, "trueScaleButton" );
    trueScaleButton->setEnabled( TRUE );
    trueScaleButton->setMinimumSize( QSize( 20, 20 ) );
    trueScaleButton->setMaximumSize( QSize( 20, 20 ) );
    Layout81->addWidget( trueScaleButton );
    Layout82->addLayout( Layout81 );
    psFormLayout->addLayout( Layout82 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    psFormLayout->addItem( spacer_2 );

    layout370 = new QHBoxLayout( 0, 0, 6, "layout370"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout370->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout370->addWidget( cancelButton );
    psFormLayout->addLayout( layout370 );
    languageChange();
    resize( QSize(270, 300).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( longPolButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( trueScaleButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );

    // tab order
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, longPolButton );
    setTabOrder( longPolButton, trueScaleButton );
    setTabOrder( trueScaleButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    longPolLabel->setBuddy( longPolButton );
    trueScaleLabel->setBuddy( trueScaleButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
psForm::~psForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void psForm::languageChange()
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
    longPolLabel->setText( tr( "Longitude below &Pole:" ) );
    longPolButton->setText( QString::null );
    trueScaleLabel->setText( tr( "Latitude of True &Scale:" ) );
    trueScaleButton->setText( QString::null );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

