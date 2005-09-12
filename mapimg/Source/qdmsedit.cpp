// $Id: qdmsedit.cpp,v 1.9 2005/09/12 21:16:01 rbuehler Exp $

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegexp>
#include <QToolTip>
#include <QWidget>

#include "mapimgvalidator.h"
#include "qdmsedit.h"


QDmsEdit::QDmsEdit( QWidget* parent, const char* name, Directionality direction )
: QFrame( parent, name)
{
   setFrameStyle( QFrame::WinPanel | QFrame::Sunken );

   dmsLabel = new QLabel( "(d° m\' s\")" );

   dEdit = new QLineEdit( "0" );
   dEdit->setMaximumWidth( 40 );
   dEdit->setAlignment( Qt::AlignRight );
   dEdit->setFrame( false );

   dLabel = new QLabel( "°" );
   QFont labelFont = dLabel->font();
   labelFont.setPointSize( 15 );
   //dLabel->setFont( labelFont );

   mEdit = new QLineEdit( "0" );
   mEdit->setMaximumWidth( 30 );
   mEdit->setAlignment( Qt::AlignRight );
   mEdit->setFrame( false );

   mLabel = new QLabel( "\'" );
   //mLabel->setFont( labelFont );

   sEdit = new QLineEdit( "0" );
   sEdit->setMaximumWidth( 80 );
   sEdit->setAlignment( Qt::AlignRight );
   sEdit->setFrame( false );

   sLabel = new QLabel( "\"" );
   //sLabel->setFont( labelFont );

   directionLabel = new QLabel;

   /*Adding the widgets to mainLayout to force them to line up properly*/
   QHBoxLayout *mainLayout = new QHBoxLayout;
   mainLayout->setMargin(0);
   mainLayout->setSpacing(0);

   mainLayout->addWidget( dmsLabel );
   mainLayout->addWidget( dEdit );
   mainLayout->addWidget( dLabel );
   mainLayout->addWidget( mEdit );
   mainLayout->addWidget( mLabel );
   mainLayout->addWidget( sEdit );
   mainLayout->addWidget( sLabel );
   mainLayout->addWidget( directionLabel );

   setLayout( mainLayout );

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
