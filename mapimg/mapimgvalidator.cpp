// $Id: mapimgvalidator.cpp,v 1.7 2005/03/04 14:58:35 jtrent Exp $

#include <qvalidator.h>
#include <qstring.h>
#include <qmessagebox.h>

#include <limits.h>
#include <math.h>

#include "mapimgvalidator.h"
#include "mapimgdatatypes.h"

MapimgValidator::MapimgValidator( QObject* parent, const char* name )
: QValidator( parent, name )
{
   b = -HUGE_VAL;
   t = HUGE_VAL;
   d = 1000;
}

MapimgValidator::MapimgValidator( QString mapimgDataType, QObject* parent, const char* name )
: QValidator( parent, name )
{
   b = 0.0;
   t = 0.0;
   d = 0;

   setDataType( mapimgDataType );
}

void MapimgValidator::setDataType( QString mapimgDataType )
{
   if( mapimgDataType.contains( "Unsigned" ) )
   {
      b = 0.0;
      d = 0;

      if( mapimgDataType.contains( "8" ) )
         t = Q_UINT8_MAX;
      else if( mapimgDataType.contains( "16" ) )
         t = Q_UINT16_MAX;
      else if( mapimgDataType.contains( "32" ) )
         t = Q_UINT32_MAX;
      else
      {
         QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
         t = Q_UINT32_MAX;
      }
   }
   else if( mapimgDataType.contains( "Signed" ) )
   {
      d = 0;

      if( mapimgDataType.contains( "8" ) )
      {
         b = Q_INT8_MIN;
         t = Q_INT8_MAX;
      }
      else if( mapimgDataType.contains( "16" ) )
      {
         b = Q_INT16_MIN;
         t = Q_INT16_MAX;
      }
      else if( mapimgDataType.contains( "32" ) )
      {
         if( mapimgDataType.contains( "Integer" ) )
         {
            b = Q_INT32_MIN;
            t = Q_INT32_MAX;
         }
         else if( mapimgDataType.contains( "Float" ) )
         {
            d = 6;
            b = FLOAT_MIN;
            t = FLOAT_MAX;
         }
         else
         {
            QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
            b = Q_INT32_MIN;
            t = Q_INT32_MAX;
         }

      }
      else if( mapimgDataType.contains( "64" ) && mapimgDataType.contains( "Float" ) )
      {
         d = 6;
         b = (double)DOUBLE_MIN;
         t = (double)DOUBLE_MAX;
      }
      else //Signed with out proper bit or int/float string
      {
         QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
         b = Q_INT32_MIN;
         t = Q_INT32_MAX;
      }
   }
   else //Completely invalid string
   {
      QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
      b = Q_INT32_MIN;
      t = Q_INT32_MAX;
      d = 0;
   }

   return;
}

MapimgValidator::MapimgValidator( double bottom, double top, int decimals, QObject* parent, const char* name )
: QValidator( parent, name )
{
   b = bottom;
   t = top;
   d = decimals;
}

MapimgValidator::~MapimgValidator()
{
}

void MapimgValidator::setRange( double minimum, double maximum, int decimals )
{
   b = minimum;
   t = maximum;
   d = decimals;
}

void MapimgValidator::setBottom( double bottom )
{
   setRange( bottom, top(), decimals() );
}

void MapimgValidator::setTop( double top )
{
   setRange( bottom(), top, decimals() );
}

void MapimgValidator::setDecimals( int decimals )
{
   setRange( bottom(), top(), decimals );
}

QValidator::State MapimgValidator::validate( QString & input, int & ) const
{
   QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
   if ( b >= 0 &&
      input.stripWhiteSpace().startsWith(QString::fromLatin1("-")) )
        return Invalid;
   if ( empty.exactMatch(input) )
      return Intermediate;
   bool ok = TRUE;
   double entered = input.toDouble( &ok );
   int nume = input.contains( 'e', FALSE );
   if ( !ok ) {
      // explicit exponent regexp
      QRegExp expexpexp( QString::fromLatin1("[Ee][+-]?\\d*$") );
      int eeePos = expexpexp.search( input );
      if ( eeePos > 0 && nume == 1 ) {
         QString mantissa = input.left( eeePos );
         entered = mantissa.toDouble( &ok );
         if ( !ok )
            return Invalid;
      } else if ( eeePos == 0 ) {
         return Intermediate;
      } else {
         return Invalid;
      }
   }

   QString tempInput = QString::number( entered );

   int tempj = tempInput.find( '.' );
   int i = input.find( '.' );

   if( (i >= 0 || tempj >= 0 || input.contains("e-", FALSE)) && d == 0 )
   {
      return Invalid;
   }

   if ( i >= 0 && nume == 0 ) {
      // has decimal point (but no E), now count digits after that
      i++;
      int j = i;
      while( input[j].isDigit() )
         j++;
      if ( j - i > d )
         return Invalid; //Intermediate;
   }

   if( entered > t )
      return Invalid;
   else if ( entered < b )
      return Intermediate;
   else
      return Acceptable;
}

void MapimgValidator::fixup( QString& input ) const
{
   double entered = input.toDouble();

   if( entered > t )
   {
   	entered = t;
   }
   else if( entered < b )
   {
   	entered = b;
   }

   input = QString::number( entered, 'f', d );

   /*fix decimal*/
   if( d == 0 )
   {
      int decimalPlace = input.find( '.' );

      if( decimalPlace != -1 )
      {
   	input = input.left( decimalPlace );
      }
   }


   return;
}
