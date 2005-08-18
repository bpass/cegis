// $Id: mapimgedit.cpp,v 1.7 2005/08/18 15:23:01 lwoodard Exp $

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSpinBox>
#include <QScrollArea>
#include <QSizePolicy>
#include <Qt>
#include <QTextStream>
#include <QToolTip>
#include <QValidator>
#include <QVBoxLayout>

#include "qgctpbox.h"
#include "gctpnames.h"
#include "mapedit.h"
#include "mapimg.h"
#include "mapimgpalette.h"
#include "mapimgedit.h"	
#include "mapimgvalidator.h"
#include "projectionedit.h"
#include "rasterinfo.h"

const uint INFO_PRECISION = 6;

/*
The QInfoFrame constructor is very simply due to the other two classes
provided in this file. It is a QTabWidget and its two tabs are one MapEdit and
then one ProjectionEdit. After those two tabs are added, all that is left for the
constructor to do is connect the QPushButtons in those tabs to slots in this
QInfoFrame.

The name QInfoFrame implies that this is a class subclassed from a QFrame.
It origionally was but has since been updated to a QTabWidget. Whether or not
it needs to be renamed is currently undecided.
*/
QInfoFrame::QInfoFrame( QWidget* parent, const char* name)
: QTabWidget( parent, name)
{
	mapTab = new MapEdit( this );
	gctpTab = new ProjectionEdit( this );

	addTab( mapTab, "Map" );
	addTab( gctpTab, "Projection" );

	connect(mapTab, SIGNAL( copyButtonClicked() ), this, SLOT(copy()));
	connect(gctpTab, SIGNAL( copyButtonClicked() ), this, SLOT(copy()));
	connect(mapTab->lockButton, SIGNAL(toggled(bool)), this, SLOT(lock(bool)) );
	connect(gctpTab->lockButton, SIGNAL(toggled(bool)), this, SLOT(lock(bool)) );
	connect(mapTab, SIGNAL( frameButtonClicked() ), this, SLOT(frame()));
	connect(mapTab, SIGNAL( fillButtonClicked() ), this, SLOT(getFill()) );

	locking = false;
	reset();
}

/*
Nothing to destruct because QObjects take care of their children.
*/
QInfoFrame::~QInfoFrame()
{}

/*This function over rides the default size hint function.  It's main purpose
is to make sure the width will be 300 to keep uniformity between the tabs*/
QSize QInfoFrame::sizeHint() const
{
	return QSize( 300, QTabWidget::sizeHint().height() );
}


/*
The reset() function sets all values in both tabs to their defaults, all
zeros except for the first gctpBox.
*/
void QInfoFrame::reset()
{
	mapTab->reset();
	gctpTab->reset();

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
	/*
	emit setMapTabWidth( w );
	emit setGctpTabWidth( w );
	*/
}

/*
This function is mainly used by the lock(bool) function to restrict or
allow all access to the parameters found in a QInfoFrame.
*/
void QInfoFrame::setReadOnly( bool ro )
{
	mapTab->setRO( ro );
	gctpTab->setRO( ro );
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
	mapTab->setAsInput();
	gctpTab->setAsInput();

	lock( true, false );
}

/*
See comments on setAsInput().
*/
void QInfoFrame::setAsOutput()
{
	mapTab->setAsOutput();
	gctpTab->setAsOutput();

	lock( false, false );
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
	connect( i->mapTab, SIGNAL( dataComboActivated( int ) ), this, 
		SLOT( partnerChanged() ) );
	connect( i->mapTab, SIGNAL( fillEditChanged( const QString & ) ), this, 
		SLOT( partnerChanged() ) );
	connect( i->mapTab, SIGNAL( noDataEditChanged( const QString & ) ), this, 
		SLOT( partnerChanged() ) );
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
	mapTab->ulLonEdit->setText( source->mapTab->ulLonEdit->text() );
	mapTab->ulLatEdit->setText( source->mapTab->ulLatEdit->text() );
	mapTab->dataCombo->setCurrentItem( 
		source->mapTab->dataCombo->currentItem() );

	if( source->mapTab->hasFillCheck->isChecked() )
		mapTab->fillEdit->setText( source->mapTab->fillEdit->text() );

	mapTab->hasNoDataCheck->setChecked( source->mapTab->hasNoDataCheck->isChecked() );
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

	if( mapTab->fillEdit->validator() != 0 )
	{
		QString fillString = mapTab->fillEdit->text();
		((MapimgValidator*)mapTab->fillEdit->validator())->setDataType( mapTab->dataCombo->currentText(), true );
		((MapimgValidator*)mapTab->fillEdit->validator())->fixup( fillString );
		mapTab->fillEdit->setText( fillString );
	}

	if( mapTab->noDataEdit->validator() != 0 )
	{
		QString noDataString = mapTab->noDataEdit->text();
		((MapimgValidator*)mapTab->noDataEdit->validator())->setDataType( mapTab->dataCombo->currentText(), true );
		((MapimgValidator*)mapTab->noDataEdit->validator())->fixup( noDataString );
		mapTab->noDataEdit->setText( noDataString );
	}


	QString fillString = partner->mapTab->fillEdit->text();

	if( fillString.upper() == "UNDEFINED" && static_cast<QLabel*>(mapTab->child( "mapLabel" ))->text().contains( "Output", false ) )
	{
		mapTab->fillEdit->setEnabled( true );
		mapTab->fillButton->setShown( true );
	}
	else
	{
		mapTab->fillEdit->setEnabled( false );
		mapTab->fillButton->setShown( false );
		mapTab->fillEdit->setText( fillString );
	}

	if( partner->mapTab->hasNoDataCheck->isChecked() )
	{  // Hide checkbox and disable edit because they are already defined
		mapTab->hasNoDataCheck->setShown( false );
		mapTab->hasNoDataCheck->setChecked( true );
		mapTab->noDataEdit->setEnabled( false );
		mapTab->noDataEdit->setText( partner->mapTab->noDataEdit->text() );
	}
	else
	{
		mapTab->hasNoDataCheck->setShown( true );
		mapTab->noDataCheckToggled( mapTab->hasNoDataCheck->isChecked() );
	}
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
	   mapTab->lockButton->setIcon( QIcon( "./Resources/locked.png" ) );
	   gctpTab->lockButton->setIcon( QIcon( "./Resources/locked.png" ) );
   }
   else
   {
	   mapTab->lockButton->setIcon( QIcon( "./Resources/unlocked.png" ) );
	   gctpTab->lockButton->setIcon( QIcon( "./Resources/unlocked.png" ) );
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

	if( inf.projectionNumber() == 0 )
	{
		mapimg::geo2eqr( inf );

		gctpTab->setFrameInfo( projNames[ inf.projectionNumber() ] );
	}
	else
	{
		if( !mapimg::readytoFrameIt( inf, this ) )
			return false;
		mapimg::frameIt( inf );
	}

	mapTab->setFrameInfo( inf.rows(), inf.cols(), QString::number( inf.ul_X(), 'f', 6 ), 
		QString::number( inf.ul_Y(), 'f', 6 ) );

	return true;
}

void QInfoFrame::getFill()
{
	RasterInfo inf;
	if( static_cast<QLabel*>(mapTab->child( "mapLabel" ))->text().contains( "Output", false ) )
		inf.copy( partner->info() );
	else
		inf.copy( info() );

	double maxValue = mapimg::calcFillValue(inf);

	if( maxValue == inf.fillValue() )
		return;

	QString fillString = "0.000000";

	if( maxValue != 0 )
	{
		fillString = QString::number(maxValue + 2, 'f', 6 );
	}

	mapTab->getFill( fillString );
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

	mapTab->ulLonEdit->setText( QString::number(input.ul_X(), 'f', 6) );
	mapTab->ulLatEdit->setText( QString::number(input.ul_Y(), 'f', 6) );

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
		((MapimgValidator*)mapTab->fillEdit->validator())->setAllowUndefined( !input.hasFillValue() );

		mapTab->hasFillCheck->setChecked( input.hasFillValue() );

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
		mapTab->noDataEdit->setDisabled( true );
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
	RasterInfo ret;

	ret.setFileName(
		xmlName );

	ret.setArea(
		mapTab->ulLonEdit->text().toDouble(),
		mapTab->ulLatEdit->text().toDouble(),
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