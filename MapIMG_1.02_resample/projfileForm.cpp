/****************************************************************************
** Form implementation generated from reading ui file 'projfileForm.ui'
**
** Created: Thu Oct 7 08:49:17 2004
**      by: The User Interface Compiler ($Id: projfileForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "projfileForm.h"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "projfileForm.ui.h"
/*
 *  Constructs a projfileForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
projfileForm::projfileForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "projfileForm" );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 250, 130 ) );
    setSizeGripEnabled( TRUE );
    projfileFormLayout = new QVBoxLayout( this, 11, 6, "projfileFormLayout"); 

    layout3 = new QGridLayout( 0, 1, 1, 0, 6, "layout3"); 

    pixelComboBox = new QComboBox( FALSE, this, "pixelComboBox" );

    layout3->addMultiCellWidget( pixelComboBox, 0, 0, 1, 2 );

    pixelEdit = new QLineEdit( this, "pixelEdit" );
    pixelEdit->setEnabled( FALSE );
    pixelEdit->setFrameShape( QLineEdit::LineEditPanel );
    pixelEdit->setFrameShadow( QLineEdit::Sunken );

    layout3->addWidget( pixelEdit, 1, 1 );

    meterLabel = new QLabel( this, "meterLabel" );

    layout3->addWidget( meterLabel, 1, 2 );

    PixelLabel = new QLabel( this, "PixelLabel" );
    PixelLabel->setTextFormat( QLabel::PlainText );

    layout3->addMultiCellWidget( PixelLabel, 0, 1, 0, 0 );
    projfileFormLayout->addLayout( layout3 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    projfileFormLayout->addItem( spacer );

    layout359 = new QHBoxLayout( 0, 0, 6, "layout359"); 

    okayButton = new QPushButton( this, "okayButton" );
    layout359->addWidget( okayButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout359->addWidget( cancelButton );
    projfileFormLayout->addLayout( layout359 );
    languageChange();
    resize( QSize(250, 130).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pixelComboBox, SIGNAL( activated(int) ), this, SLOT( pixelCheck() ) );
    connect( pixelEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( pixEditCheck() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( makeChanges() ) );
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );

    // tab order
    setTabOrder( pixelComboBox, pixelEdit );
    setTabOrder( pixelEdit, okayButton );
    setTabOrder( okayButton, cancelButton );

    // buddies
    PixelLabel->setBuddy( pixelComboBox );
}

/*
 *  Destroys the object and frees any allocated resources
 */
projfileForm::~projfileForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void projfileForm::languageChange()
{
    setCaption( tr( "Enter the Input Pixel Size" ) );
    pixelComboBox->clear();
    pixelComboBox->insertItem( tr( "Select a Pixel Size" ) );
    pixelComboBox->insertItem( tr( "-----------------------------" ) );
    pixelComboBox->insertItem( tr( "5 degrees" ) );
    pixelComboBox->insertItem( tr( "1 degree" ) );
    pixelComboBox->insertItem( tr( "30 minutes" ) );
    pixelComboBox->insertItem( tr( "5 minutes" ) );
    pixelComboBox->insertItem( tr( "30 arc seconds" ) );
    pixelComboBox->insertItem( tr( "Other..." ) );
    meterLabel->setText( tr( "<i>(in meters)</i>" ) );
    PixelLabel->setText( tr( "&Pixel Size:" ) );
    okayButton->setText( tr( "OK" ) );
    cancelButton->setText( tr( "Cancel" ) );
}

