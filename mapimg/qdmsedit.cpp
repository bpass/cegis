// $Id: qdmsedit.cpp,v 1.3 2005/05/06 14:06:44 jtrent Exp $


#include "qdmsedit.h"
#include <qvalidator.h>
#include <qregexp.h>
#include <qtooltip.h>

QDmsEdit::QDmsEdit( QWidget* parent, const char* name, Directionality direction )
   : QHBox( parent, name)
{
   setFrameStyle( QFrame::LineEditPanel | QFrame::Sunken );
   setLineWidth( 2 );
   setMargin( 0 );
   layout()->setMargin( 2 );
   setMaximumHeight( 20 );

   dmsLabel = new QLabel( this, "dmsLabel" );
   dEdit = new QLineEdit( this, "dEdit" );
   dEdit->setMaximumSize( QSize( 40, 32767 ) );
   dEdit->setAlignment( int( QLineEdit::AlignRight ) );
   dEdit->setFrameStyle( QFrame::NoFrame );
   dLabel = new QLabel( this, "dLabel" );
   mEdit = new QLineEdit( this, "mEdit" );
   mEdit->setMaximumSize( QSize( 30, 32767 ) );
   mEdit->setAlignment( int( QLineEdit::AlignRight ) );
   mEdit->setFrameStyle( QFrame::NoFrame );
   mLabel = new QLabel( this, "mLabel" );
   sEdit = new QLineEdit( this, "sEdit" );
   sEdit->setMaximumSize( QSize( 80, 32767 ) );
   sEdit->setAlignment( int( QLineEdit::AlignRight ) );
   sEdit->setFrameStyle( QFrame::NoFrame );
   sLabel = new QLabel( this, "sLabel" );

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

   directionLabel = new QLabel( this, "directionLabel" );

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

   dEdit->setValidator( new QIntValidator(min, max, dEdit) );
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
   r = dEdit->text().toInt() * 1000000
      +mEdit->text().toInt() * 1000
      +sEdit->text().toDouble();
   return r;
}


void QDmsEdit::setValue( const double val )
{
   int deg = static_cast<int>( val / 1000000 );
   uint min = static_cast<uint>( (val - deg * 1000000) / 1000 );
   double sec = val - deg * 1000000 - min * 1000 ;
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




