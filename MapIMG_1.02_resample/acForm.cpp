/****************************************************************************
** Form implementation generated from reading ui file 'acForm.ui'
**
** Created: Thu Oct 21 09:26:02 2004
**      by: The User Interface Compiler ($Id: acForm.cpp,v 1.2 2004/10/21 15:15:59 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "acForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "acForm.ui.h"
/*
 *  Constructs a acForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
acForm::acForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "acForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 220 ) );
    setSizeGripEnabled( TRUE );
    acFormLayout = new QVBoxLayout( this, 11, 6, "acFormLayout"); 

    layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    smajorLabel = new QLabel( this, "smajorLabel" );
    smajorLabel->setTextFormat( QLabel::PlainText );
    layout6->addWidget( smajorLabel );

    smajorEdit = new QLineEdit( this, "smajorEdit" );
    smajorEdit->setFrameShape( QLineEdit::LineEditPanel );
    smajorEdit->setFrameShadow( QLineEdit::Sunken );
    smajorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout6->addWidget( smajorEdit );
    layout11->addLayout( layout6 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    sminorLabel = new QLabel( this, "sminorLabel" );
    sminorLabel->setTextFormat( QLabel::PlainText );
    layout7->addWidget( sminorLabel );

    sminorEdit = new QLineEdit( this, "sminorEdit" );
    sminorEdit->setFrameShape( QLineEdit::LineEditPanel );
    sminorEdit->setFrameShadow( QLineEdit::Sunken );
    sminorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout7->addWidget( sminorEdit );
    layout11->addLayout( layout7 );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    layout8->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setFrameShape( QLineEdit::LineEditPanel );
    feEdit->setFrameShadow( QLineEdit::Sunken );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout8->addWidget( feEdit );
    layout11->addLayout( layout8 );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    layout9->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setFrameShape( QLineEdit::LineEditPanel );
    fnEdit->setFrameShadow( QLineEdit::Sunken );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout9->addWidget( fnEdit );
    layout11->addLayout( layout9 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout11->addWidget( fefnLabel );
    acFormLayout->addLayout( layout11 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    acFormLayout->addItem( spacer );

    layout26 = new QHBoxLayout( 0, 0, 6, "layout26"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout26->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout26->addWidget( cancelButton );
    acFormLayout->addLayout( layout26 );
    languageChange();
    resize( QSize(250, 220).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
acForm::~acForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void acForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    smajorLabel->setText( tr( "Semi-M&ajor Axis:" ) );
    smajorEdit->setText( tr( "0.000000" ) );
    QWhatsThis::add( smajorEdit, QString::null );
    QToolTip::add( smajorEdit, tr( "<B>Semi-Major Axis:</B> value must be between 0 and 10,000 km." ) );
    sminorLabel->setText( tr( "Semi-M&inor Axis:" ) );
    sminorEdit->setText( tr( "0.000000" ) );
    QWhatsThis::add( sminorEdit, QString::null );
    feLabel->setText( tr( "False &Easting:" ) );
    feEdit->setText( tr( "0.000000" ) );
    QToolTip::add( feEdit, tr( "<B>False Easting:</B> value must be between -10,000 km and 10,000 km." ) );
    QWhatsThis::add( feEdit, QString::null );
    fnLabel->setText( tr( "False &Northing:" ) );
    fnEdit->setText( tr( "0.000000" ) );
    QToolTip::add( fnEdit, tr( "<B>False Northing:</B> value must be between -10,000 km and 10,000 km." ) );
    fefnLabel->setText( tr( "<i>(In meters)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

