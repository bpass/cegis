// $Id: mapimgvalidator.cpp,v 1.1 2005/02/17 18:52:59 jtrent Exp $

#include <qvalidator.h>
#include <qstring.h>
#include <qmessagebox.h>

#include <limits.h>
#include <math.h>

#include "mapimgvalidator.h"

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


   if( mapimgDataType.contains( "Unsigned" ) )
   {
        b = 0.0;
   	d = 0;

        if( mapimgDataType.contains( "8" ) )
            t = _UI8_MAX;
        else if( mapimgDataType.contains( "16" ) )
            t = _UI16_MAX;
        else if( mapimgDataType.contains( "32" ) )
            t = _UI32_MAX;
        else
        {
            QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
            t = _UI32_MAX;
        }
   }
   else if( mapimgDataType.contains( "Signed" ) )
   {
   	d = 0;

        if( mapimgDataType.contains( "8" ) )
        {
            b = _I8_MIN;
            t = _I8_MAX;
        }
        else if( mapimgDataType.contains( "16" ) )
        {
            b = _I16_MIN;
            t = _I16_MAX;
        }
        else if( mapimgDataType.contains( "32" ) )
        {
            if( mapimgDataType.contains( "Integer" ) )
            {
            	b = _I32_MIN;
                t = _I32_MAX;
            }
            else if( mapimgDataType.contains( "Float" ) )
            {
            	d = 6;
            	b = _I32_MIN;                                   /* what are float min/max */
                t = _I32_MAX;
            }
            else
            {
                QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
            	b = _I32_MIN;
                t = _I32_MAX;
            }

        }
        else if( mapimgDataType.contains( "64" ) && mapimgDataType.contains( "Float" ) )
        {
            d = 6;
            b = (double)_I64_MIN;                                   /* what are double min/max */
            t = (double)_I64_MAX;
        }
        else //Signed with out proper bit or int/float string
        {
            QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
            b = _I32_MIN;
            t = _I32_MAX;
        }
   }
   else //Completely invalid
   {
        QMessageBox::information( NULL, "MapimgValidator", "Bad data type string" );
        b = _I32_MIN;
        t = _I32_MAX;
        d = 0;
   }
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

    int j = tempInput.find( '.' );
    int i = input.find( '.' );

    if( (i >= 0 || j >= 0 || input.contains("e-", FALSE)) && d == 0 )
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
	    return Intermediate;
    }

    if( entered > t )
        return Invalid;
    else if ( entered < b )
	return Intermediate;
    else
	return Acceptable;
}

