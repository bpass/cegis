/****************************************************************************
** Form implementation generated from reading ui file 'frmdimensions.ui'
**
** Created: Fri Apr 23 12:49:29 2004
**      by: The User Interface Compiler ($Id: frmdimensions.cpp,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "frmdimensions.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "frmdimensions.ui.h"

/* 
 *  Constructs a FrmDimensions as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmDimensions::FrmDimensions( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FrmDimensions" );
    FrmDimensionsLayout = new QGridLayout( this, 1, 1, 11, 6, "FrmDimensionsLayout"); 

    lblImage = new QLabel( this, "lblImage" );

    FrmDimensionsLayout->addWidget( lblImage, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    FrmDimensionsLayout->addItem( spacer, 1, 0 );

    layout12 = new QVBoxLayout( 0, 0, 6, "layout12"); 

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    lblRows = new QLabel( this, "lblRows" );
    layout14->addWidget( lblRows );
    QSpacerItem* spacer_2 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer_2 );

    spinRows = new QSpinBox( this, "spinRows" );
    spinRows->setMaxValue( 65535 );
    layout14->addWidget( spinRows );
    layout12->addLayout( layout14 );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    lblColumns = new QLabel( this, "lblColumns" );
    layout11->addWidget( lblColumns );
    QSpacerItem* spacer_3 = new QSpacerItem( 68, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer_3 );

    spinColumns = new QSpinBox( this, "spinColumns" );
    spinColumns->setMaxValue( 65535 );
    layout11->addWidget( spinColumns );
    layout12->addLayout( layout11 );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    lblDataType = new QLabel( this, "lblDataType" );
    layout10->addWidget( lblDataType );
    QSpacerItem* spacer_4 = new QSpacerItem( 16, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( spacer_4 );

    cmbDataType = new QComboBox( FALSE, this, "cmbDataType" );
    layout10->addWidget( cmbDataType );
    layout12->addLayout( layout10 );

    FrmDimensionsLayout->addLayout( layout12, 2, 0 );

    layout19 = new QVBoxLayout( 0, 0, 6, "layout19"); 

    btnOk = new QPushButton( this, "btnOk" );
    layout19->addWidget( btnOk );

    btnCancel = new QPushButton( this, "btnCancel" );
    layout19->addWidget( btnCancel );
    QSpacerItem* spacer_5 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout19->addItem( spacer_5 );

    FrmDimensionsLayout->addMultiCellLayout( layout19, 0, 2, 1, 1 );
    languageChange();
    resize( QSize(342, 144).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( btnOk, SIGNAL( clicked() ), this, SLOT( Ok() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrmDimensions::~FrmDimensions()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FrmDimensions::languageChange()
{
    setCaption( tr( "Set Dimensions" ) );
    lblImage->setText( tr( "Please enter dimensions for image:\n"
"None" ) );
    lblRows->setText( tr( "Rows:" ) );
    lblColumns->setText( tr( "Columns:" ) );
    lblDataType->setText( tr( "Select Data Type:" ) );
    cmbDataType->clear();
    cmbDataType->insertItem( tr( "Unsigned 8 Bit" ) );
    cmbDataType->insertItem( tr( "Signed 8 Bit" ) );
    cmbDataType->insertItem( tr( "Unsigned 16 Bit" ) );
    cmbDataType->insertItem( tr( "Signed 16 Bit" ) );
    cmbDataType->insertItem( tr( "Unsigned 32 Bit" ) );
    cmbDataType->insertItem( tr( "Signed 32 Bit" ) );
    cmbDataType->insertItem( tr( "IEEE 32 Bit Float" ) );
    cmbDataType->insertItem( tr( "IEEE 64 Bit Float" ) );
    btnOk->setText( tr( "&Ok" ) );
    btnCancel->setText( tr( "&Cancel" ) );
}

