/****************************************************************************
** Form implementation generated from reading ui file 'enterlonForm.ui'
**
** Created: Thu Oct 7 08:49:12 2004
**      by: The User Interface Compiler ($Id: enterlonForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "enterlonForm.h"

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

#include "enterlonForm.ui.h"
/*
 *  Constructs a enterlonForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
enterlonForm::enterlonForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "enterlonForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 220 ) );
    setSizeGripEnabled( TRUE );
    enterlonFormLayout = new QVBoxLayout( this, 11, 6, "enterlonFormLayout"); 

    layout91 = new QVBoxLayout( 0, 0, 6, "layout91"); 

    titleLabel = new QLabel( this, "titleLabel" );
    titleLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout91->addWidget( titleLabel );

    textLabel3 = new QLabel( this, "textLabel3" );
    textLabel3->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout91->addWidget( textLabel3 );
    enterlonFormLayout->addLayout( layout91 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterlonFormLayout->addItem( spacer );

    Layout226 = new QHBoxLayout( 0, 0, 6, "Layout226"); 

    degreeText = new QLabel( this, "degreeText" );
    degreeText->setTextFormat( QLabel::PlainText );
    Layout226->addWidget( degreeText );

    degreeSpinBox = new QSpinBox( this, "degreeSpinBox" );
    degreeSpinBox->setMaxValue( 180 );
    degreeSpinBox->setMinValue( -180 );
    Layout226->addWidget( degreeSpinBox );
    enterlonFormLayout->addLayout( Layout226 );

    Layout227 = new QHBoxLayout( 0, 0, 6, "Layout227"); 

    minuteText = new QLabel( this, "minuteText" );
    minuteText->setTextFormat( QLabel::PlainText );
    Layout227->addWidget( minuteText );

    minuteSpinBox = new QSpinBox( this, "minuteSpinBox" );
    minuteSpinBox->setMaxValue( 60 );
    Layout227->addWidget( minuteSpinBox );
    enterlonFormLayout->addLayout( Layout227 );

    layout90 = new QHBoxLayout( 0, 0, 6, "layout90"); 

    secondsText = new QLabel( this, "secondsText" );
    secondsText->setTextFormat( QLabel::PlainText );
    layout90->addWidget( secondsText );
    QSpacerItem* spacer_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout90->addItem( spacer_2 );

    secondsEdit = new QLineEdit( this, "secondsEdit" );
    secondsEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout90->addWidget( secondsEdit );
    enterlonFormLayout->addLayout( layout90 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    enterlonFormLayout->addItem( spacer_3 );

    layout89 = new QHBoxLayout( 0, 0, 6, "layout89"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout89->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout89->addWidget( cancelButton );
    enterlonFormLayout->addLayout( layout89 );
    languageChange();
    resize( QSize(250, 220).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( secondsEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( checkSeconds() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( setLon() ) );

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
enterlonForm::~enterlonForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void enterlonForm::languageChange()
{
    setCaption( tr( "Enter Your Longitude East" ) );
    titleLabel->setText( tr( "<i>Please enter your longitude in</i>" ) );
    textLabel3->setText( tr( "<i>degrees, minutes and seconds East</i>" ) );
    degreeText->setText( tr( "&Degrees:" ) );
    QToolTip::add( degreeSpinBox, tr( "<B>Longitude:</B> value must be between -180 and 180 degrees." ) );
    minuteText->setText( tr( "&Minutes:" ) );
    QToolTip::add( minuteSpinBox, tr( "<B>Longitude:</B> value must be between -180 and 180 degrees." ) );
    secondsText->setText( tr( "&Seconds:" ) );
    secondsEdit->setText( tr( "0" ) );
    QToolTip::add( secondsEdit, tr( "<B>Longitude:</B> value must be between -180 and 180 degrees." ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

