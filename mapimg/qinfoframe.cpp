// $Id: qinfoframe.cpp,v 1.16 2005/03/17 18:57:25 rbuehler Exp $


#include "qinfoframe.h"
#include "qgctpbox.h"
#include "mapimgimages.h"
#include "mapimg.h"
#include "rasterinfo.h"
#include "mapimgvalidator.h"

#include <qvalidator.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qtooltip.h>
#include <qcheckbox.h>

const uint INFO_PRECISION = 6;

#include "gctpnames.h"
#include "mapimgpalette.h"

/*
   The QMapTab has one constructor. It executes in three stages.

   STAGE 1: It sets its appearance by turning off the Horizontal SrcollBar, 
forcing on the Vertical ScrollBar, and generating the 'contents' widget for
holding all the contents.
   
   STAGE 2: It creates all the QWidgets and their respective QHBoxes in which
they all reside.
   
   STAGE 3: A level of polish is applied to every widget. Adding QTooltips,
applying QValidators, fixing sizes, and making connections happens here.
*/
QMapTab::QMapTab( QWidget* parent, const char* name)
   : QScrollView( parent, name)
{
   ////////STAGE 1: Appearance/Setup
   //
   setHScrollBarMode( QScrollView::AlwaysOff );
   setVScrollBarMode( QScrollView::AlwaysOn );

   contents = new QVBox( viewport(), "contents" );
   addChild( contents );

   ////////STAGE 2: Generate Widgets
   //
   //titleBox - Contains function buttons and the title of the Tab
   QHBox *titleBox = new QHBox( contents );
   copyButton = new QPushButton( QIconSet( mapimgImage( "copy" ) ),
      "", titleBox, "copyButton" );
   lockButton = new QPushButton( QIconSet( mapimgImage( "unlocked" ) ),
      "", titleBox, "lockButton" );
   QLabel *mapLabel = new QLabel( "Map Info", titleBox, "mapLabel" );
   frameButton = new QPushButton( QIconSet( mapimgImage( "calculate" ) ),
      "", titleBox, "frameButton" );

   //fileBox - Contains a lineedit for displaying the ".info" filename
   fileBox = new QVBox( contents );
   (void) new QLabel( "File Name:", fileBox);
   fileEdit = new QLineEdit( "", "", fileBox, "fileEdit" );

   //rowcolBox - Contains two spin boxes for defining the pixel dimensions
   rowcolBox = new QVBox( contents );
   (void) new QLabel( "Rows of Pixels:", rowcolBox);
   rowSpin = new QSpinBox( 0, 100000000, 1, rowcolBox, "rowSpin" );
   (void) new QLabel( "Columns of Pixels:", rowcolBox);
   colSpin = new QSpinBox( 0, 100000000, 1, rowcolBox, "colSpin" );

   //comboBox - Contains four combo boxes for selecting preset values
   comboBox = new QVBox( contents );
   (void) new QLabel( "Units", comboBox, "unitLabel");
   unitCombo = new QComboBox( comboBox, "unitCombo" );
   (void) new QLabel( "Spheroid", comboBox, "spheroidLabel" );
   spheroidCombo = new QComboBox( comboBox, "spheroidCombo" );
   (void) new QLabel( "Pixel Size", comboBox);
   pixelCombo = new QComboBox( comboBox, "pixelCombo" );
   pixelEdit = new QLineEdit( comboBox, "pixelEdit" );

   //ulBox - Contains line edits for defining the upper left corner of the map
   ulBox = new QVBox( contents );
   (void) new QLabel( "UL Latitude in meters", ulBox);
   ulLatEdit = new QLineEdit( ulBox, "ulLatEdit" );
   (void) new QLabel( "UL Longitude in meters", ulBox);
   ulLonEdit = new QLineEdit( ulBox, "ulLonEdit" );

   //dataBox - Contains one combo box for selecting what type of data it is
   dataBox = new QVBox( contents );
   (void) new QLabel( "Pixel Data Type", dataBox);
   dataCombo = new QComboBox( dataBox, "dataCombo" );
   (void) new QLabel( "Fill Value", dataBox );
   QHBox *fillBox = new QHBox( dataBox );
   hasFillCheck = new QCheckBox( fillBox, "hasFillCheck" );
   hasFillCheck->hide();
   fillEdit = new QLineEdit( fillBox, "fillEdit" );
   fillEdit->installEventFilter( this );
   fillButton = new QPushButton( "?", fillBox );

   (void) new QLabel( "No Data Value", dataBox );
   QHBox *noDataBox = new QHBox( dataBox );
   hasNoDataCheck = new QCheckBox( noDataBox, "hasNoDataCheck" );
   hasNoDataCheck->hide();
   noDataEdit = new QLineEdit( noDataBox, "noDataEdit" );

   lastFillValue = "";
   lastNoDataValue = "";

   ////////STAGE 3: Polish Widgets
   //
   //titleBox
   copyButton->setMaximumWidth( 28 ); copyButton->setMaximumHeight( 28 );
   QToolTip::add( copyButton, "Copy from input info editor." );
   lockButton->setMaximumWidth( 28 ); lockButton->setMaximumHeight( 28 );
   lockButton->setToggleButton( true );
   QToolTip::add( lockButton, "Use to allow editing of .xml file.<br><br>"
      "<b>Note</b>: Locking this info editor automatically saves to the .xml file." );
   QFont largeFont(  mapLabel->font() );
   largeFont.setPointSize( 12 );
   mapLabel->setFont( largeFont );
   mapLabel->setAlignment( int( QLabel::AlignCenter ) );
   frameButton->setMaximumWidth( 28 ); frameButton->setMaximumHeight( 28 );
   QToolTip::add( frameButton, "Calculate rows, columns, and upper left coordinates." );

   //fileBox
   fileEdit->setReadOnly( true );
   QToolTip::add( fileEdit, "File name of the loaded .xml file" );

   //rowcolBox
   QToolTip::add( rowSpin, "Height of the map in pixels" );
   QToolTip::add( colSpin, "Width of the map in pixels" );

   //comboBox
   unitCombo->insertItem( "" );
   unitCombo->insertStringList( unitNames );
   unitCombo->setCurrentText( "Meters" );
   unitCombo->setDisabled( true );
   QToolTip::add( unitCombo, "Units for all measurements<br><br>"
      "<b>Note</b>: Currently only \"Meters\" are supported." );
   spheroidCombo->insertItem( "" );
   spheroidCombo->insertStringList( spheroidNames );
   spheroidCombo->setCurrentText( "Sphere of Radius 6370997 meters" );
   spheroidCombo->setDisabled( true );
   QToolTip::add( spheroidCombo, "Name of the spheroid used in projection<br><br>"
      "<b>Note</b>: Currently only \"Sphere of Radius 6370997 meters\" is supported." );
   pixelCombo->insertItem( "" );
   pixelCombo->insertStringList( pixelSizes );
   QToolTip::add( pixelCombo, "Size of each pixel in the raster image" );
   QToolTip::add( pixelEdit, "Pixel size<br>"
      "<b>Meters:</b> Entered value must be from 0 to 10000000." );
   pixelEdit->setValidator( new MapimgValidator( 0, 10000000, 12, pixelEdit ) );
   pixelEdit->hide();

   //ulBox
   ulLatEdit->setValidator( new MapimgValidator( -100000000, 100000000, 12, ulLatEdit ) );
   QToolTip::add( ulLatEdit, "Latitude of upper left corner of map<br>"
      "<b>Meters:</b> Entered value must be from -100000000 to 100000000." );
   ulLonEdit->setValidator( new MapimgValidator( -100000000, 100000000, 12, ulLonEdit ) );
   QToolTip::add( ulLonEdit, "Longitude of upper left corner of map<br>"
      "<b>Meters:</b> Entered value must be from -100000000 to 100000000." );

   //dataBox
   dataCombo->insertItem( "" );
   dataCombo->insertStringList( dataTypes );
   QToolTip::add( dataCombo, "Data type of each pixel in the raster image" );
   fillEdit->setValidator( new MapimgValidator( -100000000, 100000000, INFO_PRECISION, fillEdit ) );
   QToolTip::add( fillEdit, "Fill value to represent a pixel outside the map frame<br>"
      "Entered value must be from -100000000 to 100000000." );
   fillButton->setMaximumSize( 20, 20 );
   QToolTip::add( fillButton, "Recommend fill value by reading file and "
      "solving for the maximum value + 2" );
   noDataEdit->setValidator( new MapimgValidator( -100000000, 100000000, INFO_PRECISION, noDataEdit ) );
   QToolTip::add( noDataEdit, "\"No Data\" value to represent a pixel inside the map frame"
      " with no value<br>"
      "Entered value must be from -100000000 to 100000000." );

   //This connection is for keeping the pixelEdit hidden until it is needed
   connect(pixelCombo, SIGNAL(activated(int)), this, SLOT(pixelChange(int)));
   connect(dataCombo, SIGNAL(activated(const QString&)), this, SLOT(dataChange(const QString&)));
   connect(hasFillCheck, SIGNAL(toggled(bool)), this, SLOT(fillCheckToggled(bool)));
   connect(hasNoDataCheck, SIGNAL(toggled(bool)), this, SLOT(noDataCheckToggled(bool)));
}

/*
   Nothing to destruct because QObjects take care of their children.
*/
QMapTab::~QMapTab()
{}




/* eventFilter is an event filter for misc GUI events. Right now it is used
to dsiplay a popup menu when a user right-clicks on teh fill value edit box
*/
bool QMapTab::eventFilter( QObject* object, QEvent* event )
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
         return QScrollView::eventFilter( object, event );
      }
   }

   return QScrollView::eventFilter( object, event );
}


void QMapTab::fillCheckToggled( bool state )
{
    if(fillEdit->validator() != 0 )
       ((MapimgValidator*)fillEdit->validator())->setAllowUndefined( !state );

    if( state == QButton::Off )
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

void QMapTab::noDataCheckToggled( bool state )
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
    }

    if( noDataEdit->validator() != 0 )
    {
    	QString noDataString = noDataEdit->text();
        ((MapimgValidator*)noDataEdit->validator())->fixup( noDataString );
        noDataEdit->setText( noDataString );
    }

    return;
}

/*
   The pixelChange(int) function is connected to the pixelCombo so that
whenever the user selects a different pixel size this function is called.
Its purpose is to keep the pixelEdit hidden until it is needed. The pixelEdit
is used for entering custom pixel sizes(meters).
*/
void QMapTab::pixelChange(int index)
{
   if( index > 0 && index < 6 )
   {
      pixelEdit->setText( pixelValues[index-1] );
   }
   pixelEdit->setShown( index == 6 );
}

/*
   The dataChange(qstring) function is connected to the dataCombo so that
whenever the user selects a different data type this function is called.
Its purpose is to keep the fill value and no data validators up to date
with which data type it is trying to validate.
*/
void QMapTab::dataChange( const QString& newDataType )
{
   QString tempText = "";
   if( newDataType.stripWhiteSpace() != "" )
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

/*
   The QGctpTab has one constructor. It executes in three stages.

   STAGE 1: It sets its appearance by turning off the Horizontal SrcollBar,
forcing on the Vertical ScrollBar, and generating the 'contents' widget for
holding all the contents.

   STAGE 2: It creates all the QWidgets and their respective QHBoxes in which
they all reside.
   
   STAGE 3: A level of polish is applied to every widget. Adding QTooltips,
applying QValidators, fixing sizes, and making connections happens here.
*/
QGctpTab::QGctpTab( QWidget* parent, const char* name)
   : QScrollView( parent, name)
{
   ////////STAGE 1: Appearance/Setup
   //
   setHScrollBarMode( QScrollView::AlwaysOff );
   setVScrollBarMode( QScrollView::AlwaysOn );

   contents = new QVBox( viewport(), "contents" );
   addChild( contents );

   ////////STAGE 2: Generate Widgets
   //
   //titleBox - Contains function buttons and the title of the Tab
   QHBox *titleBox = new QHBox( contents );
   copyButton = new QPushButton( QIconSet( mapimgImage( "copy" ) ),
      "", titleBox, "copyButton" );
   lockButton = new QPushButton( QIconSet( mapimgImage( "unlocked" ) ),
      "", titleBox, "lockButton" );
   QLabel *gctpLabel = new QLabel( "Projection Info", titleBox, "gctpLabel" );

   //projBox - Contains the combo box for selecting which projection to use
   projBox = new QVBox( contents );
   (void) new QLabel( "Projection", projBox);
   projCombo = new QComboBox( projBox, "projCombo" );

   //zoneBox - Contains a spin box for selecting  which UTM zone to use
   zoneBox = new QVBox( contents );
   (void) new QLabel( "UTM Zone Code", zoneBox);
   zoneSpin = new QSpinBox( -60, 60, 1, zoneBox, "zoneSpin" );

   //gctpBoxes - Label and edit fields adaptable to each GCTP parameter
   gctpBoxes = new QGctpBox*[15];
   for( int i = 0; i < 15; ++i )
      gctpBoxes[i] = new QGctpBox( contents );

   ////////STAGE 3: Polish Widgets
   //
   //titleBox
   copyButton->setMaximumWidth( 28 ); copyButton->setMaximumHeight( 28 );
   QToolTip::add( copyButton, "Copy from input info editor." );
   lockButton->setMaximumWidth( 28 ); lockButton->setMaximumHeight( 28 );
   lockButton->setToggleButton( true );
   QToolTip::add( lockButton, "Use to allow editing of .xml file.<br><br>"
      "<b>Note</b>: Locking this info editor automatically saves to the .xml file." );
   QFont largeFont(  gctpLabel->font() );
   largeFont.setPointSize( 12 );
   gctpLabel->setFont( largeFont ); 
   gctpLabel->setAlignment( int( QLabel::AlignCenter ) );
   titleBox->setMaximumHeight( titleBox->height() );

   //projBox
   projCombo->insertItem( "" );
   projCombo->insertStringList( projNames );
   int c = 23;
   projCombo->removeItem( c );
   projCombo->insertItem( "Space Oblique Mercator b", c );
   projCombo->insertItem( "Space Oblique Mercator a", c );
   c = 21;
   projCombo->removeItem( c );
   projCombo->insertItem( "Hotine Oblique Mercator b", c );
   projCombo->insertItem( "Hotine Oblique Mercator a", c );
   c = 9;
   projCombo->removeItem( c );
   projCombo->insertItem( "Equidistant Conic b", c );
   projCombo->insertItem( "Equidistant Conic a", c );
   QToolTip::add( projCombo, "Name of map projection" );

   //zoneBox
   zoneSpin->setValue( 0 );
   zoneBox->hide();
   QToolTip::add( zoneSpin, "Zone code used in UTM Projection" );

   //This connection is for updating the gctpBoxes whenever the projection changes
   connect( projCombo, SIGNAL( activated(int) ), this, SLOT( projChange() ) );
}

/*
   While Qt will destroy all the gctpBoxes, it won't destroy my allocated 
array of pointers to them. DELETED!
*/
QGctpTab::~QGctpTab()
{
   delete [] gctpBoxes;
}

/*
   The projChange() function is connected to the projCombo so that whenever
the user selects a new projection this function is called. Its purpose is to
update the parameter list that the user sees. The zoneBox is shown based on
whether the selected projection is UTM or not. Then, using the gctpNames()
function <gctpnames.h>, each gctpBox is passed a parameter name. The gctpBoxes
adapt to each name by updating the label and which style of edit to provide.
*/
void QGctpTab::projChange()
{
   int projNum = combo2proj( projCombo->currentItem() );
   char variation = projCombo->currentText().right(1)[0].latin1();
 
   QStringList projNames = gctpNames( projNum, variation );
   for( int i = 0; i < 15; ++i )
      gctpBoxes[i]->setGctpName( projNames[i] );
   zoneBox->setShown( projNum == 1 );
}


/*
   The QInfoFrame constructor is very simply due to the other two classes
provided in this file. It is a QTabWidget and its two tabs are one QMapTab and
then one QGctpTab. After those two tabs are added, all that is left for the
constructor to do is connect the QPushButtons in those tabs to slots in this
QInfoFrame.

   The name QInfoFrame implies that this is a class subclassed from a QFrame.
It origionally was but has since been updated to a QTabWidget. Whether or not
it needs to be renamed is currently undecided.
*/
QInfoFrame::QInfoFrame( QWidget* parent, const char* name)
   : QTabWidget( parent, name)
{
   mapTab = new QMapTab( this, "mapTab" );
   gctpTab = new QGctpTab( this, "gctpTab" );

   addTab( mapTab, "Map" );
   addTab( gctpTab, "Projection" );

   connect(mapTab->copyButton, SIGNAL( clicked() ), this, SLOT(copy()));
   connect(gctpTab->copyButton, SIGNAL( clicked() ), this, SLOT(copy()));
   connect(mapTab->lockButton, SIGNAL(toggled(bool)), this, SLOT(lock(bool)) );
   connect(gctpTab->lockButton, SIGNAL(toggled(bool)), this, SLOT(lock(bool)) );
   connect(mapTab->frameButton, SIGNAL( clicked() ), this, SLOT(frame()));
   connect(mapTab->fillButton, SIGNAL( clicked() ), this, SLOT(getFill()) );

   locking = false;
   reset();
}

/*
   Nothing to destruct because QObjects take care of their children.
*/
QInfoFrame::~QInfoFrame()
{}

/*
   The reset() function sets all values in both tabs to their defaults, all
zeros except for the first gctpBox.
*/
void QInfoFrame::reset()
{
   mapTab->fileEdit->setText( "" );
   mapTab->rowSpin->setValue( 0 );
   mapTab->colSpin->setValue( 0 );
   mapTab->unitCombo->setCurrentItem( 3 );
   mapTab->spheroidCombo->setCurrentItem( 20 );
   mapTab->pixelCombo->setCurrentItem( 0 );
   mapTab->pixelEdit->setText( "0.000000" );
   mapTab->pixelEdit->setShown( false );
   mapTab->ulLatEdit->setText( "0.000000" );
   mapTab->ulLonEdit->setText( "0.000000" );
   mapTab->dataCombo->setCurrentItem( 0 );
   mapTab->fillEdit->setText( "0.000000" );
   mapTab->noDataEdit->setText( "0.000000" );
   mapTab->hasFillCheck->setChecked( true );
   mapTab->hasNoDataCheck->setChecked( true );

   gctpTab->projCombo->setCurrentItem( 0 );
   gctpTab->projChange();
   gctpTab->gctpBoxes[0]->setValue( 6370997.000 );
   for( int i = 1; i < 15; ++i )
      gctpTab->gctpBoxes[i]->setValue( 0 );

   return;
}

/*
   The fixWidth(uint) function has a very specific purpose within mapimg. It
is used to manipulate the appearance of the tabs so they maintain shape, style,
and whitespace how I want it. It is neither dynamic nor elegant, but it is
practical and needed for my application in mapimg.
*/
void QInfoFrame::fixWidth( uint w )
{
   setMinimumWidth( w );
   mapTab->contents->setMargin( 5 );
   mapTab->contents->setSpacing( 5 );
   mapTab->contents->setMinimumWidth( w - 24 );
   gctpTab->contents->setMargin( 5 );
   gctpTab->contents->setSpacing( 5 );
   gctpTab->contents->setMinimumWidth( w - 24 );
   gctpTab->contents->setMaximumWidth( w - 24 );
}

/*
   This function is mainly used by the lock(bool) function to restrict or 
allow all access to the parameters found in a QInfoFrame.
*/
void QInfoFrame::setReadOnly( bool ro )
{
   mapTab->rowSpin->setDisabled( ro );
   mapTab->colSpin->setDisabled( ro );
   mapTab->unitCombo->setDisabled( true ); //only meters are supported
   mapTab->spheroidCombo->setDisabled( true );
   mapTab->pixelCombo->setDisabled( ro );
   mapTab->pixelEdit->setDisabled( ro );
   mapTab->ulLatEdit->setDisabled( ro );
   mapTab->ulLonEdit->setDisabled( ro );
   mapTab->dataCombo->setDisabled( ro );
   mapTab->fillEdit->setDisabled( ro );
   mapTab->fillButton->setDisabled( ro );
   mapTab->noDataEdit->setDisabled( ro );

   mapTab->hasFillCheck->setHidden( ro );
   mapTab->hasFillCheck->setDisabled( ro );
   mapTab->hasNoDataCheck->setHidden( ro );
   mapTab->hasNoDataCheck->setDisabled( ro );


   gctpTab->projCombo->setDisabled( ro );
   gctpTab->zoneSpin->setDisabled( ro );
   for( int i = 0; i < 15; ++i )
      gctpTab->gctpBoxes[i]->setDisabled( ro );
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
void QInfoFrame::setAsInput()
{
   /*int ph,ps,pv;
   paletteBackgroundColor().getHsv(&ph,&ps,&pv);
   int ih,is,iv;
   INPUT_COLOR.getHsv(&ih,&is,&iv);*/
   //QColor c(INPUT_COLOR);
   //c.setHsv(ih,is,pv>100?pv:100);

   static_cast<QLabel*>(mapTab->child( "mapLabel" ))->setText( "Input Map Info" );
   mapTab->fileEdit->setDisabled( true );
   mapTab->copyButton->hide();
   mapTab->lockButton->show();
   mapTab->fillButton->hide();

   static_cast<QLabel*>(gctpTab->child( "gctpLabel" ))->setText( "Input Projection Info" );
   gctpTab->copyButton->hide();
   gctpTab->lockButton->show();

   QPalette p( INPUT_COLOR );
   p.setColor( QColorGroup::Text, p.color( QPalette::Active, QColorGroup::Text ) );
   mapTab->viewport()->setPalette( p );
   mapTab->viewport()->setEraseColor( INPUT_COLOR );
   gctpTab->viewport()->setPalette( p );
   gctpTab->viewport()->setEraseColor( INPUT_COLOR );

   lock( true, false );
}

/*
   See comments on setAsInput().
*/
void QInfoFrame::setAsOutput()
{
   /*int ph,ps,pv;
   paletteBackgroundColor().getHsv(&ph,&ps,&pv);
   int oh,os,ov;
   OUTPUT_COLOR.getHsv(&oh,&os,&ov);*/
   //QColor c(OUTPUT_COLOR);
   //c.setHsv(oh,os,pv>100?pv:100);

   static_cast<QLabel*>(mapTab->child( "mapLabel" ))->setText( "Output Map Info" );
   mapTab->fileEdit->setDisabled( true );
   mapTab->fileEdit->setText( "Use Save button to reproject" );
   mapTab->copyButton->show();
   mapTab->lockButton->hide();
   mapTab->fillButton->hide();

   static_cast<QLabel*>(gctpTab->child( "gctpLabel" ))->setText( "Output Projection Info" );
   gctpTab->copyButton->show();
   gctpTab->lockButton->hide();

   QPalette p( OUTPUT_COLOR );
   p.setColor( QColorGroup::Text, p.color( QPalette::Active, QColorGroup::Text ) );
   mapTab->viewport()->setPalette( p );
   mapTab->viewport()->setEraseColor( OUTPUT_COLOR );
   gctpTab->viewport()->setPalette( p );
   gctpTab->viewport()->setEraseColor( OUTPUT_COLOR );

   lock( false, false );

   mapTab->rowSpin->setDisabled( true );
   mapTab->colSpin->setDisabled( true );
   mapTab->ulLatEdit->setDisabled( true );
   mapTab->ulLonEdit->setDisabled( true );
   mapTab->dataCombo->setDisabled( true );
   mapTab->fillEdit->setDisabled( true );
   mapTab->noDataEdit->setDisabled( true );

   mapTab->hasFillCheck->setDisabled( true );
   mapTab->hasFillCheck->setHidden( true );
   mapTab->hasFillCheck->setChecked( true );

   mapTab->hasNoDataCheck->setDisabled( true );
   mapTab->hasNoDataCheck->setHidden( true );
   mapTab->hasNoDataCheck->setChecked( true );

}

/*
   This setPartner() function is used so that when the copy button is clicked
and the copy() function is called it knows where to get the data from. Also,
the two connect()'s are used to make sure that the data type and fill value
are the same between the two QInfoFrames.
*/
void QInfoFrame::setPartner( QInfoFrame *i )
{
   partner = i;
   connect( i->mapTab->dataCombo, SIGNAL(activated(int)), this, SLOT(partnerChanged()) );
   connect( i->mapTab->fillEdit, SIGNAL(textChanged(const QString &)), this, SLOT(partnerChanged()) );
   connect( i->mapTab->noDataEdit, SIGNAL(textChanged(const QString &)), this, SLOT(partnerChanged()) );
}

/*
   The copy(QInfoFrame*) function is used to copy data from one frame into 
another. Its most obvious usage is to pass it a the QInfoFrame that you want
to copy. However, for my own purposes, I have added an extra feature where the
function can be called with no parameters and it will rely on a setPartner().
I added this function for mapimg so I could directly connect the copyButton in
the mapTab to this slot.
*/
void QInfoFrame::copy( QInfoFrame *src )
{
   QInfoFrame *source = src;

   if( src == 0 )
      source = partner;

   if( partner == 0 )
      return;

   mapTab->rowSpin->setValue( source->mapTab->rowSpin->value() );
   mapTab->colSpin->setValue( source->mapTab->colSpin->value() );
   mapTab->pixelCombo->setCurrentItem( 
      source->mapTab->pixelCombo->currentItem() );
   mapTab->pixelEdit->setText( source->mapTab->pixelEdit->text() );
   mapTab->ulLatEdit->setText( source->mapTab->ulLatEdit->text() );
   mapTab->ulLonEdit->setText( source->mapTab->ulLonEdit->text() );
   mapTab->dataCombo->setCurrentItem( 
      source->mapTab->dataCombo->currentItem() );
   mapTab->fillEdit->setText( source->mapTab->fillEdit->text() );
   mapTab->noDataEdit->setText( source->mapTab->noDataEdit->text() );

   gctpTab->projCombo->setCurrentText( 
      source->gctpTab->projCombo->currentText() );
   gctpTab->projChange();
   gctpTab->zoneSpin->setValue( source->gctpTab->zoneSpin->value() );
   for( int i = 0; i < 15; ++i )
      gctpTab->gctpBoxes[i]->setValue(
         source->gctpTab->gctpBoxes[i]->value() );
}

/*
   Whenever the partner changes its data type or fill value this slot catches
the signal and updates those two values.
*/
void QInfoFrame::partnerChanged()
{
   mapTab->dataCombo->setCurrentItem(
      partner->mapTab->dataCombo->currentItem() );
   mapTab->fillEdit->setText(
      partner->mapTab->fillEdit->text() );
   mapTab->noDataEdit->setText(
      partner->mapTab->noDataEdit->text() );
}

/*
   The lock(bool) function is used for locking and unlocking QInfoFrames. The
event that triggers this function is usually a click of the lockButton found
in this QInfoFrame's mapTab and gctpTab. They are toggle buttons that emit 
whether they are locking or unlocking with the current click.
*/
void QInfoFrame::lock( bool on, bool saveFile )
{
   if(locking)
      return;

   locking = true;
   setReadOnly( on );
   if( on )
   {
      mapTab->lockButton->setIconSet( QIconSet( mapimgImage( "locked" ) ) );
      gctpTab->lockButton->setIconSet( QIconSet( mapimgImage( "locked" ) ) );
   }
   else
   {
      mapTab->lockButton->setIconSet( QIconSet( mapimgImage( "unlocked" ) ) );
      gctpTab->lockButton->setIconSet( QIconSet( mapimgImage( "unlocked" ) ) );
   }
   mapTab->lockButton->setOn( on );
   gctpTab->lockButton->setOn( on );

   locking = false;

   if( on && saveFile && !xmlName.isNull() )
      emit( locked() );
}

/*
   The frame() function calls the mapimg::frameIt() function for calculating 
the ulLatitude, ulLongitude, rows, and columns of the raster based on all 
other parameters.
*/
bool QInfoFrame::frame()
{
   RasterInfo inf( info() );

   if( !mapimg::readytoFrameIt( inf, this ) )
      return false;
   mapimg::frameIt( inf );

   mapTab->rowSpin->setValue( inf.rows() );
   mapTab->colSpin->setValue( inf.cols() );
   mapTab->ulLatEdit->setText( QString::number( inf.ul_X(), 'f', 6 ) );
   mapTab->ulLonEdit->setText( QString::number( inf.ul_Y(), 'f', 6 ) );
   return true;
}

void QInfoFrame::getFill()
{
   RasterInfo inf( info() );

   double maxValue = mapimg::calcFillValue(inf);
   QString fillString = "0.000000";

   if( maxValue != 0 )
   {
      fillString = QString::number(maxValue + 2, 'f', 6 );
   }

   if( mapTab->fillEdit->validator() != 0 )
   {
       mapTab->fillEdit->validator()->fixup( fillString );
   }

   if( QMessageBox::question( 0,
                          "Replace Fill Value?",
                          QString("Are you sure you wish to replace the current fill "
                                  "value (%1) with the new estimated value of %2?")
                                  .arg( mapTab->fillEdit->text() ).arg( fillString ),
                          QMessageBox::Yes,
                          QMessageBox::No | QMessageBox::Default | QMessageBox::Escape ) == QMessageBox::Yes )
   {
       mapTab->fillEdit->setText( fillString );
   }
}

/*!!!!!!!!!!!!!!!!!
   setInfo(RasterInfo) will load all the parameters from a the RasterInfo.
It sets the filename by parsing out the path and then applies the values to
the different edit widgets.
*/
void QInfoFrame::setInfo( RasterInfo &input )
{
   ////////File Name
   if( input.xmlFileName() != QString(".xml") )
   {
      xmlName = input.xmlFileName();

      QString cap( xmlName );
      cap.replace('\\', '/');
      int index = cap.findRev("/");
      index += 1;
      cap = cap.right(cap.length() - index);

      mapTab->fileEdit->setText( cap );
   }

   ////////Inputs
   mapTab->rowSpin->setValue( input.rows() );
   mapTab->colSpin->setValue( input.cols() );

   mapTab->unitCombo->setCurrentText( 
      unitNames[input.unitNumber()] );
   mapTab->spheroidCombo->setCurrentText( 
      spheroidNames[input.datumNumber()] );

   mapTab->pixelEdit->setText( QString::number(input.pixelSize(), 'f', 6) );
   int index = pixelValues.findIndex( mapTab->pixelEdit->text() );
   if( index == -1 ) index = 5;
   mapTab->pixelCombo->setCurrentItem( index + 1 );
   mapTab->pixelEdit->setShown( index == 5 );

   mapTab->ulLatEdit->setText( QString::number(input.ul_X(), 'f', 6) );
   mapTab->ulLonEdit->setText( QString::number(input.ul_Y(), 'f', 6) );

   QString dtype(input.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(input.bitCount());
   dtype += " Bit ";
   dtype += input.dataType();
   mapTab->dataCombo->setCurrentText( dtype );

   QString fillString = "Undefined";
   if( input.hasFillValue() )
       fillString = QString::number( input.fillValue(), 'f', 6 );

   if( mapTab->fillEdit->validator() != 0 )
   {
       ((MapimgValidator*)mapTab->fillEdit->validator())->setDataType( dtype );
       ((MapimgValidator*)mapTab->fillEdit->validator())->setAllowUndefined( fillString.upper() == "UNDEFINED" );
       mapTab->hasFillCheck->setChecked( !(fillString.upper() == "UNDEFINED") );

       mapTab->fillEdit->validator()->fixup( fillString );
   }

   mapTab->fillEdit->setText( fillString );
//   mapTab->fillButton->setShown( input.fillValue() == -1.0 );


   QString noDataString = "Undefined";
   if( input.hasNoDataValue() )
       noDataString = QString::number( input.noDataValue(), 'f', 6 );

   if( mapTab->noDataEdit->validator() != 0 )
   {
       ((MapimgValidator*)mapTab->noDataEdit->validator())->setDataType( dtype );
       ((MapimgValidator*)mapTab->noDataEdit->validator())->setAllowUndefined( noDataString.upper() == "UNDEFINED" );
       mapTab->hasNoDataCheck->setChecked( !(noDataString.upper() == "UNDEFINED") );

       mapTab->noDataEdit->validator()->fixup( noDataString );
   }

   mapTab->noDataEdit->setText( noDataString );

   ////////GCTP Params
   //   Complications in here arise from the multiple variations for some
   //projections. Since they have the same projection code, the different
   //variations are only discernable by their GCTP parameters.
   int projNum = input.projectionNumber();
   char variation = 'a';
   if( projNum == 8 && input.gctpParam(8) == 1 )
      variation = 'b';
   if( ( projNum == 20 || projNum == 22 ) && input.gctpParam(12) == 1 )
      variation = 'b';
   if( projNum == 8 || projNum == 20 || projNum == 22 )
      gctpTab->projCombo->setCurrentText( 
         projNames[input.projectionNumber()] + " " + variation );
   else
      gctpTab->projCombo->setCurrentText( 
         projNames[input.projectionNumber()] );
   gctpTab->projChange();

   gctpTab->zoneSpin->setValue( input.zoneNumber() );

   for( int i = 0; i < 15; ++i )
      gctpTab->gctpBoxes[i]->setValue( input.gctpParam(i) );
}

/*
   info() saves all of the entered values to a returned RasterInfo. 
*/
RasterInfo QInfoFrame::info()
{
//   cleanUp( mapTab->pixelEdit );
//   cleanUp( mapTab->ulLatEdit );
//   cleanUp( mapTab->ulLonEdit );
//   cleanUp( mapTab->fillEdit );
//   cleanUp( mapTab->noDataEdit );

   RasterInfo ret;

   ret.setFileName(
      xmlName );

   ret.setArea(
      mapTab->ulLatEdit->text().toDouble(),
      mapTab->ulLonEdit->text().toDouble(),
      mapTab->rowSpin->value(), mapTab->colSpin->value() );

   ret.setPixelDescription(
      mapTab->dataCombo->currentText(),
      mapTab->pixelEdit->text().toDouble(),
      mapTab->fillEdit->text().toDouble(),
      mapTab->noDataEdit->text().toDouble() );

   if( mapTab->fillEdit->text().upper() == "UNDEFINED" )
       ret.setHasFillValue( false );
   else
       ret.setHasFillValue( true );

   if( mapTab->noDataEdit->text().upper() == "UNDEFINED" )
       ret.setHasNoDataValue( false );
   else
       ret.setHasNoDataValue( true );

   ret.setProjection(
      combo2proj( gctpTab->projCombo->currentItem() ),
      gctpTab->zoneSpin->value() );

   for( int i = 0; i < 15; ++i )
      ret.setGctpParam( i, gctpTab->gctpBoxes[i]->value() );

   return ret;
}





/*
   cleanUp(QLineEdit*) is designed to format user-entered values to be more
readable.

Examples:
   " .01"  --> "0.010000"
   "50 \t" --> "50.000000"
void QInfoFrame::cleanUp( QLineEdit *lEdit )
{
   QString cleanUp = lEdit->text();
   cleanUp.stripWhiteSpace();

   uint newLen = cleanUp.find( '.' ) + INFO_PRECISION + 1;
   if( newLen == INFO_PRECISION ) //No '.' found
      cleanUp.append( ".000000" );
   else if( newLen < cleanUp.length() )
      cleanUp.truncate( newLen );
   else
      cleanUp.append( QString("000000").left( newLen - cleanUp.length() ) );

   if( cleanUp.find( '.' ) == 0 )
      cleanUp.prepend( "0" );

   lEdit->setText( cleanUp );
   return;
}
*/

