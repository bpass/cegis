/****************************************************************************
** Form implementation generated from reading ui file 'sombForm.ui'
**
** Created: Thu Oct 7 08:49:19 2004
**      by: The User Interface Compiler ($Id: sombForm.cpp,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "sombForm.h"

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

#include "sombForm.ui.h"
/*
 *  Constructs a sombForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
sombForm::sombForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "sombForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 310 ) );
    setSizeGripEnabled( TRUE );
    sombFormLayout = new QVBoxLayout( this, 11, 6, "sombFormLayout"); 

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
    sombFormLayout->addLayout( Layout18 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    sombFormLayout->addItem( spacer );

    Layout32 = new QVBoxLayout( 0, 0, 6, "Layout32"); 

    Layout30 = new QHBoxLayout( 0, 0, 6, "Layout30"); 

    satnumLabel = new QLabel( this, "satnumLabel" );
    satnumLabel->setTextFormat( QLabel::PlainText );
    Layout30->addWidget( satnumLabel );

    satnumSpin = new QSpinBox( this, "satnumSpin" );
    satnumSpin->setMaxValue( 7 );
    satnumSpin->setMinValue( 1 );
    Layout30->addWidget( satnumSpin );
    Layout32->addLayout( Layout30 );

    Layout31 = new QHBoxLayout( 0, 0, 6, "Layout31"); 

    pathLabel = new QLabel( this, "pathLabel" );
    pathLabel->setTextFormat( QLabel::PlainText );
    Layout31->addWidget( pathLabel );

    pathSpin = new QSpinBox( this, "pathSpin" );
    pathSpin->setMaxValue( 251 );
    pathSpin->setMinValue( 1 );
    Layout31->addWidget( pathSpin );
    Layout32->addLayout( Layout31 );
    sombFormLayout->addLayout( Layout32 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    sombFormLayout->addItem( spacer_2 );

    layout430 = new QHBoxLayout( 0, 0, 6, "layout430"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout430->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout430->addWidget( cancelButton );
    sombFormLayout->addLayout( layout430 );
    languageChange();
    resize( QSize(250, 310).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( satnumSpin, SIGNAL( valueChanged(int) ), this, SLOT( updateToolTip() ) );

    // tab order
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, satnumSpin );
    setTabOrder( satnumSpin, pathSpin );
    setTabOrder( pathSpin, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    satnumLabel->setBuddy( satnumSpin );
    pathLabel->setBuddy( pathSpin );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
sombForm::~sombForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void sombForm::languageChange()
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
    satnumLabel->setText( tr( "Landsat &Satellite Number:" ) );
    QToolTip::add( satnumSpin, tr( "<B>Satellite Number:</B> value must be 1,2,3,4,5 or 7." ) );
    pathLabel->setText( tr( "Landsat &Path Number:" ) );
    QToolTip::add( pathSpin, tr( "<B>Satellite Path:</B> value must be between 1 and 233." ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

