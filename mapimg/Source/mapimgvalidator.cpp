// $Id: mapimgvalidator.cpp,v 1.2 2005/08/05 16:02:00 lwoodard Exp $

#include <QValidator>
#include <QString>
#include <QMessageBox>

#include <limits.h>
#include <math.h>

#include "mapimgvalidator.h"
#include "mapimgdatatypes.h"

MapimgValidator::MapimgValidator( QObject* parent, const char* name )
: QValidator( parent, name )
{
   m_bottom = -HUGE_VAL;
   m_top = HUGE_VAL;
   m_decimals = 1000;
   m_allowUndefined = false;
}

MapimgValidator::MapimgValidator( QString mapimgDataType, QObject* parent, const char* name )
: QValidator( parent, name )
{
   m_bottom = 0.0;
   m_top = 0.0;
   m_decimals = 0;
   m_allowUndefined = false;

   setDataType( mapimgDataType );
}

MapimgValidator::MapimgValidator( QString mapimgDataType, bool allowUndefined, QObject* parent, const char* name )
: QValidator( parent, name )
{
   m_bottom = 0.0;
   m_top = 0.0;
   m_decimals = 0;
   m_allowUndefined = allowUndefined;

   setDataType( mapimgDataType );
}

void MapimgValidator::setDataType( QString mapimgDataType, bool quiet )
{
   if( mapimgDataType.contains( "Unsigned" ) )
   {
      m_bottom = 0.0;
      m_decimals = 0;

      if( mapimgDataType.contains( "8" ) )
         m_top = Q_UINT8_MAX;
      else if( mapimgDataType.contains( "16" ) )
         m_top = Q_UINT16_MAX;
      else if( mapimgDataType.contains( "32" ) )
         m_top = Q_UINT32_MAX;
      else
      {
         if( !quiet ) QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
         m_top = Q_UINT32_MAX;
      }
   }
   else if( mapimgDataType.contains( "Signed" ) )
   {
      m_decimals = 0;

      if( mapimgDataType.contains( "8" ) )
      {
         m_bottom = Q_INT8_MIN;
         m_top = Q_INT8_MAX;
      }
      else if( mapimgDataType.contains( "16" ) )
      {
         m_bottom = Q_INT16_MIN;
         m_top = Q_INT16_MAX;
      }
      else if( mapimgDataType.contains( "32" ) )
      {
         if( mapimgDataType.contains( "Integer" ) )
         {
            m_bottom = Q_INT32_MIN;
            m_top = Q_INT32_MAX;
         }
         else if( mapimgDataType.contains( "Float" ) )
         {
            m_decimals = 6;
            m_bottom = FLOAT_MIN;
            m_top = FLOAT_MAX;
         }
         else
         {
            if( !quiet ) QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
            m_bottom = Q_INT32_MIN;
            m_top = Q_INT32_MAX;
         }

      }
      else if( mapimgDataType.contains( "64" ) && mapimgDataType.contains( "Float" ) )
      {
         m_decimals = 6;
         m_bottom = (double)DOUBLE_MIN;
         m_top = (double)DOUBLE_MAX;
      }
      else //Signed with out proper bit or int/float string
      {
         if( !quiet ) QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
         m_bottom = Q_INT32_MIN;
         m_top = Q_INT32_MAX;
      }
   }
   else //Completely invalid string
   {
      if( !quiet ) QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
      m_bottom = Q_INT32_MIN;
      m_top = Q_INT32_MAX;
      m_decimals = 0;
   }

   return;
}

MapimgValidator::MapimgValidator( double bottom, double top, int decimals, QObject* parent, const char* name )
: QValidator( parent, name )
{
   m_bottom = bottom;
   m_top = top;
   m_decimals = decimals;
   m_allowUndefined = false;
}

MapimgValidator::MapimgValidator( double bottom, double top, int decimals, bool allowUndefined, QObject* parent, const char* name )
: QValidator( parent, name )
{
   m_bottom = bottom;
   m_top = top;
   m_decimals = decimals;
   m_allowUndefined = allowUndefined;
}


MapimgValidator::~MapimgValidator()
{
}

void MapimgValidator::setRange( double minimum, double maximum, int decimals )
{
   m_bottom = minimum;
   m_top = maximum;
   m_decimals = decimals;
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

void MapimgValidator::setAllowUndefined( bool allow )
{
   m_allowUndefined = allow;
   return;
}

QValidator::State MapimgValidator::validate( QString & input, int & ) const
{
   if( input.upper() == "UNDEFINED" && m_allowUndefined )
   {
      input = "Undefined";
      return Acceptable;
   }

   QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
   if ( m_bottom >= 0 &&
      input.stripWhiteSpace().startsWith(QString::fromLatin1("-")) )
      return Invalid;
   if ( empty.exactMatch(input) )
      return Intermediate;
   bool ok = TRUE;
   double entered = input.toDouble( &ok );
/****/
   bool nume = FALSE;
   nume = input.contains( 'e');
/****/
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

   if( (i >= 0 || tempj >= 0 || input.contains("e-", FALSE)) && m_decimals == 0 )
   {
      return Invalid;
   }

   if ( i >= 0 && nume == 0 ) {
      // has decimal point (but no E), now count digits after that
      i++;
      int j = i;
      while( input[j].isDigit() )
         j++;
      if ( j - i > m_decimals )
         return Invalid; //Intermediate;
   }

   if( entered > m_top )
      return Invalid;
   else if ( entered < m_bottom )
      return Intermediate;
   else
      return Acceptable;
}

void MapimgValidator::fixup( QString& input ) const
{
   if( input.upper() == "UNDEFINED" && m_allowUndefined )
   {
      input = "Undefined";
      return;
   }

   double entered = input.toDouble();

   if( entered > m_top )
   {
      entered = m_top;
   }
   else if( entered < m_bottom )
   {
      entered = m_bottom;
   }

   input = QString::number( entered, 'f', m_decimals );

   /*fix decimal*/
   if( m_decimals == 0 )
   {
      int decimalPlace = input.find( '.' );

      if( decimalPlace != -1 )
      {
         input = input.left( decimalPlace );
      }
   }


   return;
}
