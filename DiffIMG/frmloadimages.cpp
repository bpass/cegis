/****************************************************************************
** Form implementation generated from reading ui file 'frmloadimages.ui'
**
** Created: Fri Oct 22 08:56:31 2004
**      by: The User Interface Compiler ($Id: frmloadimages.cpp,v 1.2 2004/10/22 15:37:22 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "frmloadimages.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "frmloadimages.ui.h"

/*
 *  Constructs a FrmLoadImages as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmLoadImages::FrmLoadImages( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FrmLoadImages" );
    FrmLoadImagesLayout = new QGridLayout( this, 1, 1, 11, 6, "FrmLoadImagesLayout"); 

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    btnShowDifference = new QPushButton( this, "btnShowDifference" );
    layout7->addWidget( btnShowDifference );

    btnClose = new QPushButton( this, "btnClose" );
    layout7->addWidget( btnClose );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout7->addItem( spacer );
    QSpacerItem* spacer_2 = new QSpacerItem( 93, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout7->addItem( spacer_2 );

    FrmLoadImagesLayout->addLayout( layout7, 0, 1 );

    originalGroup = new QGroupBox( this, "originalGroup" );
    originalGroup->setColumnLayout(0, Qt::Vertical );
    originalGroup->layout()->setSpacing( 6 );
    originalGroup->layout()->setMargin( 11 );
    originalGroupLayout = new QGridLayout( originalGroup->layout() );
    originalGroupLayout->setAlignment( Qt::AlignTop );

    layout11 = new QGridLayout( 0, 1, 1, 0, 6, "layout11"); 
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout11->addItem( spacer_3, 0, 1 );

    lblOriginal = new QLabel( originalGroup, "lblOriginal" );
    lblOriginal->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, lblOriginal->sizePolicy().hasHeightForWidth() ) );
    lblOriginal->setMinimumSize( QSize( 100, 21 ) );
    lblOriginal->setMaximumSize( QSize( 32767, 50 ) );
    lblOriginal->setAlignment( int( QLabel::AlignTop ) );

    layout11->addMultiCellWidget( lblOriginal, 0, 1, 0, 0 );

    btnLoadOriginal = new QPushButton( originalGroup, "btnLoadOriginal" );
    btnLoadOriginal->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, btnLoadOriginal->sizePolicy().hasHeightForWidth() ) );
    btnLoadOriginal->setMinimumSize( QSize( 80, 20 ) );
    btnLoadOriginal->setMaximumSize( QSize( 80, 20 ) );

    layout11->addWidget( btnLoadOriginal, 1, 1 );

    originalGroupLayout->addLayout( layout11, 0, 0 );

    layout10 = new QGridLayout( 0, 1, 1, 0, 6, "layout10"); 
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout10->addItem( spacer_4, 0, 1 );

    btnLoadReprojected = new QPushButton( originalGroup, "btnLoadReprojected" );
    btnLoadReprojected->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, btnLoadReprojected->sizePolicy().hasHeightForWidth() ) );
    btnLoadReprojected->setMinimumSize( QSize( 80, 20 ) );
    btnLoadReprojected->setMaximumSize( QSize( 80, 20 ) );

    layout10->addWidget( btnLoadReprojected, 1, 1 );

    lblReprojected = new QLabel( originalGroup, "lblReprojected" );
    lblReprojected->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, lblReprojected->sizePolicy().hasHeightForWidth() ) );
    lblReprojected->setMinimumSize( QSize( 100, 21 ) );
    lblReprojected->setMaximumSize( QSize( 32767, 50 ) );
    lblReprojected->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

    layout10->addMultiCellWidget( lblReprojected, 0, 1, 0, 0 );

    originalGroupLayout->addLayout( layout10, 1, 0 );

    lblPositions = new QLabel( originalGroup, "lblPositions" );
    lblPositions->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, lblPositions->sizePolicy().hasHeightForWidth() ) );
    lblPositions->setMinimumSize( QSize( 380, 200 ) );

    originalGroupLayout->addWidget( lblPositions, 2, 0 );

    FrmLoadImagesLayout->addWidget( originalGroup, 0, 0 );
    languageChange();
    resize( QSize(525, 399).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( btnClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( btnLoadOriginal, SIGNAL( clicked() ), this, SLOT( loadOriginal() ) );
    connect( btnLoadReprojected, SIGNAL( clicked() ), this, SLOT( loadReprojected() ) );
    connect( btnShowDifference, SIGNAL( clicked() ), this, SLOT( showDifference() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrmLoadImages::~FrmLoadImages()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FrmLoadImages::languageChange()
{
    setCaption( tr( "Difference Image Generator" ) );
    btnShowDifference->setText( tr( "&Difference Image" ) );
    btnClose->setText( tr( "&Close" ) );
    originalGroup->setTitle( tr( "Load Images:" ) );
    lblOriginal->setText( tr( "Original Image: None" ) );
    btnLoadOriginal->setText( tr( "&Load..." ) );
    btnLoadReprojected->setText( tr( "L&oad..." ) );
    lblReprojected->setText( tr( "Reprojected Image: None" ) );
    lblPositions->setText( QString::null );
}

