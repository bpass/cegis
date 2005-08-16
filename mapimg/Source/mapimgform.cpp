// $Id: mapimgform.cpp,v 1.4 2005/08/16 12:57:20 lwoodard Exp $
//Last Edited: August 2005; by: lwoodard; for:qt3 to qt4 porting

#include "mapimgform.h"

#include <QVariant>
#include <QLayout>
#include <QTooltip>
#include <QWhatsThis>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolButton>
#include <QMainWindow>
#include <QToolBar>

//	#include <q3toolbar.h>
#include <q3dragobject.h>
#include <q3widgetstack.h>
#include <q3filedialog.h>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <q3process.h>
//Added by qt3to4:
#include <Q3CString>
#include <QEvent>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>

#include "mapimgversion.h"
#include "mapimgimages.h"
#include "qimgframe.h"
#include "mapimgedit.h"
#include "rasterinfo.h"
#include "resampleinfo.h"
#include "window_flags.h"
#include "resampleform.h"
#include "aboutform.h"
#include "authorform.h"
#include "tiff2img.h"
#include "mapimg.h"
#include "mapedit.h"

/*
The mapimgForm has one constructor separated by sections of the form.

FILE MENU sets up the File menu actions and connections which users use to
exit. It also contains an undocumented bigAction which is activated by 
pressing Ctrl + B. This action returns all toolbar buttons to their actual
sizes.

INPUT sets up the input parameters view by creating a QInfoFrame and
QActions to interface with it. The inInfoAction is used to Hide/Show the
QInfoFrame, the inOpenAction prompts the user for a filename to load, and the
inSaveAction is used to save all parameters out to a file. Then the Input menu
is created and populated with these three actions.

PREVIEW sets up the middle section of the form with a QImgFrame and
QActions to interface with it. The viewShowAction is used to Hide/Show the
QImgFrame, the viewResampleAction connects with the resample slot of the
QImgFrame, the viewProjAction starts the procedure to preview a reprojection
before spending the time on it.

OUTPUT sets up the output parameters view just like INPUT. The differences
are that there is no open action and the outSaveAction actually calls the 
reprojection.

TOOLBARS creates the button toolbar with the actions to open a new input,
reproject to a file, hide/show the three frames, resample the imgframe, and
preview the reprojection.
*/
mapimgForm::mapimgForm( QWidget* parent, const char* name, Qt::WFlags fl )
: QMainWindow( parent, name, fl )
{
	setIcon( QPixmap( "./Resources/USGS Swirl Sphere.png" ) );
	setCaption( QString("MapIMG %1").arg(MAJOR_VERSION) );

	setCentralWidget( new QWidget( this, "qt_central_widget" ) );
	formLayout = new QHBoxLayout( centralWidget(), 1, 2, "formLayout");

	////////
	//FILE MENU
	////////

	//actions

	/****/
	exitAction = new QAction( QIcon( "./Resources/exit.png" ),
		"E&xit", QKeySequence("Ctrl+X"), this, "exitAction" );
	bigAction = new QAction(
		"Use &Big Icons", QKeySequence("Ctrl+B"), this, "bigAction" );
	bigAction->setCheckable( true );
	/****/

	//signals and slots
	connect( exitAction, SIGNAL( activated() ), this, SLOT( close() ) );
	connect( bigAction, SIGNAL( checked(bool) ), this, SLOT( setUsesBigPixmaps(bool) ) );

	////////
	//OPTIONS MENU
	////////

	//actions
	authAction = new QAction( QIcon( ""  ), 
		"Edit &Author", QKeySequence("Ctrl+A"), this, "authAction" );

	//signals and slots
	connect( authAction, SIGNAL( activated() ), this, SLOT( editAuthor() ) );


	////////
	//INPUT
	////////

	//widgets
	inInfoFrame = new QInfoFrame( centralWidget(), "inInfoFrame" );
	inInfoFrame->setAsInput();
	inInfoFrame->fixWidth( 300 );
	inInfoFrame->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
	formLayout->addWidget( inInfoFrame );
	//inInfoFrame->setShown( false );

	//actions
	/****/
	inInfoAction = new QAction( QIcon( "./Resources/inputinfo.png" ),
		"Show Input Parameters", QKeySequence(), this, "inInfoAction" );
	inInfoAction->setCheckable( true );
	inInfoAction->setChecked( true );
	inOpenAction = new QAction( QIcon( "./Resources/open.png" ),
		"&Open", QKeySequence("Ctrl+O"), this, "inOpenAction" );
	inSaveAction = new QAction( QIcon( "./Resources/insave.png" ),
		"Save Input Parameters", QKeySequence(), this, "inSaveAction" ); 
	/****/

	//signals and slots
	connect( inInfoAction, SIGNAL( checked(bool) ), inInfoFrame, SLOT( setShown(bool) ) );
	connect( inOpenAction, SIGNAL( activated() ), this, SLOT( inOpenClicked() ) );
	connect( inSaveAction, SIGNAL( activated() ), this, SLOT( inSaveClicked() ) );
	connect( inInfoFrame, SIGNAL( locked() ), this, SLOT( inSaveClicked() ) );


	////////
	//PREVIEW
	////////

	//widgets
	imgFrame = new QImgFrame( centralWidget(), "imgFrame" );
	imgFrame->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
	imgFrame->setLineWidth(2);
	imgFrame->setPixmap( QPixmap("./Resources/USGS Logo.png" ) );
	imgFrame->setMinimumSize( QSize( 167, 62 ) );
	imgFrame->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
	imgFrame->setShown( false );
	imgFrame->installEventFilter( this );
	formLayout->addWidget( imgFrame );

	//actions
	/****/
	viewShowAction = new QAction( QIcon( "./Resources/preview.png" ),
		"Show Preview", QKeySequence(), this, "inViewAction" );
	viewShowAction->setCheckable( true );
	viewResampleAction = new QAction( QIcon( "./Resources/resample.png" ),
		"Resample Preview", QKeySequence("F5"), this, "resampleAction" );
	previewProjAction = new QAction( QIcon( "./Resources/previewproject.png" ),
		"&Preview Reprojection", QKeySequence("Ctrl+P"), this, "resampleAction" );
	/****/

	//signals and slots
	connect( viewShowAction, SIGNAL( checked(bool) ), this, SLOT( previewClicked(bool) ) );
	connect( viewResampleAction, SIGNAL( activated() ), imgFrame, SLOT( resample() ) );
	connect( previewProjAction, SIGNAL( activated() ), this, SLOT( previewOutput() ) );

	////////
	//OUTPUT
	////////

	//widgets
	outInfoFrame = new QInfoFrame( centralWidget(), "outInfoFrame" );
	outInfoFrame->setAsOutput(); outInfoFrame->setPartner( inInfoFrame );
	outInfoFrame->fixWidth( 300 );
	outInfoFrame->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
	formLayout->addWidget( outInfoFrame );
	//outInfoFrame->setShown( false );

	//actions

	/****/
	outInfoAction = new QAction( QIcon ( "./Resources/outputinfo.png" ),
		"Show Output Parameters", QKeySequence(), this, "outInfoAction" );
	/****/
	outInfoAction->setCheckable( true );
	outInfoAction->setOn( outInfoFrame->isShown() );

	/****/
	outSaveAction = new QAction( QIcon( "./Resources/outsave.png" ),
		"Reproject and &Save", QKeySequence("Ctrl+S"), this, "outSaveAction" );
	/****/

	//signals and slots
	connect( outInfoAction, SIGNAL( checked(bool) ), outInfoFrame, SLOT( setShown(bool) ) );
	connect( outSaveAction, SIGNAL( activated() ), this, SLOT( outSaveClicked() ) );


	////////
	//TOOLS MENU
	////////

	//actions
	webDSS = new QAction( "Decision Support System", QKeySequence(""), this, "webDSS" );

	//signals and slots
	connect( webDSS, SIGNAL( activated() ), this, SLOT( webDSSClicked() ) );


	////////
	//HELP MENU
	////////

	//actions
	aboutAction = new QAction( "About MapIMG", QKeySequence(""), this, "aboutAction" );
	aboutQtAction = new QAction( "About Qt", QKeySequence(""), this, "aboutQtAction" );

	//signals and slots
	connect( aboutAction, SIGNAL( activated() ), this, SLOT( aboutClicked() ) );
	connect( aboutQtAction, SIGNAL( activated() ), this, SLOT( showAboutQt() ) );


	////////
	//TOOLBAR
	////////

	toolBar = addToolBar(tr("File"));

	toolBar->addAction( inOpenAction );
	toolBar->addAction( outSaveAction );
	toolBar->addSeparator();
	toolBar->addAction( inInfoAction );

	/****/
	viewShowButton = new QToolButton( QIcon( "./Resources/preview.png" ),
		"Show Preview", "", NULL, 0, toolBar, "previewButton" );
	/****/
	viewShowButton->installEventFilter( this );
	toolBar->addAction( outInfoAction );
	toolBar->addSeparator(); 
	toolBar->addAction( viewResampleAction );
	toolBar->addAction( previewProjAction );

	//preview button extra features
	viewShowPopup = new QMenu( "viewShowPopup", viewShowButton );

	prevInput = new QAction( "Preview Input", QKeySequence(""), this, "prevInput" );
	prevInput->setCheckable( true ); 
	viewShowPopup->addAction( prevInput );
	prevOutput = new QAction( "Preview Output", QKeySequence(""), this, "prevOutput" );
	prevOutput->setCheckable( true );
	viewShowPopup->addAction( prevOutput );

	viewShowButton->setCheckable(true);

	connect( viewShowButton, SIGNAL( checked(bool) ), this, SLOT( previewClicked(bool) ) );
	connect( prevInput, SIGNAL( checked(bool) ), this, SLOT( previewInput(bool) ) );
	connect( prevOutput, SIGNAL( checked(bool) ), this, SLOT( previewOutput(bool) ) );


	////////
	//MENUBAR
	////////

	fileMenu = menuBar()->addMenu( tr( "&File" ) ); 
	fileMenu->addAction( inOpenAction );
	fileMenu->addSeparator();
	fileMenu->addAction( inSaveAction );
	fileMenu->addAction( outSaveAction );
	fileMenu->addSeparator();
	fileMenu->addAction( exitAction );

	optionsMenu = menuBar()->addMenu( tr( "&Options" ) );
	optionsMenu->addAction( authAction );

	previewMenu = menuBar()->addMenu( tr( "&Preview" ) );
	previewMenu->addAction( viewResampleAction );
	previewMenu->addAction( previewProjAction );

	webBased = new QMenu("Web Based" );
	webBased->addAction( webDSS );

	toolsMenu = menuBar()->addMenu( tr( "&Tools" ) );
	toolsMenu->addMenu( webBased );

	helpMenu = menuBar()->addMenu( tr( "&Help" ) );
	helpMenu->addAction( aboutAction );
	helpMenu->addAction( aboutQtAction );


	////////
	// SETTINGS
	////////

	QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,"" /**QSettings::Ini**/ );
	settings->setPath( "USGS.gov", "MapIMG" );

	inPath = settings->readEntry( "/USGS/MapIMG/DefaultInputPath" );
	outPath = settings->readEntry( "/USGS/MapIMG/DefaultOutputPath" );
	authName = settings->readEntry( "/USGS/MapIMG/AuthorName" );
	authCompany = settings->readEntry( "/USGS/MapIMG/AuthorCompany" );
	authEmail = settings->readEntry( "/USGS/MapIMG/AuthorEmail" );

	if( inPath.isNull() || !QFile::exists(inPath) )
		settings->writeEntry( "/USGS/MapIMG/DefaultInputPath", QDir::currentDirPath() );
	if( outPath.isNull() || !QFile::exists(outPath) )
		settings->writeEntry( "/USGS/MapIMG/DefaultOutputPath", QDir::currentDirPath() );
	if( authName.isNull() )
	{
		settings->writeEntry( "/USGS/MapIMG/AuthorName", QString("Unknown") );
		settings->writeEntry( "/USGS/MapIMG/AuthorCompany", QString("Unknown") );
		settings->writeEntry( "/USGS/MapIMG/AuthorEmail", QString("Unknown") );
		delete settings;

		editAuthor();
	}
	else
		delete settings;


	resize( QSize(600, 524).expandedTo(minimumSizeHint()) );
	setAcceptDrops( true );

	////////
	// DEFAULT VALUES
	////////

	imgName = "";
	imgSet = false;
	newInfo = false;

	ignorePreviewSignals = false;
	prevLast = prevInput;
}

/*
Delete temporary files.
*/
mapimgForm::~mapimgForm()
{
	if( QFile::exists( "mapimg.log" ) )
		QFile::remove( "mapimg.log" );

	if( QFile::exists( "temp_preview.xml" ) )
		QFile::remove( "temp_preview.xml" );

	if( QFile::exists( "temp_preview.img" ) )
		QFile::remove( "temp_preview.img" );

	if( QFile::exists( "temp_small.xml" ) )
		QFile::remove( "temp_small.xml" );

	if( QFile::exists( "temp_small.img" ) )
		QFile::remove( "temp_small.img" );
}

/*
The eventFilter() function is used to capture left clicks on the preview
toolbar button and right clicks in the imgFrame. If the left click is on the
triangle of the preview button then the result is the viewShowPopup to allow
the user to select between input and output. Right-clicking anywhere in the
imgFrame will have the same effect.

The arrow in the 
*/
bool mapimgForm::eventFilter( QObject* object, QEvent* event )
{
	if( object == imgFrame && event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent *mouseEvent = (QMouseEvent*)event;
		if( mouseEvent->button() == Qt::RightButton )
			viewShowPopup->exec(mouseEvent->globalPos());
	}
	else if( object == viewShowButton && event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent *mouseEvent = (QMouseEvent*)event;
		if( mouseEvent->button() == Qt::LeftButton )
		{
			QRect hotspot( viewShowButton->geometry() );
			hotspot.moveTopLeft( QPoint(0,0) );
			hotspot.setTopLeft( QPoint(hotspot.center().x(), hotspot.height()*3/5) );
			if( hotspot.contains( mouseEvent->pos() ) )
				viewShowPopup->exec( viewShowButton->mapToGlobal( QPoint(0,viewShowButton->height()) ) );
			else
				viewShowButton->toggle( );
			return true;
		}
	}
	return QMainWindow::eventFilter( object, event ); /*************************/
}

void mapimgForm::dragEnterEvent( QDragEnterEvent *evt )
{
	if ( Q3TextDrag::canDecode( evt ) )
		evt->accept();
}

void mapimgForm::dropEvent( QDropEvent *evt )
{
	QString text;

	if ( Q3TextDrag::decode( evt, text ) )
	{
		int gi = text.findRev( "img" );
		int li = text.findRev( "xml" );
		int fi = text.findRev( "tif" );
		if( gi > li && gi > fi )
		{
			text.truncate( gi );
			text.append( "img" );     
		}
		else if( li > fi )
		{
			text.truncate( li );
			text.append( "img" ); 
		}
		else
		{
			text.truncate( fi );
			text.append( "tif" ); 
		}

		if( text.left(8) == "file:///" )
			text.remove( 0, 8 );
		text.replace( "%20", " " );
		text.replace( "%5c", "/" );
		text.replace( '\\', '/' );

		if( openFile( text ) )
		{
			outInfoAction->setOn(false);
			viewShowAction->setOn(true);
		}
	}
}

/*
The inOpenClicked() function is used to load a new file as input. It has
checks for the existence of both the .img file and its meta data file. When
the .img file is missing it bails out. If the meta data is missing then it
messages the user and asks them to genererate one using the input frame.
*/
void mapimgForm::inOpenClicked()
{
	QString temp = Q3FileDialog::getOpenFileName(
		inPath, "MapIMG Files (*.img);;Tiff Files (*.tif)", this, "", "Choose an image");

	if( openFile(temp) )
	{   
		inPath = imgName.left( imgName.findRev( "/" ) );
		QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,"" /*QSettings::Ini*/ );
		settings->setPath( "USGS.gov", "MapIMG" );
		settings->writeEntry( "/USGS/MapIMG/DefaultInputPath", inPath );
		delete settings;
	}
}

bool mapimgForm::openFile( QString inFile )
{
	if( inFile == "" )
		return false;

	if( !QFile::exists( inFile ) )
	{
		QMessageBox::warning( this, "Open", QString( "%1\n"
			"File not found.\n"
			"Please verify that the correct file name was given.")
			.arg(inFile) );
		return false;
	}

	if( inFile.endsWith( ".tif" ) )
	{
		int convert = QMessageBox::information( this, "Open",
			"Tiff images cannot be projected directly\n"
			"Would you like to export to generic binary?",
			QMessageBox::Yes, QMessageBox::No );

		if( convert == QMessageBox::No )
			return false;

		QString tiffFile = inFile;
		inFile.append( ".img" );

		if( tiff2img( tiffFile, inFile ) )
		{
			QMessageBox::information( this, "Open",
				"TIFF conversion completed succesfully." );
		}
		else
		{
			QMessageBox::information( this, "Tiff Image",
				"Conversion failed for unknown reason." );
			return false;
		}
	}

	if( !inFile.endsWith( ".img" ) )
	{
		QMessageBox::information( this, "Open", 
			QString( "%1\n"
			"MapIMG cannot read this file.\n"
			"This is not a valid raster image, or its format is not currently supported." )
			.arg(inFile) );
		return false;
	}

	imgName = inFile;
	imgSet = true;
	RasterInfo info;

	if( info.load( imgName ) )
	{
		inInfoFrame->setInfo( info );
		inInfoFrame->lock( true, false );
		newInfo = false;

		prevLast = NULL;
		previewInput( imgFrame->isShown() );
	}
	else
	{
		info.setAuthor( authName, authCompany, authEmail );
		inInfoFrame->lock( false, false );
		inInfoAction->setOn( true );
		outInfoAction->setOn( false );

		imgFrame->setPixmap( QPixmap( "./Resources/USGS Logo.png" ) );
		viewShowButton->setOn( false );
		inInfoFrame->setInfo( info );

		QMessageBox::information( this, "Open", 
			QString( "%1\n"
			"XML file corrupted or not found.\n\n"
			"Please use the Input Info Editor to define the map and projection parameters.\n\n"
			"-The lock button saves changes.\n"
			"-The calculator button figures out the rows, columns and upper left coordinates." )
			.arg( info.xmlFileName() ) );
		newInfo = true;
	}


	////////Parse fileName for window caption
	QString cap( imgName );
	cap.replace('\\', '/');
	int index = cap.findRev("/");
	index++;
	cap = cap.right(cap.length() - index);
	setCaption(cap + " - MapIMG");

	return true;
}

/*
inSaveClicked() is how the user saves changes the input parameters. It is
connected to the locked() signal from the input InfoFrame's lockButton. There
are three conditions where it will not make it to the end of the function 
where it saves; If no image has been loaded (!imgSet), the info is corrupt
(!i.ready()), or the user chooses not to overwrite (arg==1).
To speed up the process of generating a new meta data file the overwrite
message box is not shown if the current .img file did not have a .info or .xml
file when it was first loaded. However, when the user us working with a file
where the parameters were pre defined to message box will always show because
it would be too easy to accidentally overwrite the file otherwise.
*/
void mapimgForm::inSaveClicked()
{
	if( !imgSet )
	{
		QMessageBox::critical( this, "Save Input Parameters",
			"No input image has been loaded yet." );
		return;
	}

	RasterInfo i( inInfoFrame->info() );
	i.setAuthor( authName, authCompany, authEmail );

	if( !mapimg::readytoReproject(i, this) )
	{
		inInfoFrame->lock( false, false );
		return;
	}

	if( !newInfo && QFile::exists( i.xmlFileName() ) )
	{
		RasterInfo j( i.xmlFileName() );
		j.load();

		if( QMessageBox::warning( this, "Save Input Parameters",
			QString("%1 already exists.\nDo you want to replace it?").arg(i.xmlFileName()),
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::No )
		{
			inInfoFrame->setInfo( j );
			return;
		}

		i.setAuthor( j.author(), j.company(), j.email() );
	}

	i.save();
	prevLast = NULL;
	previewInput( imgFrame->isShown() );
	return;
}

/*

*/
void mapimgForm::previewClicked( bool on )
{
	if( ignorePreviewSignals )
		return;

	if( !prevLast )               /*if a file is freshly loaded, but not displayed, prevLast will be NULL*/
		previewInput( true );     /*this addition will generate the preview for this case*/

	prevLast->setOn(on);

	ignorePreviewSignals = true;
	{
		viewShowButton->setOn(on);
	}
	ignorePreviewSignals = false;
}

void mapimgForm::previewInput( bool on )
{
	if( ignorePreviewSignals )
		return;

	if( on && prevLast != prevInput )
	{
		imgFrame->loadImg( inInfoFrame->info().imgFileName() );
		prevLast = prevInput;
	}

	ignorePreviewSignals = true;                      /* what is this block? is it conditional? */
	{
		prevInput->setOn(on);
		prevOutput->setOn(false);
		viewShowButton->setOn(on);
		imgFrame->setShown(on);
	}
	ignorePreviewSignals = false;
}

void mapimgForm::previewOutput( bool on )
{
	if( ignorePreviewSignals )
		return;

	ignorePreviewSignals = true;
	{
		prevOutput->setOn(on);
		if( on )
		{
			if( previewProjection() )
			{
				imgFrame->loadImg( QDir::currentDirPath().append("/temp_preview.img"), true );
				prevLast = prevOutput;
				prevInput->setOn(false);
				viewShowButton->setOn(true);
				imgFrame->setShown(true);
			}
			else
			{
				prevOutput->setOn(false);
			}
		}
		else
		{
			prevInput->setOn(false);
			viewShowButton->setOn(false);
			imgFrame->setShown(false);
		}
	}
	ignorePreviewSignals = false;
}

/*
previewProjection() is used to preview a projection. This is useful
because many reprojections can take hours but using this function the output
can be preivewed before investing the time. The reprojection function is
linear with size increases having a proportional effect on run-time. This
preview down samples the input with a constant time algorithm and then
reprojects to a much smaller output.
*/
bool mapimgForm::previewProjection()
{
	if( !imgSet )
	{
		QMessageBox::critical( this, "Input not set",
			"No input to preview reprojection from." );
		prevOutput->setOn(false);
		ignorePreviewSignals = false;
		return false;
	}

	RasterInfo input( inInfoFrame->info() );
	if( !mapimg::readytoReproject(input, this) )
		return false;

	RasterInfo output( outInfoFrame->info() );
	output.setDataType( input.isSigned(), input.bitCount(), input.type() );

	if( !mapimg::readytoFrameIt( output, this ) )
		return false;
	mapimg::frameIt( output );
	outInfoFrame->setInfo( output );
	if( !mapimg::readytoReproject( output, this, false ) )
		return false;

	RasterInfo smallInput;
	smallInput.setFileName( QDir::currentDirPath().append("/temp_small.img") );
	mapimg::downSampleImg( input, smallInput, 720, this );
	smallInput.save();

	if( !mapimg::downSizeProjection( output, 720 ) )
	{
		QMessageBox::critical( this, "Output Error",
			"Unexpected failure with output operations" );
		return false;
	}

	output.setFileName( QDir::currentDirPath().append("/temp_preview.img") );
	output.save();

	ResampleInfo resample;
	resample.setResampleCode( ResampleInfo::NearestNeighbor );
	resample.setFillValue( input.fillValue() );
	resample.setNoDataValue( input.noDataValue() );

	return !mapimg::reproject( smallInput, output, resample );
}

/*
The outSaveClicked() function is used to actually reproject. If both the
input and output parameters check out ok then the user is prompted for the
destination to reproject to. After checking the output filename the
the reprojection begins.
*/
void mapimgForm::outSaveClicked()
{
	// Check that all input settings are ready
	if( !imgSet )
	{
		QMessageBox::critical( this, "Reproject and Save",
			"No file loaded for reprojection input." );
		return;
	}

	RasterInfo input( inInfoFrame->info() );

	if( input.projectionNumber() == 0 )
	{
		mapimg::geo2eqr( input );
	}
	else
	{
		if( !mapimg::readytoReproject(input, this) )
			return;
	}

	// Check that all output settings are ready
	RasterInfo output( outInfoFrame->info() );
	output.setAuthor( authName, authCompany, authEmail );

	if( !mapimg::readytoFrameIt( output, this ) )
		return;
	mapimg::frameIt( output );
	outInfoFrame->setInfo( output );
	if( !mapimg::readytoReproject( output, this, false ) )
		return;

	// Prompt for destination of new projection
	QString temp = Q3FileDialog::getSaveFileName(
		outPath, "MapIMG Raster Files (*.img)", this, "", "Choose a destination for the reprojection");
	if( temp.isNull() )
		return;

	if( !output.setFileName( temp ) )
	{
		QMessageBox::critical( this, "Reproject and Save",
			QString( "%1\n"
			"Invalid output file name." )
			.arg( temp ) );
		return;
	}

	// Prompt on overwrite of output
	if( QFile::exists( output.imgFileName() ) )
	{
		int status = QMessageBox::question( this, "Reproject and Save", 
			output.imgFileName() + " already exists."
			"\nDo you want to replace it?",
			QMessageBox::Yes, QMessageBox::No );

		if( status == QMessageBox::No )
			return;

		QFile::remove( output.imgFileName() );
		if( QFile::exists( output.xmlFileName() ) )
			QFile::remove( output.xmlFileName() );
	}

	// Prompt for resample parameters
	ResampleForm *resForm = new ResampleForm( input, output, this, "resForm", false,
		WINDOW_FLAGS );

	if( resForm->exec() == QDialog::Rejected )
	{
		delete resForm;
		return;
	}

	ResampleInfo resample( resForm->info() );
	resample.setFillValue( output.fillValue() );
	resample.setNoDataValue( output.noDataValue() );
	resample.setHasNoDataValue( output.hasNoDataValue() );
	delete resForm;

	// Reproject (finally)
	if( mapimg::reproject( input, output, resample, this ) )
	{
		if( QFile::exists( output.imgFileName() ) )
			QFile::remove( output.imgFileName() );
		return;
	}

	output.save();

	// Save output path to settings
	outPath = temp.left( temp.findRev( "/" ) );
	QSettings *settings = new QSettings( QSettings::IniFormat, QSettings::SystemScope,"" /**QSettings::Ini**/ );
	settings->setPath( "USGS.gov", "MapIMG" );
	settings->writeEntry( "/USGS/MapIMG/DefaultOutputPath", outPath );
	delete settings;
}

void mapimgForm::editAuthor()
{
	QSettings *settings = new QSettings( QSettings::IniFormat, QSettings::SystemScope,""/**QSettings::Ini**/ );
	settings->setPath( "USGS.gov", "MapIMG" );

	authorForm *form = new authorForm(settings, this, "about", false,
		WINDOW_FLAGS );
	form->exec();
	delete form;

	authName = settings->readEntry( "/USGS/MapIMG/AuthorName" );
	authCompany = settings->readEntry( "/USGS/MapIMG/AuthorCompany" );
	authEmail = settings->readEntry( "/USGS/MapIMG/AuthorEmail" );

	delete settings;
}

void mapimgForm::aboutClicked()
{
	aboutForm *about = new aboutForm(this, "about", false,
		WINDOW_FLAGS );
	about->exec();
	delete about;
}

QString mapimgForm::dataType() const
{
	QString currentDataType = "";

	if( inInfoFrame )
	{
		if( inInfoFrame->mapTab )
		{
			if( inInfoFrame->mapTab->dataCombo )
				currentDataType = inInfoFrame->mapTab->dataCombo->currentText();
		}
	}

	return currentDataType;
}


//Tools
void mapimgForm::webDSSClicked()
{
	launchWebTool( "http://mcmcweb.er.usgs.gov/research/DSSMain/DSSApplet.html" );

	return;
}

void mapimgForm::launchWebTool( const QString& url )
{
	Q3Process* web = new Q3Process( this, "webTool" );
	bool supportedPlatform = true;
	bool executeProcess = false;

#if defined(Q_OS_WIN32)
#include <windows.h>

	//Trolltech's Windows Version Independent Default Browser Launch
	QT_WA( {ShellExecute( winId(), 0, (TCHAR*)url.ucs2(), 0, 0, SW_SHOWNORMAL );},
	{ShellExecuteA( winId(), 0, url.local8Bit(), 0, 0, SW_SHOWNORMAL );} );

	supportedPlatform = true;
	executeProcess = false;
#elif defined(Q_OS_LINUX)
#include <stdio.h>
#include <stdlib.h>

	//get : delimited list of all perferred browsers
	Q3CString allBrowsers = getenv( "BROWSER" );

	if( allBrowsers.isEmpty() ) //if it is empty we're hosed
	{
		qDebug( "No $BROWSER environment varialbe set." );
		supportedPlatform = false;
		executeProcess = false;
	}
	else
	{
		//split the list
		QStringList browsers = QStringList::split( ":" , allBrowsers );

		//grab the first (default)
		QString browser = *browsers.begin();

		if( browser.contains( "kfmclient" ) )
		{
			web->addArgument( "kfmclient" );
			web->addArgument( "exec" );
		}
		else
		{
			web->addArgument( browser );
		}

		web->addArgument( url );

		executeProcess = true;
	}
#elif defined(Q_OS_MACX)
	//Trolltech's Mac OSX Version Independent Default Browser Launch
	web->addArgument( "/usr/bin/open" );
	web->addArgument( url );
	executeProcess = true;
#else
	supportedPlatform = false;
	executeProcess = false;
#endif

	int returnValue = 0;
	QObject::connect( web, SIGNAL( processExited() ), web, SLOT( deleteLater() ) );

	if( executeProcess )
		returnValue = !web->start();

	if( returnValue || !supportedPlatform )
		QMessageBox::information( this, "Mapimg", QString("Unable to launch web browser to %1").arg( url ) );

	return;
}

bool mapimgForm::allowIgnoreValues() const
{
	return outInfoFrame->mapTab->hasNoDataCheck->isChecked();
}