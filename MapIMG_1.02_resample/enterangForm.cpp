/****************************************************************************
** Form implementation generated from reading ui file 'enterangForm.ui'
**
** Created: Thu Oct 7 08:49:11 2004
**      by: The User Interface Compiler ($Id: enterangForm.cpp,v 1.1 2004/10/20 16:36:39 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "enterangForm.h"

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

#include "enterangForm.ui.h"
/*
 *  Constructs a enterangForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
enterangForm::enterangForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "enterangForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 220, 220 ) );
    setSizeGripEnabled( TRUE );
    enterangFormLayout = new QVBoxLayout( this, 11, 6, "enterangFormLayout"); 

    layout60 = new QVBoxLayout( 0, 0, 6, "layout60"); 

    titleLabel = new QLabel( this, "titleLabel" );
    titleLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout60->addWidget( titleLabel );

    titleLabel2 = new QLabel( this, "titleLabel2" );
    titleLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout60->addWidget( titleLabel2 );
    enterangFormLayout->addLayout( layout60 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterangFormLayout->addItem( spacer );

    Layout226 = new QHBoxLayout( 0, 0, 6, "Layout226"); 

    degreeText = new QLabel( this, "degreeText" );
    degreeText->setTextFormat( QLabel::PlainText );
    Layout226->addWidget( degreeText );

    degreeSpinBox = new QSpinBox( this, "degreeSpinBox" );
    degreeSpinBox->setMaxValue( 360 );
    degreeSpinBox->setMinValue( 0 );
    Layout226->addWidget( degreeSpinBox );
    enterangFormLayout->addLayout( Layout226 );

    Layout227 = new QHBoxLayout( 0, 0, 6, "Layout227"); 

    minuteText = new QLabel( this, "minuteText" );
    minuteText->setTextFormat( QLabel::PlainText );
    Layout227->addWidget( minuteText );

    minuteSpinBox = new QSpinBox( this, "minuteSpinBox" );
    minuteSpinBox->setMaxValue( 60 );
    Layout227->addWidget( minuteSpinBox );
    enterangFormLayout->addLayout( Layout227 );

    layout59 = new QHBoxLayout( 0, 0, 6, "layout59"); 

    secondsText = new QLabel( this, "secondsText" );
    secondsText->setTextFormat( QLabel::PlainText );
    layout59->addWidget( secondsText );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout59->addItem( spacer_2 );

    secondsEdit = new QLineEdit( this, "secondsEdit" );
    secondsEdit->setFrameShape( QLineEdit::LineEditPanel );
    secondsEdit->setFrameShadow( QLineEdit::Sunken );
    secondsEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout59->addWidget( secondsEdit );
    enterangFormLayout->addLayout( layout59 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterangFormLayout->addItem( spacer_3 );

    layout57 = new QHBoxLayout( 0, 0, 6, "layout57"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout57->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout57->addWidget( cancelButton );
    enterangFormLayout->addLayout( layout57 );
    languageChange();
    resize( QSize(220, 220).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( secondsEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( checkSeconds() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( setAng() ) );

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
enterangForm::~enterangForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void enterangForm::languageChange()
{
    setCaption( tr( "Enter Your Angle" ) );
    titleLabel->setText( tr( "<i>Please enter your longitude in</i>" ) );
    titleLabel2->setText( tr( "<i>degrees, minutes and seconds</i>" ) );
    degreeText->setText( tr( "&Degrees:" ) );
    QToolTip::add( degreeSpinBox, tr( "<B>Angle:</B> value must be between 0 and 360 degrees." ) );
    minuteText->setText( tr( "&Minutes:" ) );
    QToolTip::add( minuteSpinBox, tr( "<B>Angle:</B> value must be between 0 and 360 degrees." ) );
    secondsText->setText( tr( "&Seconds:" ) );
    secondsEdit->setText( tr( "0" ) );
    QToolTip::add( secondsEdit, tr( "<B>Angle:</B> value must be between 0 and 360 degrees." ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

