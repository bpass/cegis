/****************************************************************************
** Form implementation generated from reading ui file 'enterlatForm.ui'
**
** Created: Thu Oct 7 08:49:12 2004
**      by: The User Interface Compiler ($Id: enterlatForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "enterlatForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "enterlatForm.ui.h"
/*
 *  Constructs a enterlatForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
enterlatForm::enterlatForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "enterlatForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 220 ) );
    setSizeGripEnabled( TRUE );
    enterlatFormLayout = new QVBoxLayout( this, 11, 6, "enterlatFormLayout"); 

    layout81 = new QVBoxLayout( 0, 0, 6, "layout81"); 

    titleLabel = new QLabel( this, "titleLabel" );
    titleLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout81->addWidget( titleLabel );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout81->addWidget( textLabel2 );
    enterlatFormLayout->addLayout( layout81 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterlatFormLayout->addItem( spacer );

    Layout226 = new QHBoxLayout( 0, 0, 6, "Layout226"); 

    degreeText = new QLabel( this, "degreeText" );
    degreeText->setTextFormat( QLabel::PlainText );
    Layout226->addWidget( degreeText );

    degreeSpinBox = new QSpinBox( this, "degreeSpinBox" );
    degreeSpinBox->setMaxValue( 90 );
    degreeSpinBox->setMinValue( -90 );
    Layout226->addWidget( degreeSpinBox );
    enterlatFormLayout->addLayout( Layout226 );

    Layout227 = new QHBoxLayout( 0, 0, 6, "Layout227"); 

    minuteText = new QLabel( this, "minuteText" );
    minuteText->setTextFormat( QLabel::PlainText );
    Layout227->addWidget( minuteText );

    minuteSpinBox = new QSpinBox( this, "minuteSpinBox" );
    minuteSpinBox->setMaxValue( 60 );
    Layout227->addWidget( minuteSpinBox );
    enterlatFormLayout->addLayout( Layout227 );

    layout79 = new QHBoxLayout( 0, 0, 6, "layout79"); 

    secondsText = new QLabel( this, "secondsText" );
    secondsText->setTextFormat( QLabel::PlainText );
    layout79->addWidget( secondsText );
    QSpacerItem* spacer_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout79->addItem( spacer_2 );

    secondsEdit = new QLineEdit( this, "secondsEdit" );
    secondsEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout79->addWidget( secondsEdit );
    enterlatFormLayout->addLayout( layout79 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterlatFormLayout->addItem( spacer_3 );

    layout80 = new QHBoxLayout( 0, 0, 6, "layout80"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout80->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout80->addWidget( cancelButton );
    enterlatFormLayout->addLayout( layout80 );
    languageChange();
    resize( QSize(250, 217).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( secondsEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( checkSeconds() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( setLat() ) );

    // tab order
    setTabOrder( degreeSpinBox, minuteSpinBox );
    setTabOrder( minuteSpinBox, secondsEdit );
    setTabOrder( secondsEdit, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    degreeText->setBuddy( degreeSpinBox );
    minuteText->setBuddy( minuteSpinBox );
    secondsText->setBuddy( secondsEdit );
}

/*
 *  Destroys the object and frees any allocated resources
 */
enterlatForm::~enterlatForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void enterlatForm::languageChange()
{
    setCaption( tr( "Enter Your Latitude North" ) );
    titleLabel->setText( tr( "<i>Please enter your latitude in</i>" ) );
    textLabel2->setText( tr( "<i>degrees, minutes and seconds North</i>" ) );
    degreeText->setText( tr( "&Degrees:" ) );
    QToolTip::add( degreeSpinBox, tr( "<B>Latitude:</B> value must be between -90 and 90 degrees." ) );
    minuteText->setText( tr( "&Minutes:" ) );
    QToolTip::add( minuteSpinBox, tr( "<B>Latitude:</B> value must be between -90 and 90 degrees." ) );
    secondsText->setText( tr( "&Seconds:" ) );
    secondsEdit->setText( tr( "0" ) );
    QToolTip::add( secondsEdit, tr( "<B>Latitude:</B> value must be between -90 and 90 degrees." ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

