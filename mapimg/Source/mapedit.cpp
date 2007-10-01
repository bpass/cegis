#include <QCheckBox>
#include <QColor>
#include <QColorGroup>
#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPalette>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QSizePolicy>
#include <QSpinBox>
#include <QString>
#include <Qt>
#include <QToolTip>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

const uint INFO_PRECISION = 6;

#include "gctpnames.h"
#include "mapedit.h"
#include "mapimgedit.h"	//QInfoFrame class
#include "mapimgpalette.h"
#include "mapimgvalidator.h"


/*
The MapEdit has one constructor. It executes in three stages.

STAGE 1: It sets its appearance by turning off the Horizontal SrcollBar, 
forcing on the Vertical ScrollBar, and generating the 'contents' widget for
holding all the contents.

STAGE 2: It creates all the QWidgets and their respective QHBoxes in which
they all reside.

STAGE 3: A level of polish is applied to every widget. Adding QTooltips,
applying QValidators, fixing sizes, and making connections happens here.
*/
MapEdit::MapEdit( QWidget* parent )
: QScrollArea( parent )	
{
	appearanceSetup();
	generateWidgets();
	polishWidgets();

		//Initialize Strings
	lastFillValue = "";
	lastNoDataValue = "";

	connect( copyButton, SIGNAL( clicked() ), this, emit SIGNAL( copyButtonClicked() ) );
	connect( lockButton, SIGNAL(toggled(bool)), this, emit SIGNAL( lockButtonToggled() ) );
	connect( frameButton, SIGNAL( clicked() ), this, emit SIGNAL( frameButtonClicked() ) );
	connect( fillButton, SIGNAL( clicked() ), this, emit SIGNAL( fillButtonClicked() ) );
	connect( dataCombo, SIGNAL(activated(int)), this, emit SIGNAL(dataComboActivated(int)));
	connect( fillEdit, SIGNAL( textChanged( const QString & ) ), this, 
		emit SIGNAL( fillEditChanged( const QString & ) ) );
	connect( noDataEdit, SIGNAL( textChanged( const QString & ) ), this,
		emit SIGNAL( noDataEditChanged( const QSting &) ) );
	connect( noDataEdit, SIGNAL( textChanged( const QString & ) ), this, 
		emit SIGNAL( changeMyPartner() ) );
	connect( hasNoDataCheck, SIGNAL(toggled()), this, SLOT( QInfoFrame::partnerChanged()));
}

/*
Nothing to destruct because QObjects take care of their children.
*/
MapEdit::~MapEdit()
{}


/* eventFilter is an event filter for misc GUI events. Right now it is used
to dsiplay a popup menu when a user right-clicks on the fill value edit box
*/
bool MapEdit::eventFilter( QObject* object, QEvent* event )
{
	if( (object == fillEdit) && (event->type() == QEvent::MouseButtonPress ) )
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if( mouseEvent->button() == Qt::RightButton )
		{
			mouseEvent->accept();
			/*
			QPopupMenu* estimatePopup = new QPopupMenu( this, "estimatePopup" );
			estimatePopup->insertItem( "Estimate Value", fillButton, SLOT( animateClick() ) );

			estimatePopup->exec( mouseEvent->globalPos() );
			delete estimatePopup;
			*/
			return true;
		}
		else
		{
			return QScrollArea::eventFilter( object, event );
		}
	}

	return QScrollArea::eventFilter( object, event );
}


void MapEdit::fillCheckToggled( bool state )
{
	if(fillEdit->validator() != 0 )
		((MapimgValidator*)fillEdit->validator())->setAllowUndefined( !state );

	if( state == Qt::Unchecked ) 
	{
		lastFillValue = fillEdit->text();
		fillEdit->setText( "Undefined" );
		fillEdit->setDisabled( true );
	}
	else
	{
		fillEdit->setText( lastFillValue );
		fillEdit->setDisabled( false );
	}

	if( fillEdit->validator() != 0 )
	{
		QString fillString = fillEdit->text();
		((MapimgValidator*)fillEdit->validator())->fixup( fillString );
		fillEdit->setText( fillString );
	}

	return;
}

void MapEdit::noDataCheckToggled( bool state )
{
	if( noDataEdit->validator() != 0 )
		((MapimgValidator*)noDataEdit->validator())->setAllowUndefined( !state );

	if( !state )
	{
		lastNoDataValue = noDataEdit->text();
		noDataEdit->setText( "Undefined" );
		noDataEdit->setDisabled( true );
	}
	else
	{
		noDataEdit->setText( lastNoDataValue );
		noDataEdit->setDisabled( false );
		hasNoDataCheck->setDisabled( false );
	}

	if( noDataEdit->validator() != 0 )
	{
		QString noDataString = noDataEdit->text();
		((MapimgValidator*)noDataEdit->validator())->fixup( noDataString );
		noDataEdit->setText( noDataString );
	}
	emit changeMyPartner();
	return;
}

/*
The pixelChange(int) function is connected to the pixelCombo so that
whenever the user selects a different pixel size this function is called.
Its purpose is to keep the pixelEdit hidden until it is needed. The pixelEdit
is used for entering custom pixel sizes(meters).
*/
void MapEdit::pixelChange(int index)
{
	if( index > 0 && index < 6 )
	{
		pixelEdit->setText( pixelValues[index-1] );
	}
	pixelEdit->setShown( index == 6 );

    if( index == 6)
		contents->resize( size().width(), size().height() );
}

/*
The dataChange(qstring) function is connected to the dataCombo so that
whenever the user selects a different data type this function is called.
Its purpose is to keep the fill value and no data validators up to date
with which data type it is trying to validate.
*/
void MapEdit::dataChange( const QString& newDataType )
{
	QString tempText = "";

	if( newDataType.trimmed() != "" )
	{
		((MapimgValidator*)fillEdit->validator())->setDataType( newDataType );
		tempText = fillEdit->text();
		fillEdit->validator()->fixup( tempText );
		fillEdit->setText( tempText );

		((MapimgValidator*)noDataEdit->validator())->setDataType( newDataType );
		tempText = noDataEdit->text();
		noDataEdit->validator()->fixup( tempText );
		noDataEdit->setText( tempText );
	}

	return;
}

/*This slot resets the fields that are displayed by the mapedit object*/
void MapEdit::reset()
{
	fileEdit->setText( "" );
	rowSpin->setValue( 0 );
	colSpin->setValue( 0 );
	unitCombo->setCurrentIndex( 3 );
	spheroidCombo->setCurrentIndex( 20 );
	pixelCombo->setCurrentIndex( 0 );
	pixelEdit->setText( "0.000000" );
	pixelEdit->setShown( false );
	ulLonEdit->setText( "0.000000" );
	ulLatEdit->setText( "0.000000" );
	dataCombo->setCurrentIndex( 0 );
	fillEdit->setText( "0.000000" );
	noDataEdit->setText( "0.000000" );
	hasFillCheck->setChecked( true );
	hasNoDataCheck->setChecked( true );
}

/*
This function is mainly used by the lock(bool) function to restrict or
allow all access to the parameters found in a MapEdit
*/
void MapEdit::setRO( bool ro, int inOut )
{
	rowSpin->setDisabled( ro );
	colSpin->setDisabled( ro );
	unitCombo->setDisabled( true ); //only meters are supported
	spheroidCombo->setDisabled( true );
	pixelCombo->setDisabled( ro );
	pixelEdit->setDisabled( ro );
	ulLonEdit->setDisabled( ro );
	ulLatEdit->setDisabled( ro );
	dataCombo->setDisabled( ro );

	if(inOut == 1 )
	{
		pixelCombo->setDisabled( false );
		pixelEdit->setDisabled( false );
	}

	hasFillCheck->setHidden( ro );
	hasFillCheck->setDisabled( ro );
	fillButton->setDisabled( ro );
	hasNoDataCheck->setHidden( ro );
	hasNoDataCheck->setDisabled( ro );

	if( ro || (!ro && hasFillCheck->isChecked()) )
		fillEdit->setDisabled( ro );
	if( ro || (!ro && hasNoDataCheck->isChecked()) )
		noDataEdit->setDisabled( ro );

}

/*
Similar to the fixWidth(uint) function, the setAsInput() and setAsOutput()
functions are practical for my purposes in mapimg. mapimg needed to info
editors, one for input and one for output. The differences are significant but
but not enough to warrant a new class with so much repeated code. Also, I was
too lazy to create a base class and two subclasses.

The setAs*() fuctions are not repeatable. Do not call setAsInput() and then
setAsOutput(). These functions were not made for that purpose. They are merely
here as an additional step in initialization.
*/
void MapEdit::setAsInput()
{
	findChild<QLabel*>( "mapLabel" )->setText( "Input Map Info" );
	fileEdit->setDisabled( true );
	copyButton->hide();
	lockButton->show();
	fillButton->hide();

	QPalette p( QColor( 151, 160, 148 ) ); //INPUT_COLOR mapimgpalette.h
	p.setColor( QPalette::Text, p.color( QPalette::Text ) );
	viewport()->setPalette( p );
}

/*
See comments on setAsInput().
*/
void MapEdit::setAsOutput()
{
	findChild<QLabel*>( "mapLabel" )->setText( "Output Map Info" );
	fileEdit->setDisabled( true );
	fileEdit->setText( "Use Save button to reproject" );
	copyButton->show();
	lockButton->hide();
	fillButton->hide();

	QPalette p( QColor( 163, 146, 146 ) ); //OUTPUT_COLOR mapimgpalette.h
	p.setColor( QPalette::Text, p.color( QPalette::Text ) );
	viewport()->setPalette( p );

	rowSpin->setDisabled( true );
	colSpin->setDisabled( true );
	ulLonEdit->setDisabled( true );
	ulLatEdit->setDisabled( true );
	dataCombo->setDisabled( true );
	fillEdit->setDisabled( true );
	noDataEdit->setDisabled( true );

	hasFillCheck->setDisabled( true );
	hasFillCheck->setHidden( true );
	hasFillCheck->setChecked( true );

	hasNoDataCheck->setDisabled( false );
	hasNoDataCheck->setHidden( true );
	hasNoDataCheck->setChecked( true );
}

/*
The frame() function calls the mapimg::frameIt() function for calculating
the ulLatitude, ulLongitude, rows, and columns of the raster based on all
other parameters.
*/
void MapEdit::setFrameInfo( int rows, int cols, QString ul_X, QString ul_Y )
{
	rowSpin->setValue( rows );
	colSpin->setValue( cols );
	ulLonEdit->setText( ul_X );
	ulLatEdit->setText( ul_Y );
}

void MapEdit::getFill( QString fillString )
{
	if( fillEdit->validator() != 0 )
	{
		fillEdit->validator()->fixup( fillString );
	}

	if( QMessageBox::question( 0,
		"Replace Fill Value?",
		QString("Are you sure you wish to replace the current fill "
		"value (%1) with the new estimated value of %2?")
		.arg( fillEdit->text() ).arg( fillString ),
		QMessageBox::Yes,
		QMessageBox::No | QMessageBox::Default | QMessageBox::Escape ) == QMessageBox::Yes )
	{
		fillEdit->setText( fillString );
	}
}

void MapEdit::appearanceSetup()
{
	////////STAGE 1: Appearance/Setup
	//
		//Allows the contents widget to resize if something is added or taken out
	setWidgetResizable( true );	
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	
	contents = new QWidget( viewport(), 0 );	

		//Created a mainLayout to inorder to insert multiple Layouts into 
	mainLayout = new QVBoxLayout( contents );	//the contents widget
	mainLayout->setAlignment( Qt::AlignTop );
	mainLayout->setMargin( 5 );		
	mainLayout->setSpacing( 3 );	
	contents->setLayout( mainLayout );	
}

void MapEdit::generateWidgets()
{
	////////STAGE 2: Generate Widgets
	//
	//titleBox - Contains function buttons and the title of the Tab
	titleBox = new QHBoxLayout( 0 );
	titleBox->setParent( mainLayout );
	mainLayout->addLayout( titleBox );

	copyButton = new QPushButton( QIcon( ":/Resources/copy.png" ),
		"", this );
	lockButton = new QPushButton( QIcon( ":/Resources/unlocked.png" ),
		"", this );
	mapLabel = new QLabel("Map Info", this );
	mapLabel->setObjectName( "mapLabel" );
	frameButton = new QPushButton( QIcon( ":/Resources/calculate.png" ),
		"", this );	
	
		//Adding widgets to the titleBox group layout
	titleBox->addWidget( copyButton );	
	titleBox->addWidget( lockButton );	
	titleBox->addWidget( mapLabel );
	titleBox->addWidget( frameButton );		

	//fileBox - Contains a lineedit for displaying the ".info" filename
	fileBox = new QVBoxLayout( 0 );
	fileBox->setParent( mainLayout );
	mainLayout->addLayout( fileBox );
	fileNameLabel = new QLabel( "File Name:", this );
	fileEdit = new QLineEdit( "", this ); 
	
	fileBox->addWidget( fileNameLabel );
	fileBox->addWidget( fileEdit );

	//rowcolBox - Contains two spin boxes for defining the pixel dimensions
	rowcolBox = new QVBoxLayout( 0 );
	rowcolBox->setParent( mainLayout );
	mainLayout->addLayout( rowcolBox );
	rowsOfPixelsLabel = new QLabel( "Rows of Pixels:", this );
	rowSpin = new QSpinBox( this );
	rowSpin->setMinimum( 0 );
	rowSpin->setMaximum( 100000000 );
	rowSpin->setSingleStep( 1 );
	colsOfPixelsLabel = new QLabel( "Columns of Pixels:", this );
	colSpin = new QSpinBox( this );
	colSpin->setMinimum( 0 );
	colSpin->setMaximum( 100000000 );
	colSpin->setSingleStep( 1 );
		
		//Adding widgets to the rowcolBox group layout
	rowcolBox->addWidget( rowsOfPixelsLabel );
	rowcolBox->addWidget( rowSpin );
	rowcolBox->addWidget( colsOfPixelsLabel );
	rowcolBox->addWidget( colSpin );

	//comboBox - Contains four combo boxes for selecting preset values
	comboBox = new QVBoxLayout( 0 );
	comboBox->setParent( mainLayout );
	mainLayout->addLayout( comboBox );
	comboBox->setSizeConstraint( QLayout::SetNoConstraint );
	unitLabel = new QLabel( "Units", this );
	unitCombo = new QComboBox( this );
	spheroidLabel = new QLabel( "Spheroid", this );
	spheroidCombo = new QComboBox( this );
	pixelSizeLabel = new QLabel( "Pixel Size", this );
	pixelCombo = new QComboBox( this );
	pixelEdit = new QLineEdit( this );

		//Adding widgets to comboBox layout
	comboBox->addWidget( unitLabel );
	comboBox->addWidget( unitCombo );
	comboBox->addWidget( spheroidLabel );
	comboBox->addWidget( spheroidCombo );
	comboBox->addWidget( pixelSizeLabel );
	comboBox->addWidget( pixelCombo );
	comboBox->addWidget( pixelEdit );

	//ulBox - Contains line edits for defining the upper left corner of the map
	ulBox = new QVBoxLayout( 0 );
	ulBox->setParent( mainLayout );
	mainLayout->addLayout( ulBox );
	ulLonLabel = new QLabel( "UL Longitude in meters", this );
	ulLonEdit = new QLineEdit( this );
	ulLatLabel =  new QLabel( "UL Latitude in meters", this);
	ulLatEdit = new QLineEdit( this );
		
		//Adding widgets to the ulBox layout
	ulBox->addWidget( ulLonLabel );
	ulBox->addWidget( ulLonEdit );
	ulBox->addWidget( ulLatLabel );
	ulBox->addWidget( ulLatEdit );

	//dataBox - Contains one combo box for selecting what type of data it is
	dataBox = new QVBoxLayout( 0 );
	dataBox->setParent( mainLayout );
	mainLayout->addLayout( dataBox );
	dataBox->setSizeConstraint( QLayout::SetNoConstraint );
	dataBox->setSpacing( 0 );
	pixelDataTypeLabel = new QLabel( "Pixel Data Type", this);
	dataCombo = new QComboBox( this );
	fillValueLabel = new QLabel( "Fill Value", this );
	
	dataBox->addWidget( pixelDataTypeLabel );
	dataBox->addWidget( dataCombo );
	dataBox->addWidget( fillValueLabel );

	//fillBox - Contains hidable checkbox, a lineEdit and a pushbutton; goes into dataBox
	fillBox = new QHBoxLayout( 0 );	
	fillBox->setParent( dataBox );
	dataBox->addLayout( fillBox );
	fillBox->setSizeConstraint( QLayout::SetNoConstraint );
	hasFillCheck = new QCheckBox( this );
	hasFillCheck->hide(); 
	fillEdit = new QLineEdit( this );
	fillEdit->installEventFilter( this );
	fillButton = new QPushButton( "?", this );
	
		//Adding widgets to fillBox layout and fillBox layout to dataBox layout
	fillBox->addWidget( hasFillCheck );
	fillBox->addWidget( fillEdit );
	fillBox->addWidget( fillButton );

	noDataValueLabel = new QLabel( "No Data Value", this );
	dataBox->addWidget( noDataValueLabel );

	//noDataBox - Contains hidable checkbox and a lineEdit; goes into dataBox
	noDataBox = new QHBoxLayout( 0 );	
	noDataBox->setParent( dataBox );
	dataBox->addLayout( noDataBox );
	noDataBox->setSizeConstraint( QLayout::SetMinimumSize );
	hasNoDataCheck = new QCheckBox( this );
	hasNoDataCheck->hide();
	noDataEdit = new QLineEdit( this );

		//Adding widgets to noDataBox and adding noDataBox to dataBox
	noDataBox->addWidget( hasNoDataCheck );
	noDataBox->addWidget( noDataEdit );
}

void MapEdit::polishWidgets()
{
	////////STAGE 3: Polish Widgets
	//
	//titleBox
	copyButton->setMaximumWidth( 28 ); copyButton->setMaximumHeight( 28 );
	copyButton->setToolTip( "Copy from input info editor." );
	lockButton->setMaximumWidth( 28 ); lockButton->setMaximumHeight( 28 );
	lockButton->setCheckable( true );
	lockButton->setToolTip( "Use to allow editing of .xml file.<br><br>"
		"<b>Note</b>: Locking this info editor automatically saves to the .xml file." );
	QFont largeFont(  mapLabel->font() );
	largeFont.setPointSize( 12 );
	mapLabel->setFont( largeFont );
	mapLabel->setAlignment(Qt::AlignCenter);
	frameButton->setMaximumWidth( 28 ); frameButton->setMaximumHeight( 28 );
	frameButton->setToolTip( "Calculate rows, columns, and upper left coordinates." );

	//fileBox
	fileEdit->setReadOnly( true );
	fileEdit->setToolTip( "File name of the loaded .xml file" );

	//rowcolBox
	rowSpin->setToolTip( "Height of the map in pixels" );
	colSpin->setToolTip( "Width of the map in pixels" );

	//comboBox
	unitCombo->addItem( "" );
	unitCombo->addItems( unitNames );
	unitCombo->setCurrentIndex( 3 );	//displays Meters
	unitCombo->setDisabled( true );
	unitCombo->setToolTip( "Units for all measurements<br><br>"
		"<b>Note</b>: Currently only \"Meters\" are supported." );
	spheroidCombo->addItem( "" );
	spheroidCombo->addItems( spheroidNames );
	spheroidCombo->setCurrentIndex( 20 ); //Sphere of Radius 6370997 meters
	spheroidCombo->setDisabled( true );
	spheroidCombo->setToolTip( "Name of the spheroid used in projection<br><br>"
		"<b>Note</b>: Currently only \"Sphere of Radius 6370997 meters\" is supported." );
	pixelCombo->addItem( "" );
	pixelCombo->addItems( pixelSizes );
	pixelCombo->setToolTip( "Size of each pixel in the raster image" );
	pixelEdit->setToolTip( "Pixel size<br>"
		"<b>Meters:</b> Entered value must be from 0 to 10000000." );
	pixelEdit->setValidator( new MapimgValidator( 0, 10000000, 12, pixelEdit ) );
	pixelEdit->hide();

	//ulBox
	ulLonEdit->setValidator( new MapimgValidator( -100000000, 100000000, 12, ulLonEdit ) );
	ulLonEdit->setToolTip( "Longitude of upper left corner of map<br>"
		"<b>Meters:</b> Entered value must be from -100000000 to 100000000." );
	ulLatEdit->setValidator( new MapimgValidator( -100000000, 100000000, 12, ulLatEdit ) );
	ulLatEdit->setToolTip( "Latitude of upper left corner of map<br>"
		"<b>Meters:</b> Entered value must be from -100000000 to 100000000." );

	//dataBox
	dataCombo->addItem( "" );
	dataCombo->addItems( dataTypes );
	dataCombo->setToolTip( "Data type of each pixel in the raster image" );
	fillEdit->setValidator( new MapimgValidator( -100000000, 100000000, INFO_PRECISION, fillEdit ) );
	hasFillCheck->setToolTip( "Toggle whether fill value is defined.<br>"
		"<b>Note</b>: The fill value is optional for input parameters only, "
		"but if it is defined in the input then it is forced in the output." );
	fillEdit->setToolTip( "Fill value to represent a pixel outside the map frame<br>"
		"Entered value must be from -100000000 to 100000000." );
	fillButton->setMaximumSize( 20, 20 );
	hasNoDataCheck->setToolTip( "Toggle whether No Data Value is defined.<br>"
		"<b>Note</b>: The no data value is optional for both input and output parameters, "
		"but if it is defined in the input then it is forced in the output." );
	fillButton->setToolTip( "Recommend fill value by reading file and "
		"solving for the maximum value + 2" );
	noDataEdit->setValidator( new MapimgValidator( -100000000, 100000000, INFO_PRECISION, noDataEdit ) );
	noDataEdit->setToolTip( "\"No Data\" value to represent a pixel inside the map frame"
		" with no value<br>"
		"Entered value must be from -100000000 to 100000000." );

	//This connection is for keeping the pixelEdit hidden until it is needed
	connect(pixelCombo, SIGNAL(activated(int)), this, SLOT(pixelChange(int)));
	connect(dataCombo, SIGNAL(activated(const QString&)), this, SLOT(dataChange(const QString&)));
	connect(hasFillCheck, SIGNAL(toggled(bool)), this, SLOT(fillCheckToggled(bool)));
	connect(hasNoDataCheck, SIGNAL(toggled(bool)), this, SLOT(noDataCheckToggled(bool)));

	setWidget( contents );	//Allows the contents top be moved by scrollbar
}
