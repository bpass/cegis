// $Id: mapimgform.cpp,v 1.11 2005/09/20 19:46:31 lwoodard Exp $
//Last Edited: August 2005; by: lwoodard; for:qt3 to qt4 porting

#include "mapimgform.h"

#include <QAction>
#include <QByteArray>
#include <QDataStream>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QProcess>
#include <QSettings>
#include <QToolButton>
#include <QToolBar>
#include <QTooltip>
#include <QUrl>
#include <QWhatsThis>
#include <QVariant>

#include "mapimgversion.h"
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
	setAcceptDrops( true );

	setupContents();
	createActions();
	setupMenubar();
	setupToolbar();
	makeConnections();
	loadSettings();

	imgName = "";
	imgSet = false;
	newInfo = false;

	ignorePreviewSignals = false;
	prevLast = prevInput;

	resize( QSize(600, 524).expandedTo(minimumSizeHint()) );	
}

void mapimgForm::setupContents()
{
	setCentralWidget( new QWidget( this, "qt_central_widget" ) );

	contentLayout = new QHBoxLayout( centralWidget() );
	contentLayout->setMargin(2);
	contentLayout->setSpacing(1);
	contentLayout->setSizeConstraint( QLayout::Minimum );

	inInfoFrame = new QInfoFrame( centralWidget() );
	inInfoFrame->setAsInput();
	inInfoFrame->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::MinimumExpanding );
	contentLayout->addWidget( inInfoFrame );

	imgFrame = new QImgFrame( centralWidget() );
	imgFrame->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	imgFrame->setLineWidth(2);
	imgFrame->setPixmap( QPixmap("./Resources/USGS Logo.png" ) );
	imgFrame->setMinimumSize( QSize( 167, 62 ) );
	imgFrame->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
	imgFrame->hide();
	imgFrame->installEventFilter( this );
	contentLayout->addWidget( imgFrame );

	outInfoFrame = new QInfoFrame( centralWidget(), "outInfoFrame" );
	outInfoFrame->setAsOutput(); 
	outInfoFrame->setPartner( inInfoFrame );
	outInfoFrame->setReadOnly( true, 1 );
	outInfoFrame->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::MinimumExpanding );
	contentLayout->addWidget( outInfoFrame );
}

void mapimgForm::createActions()
{
	//QIcon *openIcon
	inOpenAction = new QAction( tr("&Open"), this );
	inOpenAction->setIcon( QIcon( QPixmap("./Resources/open.png").scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation) ) );
	inOpenAction->setShortcut( tr("Ctrl+O") );
	inOpenAction->setToolTip( tr("Open an input file.") );

	inSaveAction = new QAction( tr("Save Input Parameters"), this);
	inSaveAction->setIcon( QIcon( "./Resources/insave.png" ) );
	inSaveAction->setToolTip( tr("") );

	exitAction = new QAction( tr("Exit"), this);
	exitAction->setIcon( QIcon( "./Resources/exit.png" ) );
	exitAction->setToolTip("Close the MapIMG application.");

	authAction = new QAction( tr("Edit Author"), this );
	authAction->setToolTip( tr("Change the author properties for the current user.") );

	inInfoAction = new QAction( "Show Input Parameters", this );
	inInfoAction->setIcon( QIcon( "./Resources/inputinfo.png" ) );
	inInfoAction->setCheckable( true );
	inInfoAction->setOn( inInfoFrame->isShown() );

	viewShowAction = new QAction(	"Show Preview", this );
	viewShowAction->setIcon( QIcon( "./Resources/preview.png" ) );
	viewShowAction->setCheckable( true );

	outInfoAction = new QAction( "Show Output Parameters", this );
	outInfoAction->setIcon( QIcon ( "./Resources/outputinfo.png" ) );
	outInfoAction->setCheckable( true );
	outInfoAction->setOn( outInfoFrame->isShown() );

	outSaveAction = new QAction( "Reproject and &Save", this );
	outSaveAction->setIcon( QIcon( "./Resources/outsave.png" ) );
	outSaveAction->setShortcut( tr("Ctrl+S") );

	viewResampleAction = new QAction( tr("Resample Preview"), this );
	viewResampleAction->setShortcut( tr("F5") );
	viewResampleAction->setIcon( QIcon( "./Resources/resample.png" ) );
	viewResampleAction->setToolTip( tr("") );

	previewProjAction = new QAction( "Preview Reprojection", this );
	previewProjAction->setIcon( QIcon( "./Resources/previewproject.png" ) );

	webDSSAction = new QAction( "Decision Support System", this );
	webDSSAction->setToolTip( tr("") );

	aboutAction = new QAction( "About MapIMG", this );

	aboutQtAction = new QAction( "About Qt", this );
}

void mapimgForm::setupMenubar()
{
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

	toolsMenu = menuBar()->addMenu( tr( "&Tools" ) );
	toolsMenu->addAction( webDSSAction );

	helpMenu = menuBar()->addMenu( tr( "&Help" ) );
	helpMenu->addAction( aboutAction );
	helpMenu->addAction( aboutQtAction );
}

void mapimgForm::setupToolbar()
{
	toolBar = addToolBar(tr("File"));
	toolBar->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	toolBar->setMovable(false);
	toolBar->setIconSize( QSize(25,25) );

	toolBar->addAction( inOpenAction );
	toolBar->addAction( outSaveAction );
	toolBar->addSeparator();
	toolBar->addAction( inInfoAction );

	viewShowButton = new QToolButton( QIcon( "./Resources/preview.png" ),
		"Show Preview", "", NULL, 0, this, "previewButton" );
	viewShowButton->installEventFilter( this );

	toolBar->addWidget( viewShowButton );
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
}

void mapimgForm::makeConnections()
{
	connect( exitAction, SIGNAL( activated() ), this, SLOT( close() ) );
	connect( authAction, SIGNAL( activated() ), this, SLOT( editAuthor() ) );

	connect( inInfoAction, SIGNAL( toggled(bool) ), inInfoFrame, SLOT( setShown(bool) ) );
	connect( inOpenAction, SIGNAL( activated() ), this, SLOT( inOpenClicked() ) );
	connect( inSaveAction, SIGNAL( activated() ), this, SLOT( inSaveClicked() ) );
	connect( inInfoFrame, SIGNAL( locked() ), this, SLOT( inSaveClicked() ) );

	connect( viewShowAction, SIGNAL( toggled(bool) ), this, SLOT( showPreview(bool) ) );
	connect( viewResampleAction, SIGNAL( activated() ), imgFrame, SLOT( resample() ) );
	connect( previewProjAction, SIGNAL( activated() ), this, SLOT( showOutputPreview() ) );

	connect( viewShowButton, SIGNAL( toggled(bool) ), this, SLOT( showPreview(bool) ) );
	connect( prevInput, SIGNAL( toggled(bool) ), this, SLOT( showInputPreview(bool) ) );
	connect( prevOutput, SIGNAL( toggled(bool) ), this, SLOT( showOutputPreview(bool) ) );

	connect( outInfoAction, SIGNAL( toggled(bool) ), outInfoFrame, SLOT( setShown(bool) ) );
	connect( outSaveAction, SIGNAL( activated() ), this, SLOT( outSaveClicked() ) );

	connect( webDSSAction, SIGNAL( activated() ), this, SLOT( webDSSClicked() ) );

	connect( aboutAction, SIGNAL( activated() ), this, SLOT( aboutClicked() ) );
	connect( aboutQtAction, SIGNAL( activated() ), this, SLOT( showAboutQt() ) );
}


void mapimgForm::loadSettings()
{
	QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,"" );
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
	return QMainWindow::eventFilter( object, event ); 
}

void mapimgForm::dragEnterEvent( QDragEnterEvent *evt )
{
	if ( evt->mimeData()->hasUrls() )	//File name is in url form
		evt->accept();
}

void mapimgForm::dropEvent( QDropEvent *evt )
{
	QString text;

	if ( evt->mimeData()->hasUrls() )	
	{
			//.toLocalFile automatically changes the / directions so you don't have
			//to do parsing yourself.
		text = evt->mimeData()->urls()[0].toLocalFile();

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
	QString temp = QFileDialog::getOpenFileName(
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

	info.setAuthor( authName, authCompany, authEmail ); 

	if( info.load( imgName ) )
	{
		inInfoFrame->setInfo( info );
		inInfoFrame->lock( true, false );
		newInfo = false;

		prevLast = NULL;
		showInputPreview( imgFrame->isShown() );
	}
	else
	{
		info.setAuthor( authName, authCompany, authEmail );
		inInfoFrame->lock( false, false );
		inInfoAction->setOn( true );
		outInfoAction->setOn( false );

		imgFrame->setPixmap( QPixmap( "./Resources/USGS Logo.png" ) );
		viewShowButton->setOn( false );
		inInfoFrame->setInfo( info );	//This is where it breaks

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
	showInputPreview( imgFrame->isShown() );
	return;
}

/*

*/
void mapimgForm::showPreview( bool on )
{
	if( ignorePreviewSignals )
		return;

	if( !prevLast )               /*if a file is freshly loaded, but not displayed, prevLast will be NULL*/
		showInputPreview( true );     /*this addition will generate the preview for this case*/

	prevLast->setOn(on);

	ignorePreviewSignals = true;
	{
		viewShowButton->setOn(on);
	}
	ignorePreviewSignals = false;

}

void mapimgForm::showInputPreview( bool on )
{
	if( ignorePreviewSignals )
		return;

	if( on && prevLast != prevInput )
	{
		imgFrame->loadImg( inInfoFrame->info().imgFileName() );
		prevLast = prevInput;
	}

	ignorePreviewSignals = true;       /* what is this block? is it conditional? */
	{
		prevInput->setOn(on);
		prevOutput->setOn(false);
		viewShowButton->setOn(on);
		imgFrame->setShown(on);
	}
	ignorePreviewSignals = false;
}

void mapimgForm::showOutputPreview( bool on )
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
	input.setAuthor("Unknown", "Unknown", "Unknown");
	if( !mapimg::readytoReproject(input, this) )
		return false;

	RasterInfo output( outInfoFrame->info() );
	output.setAuthor("Unknown", "Unknown", "Unknown");
	output.setDataType( input.isSigned(), input.bitCount(), input.type() );

	if( !mapimg::readytoFrameIt( output, this ) )
		return false;
	mapimg::frameIt( output );
	outInfoFrame->setInfo( output );
	if( !mapimg::readytoReproject( output, this, false ) )
		return false;

	RasterInfo smallInput;
	smallInput.setFileName( QDir::currentDirPath().append("/temp_small.img") );
	smallInput.setAuthor("Unknown", "Unknown", "Unknown");
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

	return mapimg::reproject( smallInput, output, resample );
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
	QString temp = QFileDialog::getSaveFileName(
		outPath, "MapIMG Raster Files (*.img)", this, "", "Choose a destination for the reprojection");
	if( temp.isEmpty() )
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

	authorForm *form = new authorForm(settings, this, false,
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


//Tools
void mapimgForm::webDSSClicked()
{
	launchWebTool( "http://mcmcweb.er.usgs.gov/research/DSSMain/DSSApplet.html" );

	return;
}

void mapimgForm::launchWebTool( const QString& url )
{
	QProcess* web = new QProcess( this );
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
	QByteArray allBrowsers = getenv( "BROWSER" );

	if( allBrowsers.isEmpty() ) //if it is empty we're hosed
	{
		qDebug( "No $BROWSER environment varialbe set." );
		supportedPlatform = false;
		executeProcess = false;
	}
	else
	{
		//split the list
		QStringList browsers = QString( allBrowsers ).split( ':' );

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

	QObject::connect( web, SIGNAL( processExited() ), web, SLOT( deleteLater() ) );

	if( !QUrl( url ).isValid() || !supportedPlatform )	
		QMessageBox::information( this, "Mapimg", QString("Unable to launch web browser to %1").arg( url ) );

	return;
}