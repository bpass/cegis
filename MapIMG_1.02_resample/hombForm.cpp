/****************************************************************************
** Form implementation generated from reading ui file 'hombForm.ui'
**
** Created: Thu Oct 7 08:49:14 2004
**      by: The User Interface Compiler ($Id: hombForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hombForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "hombForm.ui.h"
/*
 *  Constructs a hombForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hombForm::hombForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "hombForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 400 ) );
    setSizeGripEnabled( TRUE );
    hombFormLayout = new QVBoxLayout( this, 11, 6, "hombFormLayout"); 

    Layout19 = new QHBoxLayout( 0, 0, 6, "Layout19"); 

    factorLabel = new QLabel( this, "factorLabel" );
    factorLabel->setTextFormat( QLabel::PlainText );
    Layout19->addWidget( factorLabel );

    factorEdit = new QLineEdit( this, "factorEdit" );
    factorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout19->addWidget( factorEdit );
    hombFormLayout->addLayout( Layout19 );
    QSpacerItem* spacer = new QSpacerItem( 20, 19, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hombFormLayout->addItem( spacer );

    Layout18 = new QVBoxLayout( 0, 0, 6, "Layout18"); 

    Layout246 = new QHBoxLayout( 0, 0, 6, "Layout246"); 

    smajorLabel = new QLabel( this, "smajorLabel" );
    smajorLabel->setTextFormat( QLabel::PlainText );
    Layout246->addWidget( smajorLabel );

    smajorEdit = new QLineEdit( this, "smajorEdit" );
    smajorEdit->setFrameShape( QLineEdit::LineEditPanel );
    smajorEdit->setFrameShadow( QLineEdit::Sunken );
    smajorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout246->addWidget( smajorEdit );
    Layout18->addLayout( Layout246 );

    Layout247 = new QHBoxLayout( 0, 0, 6, "Layout247"); 

    sminorLabel = new QLabel( this, "sminorLabel" );
    sminorLabel->setTextFormat( QLabel::PlainText );
    Layout247->addWidget( sminorLabel );

    sminorEdit = new QLineEdit( this, "sminorEdit" );
    sminorEdit->setFrameShape( QLineEdit::LineEditPanel );
    sminorEdit->setFrameShadow( QLineEdit::Sunken );
    sminorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout247->addWidget( sminorEdit );
    Layout18->addLayout( Layout247 );

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout89->addWidget( feEdit );
    Layout18->addLayout( Layout89 );

    Layout90 = new QHBoxLayout( 0, 0, 6, "Layout90"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    Layout90->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setFrameShape( QLineEdit::LineEditPanel );
    fnEdit->setFrameShadow( QLineEdit::Sunken );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout18->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout18->addWidget( fefnLabel );
    hombFormLayout->addLayout( Layout18 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hombFormLayout->addItem( spacer_2 );

    layout25 = new QVBoxLayout( 0, 0, 6, "layout25"); 

    Layout220 = new QHBoxLayout( 0, 0, 6, "Layout220"); 

    aziangleLabel = new QLabel( this, "aziangleLabel" );
    Layout220->addWidget( aziangleLabel );

    aziangleButton = new QPushButton( this, "aziangleButton" );
    aziangleButton->setMinimumSize( QSize( 20, 20 ) );
    aziangleButton->setMaximumSize( QSize( 20, 20 ) );
    Layout220->addWidget( aziangleButton );
    layout25->addLayout( Layout220 );

    Layout175_2 = new QHBoxLayout( 0, 0, 6, "Layout175_2"); 

    azmthptLabel = new QLabel( this, "azmthptLabel" );
    azmthptLabel->setTextFormat( QLabel::PlainText );
    Layout175_2->addWidget( azmthptLabel );

    azmthptButton = new QPushButton( this, "azmthptButton" );
    azmthptButton->setMinimumSize( QSize( 20, 20 ) );
    azmthptButton->setMaximumSize( QSize( 20, 20 ) );
    Layout175_2->addWidget( azmthptButton );
    layout25->addLayout( Layout175_2 );

    Layout175 = new QHBoxLayout( 0, 0, 6, "Layout175"); 

    origLatLabel = new QLabel( this, "origLatLabel" );
    Layout175->addWidget( origLatLabel );

    origLatButton = new QPushButton( this, "origLatButton" );
    origLatButton->setMinimumSize( QSize( 20, 20 ) );
    origLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout175->addWidget( origLatButton );
    layout25->addLayout( Layout175 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout25->addWidget( zeroAssumed );
    hombFormLayout->addLayout( layout25 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hombFormLayout->addItem( spacer_3 );

    layout250 = new QHBoxLayout( 0, 0, 6, "layout250"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout250->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout250->addWidget( cancelButton );
    hombFormLayout->addLayout( layout250 );
    languageChange();
    resize( QSize(350, 397).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( azmthptButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( origLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( aziangleButton, SIGNAL( clicked() ), this, SLOT( getang() ) );
    connect( factorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( scaleCheck(const QString&) ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( factorEdit, smajorEdit );
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, aziangleButton );
    setTabOrder( aziangleButton, azmthptButton );
    setTabOrder( azmthptButton, origLatButton );
    setTabOrder( origLatButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    factorLabel->setBuddy( factorEdit );
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
hombForm::~hombForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hombForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    factorLabel->setText( tr( "&Scale Factor at Center: " ) );
    factorEdit->setText( tr( "1.000000" ) );
    QToolTip::add( factorEdit, tr( "<B>Scale Factor:</B> value is typically between 0 and 2." ) );
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
    aziangleLabel->setText( tr( "Azimuth Angle E of N:" ) );
    aziangleButton->setText( QString::null );
    azmthptLabel->setText( tr( "Longitude where Azimuth Occurs:" ) );
    azmthptButton->setText( QString::null );
    origLatLabel->setText( tr( "Latitude of Projection Origin:" ) );
    origLatButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

