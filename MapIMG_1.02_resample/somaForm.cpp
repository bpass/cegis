/****************************************************************************
** Form implementation generated from reading ui file 'somaForm.ui'
**
** Created: Thu Oct 7 08:49:18 2004
**      by: The User Interface Compiler ($Id: somaForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "somaForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "somaForm.ui.h"
/*
 *  Constructs a somaForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
somaForm::somaForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "somaForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 270, 430 ) );
    setSizeGripEnabled( TRUE );
    somaFormLayout = new QVBoxLayout( this, 11, 6, "somaFormLayout"); 

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
    feEdit->setFrameShape( QLineEdit::LineEditPanel );
    feEdit->setFrameShadow( QLineEdit::Sunken );
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
    somaFormLayout->addLayout( Layout18 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    somaFormLayout->addItem( spacer );

    Layout518 = new QVBoxLayout( 0, 0, 6, "Layout518"); 

    Layout517 = new QHBoxLayout( 0, 0, 6, "Layout517"); 

    incangleLabel = new QLabel( this, "incangleLabel" );
    incangleLabel->setTextFormat( QLabel::PlainText );
    Layout517->addWidget( incangleLabel );

    incangleButton = new QPushButton( this, "incangleButton" );
    incangleButton->setEnabled( TRUE );
    incangleButton->setMinimumSize( QSize( 20, 20 ) );
    incangleButton->setMaximumSize( QSize( 20, 20 ) );
    Layout517->addWidget( incangleButton );
    Layout518->addLayout( Layout517 );

    Layout264 = new QHBoxLayout( 0, 0, 6, "Layout264"); 

    psrevLabel = new QLabel( this, "psrevLabel" );
    psrevLabel->setTextFormat( QLabel::PlainText );
    Layout264->addWidget( psrevLabel );

    psrevEdit = new QLineEdit( this, "psrevEdit" );
    psrevEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout264->addWidget( psrevEdit );
    Layout518->addLayout( Layout264 );

    Layout261_2 = new QHBoxLayout( 0, 0, 6, "Layout261_2"); 

    lratLabel = new QLabel( this, "lratLabel" );
    lratLabel->setTextFormat( QLabel::PlainText );
    Layout261_2->addWidget( lratLabel );

    lratEdit = new QLineEdit( this, "lratEdit" );
    lratEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout261_2->addWidget( lratEdit );
    Layout518->addLayout( Layout261_2 );

    Layout267 = new QHBoxLayout( 0, 0, 6, "Layout267"); 

    pflagLabel = new QLabel( this, "pflagLabel" );
    pflagLabel->setTextFormat( QLabel::PlainText );
    Layout267->addWidget( pflagLabel );

    pflagSpin = new QSpinBox( this, "pflagSpin" );
    pflagSpin->setMaxValue( 1 );
    Layout267->addWidget( pflagSpin );
    Layout518->addLayout( Layout267 );
    somaFormLayout->addLayout( Layout518 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    somaFormLayout->addItem( spacer_2 );

    Layout321 = new QVBoxLayout( 0, 0, 6, "Layout321"); 

    Layout262 = new QHBoxLayout( 0, 0, 6, "Layout262"); 

    asclonLabel = new QLabel( this, "asclonLabel" );
    Layout262->addWidget( asclonLabel );

    asclonButton = new QPushButton( this, "asclonButton" );
    asclonButton->setEnabled( TRUE );
    asclonButton->setMinimumSize( QSize( 20, 20 ) );
    asclonButton->setMaximumSize( QSize( 20, 20 ) );
    Layout262->addWidget( asclonButton );
    Layout321->addLayout( Layout262 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout321->addWidget( zeroAssumed );
    somaFormLayout->addLayout( Layout321 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    somaFormLayout->addItem( spacer_3 );

    layout421 = new QHBoxLayout( 0, 0, 6, "layout421"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout421->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout421->addWidget( cancelButton );
    somaFormLayout->addLayout( layout421 );
    languageChange();
    resize( QSize(270, 430).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( asclonButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( psrevEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( revCheck(const QString&) ) );
    connect( lratEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( ratioCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( incangleButton, SIGNAL( clicked() ), this, SLOT( getang() ) );

    // tab order
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, incangleButton );
    setTabOrder( incangleButton, psrevEdit );
    setTabOrder( psrevEdit, lratEdit );
    setTabOrder( lratEdit, pflagSpin );
    setTabOrder( pflagSpin, asclonButton );
    setTabOrder( asclonButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    incangleLabel->setBuddy( incangleButton );
    psrevLabel->setBuddy( psrevEdit );
    lratLabel->setBuddy( lratEdit );
    pflagLabel->setBuddy( pflagSpin );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
somaForm::~somaForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void somaForm::languageChange()
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
    incangleLabel->setText( tr( "Inclination of &Orbit:" ) );
    incangleButton->setText( QString::null );
    psrevLabel->setText( tr( "&Period of Revolution (min) :" ) );
    psrevEdit->setText( tr( "0.000000" ) );
    QToolTip::add( psrevEdit, tr( "<B>Period of Revolution:</B> value must be a valid double." ) );
    lratLabel->setText( tr( "Landsat &Ratio:" ) );
    lratEdit->setText( tr( "0.5201613" ) );
    QToolTip::add( lratEdit, tr( "<B>Landsat Ratio:</B> value must be between 0 and 1." ) );
    pflagLabel->setText( tr( "End of Path &Flag:" ) );
    asclonLabel->setText( tr( "Longitude of Ascending Orbit:" ) );
    asclonButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

