// $Id: mapimgform.cpp,v 1.8 2005/02/03 18:12:18 jtrent Exp $


#include "mapimgform.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qdragobject.h>
#include <qwidgetstack.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qsettings.h>

#include "mapimgversion.h"
#include "mapimgimages.h"
#include "qimgframe.h"
#include "qinfoframe.h"
#include "rasterinfo.h"
#include "resampleinfo.h"
#include "window_flags.h"
#include "resampleform.h"
#include "aboutform.h"
#include "authorform.h"
#include "tiff2img.h"
#include "mapimg.h"

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
mapimgForm::mapimgForm( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
   setIcon( mapimgImage( "USGS Swirl Sphere" ) );
   setCaption( QString("mapimg%1").arg(MAJOR_VERSION) );

   setCentralWidget( new QWidget( this, "qt_central_widget" ) );
   formLayout = new QHBoxLayout( centralWidget(), 1, 2, "formLayout");

   ////////
   //FILE MENU
   ////////

   //actions
   exitAction = new QAction( QIconSet( mapimgImage( "exit" ) ), 
      "E&xit", QKeySequence("Ctrl+X"), this, "exitAction" );
   bigAction = new QAction( 
      "Use &Big Icons", QKeySequence("Ctrl+B"), this, "bigAction" );
   bigAction->setToggleAction( true );

   //signals and slots
   connect( exitAction, SIGNAL( activated() ), this, SLOT( close() ) );
   connect( bigAction, SIGNAL( toggled(bool) ), this, SLOT( setUsesBigPixmaps(bool) ) );

   ////////
   //OPTIONS MENU
   ////////

   //actions
   authAction = new QAction( QIconSet( mapimgImage( "" ) ), 
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
   inInfoAction = new QAction( QIconSet( mapimgImage( "inputinfo" ) ), 
      "Show Input Parameters", QKeySequence(), this, "inInfoAction" );
   inInfoAction->setToggleAction( true );
   inInfoAction->setOn( inInfoFrame->isShown() );
   inOpenAction = new QAction( QIconSet( mapimgImage( "open" ) ), 
      "&Open", QKeySequence("Ctrl+O"), this, "inOpenAction" );
   inSaveAction = new QAction( QIconSet( mapimgImage( "insave" ) ), 
      "Save Input Parameters", QKeySequence(), this, "inSaveAction" );


   //signals and slots
   connect( inInfoAction, SIGNAL( toggled(bool) ), inInfoFrame, SLOT( setShown(bool) ) );
   connect( inOpenAction, SIGNAL( activated() ), this, SLOT( inOpenClicked() ) );
   connect( inSaveAction, SIGNAL( activated() ), this, SLOT( inSaveClicked() ) );
   connect( inInfoFrame, SIGNAL( locked() ), this, SLOT( inSaveClicked() ) );


   ////////
   //PREVIEW
   ////////

   //widgets
   imgFrame = new QImgFrame( centralWidget(), "imgFrame" );
   imgFrame->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
   imgFrame->setLineWidth(2);
   imgFrame->setPixmap( mapimgImage( "USGS Logo" ) );
   imgFrame->setMinimumSize( QSize( 167, 62 ) );
   imgFrame->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
   imgFrame->setShown( false );
   formLayout->addWidget( imgFrame );

   //actions
   viewShowAction = new QAction( QIconSet( mapimgImage( "preview" ) ), 
      "Show Preview", QKeySequence(), this, "inViewAction" );
   viewShowAction->setToggleAction( true );
   viewResampleAction = new QAction( QIconSet( mapimgImage( "resample" ) ), 
      "Resample Preview", QKeySequence("F5"), this, "resampleAction" );
   previewProjAction = new QAction( QIconSet( mapimgImage( "previewproject" ) ), 
      "&Preview Reprojection", QKeySequence("Ctrl+P"), this, "resampleAction" );


   //signals and slots
   connect( viewShowAction, SIGNAL( toggled(bool) ), imgFrame, SLOT( setShown(bool) ) );
   connect( viewResampleAction, SIGNAL( activated() ), imgFrame, SLOT( resample() ) );
   connect( previewProjAction, SIGNAL( activated() ), this, SLOT( previewProjClicked() ) );

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
   outInfoAction = new QAction( QIconSet( mapimgImage( "outputinfo" ) ), 
      "Show Output Parameters", QKeySequence(), this, "outInfoAction" );
   outInfoAction->setToggleAction( true );
   outInfoAction->setOn( outInfoFrame->isShown() );
   outSaveAction = new QAction( QIconSet( mapimgImage( "outsave" ) ), 
      "Reproject and &Save", QKeySequence("Ctrl+S"), this, "outSaveAction" );


   //signals and slots
   connect( outInfoAction, SIGNAL( toggled(bool) ), outInfoFrame, SLOT( setShown(bool) ) );
   connect( outSaveAction, SIGNAL( activated() ), this, SLOT( outSaveClicked() ) );


   ////////
   //HELP MENU
   ////////

   //actions
   aboutAction = new QAction( "About mapimg2", QKeySequence(""), this, "aboutAction" );
   aboutQtAction = new QAction( "About Qt", QKeySequence(""), this, "aboutQtAction" );

   //signals and slots
   connect( aboutAction, SIGNAL( activated() ), this, SLOT( aboutClicked() ) );
   connect( aboutQtAction, SIGNAL( activated() ), this, SLOT( showAboutQt() ) );


   ////////
   //TOOLBARS
   ////////
   toolBar = new QToolBar( this, "toolBar" );
   
   inOpenAction->addTo( toolBar );
   outSaveAction->addTo( toolBar );
   toolBar->addSeparator();
   inInfoAction->addTo( toolBar );
   viewShowAction->addTo( toolBar );
   outInfoAction->addTo( toolBar );
   toolBar->addSeparator();
   viewResampleAction->addTo( toolBar );
   previewProjAction->addTo( toolBar );


   ////////
   //MENUBAR
   ////////

   menuBar = new QMenuBar( this, "menuBar" );

   //File
   File = new QPopupMenu( this );
   inOpenAction->addTo( File );
   File->insertSeparator();
   inSaveAction->addTo( File );
   outSaveAction->addTo( File );
   File->insertSeparator();
   exitAction->addTo( File );
   menuBar->insertItem( "File", File);

   //Options
   Options = new QPopupMenu( this );
   authAction->addTo( Options );
   menuBar->insertItem( "Options", Options );

   //Preview
   Preview = new QPopupMenu( this );
   viewResampleAction->addTo( Preview );
   previewProjAction->addTo( Preview );
   menuBar->insertItem( "Preview", Preview);

   //Help
   Help = new QPopupMenu( this );
   aboutAction->addTo( Help );
   aboutQtAction->addTo( Help );
   menuBar->insertItem( "Help", Help);


   QSettings *settings = new QSettings( QSettings::Ini );
   settings->setPath( "USGS.gov", "mapimg2" );

   inPath = settings->readEntry( "/USGS/mapimg2/DefaultInputPath" );
   outPath = settings->readEntry( "/USGS/mapimg2/DefaultOutputPath" );
   authName = settings->readEntry( "/USGS/mapimg2/AuthorName" );
   authCompany = settings->readEntry( "/USGS/mapimg2/AuthorCompany" );
   authEmail = settings->readEntry( "/USGS/mapimg2/AuthorEmail" );

   if( inPath.isNull() || !QFile::exists(inPath) )
      settings->writeEntry( "/USGS/mapimg2/DefaultInputPath", QDir::currentDirPath() );
   if( outPath.isNull() || !QFile::exists(outPath) )
      settings->writeEntry( "/USGS/mapimg2/DefaultOutputPath", QDir::currentDirPath() );
   if( authName.isNull() )
   {
      settings->writeEntry( "/USGS/mapimg2/AuthorName", QString("Unknown") );
      settings->writeEntry( "/USGS/mapimg2/AuthorCompany", QString("Unknown") );
      settings->writeEntry( "/USGS/mapimg2/AuthorEmail", QString("Unknown") );

      editAuthor();
   }

   delete settings;

   resize( QSize(600, 524).expandedTo(minimumSizeHint()) );
   clearWState( WState_Polished );
   setAcceptDrops( true );

   imgName = "";
   imgSet = false;
   newInfo = false;
}

/*
   Delete temporary files.
*/
mapimgForm::~mapimgForm()
{
   if( QFile::exists( "mapimg.log" ) )
      QFile::remove( "mapimg.log" );

   if( QFile::exists( "temp.xml" ) )
      QFile::remove( "temp.xml" );

   if( QFile::exists( "temp.img" ) )
      QFile::remove( "temp.img" );

   if( QFile::exists( "mapimg.xml" ) )
      QFile::remove( "mapimg.xml" );

   if( QFile::exists( "mapimg.img" ) )
      QFile::remove( "mapimg.img" );
}

void mapimgForm::dragEnterEvent( QDragEnterEvent *evt )
{
   if ( QTextDrag::canDecode( evt ) ) 
      evt->accept();
}

void mapimgForm::dropEvent( QDropEvent *evt )
{
   QString text;

   if ( QTextDrag::decode( evt, text ) )
   {
      int gi = text.findRev( "img" ), li = text.findRev( "xml" );
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
         inInfoAction->setOn(true);
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
         inPath, "mapimg Files (*.img);;Tiff Files (*.tif)", this, "", "Choose an image");

   if( openFile(temp) )
   {   
      inPath = imgName.left( imgName.findRev( "/" ) );
      QSettings *settings = new QSettings( QSettings::Ini );
      settings->setPath( "USGS.gov", "mapimg2" );
      settings->writeEntry( "/USGS/mapimg2/DefaultInputPath", inPath );
      delete settings;
   }
}

bool mapimgForm::openFile( QString inFile )
{
   if( inFile == "" )
      return false;

   if( !QFile::exists( inFile ) )
   {
      QMessageBox::information( this, "File not found", inFile );
      return false;
   }

   if( inFile.endsWith( ".tif" ) )
   {
      int convert = QMessageBox::information( this, "Tiff Image",
         "Tiff images cannot be projected directly\n"
         "Would you like to export to generic binary?",
         QMessageBox::Yes, QMessageBox::No );

      if( convert == 4 ) //"No" Button
         return false;

      QString tiffFile = inFile;
      inFile.replace( ".tif", ".img" );

      if( tiff2img( tiffFile, inFile ) )
         QMessageBox::information( this, "Tiff Image",
            "Conversion completed succesfully." );
      else
      {
         QMessageBox::information( this, "Tiff Image",
            "Conversion failed for unknown reason." );
         return false;
      }
   }

   if( !inFile.endsWith( ".img" ) )
   {
      QMessageBox::information( this, "Unknown file type", inFile );
      return false;
   }

   imgName = inFile;
   imgSet = true;
   RasterInfo info;

   if( info.load( imgName ) )
   {
      inInfoFrame->setInfo( info );
      inInfoFrame->lock( true, false );

      imgFrame->loadImg( info.imgFileName() );
      newInfo = false;
   }
   else
   {
      inInfoFrame->lock( false, false );
      inInfoAction->setOn( true );
      outInfoAction->setOn( false );

      imgFrame->setPixmap( mapimgImage( "USGS Logo" ) );
      viewShowAction->setOn( false );
      inInfoFrame->setInfo( info );

      QMessageBox::information( this, "XML file not found",
         "Please use the Input Info Editor to define the map and projection parameters.\n"
         "\n-The lock button saves changes."
         "\n-The calculator button figures out the rows, columns and upper left coordinates.");
      newInfo = true;
   }

   ////////Parse fileName
   QString cap( imgName );
   cap.replace('\\', '/');
   int index = cap.findRev("/");
   index++;
   cap = cap.right(cap.length() - index);
   setCaption(cap + " - mapimg2");

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
      QMessageBox::critical( this, "Input not set",
         "No input to save." );
      return;
   }

   RasterInfo i( inInfoFrame->info() );

   if( !mapimg::readytoReproject(i, this) )
   {
      inInfoFrame->lock( false, false );
      return;
   }

   if( !newInfo && QFile::exists( i.xmlFileName() ) )
   {
      int arg = QMessageBox::question( this, "Overwrite File?",
         QString("%1\n\nInput XML file already exists. Do you want to overwrite it?").arg(i.xmlFileName()),
         "Yes", "No");

      if( arg == 1 )
      {
         i.load();
         inInfoFrame->setInfo( i );
         return;
      }
   }

   i.save();
   imgFrame->loadImg( imgName, true );
   return;
}

/*
   previewProjClicked() is used to preview a projection. This is useful
because many reprojections can take hours but using this function the output
can be preivewed before investing the time. The reprojection function is
linear with size increases having a proportional effect on run-time. This 
preview down samples the input with a constant time algorithm and then 
reprojects to a much smaller output.
*/
void mapimgForm::previewProjClicked()
{
   if( !imgSet )
   {
      QMessageBox::critical( this, "Input not set",
         "No input to preview reprojection from." );
      return;
   }

   RasterInfo input( inInfoFrame->info() );
   if( !mapimg::readytoReproject(input, this) )
      return;

   RasterInfo output( outInfoFrame->info() );
   output.setDataType( input.isSigned(), input.bitCount(), input.type() );
   output.setFillValue( input.fillValue() );
   if( !mapimg::readytoReproject(input, this) )
      return;
   mapimg::frameIt( output );

   mapimg::downSampleImg( input, 720 );
   input.save();

   if( !mapimg::downSizeProjection( output, 720 ) )
   {
      QMessageBox::critical( this, "Output Error", 
         "Unexpected failure with output operations" );
      return;
   }

   output.setFileName( QDir::currentDirPath().append("/mapimg.img") );
   output.save();

   ResampleInfo resample;
   resample.setResampleCode( ResampleInfo::NearestNeighbor );
   resample.setFillValue( input.fillValue() );
   resample.setNoDataValue( input.noDataValue() );

   mapimg::reproject( input, output, resample );

   inInfoAction->setOn(false);
   viewShowAction->setOn(true);
   imgFrame->loadImg( output.imgFileName(), true );
}

/*
   The outSaveClicked() function is used to actually reproject. If both the
input and output parameters check out ok then the user is prompted for the
destination to reproject to. After checking the output filename the
the reprojection begins.
*/
void mapimgForm::outSaveClicked()
{
   if( !imgSet )
   {
      QMessageBox::critical( this, "Input not set",
         "No input to reproject from." );
      return;
   }

   RasterInfo input( inInfoFrame->info() );
   if( !mapimg::readytoReproject(input, this) )
      return;

   RasterInfo output( outInfoFrame->info() );
   output.setDataType( input.isSigned(), input.bitCount(), input.type() );
   output.setFillValue( input.fillValue() );
   output.setAuthor( authName, authCompany, authEmail );

   if( !mapimg::readytoReproject( output, this ) )
      return;

   mapimg::frameIt( output );
   outInfoFrame->setInfo( output );

   QString temp = QFileDialog::getSaveFileName(
         outPath, "mapimg Raster Files (*.img)", this, "", "Choose a destination for the reprojection");

   if( temp.isNull() )
      return;

   if( !output.setFileName( temp ) )
   {
      QMessageBox::critical( this, "Output Error", 
         "Invalid output file name" );
      return;
   }

   if( QFile::exists( output.imgFileName() ) )
   {
      int status = QMessageBox::question( this, "Save", 
         output.imgFileName() + " already exists."
         "\nDo you want to replace it?",
         QMessageBox::Yes, QMessageBox::No );

      if( status == QMessageBox::No )
         return;
      
      QFile::remove( output.imgFileName() );
      if( QFile::exists( output.xmlFileName() ) )
         QFile::remove( output.xmlFileName() );
   }

   if( !output.save( temp ) )
   {
      QMessageBox::critical( this, "Output Error", 
         "Unable to save .xml file" );
      if( QFile::exists( output.xmlFileName() ) )
         QFile::remove( output.xmlFileName() );
      return;
   }

   ResampleForm *resForm = new ResampleForm( this, "resForm", false,
      WINDOW_FLAGS );
   resForm->exec();

   if( resForm->wasCanceled() )
   {
      delete resForm;
      return;
   }

   ResampleInfo resample( resForm->info() );
   resample.setFillValue( input.fillValue() );
   resample.setNoDataValue( input.noDataValue() );

   mapimg::reproject( input, output, resample, this );
   delete resForm;

   outPath = temp.left( temp.findRev( "/" ) );
   QSettings *settings = new QSettings( QSettings::Ini );
   settings->setPath( "USGS.gov", "mapimg2" );
   settings->writeEntry( "/USGS/mapimg2/DefaultOutputPath", outPath );
   delete settings;
}

void mapimgForm::editAuthor()
{
   QSettings *settings = new QSettings( QSettings::Ini );
   settings->setPath( "USGS.gov", "mapimg2" );

   authorForm *form = new authorForm(settings, this, "about", false,
      WINDOW_FLAGS );
   form->exec();
   delete form;

   authName = settings->readEntry( "/USGS/mapimg2/AuthorName" );
   authCompany = settings->readEntry( "/USGS/mapimg2/AuthorCompany" );
   authEmail = settings->readEntry( "/USGS/mapimg2/AuthorEmail" );

   delete settings;
}

void mapimgForm::aboutClicked()
{
   aboutForm *about = new aboutForm(this, "about", false,
      WINDOW_FLAGS );
   about->exec();
   delete about;
}
