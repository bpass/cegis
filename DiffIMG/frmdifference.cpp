/****************************************************************************
** Form implementation generated from reading ui file 'frmdifference.ui'
**
** Created: Mon Apr 26 08:23:56 2004
**      by: The User Interface Compiler ($Id: frmdifference.cpp,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "frmdifference.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "frmdifference.ui.h"

/* 
 *  Constructs a FrmDifference as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmDifference::FrmDifference( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FrmDifference" );
    FrmDifferenceLayout = new QGridLayout( this, 1, 1, 11, 6, "FrmDifferenceLayout"); 

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    differenceGroup = new QGroupBox( this, "differenceGroup" );
    differenceGroup->setColumnLayout(0, Qt::Vertical );
    differenceGroup->layout()->setSpacing( 6 );
    differenceGroup->layout()->setMargin( 11 );
    differenceGroupLayout = new QGridLayout( differenceGroup->layout() );
    differenceGroupLayout->setAlignment( Qt::AlignTop );

    pixDifference = new QLabel( differenceGroup, "pixDifference" );
    pixDifference->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, pixDifference->sizePolicy().hasHeightForWidth() ) );
    pixDifference->setScaledContents( TRUE );

    differenceGroupLayout->addWidget( pixDifference, 0, 0 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    lblDifference = new QLabel( differenceGroup, "lblDifference" );
    lblDifference->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, lblDifference->sizePolicy().hasHeightForWidth() ) );
    lblDifference->setMinimumSize( QSize( 100, 21 ) );
    lblDifference->setMaximumSize( QSize( 32767, 21 ) );
    layout7->addWidget( lblDifference );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout7->addItem( spacer );

    differenceGroupLayout->addLayout( layout7, 1, 0 );
    layout8->addWidget( differenceGroup );

    histoGroup = new QGroupBox( this, "histoGroup" );
    histoGroup->setColumnLayout(0, Qt::Vertical );
    histoGroup->layout()->setSpacing( 6 );
    histoGroup->layout()->setMargin( 11 );
    histoGroupLayout = new QGridLayout( histoGroup->layout() );
    histoGroupLayout->setAlignment( Qt::AlignTop );

    pixHisto = new QLabel( histoGroup, "pixHisto" );
    pixHisto->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, pixHisto->sizePolicy().hasHeightForWidth() ) );
    pixHisto->setScaledContents( TRUE );

    histoGroupLayout->addWidget( pixHisto, 0, 0 );
    layout8->addWidget( histoGroup );

    FrmDifferenceLayout->addLayout( layout8, 0, 0 );

    layout9 = new QVBoxLayout( 0, 0, 6, "layout9"); 

    btnSaveDifference = new QPushButton( this, "btnSaveDifference" );
    layout9->addWidget( btnSaveDifference );

    btnSaveHisto = new QPushButton( this, "btnSaveHisto" );
    layout9->addWidget( btnSaveHisto );

    btnClose = new QPushButton( this, "btnClose" );
    layout9->addWidget( btnClose );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 330, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout9->addItem( spacer_2 );

    FrmDifferenceLayout->addLayout( layout9, 0, 1 );
    languageChange();
    resize( QSize(530, 502).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( btnSaveDifference, SIGNAL( clicked() ), this, SLOT( saveImage() ) );
    connect( btnClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( btnSaveHisto, SIGNAL( clicked() ), this, SLOT( saveHisto() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrmDifference::~FrmDifference()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FrmDifference::languageChange()
{
    setCaption( tr( "Difference Image" ) );
    differenceGroup->setTitle( tr( "Difference Image:" ) );
    lblDifference->setText( tr( "Image: None" ) );
    histoGroup->setTitle( tr( "Difference Histogram:" ) );
    btnSaveDifference->setText( tr( "Save &Image" ) );
    btnSaveHisto->setText( tr( "Save &Histo" ) );
    btnClose->setText( tr( "&Close" ) );
}

