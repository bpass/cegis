/****************************************************************************
** Form implementation generated from reading ui file 'mapimgform.ui'
**
** Created: Thu Oct 7 08:49:15 2004
**      by: The User Interface Compiler ($Id: mapimgform.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mapimgform.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qframe.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "mapimgform.ui.h"
/*
 *  Constructs a mapimgForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
mapimgForm::mapimgForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "mapimgForm" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 297, 428 ) );
    setMaximumSize( QSize( 700, 1040 ) );
    setSizeIncrement( QSize( 0, 0 ) );
    setBaseSize( QSize( 0, 0 ) );
    setFocusPolicy( QDialog::NoFocus );
    setSizeGripEnabled( TRUE );
    mapimgFormLayout = new QVBoxLayout( this, 11, 6, "mapimgFormLayout"); 

    layout19 = new QGridLayout( 0, 1, 1, 0, 6, "layout19"); 

    layout16 = new QVBoxLayout( 0, 0, 6, "layout16"); 

    layout42 = new QHBoxLayout( 0, 0, 6, "layout42"); 

    inFilenameEdit = new QLineEdit( this, "inFilenameEdit" );
    inFilenameEdit->setEnabled( TRUE );
    inFilenameEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, inFilenameEdit->sizePolicy().hasHeightForWidth() ) );
    inFilenameEdit->setFrameShape( QLineEdit::PopupPanel );
    inFilenameEdit->setFrameShadow( QLineEdit::Sunken );
    inFilenameEdit->setReadOnly( FALSE );
    layout42->addWidget( inFilenameEdit );

    infileButton = new QPushButton( this, "infileButton" );
    infileButton->setFocusPolicy( QPushButton::TabFocus );
    layout42->addWidget( infileButton );
    layout16->addLayout( layout42 );

    inputDataTypeComboBox = new QComboBox( FALSE, this, "inputDataTypeComboBox" );
    layout16->addWidget( inputDataTypeComboBox );

    inputProjectionComboBox = new QComboBox( FALSE, this, "inputProjectionComboBox" );
    inputProjectionComboBox->setSizeLimit( 15 );
    layout16->addWidget( inputProjectionComboBox );

    numRowsSpin = new QSpinBox( this, "numRowsSpin" );
    numRowsSpin->setEnabled( FALSE );
    numRowsSpin->setFocusPolicy( QSpinBox::StrongFocus );
    numRowsSpin->setMaxValue( 1000000 );
    numRowsSpin->setMinValue( 0 );
    numRowsSpin->setValue( 0 );
    layout16->addWidget( numRowsSpin );

    numColsSpin = new QSpinBox( this, "numColsSpin" );
    numColsSpin->setEnabled( FALSE );
    numColsSpin->setMaxValue( 1000000 );
    numColsSpin->setValue( 0 );
    layout16->addWidget( numColsSpin );

    layout19->addLayout( layout16, 1, 1 );

    inInfoText = new QLabel( this, "inInfoText" );
    inInfoText->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, inInfoText->sizePolicy().hasHeightForWidth() ) );
    inInfoText->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    layout19->addMultiCellWidget( inInfoText, 0, 0, 0, 1 );

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    inFilenameLabel = new QLabel( this, "inFilenameLabel" );
    layout15->addWidget( inFilenameLabel );

    inputBitageText = new QLabel( this, "inputBitageText" );
    layout15->addWidget( inputBitageText );

    inputProjectionText = new QLabel( this, "inputProjectionText" );
    layout15->addWidget( inputProjectionText );

    numRowsText = new QLabel( this, "numRowsText" );
    layout15->addWidget( numRowsText );

    numColsText = new QLabel( this, "numColsText" );
    layout15->addWidget( numColsText );

    layout19->addLayout( layout15, 1, 0 );
    mapimgFormLayout->addLayout( layout19 );

    line7 = new QFrame( this, "line7" );
    line7->setFrameShape( QFrame::HLine );
    line7->setFrameShadow( QFrame::Sunken );
    line7->setFrameShape( QFrame::HLine );
    mapimgFormLayout->addWidget( line7 );

    layout29 = new QVBoxLayout( 0, 0, 6, "layout29"); 

    outInfoText = new QLabel( this, "outInfoText" );
    outInfoText->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout29->addWidget( outInfoText );

    layout17 = new QGridLayout( 0, 1, 1, 0, 6, "layout17"); 

    layout56 = new QVBoxLayout( 0, 0, 6, "layout56"); 

    layout51 = new QVBoxLayout( 0, 0, 6, "layout51"); 

    outFilenameLabel = new QLabel( this, "outFilenameLabel" );
    layout51->addWidget( outFilenameLabel );

    outputProjectionText = new QLabel( this, "outputProjectionText" );
    layout51->addWidget( outputProjectionText );
    layout56->addLayout( layout51 );

    pixelText = new QLabel( this, "pixelText" );
    layout56->addWidget( pixelText );

    layout17->addLayout( layout56, 0, 0 );

    layout16_2 = new QVBoxLayout( 0, 0, 6, "layout16_2"); 

    layout46 = new QHBoxLayout( 0, 0, 6, "layout46"); 

    outFilenameEdit = new QLineEdit( this, "outFilenameEdit" );
    outFilenameEdit->setEnabled( TRUE );
    outFilenameEdit->setFrameShape( QLineEdit::LineEditPanel );
    outFilenameEdit->setFrameShadow( QLineEdit::Sunken );
    outFilenameEdit->setReadOnly( FALSE );
    layout46->addWidget( outFilenameEdit );

    outfileButton = new QPushButton( this, "outfileButton" );
    outfileButton->setFocusPolicy( QPushButton::TabFocus );
    layout46->addWidget( outfileButton );
    layout16_2->addLayout( layout46 );

    projectionComboBox = new QComboBox( FALSE, this, "projectionComboBox" );
    projectionComboBox->setSizeLimit( 15 );
    layout16_2->addWidget( projectionComboBox );

    pixelComboBox = new QComboBox( FALSE, this, "pixelComboBox" );
    layout16_2->addWidget( pixelComboBox );

    layout15_2 = new QHBoxLayout( 0, 0, 6, "layout15_2"); 

    pixelEdit = new QLineEdit( this, "pixelEdit" );
    pixelEdit->setEnabled( FALSE );
    layout15_2->addWidget( pixelEdit );

    pixsizeLabel = new QLabel( this, "pixsizeLabel" );
    pixsizeLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout15_2->addWidget( pixsizeLabel );
    layout16_2->addLayout( layout15_2 );

    layout17->addLayout( layout16_2, 0, 1 );
    layout29->addLayout( layout17 );
    mapimgFormLayout->addLayout( layout29 );

    line5 = new QFrame( this, "line5" );
    line5->setFrameShape( QFrame::HLine );
    line5->setFrameShadow( QFrame::Sunken );
    line5->setFrameShape( QFrame::HLine );
    mapimgFormLayout->addWidget( line5 );

    layout63 = new QHBoxLayout( 0, 0, 6, "layout63"); 

    fillvalText = new QLabel( this, "fillvalText" );
    layout63->addWidget( fillvalText );

    fillvalSpin = new QSpinBox( this, "fillvalSpin" );
    fillvalSpin->setMaxValue( 999999999 );
    fillvalSpin->setLineStep( 1 );
    layout63->addWidget( fillvalSpin );
    mapimgFormLayout->addLayout( layout63 );

    line6 = new QFrame( this, "line6" );
    line6->setFrameShape( QFrame::HLine );
    line6->setFrameShadow( QFrame::Sunken );
    line6->setFrameShape( QFrame::HLine );
    mapimgFormLayout->addWidget( line6 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    clearRadio = new QRadioButton( buttonGroup1, "clearRadio" );
    clearRadio->setChecked( TRUE );
    buttonGroup1Layout->addWidget( clearRadio );

    keepInputRadio = new QRadioButton( buttonGroup1, "keepInputRadio" );
    buttonGroup1Layout->addWidget( keepInputRadio );

    moveOutputRadio = new QRadioButton( buttonGroup1, "moveOutputRadio" );
    buttonGroup1Layout->addWidget( moveOutputRadio );
    mapimgFormLayout->addWidget( buttonGroup1 );

    layout15_3 = new QHBoxLayout( 0, 0, 6, "layout15_3"); 

    okayButton = new QPushButton( this, "okayButton" );
    okayButton->setFocusPolicy( QPushButton::TabFocus );
    layout15_3->addWidget( okayButton );

    exitButton = new QPushButton( this, "exitButton" );
    exitButton->setFocusPolicy( QPushButton::TabFocus );
    layout15_3->addWidget( exitButton );
    mapimgFormLayout->addLayout( layout15_3 );
    languageChange();
    resize( QSize(350, 520).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( okayButton, SIGNAL( clicked() ), this, SLOT( executemapimg() ) );
    connect( infileButton, SIGNAL( clicked() ), this, SLOT( getInfile() ) );
    connect( outfileButton, SIGNAL( clicked() ), this, SLOT( getOutfile() ) );
    connect( pixelEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( pixEditCheck() ) );
    connect( pixelComboBox, SIGNAL( activated(int) ), this, SLOT( pixelCheck() ) );
    connect( exitButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( inputProjectionComboBox, SIGNAL( activated(const QString&) ), this, SLOT( comboCheckIn() ) );
    connect( projectionComboBox, SIGNAL( activated(const QString&) ), this, SLOT( comboCheck() ) );
    connect( inFilenameEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( infileValid(const QString&) ) );
    connect( outFilenameEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( outfileValid(const QString&) ) );
    connect( numColsSpin, SIGNAL( valueChanged(int) ), this, SLOT( calcDataType() ) );
    connect( numRowsSpin, SIGNAL( valueChanged(int) ), this, SLOT( calcDataType() ) );
    connect( inputDataTypeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( comboCheckData() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
mapimgForm::~mapimgForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mapimgForm::languageChange()
{
    setCaption( tr( "  U.S. Geological Survey -- MapIMG" ) );
    inFilenameEdit->setText( QString::null );
    infileButton->setText( tr( "Browse..." ) );
    inputDataTypeComboBox->clear();
    inputDataTypeComboBox->insertItem( tr( "Select Image Data Type" ) );
    inputDataTypeComboBox->insertItem( tr( "------------------------------" ) );
    inputDataTypeComboBox->insertItem( tr( "Unsigned 8 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "Signed 8 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "Unsigned 16 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "Signed 16 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "Unsigned 32 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "Signed 32 Bit" ) );
    inputDataTypeComboBox->insertItem( tr( "IEEE 32 Bit Float" ) );
    inputDataTypeComboBox->insertItem( tr( "IEEE 64 Bit Float" ) );
    inputProjectionComboBox->clear();
    inputProjectionComboBox->insertItem( tr( "Select a Projection" ) );
    inputProjectionComboBox->insertItem( tr( "------------------------------" ) );
    inputProjectionComboBox->insertItem( tr( "Geographic" ) );
    inputProjectionComboBox->insertItem( tr( "UTM" ) );
    inputProjectionComboBox->insertItem( tr( "State Plane" ) );
    inputProjectionComboBox->insertItem( tr( "Albers Equal Area" ) );
    inputProjectionComboBox->insertItem( tr( "Lambert Conformal Conic" ) );
    inputProjectionComboBox->insertItem( tr( "Mercator" ) );
    inputProjectionComboBox->insertItem( tr( "Polar Stereographic" ) );
    inputProjectionComboBox->insertItem( tr( "Polyconic" ) );
    inputProjectionComboBox->insertItem( tr( "Equidistant Conic A" ) );
    inputProjectionComboBox->insertItem( tr( "Equidistant Conic B" ) );
    inputProjectionComboBox->insertItem( tr( "Transverse Mercator" ) );
    inputProjectionComboBox->insertItem( tr( "Stereographic" ) );
    inputProjectionComboBox->insertItem( tr( "Lambert Azimuthal" ) );
    inputProjectionComboBox->insertItem( tr( "Azimuthal" ) );
    inputProjectionComboBox->insertItem( tr( "Gnomonic" ) );
    inputProjectionComboBox->insertItem( tr( "Orthographic" ) );
    inputProjectionComboBox->insertItem( tr( "Gen. Vert. Near Per" ) );
    inputProjectionComboBox->insertItem( tr( "Sinusiodal" ) );
    inputProjectionComboBox->insertItem( tr( "Equirectangular" ) );
    inputProjectionComboBox->insertItem( tr( "Miller Cylindrical" ) );
    inputProjectionComboBox->insertItem( tr( "Van der Grinten" ) );
    inputProjectionComboBox->insertItem( tr( "Hotine Oblique Merc A" ) );
    inputProjectionComboBox->insertItem( tr( "Hotine Oblique Merc B" ) );
    inputProjectionComboBox->insertItem( tr( "Robinson" ) );
    inputProjectionComboBox->insertItem( tr( "Space Oblique Merc A" ) );
    inputProjectionComboBox->insertItem( tr( "Space Oblique Merc B" ) );
    inputProjectionComboBox->insertItem( tr( "Alaska Conformal" ) );
    inputProjectionComboBox->insertItem( tr( "Interrupted Goode" ) );
    inputProjectionComboBox->insertItem( tr( "Mollweide" ) );
    inputProjectionComboBox->insertItem( tr( "Interrupted Mollweide" ) );
    inputProjectionComboBox->insertItem( tr( "Hammer" ) );
    inputProjectionComboBox->insertItem( tr( "Wagner IV" ) );
    inputProjectionComboBox->insertItem( tr( "Wagner VII" ) );
    inputProjectionComboBox->insertItem( tr( "Oblated Equal Area" ) );
    inputProjectionComboBox->insertItem( tr( "Integerized Sinusoid" ) );
    inInfoText->setText( tr( "<u>Input Parameters</u>" ) );
    inFilenameLabel->setText( tr( "Input File Name:" ) );
    inputBitageText->setText( tr( "Data Type:" ) );
    inputProjectionText->setText( tr( "Projection:" ) );
    numRowsText->setText( tr( "Rows:" ) );
    numColsText->setText( tr( "Columns:" ) );
    outInfoText->setText( tr( "<u>Desired Output Parameters</u>" ) );
    outFilenameLabel->setText( tr( "Output File Name:" ) );
    outputProjectionText->setText( tr( "Projection:" ) );
    pixelText->setText( tr( "Pixel Size:" ) );
    outFilenameEdit->setText( QString::null );
    outfileButton->setText( tr( "Browse..." ) );
    projectionComboBox->clear();
    projectionComboBox->insertItem( tr( "Select a Projection" ) );
    projectionComboBox->insertItem( tr( "------------------------------" ) );
    projectionComboBox->insertItem( tr( "UTM" ) );
    projectionComboBox->insertItem( tr( "State Plane" ) );
    projectionComboBox->insertItem( tr( "Albers Equal Area" ) );
    projectionComboBox->insertItem( tr( "Lambert Conformal Conic" ) );
    projectionComboBox->insertItem( tr( "Mercator" ) );
    projectionComboBox->insertItem( tr( "Polar Stereographic" ) );
    projectionComboBox->insertItem( tr( "Polyconic" ) );
    projectionComboBox->insertItem( tr( "Equidistant Conic A" ) );
    projectionComboBox->insertItem( tr( "Equidistant Conic B" ) );
    projectionComboBox->insertItem( tr( "Transverse Mercator" ) );
    projectionComboBox->insertItem( tr( "Stereographic" ) );
    projectionComboBox->insertItem( tr( "Lambert Azimuthal" ) );
    projectionComboBox->insertItem( tr( "Azimuthal" ) );
    projectionComboBox->insertItem( tr( "Gnomonic" ) );
    projectionComboBox->insertItem( tr( "Orthographic" ) );
    projectionComboBox->insertItem( tr( "Gen. Vert. Near Per" ) );
    projectionComboBox->insertItem( tr( "Sinusiodal" ) );
    projectionComboBox->insertItem( tr( "Equirectangular" ) );
    projectionComboBox->insertItem( tr( "Miller Cylindrical" ) );
    projectionComboBox->insertItem( tr( "Van der Grinten" ) );
    projectionComboBox->insertItem( tr( "Hotine Oblique Merc A" ) );
    projectionComboBox->insertItem( tr( "Hotine Oblique Merc B" ) );
    projectionComboBox->insertItem( tr( "Robinson" ) );
    projectionComboBox->insertItem( tr( "Space Oblique Merc A" ) );
    projectionComboBox->insertItem( tr( "Space Oblique Merc B" ) );
    projectionComboBox->insertItem( tr( "Alaska Conformal" ) );
    projectionComboBox->insertItem( tr( "Interrupted Goode" ) );
    projectionComboBox->insertItem( tr( "Mollweide" ) );
    projectionComboBox->insertItem( tr( "Interrupted Mollweide" ) );
    projectionComboBox->insertItem( tr( "Hammer" ) );
    projectionComboBox->insertItem( tr( "Wagner IV" ) );
    projectionComboBox->insertItem( tr( "Wagner VII" ) );
    projectionComboBox->insertItem( tr( "Oblated Equal Area" ) );
    projectionComboBox->insertItem( tr( "Integerized Sinusoid" ) );
    pixelComboBox->clear();
    pixelComboBox->insertItem( tr( "Select a Pixel Size" ) );
    pixelComboBox->insertItem( tr( "-----------------------------" ) );
    pixelComboBox->insertItem( tr( "5 degrees" ) );
    pixelComboBox->insertItem( tr( "1 degree" ) );
    pixelComboBox->insertItem( tr( "30 minutes" ) );
    pixelComboBox->insertItem( tr( "5 minutes" ) );
    pixelComboBox->insertItem( tr( "30 arc seconds" ) );
    pixelComboBox->insertItem( tr( "Meters..." ) );
    pixsizeLabel->setText( tr( "(in meters)" ) );
    fillvalText->setText( tr( "Fill Value:" ) );
    buttonGroup1->setTitle( tr( "Options Upon Completion" ) );
    clearRadio->setText( tr( "Clear All Parameters" ) );
    keepInputRadio->setText( tr( "Keep Same Input Parameters" ) );
    moveOutputRadio->setText( tr( "Move Output Parameters to Input Parameters" ) );
    okayButton->setText( tr( "OK" ) );
    exitButton->setText( tr( "Exit" ) );
}

