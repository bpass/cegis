/****************************************************************************
** Form implementation generated from reading ui file 'homaForm.ui'
**
** Created: Thu Oct 7 08:49:13 2004
**      by: The User Interface Compiler ($Id: homaForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "homaForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "homaForm.ui.h"
/*
 *  Constructs a homaForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
homaForm::homaForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "homaForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 450 ) );
    setSizeGripEnabled( TRUE );
    homaFormLayout = new QVBoxLayout( this, 11, 6, "homaFormLayout"); 

    Layout19 = new QHBoxLayout( 0, 0, 6, "Layout19"); 

    factorLabel = new QLabel( this, "factorLabel" );
    factorLabel->setTextFormat( QLabel::PlainText );
    Layout19->addWidget( factorLabel );

    factorEdit = new QLineEdit( this, "factorEdit" );
    factorEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout19->addWidget( factorEdit );
    homaFormLayout->addLayout( Layout19 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    homaFormLayout->addItem( spacer );

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
    homaFormLayout->addLayout( Layout18 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    homaFormLayout->addItem( spacer_2 );

    Layout79 = new QVBoxLayout( 0, 0, 6, "Layout79"); 

    Layout72 = new QHBoxLayout( 0, 0, 6, "Layout72"); 

    origLatLabel = new QLabel( this, "origLatLabel" );
    origLatLabel->setTextFormat( QLabel::PlainText );
    Layout72->addWidget( origLatLabel );

    origLatButton = new QPushButton( this, "origLatButton" );
    origLatButton->setMinimumSize( QSize( 20, 20 ) );
    origLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout72->addWidget( origLatButton );
    Layout79->addLayout( Layout72 );

    Layout72_2 = new QHBoxLayout( 0, 0, 6, "Layout72_2"); 

    long1Label = new QLabel( this, "long1Label" );
    Layout72_2->addWidget( long1Label );

    long1Button = new QPushButton( this, "long1Button" );
    long1Button->setMinimumSize( QSize( 20, 20 ) );
    long1Button->setMaximumSize( QSize( 20, 20 ) );
    Layout72_2->addWidget( long1Button );
    Layout79->addLayout( Layout72_2 );

    Layout72_3 = new QHBoxLayout( 0, 0, 6, "Layout72_3"); 

    long2Label = new QLabel( this, "long2Label" );
    Layout72_3->addWidget( long2Label );

    long2Button = new QPushButton( this, "long2Button" );
    long2Button->setMinimumSize( QSize( 20, 20 ) );
    long2Button->setMaximumSize( QSize( 20, 20 ) );
    Layout72_3->addWidget( long2Button );
    Layout79->addLayout( Layout72_3 );

    Layout72_4 = new QHBoxLayout( 0, 0, 6, "Layout72_4"); 

    lat1Label = new QLabel( this, "lat1Label" );
    Layout72_4->addWidget( lat1Label );

    lat1Button = new QPushButton( this, "lat1Button" );
    lat1Button->setMinimumSize( QSize( 20, 20 ) );
    lat1Button->setMaximumSize( QSize( 20, 20 ) );
    Layout72_4->addWidget( lat1Button );
    Layout79->addLayout( Layout72_4 );

    Layout72_5 = new QHBoxLayout( 0, 0, 6, "Layout72_5"); 

    lat2Label = new QLabel( this, "lat2Label" );
    Layout72_5->addWidget( lat2Label );

    lat2Button = new QPushButton( this, "lat2Button" );
    lat2Button->setMinimumSize( QSize( 20, 20 ) );
    lat2Button->setMaximumSize( QSize( 20, 20 ) );
    Layout72_5->addWidget( lat2Button );
    Layout79->addLayout( Layout72_5 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout79->addWidget( zeroAssumed );
    homaFormLayout->addLayout( Layout79 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    homaFormLayout->addItem( spacer_3 );

    layout208 = new QHBoxLayout( 0, 0, 6, "layout208"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout208->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout208->addWidget( cancelButton );
    homaFormLayout->addLayout( layout208 );
    languageChange();
    resize( QSize(350, 450).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( origLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( long1Button, SIGNAL( clicked() ), this, SLOT( getlon1() ) );
    connect( long2Button, SIGNAL( clicked() ), this, SLOT( getlon2() ) );
    connect( lat1Button, SIGNAL( clicked() ), this, SLOT( getlat1() ) );
    connect( lat2Button, SIGNAL( clicked() ), this, SLOT( getlat2() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( factorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( scaleCheck(const QString&) ) );
    connect( smajorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( smajCheck(const QString&) ) );
    connect( sminorEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sminCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );

    // tab order
    setTabOrder( factorEdit, smajorEdit );
    setTabOrder( smajorEdit, sminorEdit );
    setTabOrder( sminorEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, origLatButton );
    setTabOrder( origLatButton, long1Button );
    setTabOrder( long1Button, long2Button );
    setTabOrder( long2Button, lat1Button );
    setTabOrder( lat1Button, lat2Button );
    setTabOrder( lat2Button, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    factorLabel->setBuddy( factorEdit );
    smajorLabel->setBuddy( smajorEdit );
    sminorLabel->setBuddy( sminorEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    origLatLabel->setBuddy( origLatButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
homaForm::~homaForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void homaForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    factorLabel->setText( tr( "&Scale Factor at Center of Projection:" ) );
    factorEdit->setText( tr( "1.000000" ) );
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
    origLatLabel->setText( tr( "Latitude of Projection &Origin:" ) );
    origLatButton->setText( QString::null );
    long1Label->setText( tr( "Longitude of 1st Center Line Point:" ) );
    long1Button->setText( QString::null );
    long2Label->setText( tr( "Longitude of 2nd Center Line Point:" ) );
    long2Button->setText( QString::null );
    lat1Label->setText( tr( "Latitude of 1st Center Line Point:" ) );
    lat1Button->setText( QString::null );
    lat2Label->setText( tr( "Latitude of 2nd Center Line Point:" ) );
    lat2Button->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

