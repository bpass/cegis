// $Id: qdmsedit.cpp,v 1.7 2005/08/29 16:48:14 lwoodard Exp $

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegexp>
#include <QToolTip>
#include <QWidget>

#include "mapimgvalidator.h"
#include "qdmsedit.h"


QDmsEdit::QDmsEdit( QWidget* parent, const char* name, Directionality direction )
: QWidget( parent, name)
{	
	QHBoxLayout *mainLayout = new QHBoxLayout( this );
	setLayout( mainLayout );
//	setFrameStyle( QFrame::LineEditPanel | QFrame::Sunken );
//	setLineWidth( 2 );
	mainLayout->setMargin( 0 );
	layout()->setMargin( 0 );		//was value 2
	setMaximumHeight( 20 );
	mainLayout->setSizeConstraint( QLayout::Minimum );

	dmsLabel = new QLabel( this, "dmsLabel" );
	dEdit = new QLineEdit( this, "dEdit" );
	dEdit->setMaximumSize( QSize( 40, 32767 ) );
	dEdit->setAlignment( Qt::AlignRight );
//	dEdit->setFrame( false );		Until can figure how to make entire line sunken
	dLabel = new QLabel( this, "dLabel" );
	mEdit = new QLineEdit( this, "mEdit" );
	mEdit->setMaximumSize( QSize( 30, 32767 ) );
	mEdit->setAlignment( Qt::AlignRight );
//	mEdit->setFrame( false );		Until can figure how to make entire line sunken
	mLabel = new QLabel( this, "mLabel" );
	sEdit = new QLineEdit( this, "sEdit" );
	sEdit->setMaximumSize( QSize( 80, 32767 ) );
	sEdit->setAlignment( Qt::AlignRight );
//	sEdit->setFrame( false );		Until can figure how to make entire line sunken
	sLabel = new QLabel( this, "sLabel" );
	directionLabel = new QLabel( this, "directionLabel" );

	/*Setting the widgets to layouts and adding to mainLayout to force 
	them to line up properly*/
	mainLayout->addWidget( dmsLabel );
	QHBoxLayout *degreeLayout = new QHBoxLayout( mainLayout);
	degreeLayout->addWidget( dEdit );
	degreeLayout->addWidget( dLabel );
	QHBoxLayout *minuteLayout = new QHBoxLayout( mainLayout );
	minuteLayout->addWidget( mEdit );
	minuteLayout->addWidget( mLabel );
	QHBoxLayout *secondLayout = new QHBoxLayout( mainLayout );
	secondLayout->addWidget( sEdit );
	secondLayout->addWidget( sLabel );
	mainLayout->addWidget( directionLabel );

	QFont labelFont = dmsLabel->font();
	labelFont.setPointSize( 15 );
	dLabel->setFont( labelFont );
	mLabel->setFont( labelFont );
	sLabel->setFont( labelFont );

	dmsLabel->setText( "(d° m\' s\")" );
	dEdit->setText( "0" );
	dLabel->setText( "°" );
	mEdit->setText( "0" );
	mLabel->setText( "\'" );
	sEdit->setText( "0" );
	sLabel->setText( "\"" );

	if( direction != Unspecified )
	{
		if( direction == North )
			directionLabel->setText( "N" );
		else if( direction == East )
			directionLabel->setText( "E" );
		else if( direction == South )
			directionLabel->setText( "S" );
		else if( direction == West )
			directionLabel->setText( "W" );
		else
			directionLabel->setText( "" );
	}

	dEdit->setValidator( new MapimgValidator(min, max, 0, dEdit) );
	mEdit->setValidator( new QIntValidator(0, 60, mEdit) );
	QRegExp exp( QString::fromLatin1("^(60|[1-5]?\\d(\\.\\d*)?)$") );
	sEdit->setValidator( new QRegExpValidator(exp, sEdit) );
	QToolTip::add( dEdit, QString("<b>degrees</b>: integer from %1 to %2" ).
		arg(min).arg(max));
	QToolTip::add( mEdit, QString("<b>minutes</b>: integer from 0 to 60" ));
	QToolTip::add( sEdit, QString("<b>seconds</b>: real from 0 to 60" ));

	min = 0;
	max = 360;
}


int QDmsEdit::minVal() const
{
	return min;
}


void QDmsEdit::setMinVal( const int &newMin )
{
	min = newMin;
	if(dEdit->text().toInt() < min)
	{
		dEdit->setText(QString::number(min));
		mEdit->setText("0");
		sEdit->setText("0");
	}
	dEdit->setValidator( new QIntValidator(min, max, dEdit) );
	QToolTip::remove( dEdit );
	QToolTip::add( dEdit, QString("<b>degrees</b>: integer from %1 to %2" ).
		arg(min).arg(max));
}


int QDmsEdit::maxVal() const
{
	return max;
}


void QDmsEdit::setMaxVal( const int &newMax )
{
	max = newMax;
	if(dEdit->text().toInt() >= max)
	{
		dEdit->setText(QString("%1").arg(max));
		mEdit->setText("0");
		sEdit->setText("0");
	}
	dEdit->setValidator( new QIntValidator(min, max, dEdit) );
	QToolTip::remove( dEdit );
	QToolTip::add( dEdit, QString("<b>degrees</b>: integer from %1 to %2" ).
		arg(min).arg(max));
}


double QDmsEdit::value()
{
	double r;
	r = dEdit->text().toInt() * 1000000;
	if( r >= 0 )
		r+=mEdit->text().toInt() * 1000
		+sEdit->text().toDouble();
	else
		r-=mEdit->text().toInt() * 1000
		+sEdit->text().toDouble();

	return r;
}


void QDmsEdit::setValue( const double val )
{
	double tmp = val;
	int deg = 1;
	uint min;
	double sec;

	if( val < 0 )
		tmp = -val;

	deg = static_cast<int>( tmp / 1000000 );
	min = static_cast<uint>( (tmp - deg * 1000000) / 1000 );
	sec = tmp - deg * 1000000 - min * 1000 ;

	if( val < 0 )
		deg = -deg;

	dEdit->setText( QString::number( deg ) );
	mEdit->setText( QString::number( min ) );
	sEdit->setText( QString::number( sec, 'f' ) );
}

void QDmsEdit::setValue( const int deg, const int min, const double sec )
{
	dEdit->setText( QString::number( deg ) );
	mEdit->setText( QString::number( min ) );
	sEdit->setText( QString::number( sec, 'f', 6 ) );
}

QDmsEdit::Directionality QDmsEdit::direction() const
{
	Directionality returnValue = Unspecified;

	if( directionLabel->text().upper() == "N" )
		returnValue = North;
	else if( directionLabel->text().upper() == "E" )
		returnValue = East;
	else if( directionLabel->text().upper() == "S" )
		returnValue = South;
	else if( directionLabel->text().upper() == "W" )
		returnValue = West;

	return returnValue;
}
void QDmsEdit::setDirection( const Directionality direction )
{
	if( direction == North )
		directionLabel->setText( "N" );
	else if( direction == East )
		directionLabel->setText( "E" );
	else if( direction == South )
		directionLabel->setText( "S" );
	else if( direction == West )
		directionLabel->setText( "W" );
	else
		directionLabel->setText( "" );

	return;
}
