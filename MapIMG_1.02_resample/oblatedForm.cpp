/****************************************************************************
** Form implementation generated from reading ui file 'oblatedForm.ui'
**
** Created: Thu Oct 7 08:49:16 2004
**      by: The User Interface Compiler ($Id: oblatedForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "oblatedForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "oblatedForm.ui.h"
/*
 *  Constructs a oblatedForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
oblatedForm::oblatedForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "oblatedForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 400 ) );
    setSizeGripEnabled( TRUE );
    oblatedFormLayout = new QVBoxLayout( this, 11, 6, "oblatedFormLayout"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    oblatedFormLayout->addLayout( Layout58 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    oblatedFormLayout->addItem( spacer );

    Layout191 = new QVBoxLayout( 0, 0, 6, "Layout191"); 

    Layout89 = new QHBoxLayout( 0, 0, 6, "Layout89"); 

    feLabel = new QLabel( this, "feLabel" );
    feLabel->setTextFormat( QLabel::PlainText );
    Layout89->addWidget( feLabel );

    feEdit = new QLineEdit( this, "feEdit" );
    feEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout89->addWidget( feEdit );
    Layout191->addLayout( Layout89 );

    Layout90 = new QHBoxLayout( 0, 0, 6, "Layout90"); 

    fnLabel = new QLabel( this, "fnLabel" );
    fnLabel->setTextFormat( QLabel::PlainText );
    Layout90->addWidget( fnLabel );

    fnEdit = new QLineEdit( this, "fnEdit" );
    fnEdit->setFrameShape( QLineEdit::LineEditPanel );
    fnEdit->setFrameShadow( QLineEdit::Sunken );
    fnEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout90->addWidget( fnEdit );
    Layout191->addLayout( Layout90 );

    fefnLabel = new QLabel( this, "fefnLabel" );
    fefnLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout191->addWidget( fefnLabel );
    oblatedFormLayout->addLayout( Layout191 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    oblatedFormLayout->addItem( spacer_2 );

    Layout190 = new QVBoxLayout( 0, 0, 6, "Layout190"); 

    Layout131 = new QHBoxLayout( 0, 0, 6, "Layout131"); 

    centLatLabel = new QLabel( this, "centLatLabel" );
    centLatLabel->setTextFormat( QLabel::PlainText );
    Layout131->addWidget( centLatLabel );

    centLatButton = new QPushButton( this, "centLatButton" );
    centLatButton->setMinimumSize( QSize( 20, 20 ) );
    centLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout131->addWidget( centLatButton );
    Layout190->addLayout( Layout131 );

    Layout130 = new QHBoxLayout( 0, 0, 6, "Layout130"); 

    centLonLabel = new QLabel( this, "centLonLabel" );
    centLonLabel->setTextFormat( QLabel::PlainText );
    Layout130->addWidget( centLonLabel );

    centLonButton = new QPushButton( this, "centLonButton" );
    centLonButton->setEnabled( TRUE );
    centLonButton->setMinimumSize( QSize( 20, 20 ) );
    centLonButton->setMaximumSize( QSize( 20, 20 ) );
    Layout130->addWidget( centLonButton );
    Layout190->addLayout( Layout130 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout190->addWidget( zeroAssumed );
    oblatedFormLayout->addLayout( Layout190 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    oblatedFormLayout->addItem( spacer_3 );

    Layout428 = new QVBoxLayout( 0, 0, 6, "Layout428"); 

    Layout36 = new QHBoxLayout( 0, 0, 6, "Layout36"); 

    shapemText = new QLabel( this, "shapemText" );
    shapemText->setTextFormat( QLabel::PlainText );
    Layout36->addWidget( shapemText );

    shapemEdit = new QLineEdit( this, "shapemEdit" );
    shapemEdit->setEnabled( TRUE );
    shapemEdit->setFrameShape( QLineEdit::LineEditPanel );
    shapemEdit->setFrameShadow( QLineEdit::Sunken );
    shapemEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout36->addWidget( shapemEdit );
    Layout428->addLayout( Layout36 );

    Layout37 = new QHBoxLayout( 0, 0, 6, "Layout37"); 

    shapenText = new QLabel( this, "shapenText" );
    shapenText->setTextFormat( QLabel::PlainText );
    Layout37->addWidget( shapenText );

    shapenEdit = new QLineEdit( this, "shapenEdit" );
    shapenEdit->setEnabled( TRUE );
    shapenEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout37->addWidget( shapenEdit );
    Layout428->addLayout( Layout37 );

    Layout426 = new QHBoxLayout( 0, 0, 6, "Layout426"); 

    angleText = new QLabel( this, "angleText" );
    angleText->setTextFormat( QLabel::PlainText );
    Layout426->addWidget( angleText );

    angleButton = new QPushButton( this, "angleButton" );
    angleButton->setEnabled( TRUE );
    angleButton->setMinimumSize( QSize( 20, 20 ) );
    angleButton->setMaximumSize( QSize( 20, 20 ) );
    Layout426->addWidget( angleButton );
    Layout428->addLayout( Layout426 );
    oblatedFormLayout->addLayout( Layout428 );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    oblatedFormLayout->addItem( spacer_4 );

    layout345 = new QHBoxLayout( 0, 0, 6, "layout345"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout345->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout345->addWidget( cancelButton );
    oblatedFormLayout->addLayout( layout345 );
    languageChange();
    resize( QSize(350, 400).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( centLonButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( shapemEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( mCheck(const QString&) ) );
    connect( shapenEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( nCheck(const QString&) ) );
    connect( angleButton, SIGNAL( pressed() ), this, SLOT( getang() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( sphereEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, centLatButton );
    setTabOrder( centLatButton, centLonButton );
    setTabOrder( centLonButton, shapemEdit );
    setTabOrder( shapemEdit, shapenEdit );
    setTabOrder( shapenEdit, angleButton );
    setTabOrder( angleButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centLatLabel->setBuddy( centLatButton );
    centLonLabel->setBuddy( centLonButton );
    shapemText->setBuddy( shapemEdit );
    shapenText->setBuddy( shapenEdit );
    angleText->setBuddy( angleButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
oblatedForm::~oblatedForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void oblatedForm::languageChange()
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
    centLatLabel->setText( tr( "&Latitude of Projection Center:" ) );
    centLatButton->setText( QString::null );
    centLonLabel->setText( tr( "L&ongitude of Projection Center:" ) );
    centLonButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    shapemText->setText( tr( "Oval Shape Parameter &m:" ) );
    shapemEdit->setText( tr( "0.000000" ) );
    QToolTip::add( shapemEdit, tr( "<B>Shape m value:</B> value must be less than or equal to 2." ) );
    shapenText->setText( tr( "Oval Shape Parameter &n:" ) );
    shapenEdit->setText( tr( "2.000000" ) );
    QToolTip::add( shapenEdit, tr( "<B>Shape n value:</B> value must be greater than or equal to 2." ) );
    angleText->setText( tr( "Oval Rotation &Angle:" ) );
    angleButton->setText( QString::null );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

