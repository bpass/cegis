/****************************************************************************
** Form implementation generated from reading ui file 'genVertForm.ui'
**
** Created: Thu Oct 7 08:49:13 2004
**      by: The User Interface Compiler ($Id: genVertForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "genVertForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "genVertForm.ui.h"
/*
 *  Constructs a genVertForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
genVertForm::genVertForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "genVertForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 350 ) );
    setSizeGripEnabled( TRUE );
    genVertFormLayout = new QVBoxLayout( this, 11, 6, "genVertFormLayout"); 

    layout228 = new QVBoxLayout( 0, 0, 6, "layout228"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    layout228->addLayout( Layout58 );

    Layout30 = new QVBoxLayout( 0, 0, 6, "Layout30"); 

    Layout104 = new QHBoxLayout( 0, 0, 6, "Layout104"); 

    heightLabel = new QLabel( this, "heightLabel" );
    heightLabel->setTextFormat( QLabel::PlainText );
    Layout104->addWidget( heightLabel );

    heightEdit = new QLineEdit( this, "heightEdit" );
    heightEdit->setEnabled( TRUE );
    heightEdit->setFrameShape( QLineEdit::LineEditPanel );
    heightEdit->setFrameShadow( QLineEdit::Sunken );
    heightEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout104->addWidget( heightEdit );
    Layout30->addLayout( Layout104 );
    layout228->addLayout( Layout30 );

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
    layout228->addLayout( Layout151 );
    genVertFormLayout->addLayout( layout228 );
    QSpacerItem* spacer = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    genVertFormLayout->addItem( spacer );

    Layout150 = new QVBoxLayout( 0, 0, 6, "Layout150"); 

    Layout114 = new QHBoxLayout( 0, 0, 6, "Layout114"); 

    centLatLabel = new QLabel( this, "centLatLabel" );
    centLatLabel->setTextFormat( QLabel::PlainText );
    Layout114->addWidget( centLatLabel );

    centLatButton = new QPushButton( this, "centLatButton" );
    centLatButton->setMinimumSize( QSize( 20, 20 ) );
    centLatButton->setMaximumSize( QSize( 20, 20 ) );
    Layout114->addWidget( centLatButton );
    Layout150->addLayout( Layout114 );

    Layout131 = new QHBoxLayout( 0, 0, 6, "Layout131"); 

    centLonLabel = new QLabel( this, "centLonLabel" );
    centLonLabel->setTextFormat( QLabel::PlainText );
    Layout131->addWidget( centLonLabel );

    centLonButton = new QPushButton( this, "centLonButton" );
    centLonButton->setMinimumSize( QSize( 20, 20 ) );
    centLonButton->setMaximumSize( QSize( 20, 20 ) );
    Layout131->addWidget( centLonButton );
    Layout150->addLayout( Layout131 );

    zeroAssumed = new QLabel( this, "zeroAssumed" );
    zeroAssumed->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    Layout150->addWidget( zeroAssumed );
    genVertFormLayout->addLayout( Layout150 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    genVertFormLayout->addItem( spacer_2 );

    layout194 = new QHBoxLayout( 0, 0, 6, "layout194"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout194->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout194->addWidget( cancelButton );
    genVertFormLayout->addLayout( layout194 );
    languageChange();
    resize( QSize(350, 350).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( centLatButton, SIGNAL( clicked() ), this, SLOT( getlat() ) );
    connect( centLonButton, SIGNAL( clicked() ), this, SLOT( getlon() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( fnEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fnCheck(const QString&) ) );
    connect( heightEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( heightCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( feEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( feCheck(const QString&) ) );

    // tab order
    setTabOrder( sphereEdit, heightEdit );
    setTabOrder( heightEdit, feEdit );
    setTabOrder( feEdit, fnEdit );
    setTabOrder( fnEdit, centLatButton );
    setTabOrder( centLatButton, centLonButton );
    setTabOrder( centLonButton, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    heightLabel->setBuddy( heightEdit );
    feLabel->setBuddy( feEdit );
    fnLabel->setBuddy( fnEdit );
    centLatLabel->setBuddy( centLatButton );
    centLonLabel->setBuddy( centLonButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
genVertForm::~genVertForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void genVertForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    sphereText->setText( tr( "&Radius of the Reference Sphere:" ) );
    sphereEdit->setText( tr( "6370997.000000 " ) );
    QToolTip::add( sphereEdit, tr( "<B>Sphere Radius:</B> value must be a valid double." ) );
    heightLabel->setText( tr( "&Perspective Point Height:" ) );
    heightEdit->setText( tr( "0.000000" ) );
    QToolTip::add( heightEdit, tr( "<B>Height:</B> value must be between -1,000,000 km and 1,000,000 km." ) );
    feLabel->setText( tr( "False &Easting:" ) );
    feEdit->setText( tr( "0.000000" ) );
    QToolTip::add( feEdit, tr( "<B>False Easting:</B> value must be between -10,000 km and 10,000 km." ) );
    fnLabel->setText( tr( "False &Northing:" ) );
    fnEdit->setText( tr( "0.000000" ) );
    QToolTip::add( fnEdit, tr( "<B>False Northing:</B> value must be between -10,000 km and 10,000 km." ) );
    fefnLabel->setText( tr( "<i>(In meters)</i>" ) );
    centLatLabel->setText( tr( "L&atitude of Projection Center:" ) );
    centLatButton->setText( QString::null );
    centLonLabel->setText( tr( "L&ongitude of Projection Center:" ) );
    centLonButton->setText( QString::null );
    zeroAssumed->setText( tr( "<i>(Zero assumed unless specified)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

