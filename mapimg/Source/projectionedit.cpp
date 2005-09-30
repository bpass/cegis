#include <QCheckBox>
#include <QColor>
#include <QColorGroup>
#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPalette>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QSizePolicy>
#include <QSpinBox>
#include <QString>
#include <Qt>
#include <QTabWidget>
#include <QToolTip>
#include <QVBoxLayout>
#include <QWidget>

#include "gctpnames.h"
#include "mapimgedit.h"	//QInfoFrame class
#include "mapimgpalette.h"
#include "mapimgvalidator.h"
#include "projectionedit.h"
#include "qgctpbox.h"

/*
The ProjectionEdit has one constructor. It executes in three stages.

STAGE 1: It sets its appearance by turning off the Horizontal SrcollBar,
forcing on the Vertical ScrollBar, and generating the 'contents' widget for
holding all the contents.

STAGE 2: It creates all the QWidgets and their respective QHBoxes in which
they all reside.

STAGE 3: A level of polish is applied to every widget. Adding QTooltips,
applying QValidators, fixing sizes, and making connections happens here.
*/
ProjectionEdit::ProjectionEdit( QWidget* parent )
: QScrollArea( parent )
{
	appearanceSetup();
	generateWidgets();
	polishWidgets();

	connect( copyButton, SIGNAL( clicked() ), this, emit SIGNAL( copyButtonClicked() ) );
	connect( lockButton, SIGNAL(toggled(bool)), this, emit SIGNAL( lockButtonToggled() ) );
}

/*
While Qt will destroy all the gctpBoxes, it won't destroy my allocated 
array of pointers to them. DELETED!
*/
ProjectionEdit::~ProjectionEdit()
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
void ProjectionEdit::projChange()
{
	int projNum = combo2proj( projCombo->currentIndex() );
	char variation = projCombo->currentText().right(1)[0].toLatin1();

	switch(projNum)
	{
	case 0: case 1: case 2: case 4: case 6: case 10: case 13: case 23: case 26:
		badProjLabel->show();
		break;
	default:
		badProjLabel->hide();
		break;
	}

	QStringList projNames = gctpNames( projNum, variation );
	for( int i = 0; i < 15; ++i )
		gctpBoxes[i]->setGctpName( projNames[i] );

	zoneBox->setShown( projNum == 1 );

		//To keep the vertical scrollbar insync with the height of the scrollview contents
	contents->resize( size().width(), size().height() ); 
}

/*This slot is called from the imgedit (infoFrame) when it needs to reset values)*/
void ProjectionEdit::reset()
{
	projCombo->setCurrentIndex( 0 );
	projChange();
	gctpBoxes[0]->setValue( 6370997.000 );
	for( int i = 1; i < 15; ++i )
		gctpBoxes[i]->setValue( 0 );
}

/*
This function is mainly used by the lock(bool) function to restrict or
allow all access to the parameters found in a ProjectionEdit
*/
void ProjectionEdit::setRO( bool ro, int inOut )
{
	if( inOut == 0 )
	{
		projCombo->setDisabled( ro );
		zoneSpin->setDisabled( ro );
		for( int i = 0; i < 15; ++i )
			gctpBoxes[i]->setDisabled( ro );
	}
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
void ProjectionEdit::setAsInput()
{
	findChild<QLabel*>( "gctpLabel" )->setText( "Input Projection Info" );
	copyButton->hide();
	lockButton->show();

	QPalette p( QColor( 151, 160, 148 ) ); //INPUT_COLOR mapimgpalette.h
	p.setColor( QPalette::Text, p.color( QPalette::Active, QPalette::Text ) );
	viewport()->setPalette( p );
}

/*
See comments on setAsInput().
*/
void ProjectionEdit::setAsOutput()
{
	findChild<QLabel*>( "gctpLabel" )->setText( "Output Projection Info" );
	copyButton->show();
	lockButton->hide();

	QPalette p( QColor( 163, 146, 146 ) ); //OUTPUT_COLOR mapimgpalette.h
	p.setColor( QPalette::Text, p.color( QPalette::Active, QPalette::Text ) );
	viewport()->setPalette( p );
}

/*
The frame() function calls the mapimg::frameIt() function for calculating
the ulLatitude, ulLongitude, rows, and columns of the raster based on all
other parameters.
*/
void ProjectionEdit::setFrameInfo( QString projName )
{
	projCombo->setCurrentIndex( projCombo->findText( projName ) );
}

void ProjectionEdit::appearanceSetup()
{
	////////STAGE 1: Appearance/Setup
	//
		//Allows for the expanding of the contents widget when widgets added
	setWidgetResizable( true );	
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	contents = new QWidget;
	setWidget( contents );
}

void ProjectionEdit::generateWidgets()
{ 
	////////STAGE 2: Generate Widgets
   //
	mainLayout = new QVBoxLayout( contents );
	mainLayout->setAlignment( Qt::AlignTop );
	mainLayout->setMargin( 5 );		
	
   //titleBox - Contains function buttons and the title of the Tab
   titleBox = new QHBoxLayout( 0 );
   titleBox->setParent( mainLayout );
   mainLayout->addLayout( titleBox );

	copyButton = new QPushButton( QIcon( ":/Resources/copy.png" ),
		"", this );
	lockButton = new QPushButton( QIcon( ":/Resources/unlocked.png" ),
		"", this );
   gctpLabel = new QLabel( "Projection Info", this );
   gctpLabel->setObjectName( "gctpLabel" );

   titleBox->addWidget( copyButton );
   titleBox->addWidget( lockButton );
   titleBox->addWidget( gctpLabel );

   //projBox - Contains the combo box for selecting which projection to use
   //    Also contains the bad projection label
   projBox = new QVBoxLayout( 0 );
   projBox->setParent( mainLayout );
   mainLayout->addLayout( projBox );

   projectionLabel = new QLabel( "Projection", this);
   projCombo = new QComboBox( this );
   badProjBlank = new QWidget( this );
   badProjLabel = new QLabel( badProjBlank );

   projBox->addWidget( projectionLabel );
   projBox->addWidget( projCombo );
   projBox->addWidget( badProjBlank );

   //zoneBox - Contains a spin box for selecting  which UTM zone to use
   zoneBox = new QWidget( this );
   mainLayout->addWidget( zoneBox );

   zoneBoxLayout = new QVBoxLayout( zoneBox ); //puts layout in widget
   zoneLabel = new QLabel( "UTM Zone Code", this); 
   zoneSpin = new QSpinBox( this );
   zoneSpin->setMinimum( -60 );
   zoneSpin->setMaximum( 60 );
   zoneSpin->setSingleStep( 1 );

   zoneBoxLayout->addWidget( zoneLabel );
   zoneBoxLayout->addWidget( zoneSpin );

   //gctpBoxes - Label and edit fields adaptable to each GCTP parameter
   gctpBoxes = new QGctpBox*[15];
   for( int i = 0; i < 15; ++i )
   {
	   gctpBoxes[i] = new QGctpBox( this );
	   
	   mainLayout->addWidget( gctpBoxes[i] );
   }
}

void ProjectionEdit::polishWidgets()
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
   QFont largeFont(  gctpLabel->font() );
   largeFont.setPointSize( 12 );
   gctpLabel->setFont( largeFont ); 
  gctpLabel->setAlignment( Qt::AlignCenter );

   //projBox
   projCombo->addItem( "" );
   projCombo->addItems( projNames );
   int c = 23;
   projCombo->removeItem( c );
   projCombo->addItem( "Space Oblique Mercator b", c );
   projCombo->addItem( "Space Oblique Mercator a", c );
   c = 21;
   projCombo->removeItem( c );
   projCombo->addItem( "Hotine Oblique Mercator b", c );
   projCombo->addItem( "Hotine Oblique Mercator a", c );
   c = 9;
   projCombo->removeItem( c );
   projCombo->addItem( "Equidistant Conic b", c );
   projCombo->addItem( "Equidistant Conic a", c );
   projCombo->setToolTip( "Name of map projection" );

   badProjBlank->setFixedHeight(10);
   badProjLabel->setFixedHeight(10);
   badProjLabel->hide();
   badProjLabel->setText( "Bad Projection *" );
   badProjLabel->setToolTip(
      "<b>Bad Projection</b>: This projection either generates "
      "useless data or crashes. It is recommended that you "
      "choose a different one. This issue may be addressed in "
      "Future versions of mapimg." );

   //zoneBox
   zoneSpin->setValue( 0 );
   zoneBox->hide();
   zoneSpin->setToolTip( "Zone code used in UTM Projection" );

   //This connection is for updating the gctpBoxes whenever the projection changes
   connect( projCombo, SIGNAL( activated(int) ), this, SLOT( projChange() ) );
}