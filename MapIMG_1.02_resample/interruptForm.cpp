/****************************************************************************
** Form implementation generated from reading ui file 'interruptForm.ui'
**
** Created: Thu Oct 7 08:49:14 2004
**      by: The User Interface Compiler ($Id: interruptForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "interruptForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "interruptForm.ui.h"
/*
 *  Constructs a interruptForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
interruptForm::interruptForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "interruptForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 350, 125 ) );
    setSizeGripEnabled( TRUE );
    interruptFormLayout = new QVBoxLayout( this, 11, 6, "interruptFormLayout"); 

    layout191 = new QVBoxLayout( 0, 0, 6, "layout191"); 

    Layout58 = new QHBoxLayout( 0, 0, 6, "Layout58"); 

    sphereText = new QLabel( this, "sphereText" );
    sphereText->setTextFormat( QLabel::PlainText );
    sphereText->setAlignment( int( QLabel::AlignVCenter ) );
    Layout58->addWidget( sphereText );

    sphereEdit = new QLineEdit( this, "sphereEdit" );
    sphereEdit->setFrameShape( QLineEdit::LineEditPanel );
    sphereEdit->setFrameShadow( QLineEdit::Sunken );
    sphereEdit->setAlignment( int( QLineEdit::AlignRight ) );
    Layout58->addWidget( sphereEdit );
    layout191->addLayout( Layout58 );

    fefnLabel_2 = new QLabel( this, "fefnLabel_2" );
    fefnLabel_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout191->addWidget( fefnLabel_2 );
    interruptFormLayout->addLayout( layout191 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    interruptFormLayout->addItem( spacer );

    layout289 = new QHBoxLayout( 0, 0, 6, "layout289"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout289->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout289->addWidget( cancelButton );
    interruptFormLayout->addLayout( layout289 );
    languageChange();
    resize( QSize(346, 125).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( sphereEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sphereCheck(const QString&) ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );

    // tab order
    setTabOrder( sphereEdit, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    sphereText->setBuddy( sphereEdit );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
interruptForm::~interruptForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void interruptForm::languageChange()
{
    setCaption( tr( "Enter Your Data" ) );
    sphereText->setText( tr( "&Radius of the Reference Sphere:" ) );
    sphereEdit->setText( tr( "6370997.000000 " ) );
    QToolTip::add( sphereEdit, tr( "<B>Sphere Radius:</B> value must be a valid double." ) );
    fefnLabel_2->setText( tr( "<i>(In meters)</i>" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

