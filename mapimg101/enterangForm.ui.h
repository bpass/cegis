// $Id: enterangForm.ui.h,v 1.3 2005/01/11 17:11:27 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
**
** Created by S. Posch - 08/2002
**
*****************************************************************************/
#include <qstring.h>
#include <qmessagebox.h>
#include <ctype.h>
#include "window_flags.h"

#include "qembed_images.h"

// Check seconds edit for valid double
void enterangForm::checkSeconds()
{
    QString secstring = secondsEdit->text();
    char csecstring[200] = "";
    bool bFlag = true;
    double zero = 0.0;
    double sixty = 60.0;
    
    strcpy(csecstring,secstring);
    
    int i = 0;
    int decimals = 0;
    
    // check Pixel Edit Box for a "value"
    while (i < 200 && csecstring[i] != '\0')
    {
		// if digit found, proceed
		if(isdigit(csecstring[i]))
		{
			i++;
		}
		
		// if decimal found, and only one decimal exists, proceed
		else if(csecstring[i] == '.')
		{
			if(decimals < 1)
			{
				decimals++;
				i++;
			}
			else 
			{
				secondsEdit->setText("0");
				bFlag = false;
				break;
			}
		}
	
		// otherwise, delete contents from the edit box
		else
		{
			secondsEdit->setText("0");
			bFlag = false;
			break;
		}
    }

    if(bFlag)
    {
	double testit = secondsEdit->text().toDouble();
	if(testit < zero || testit > sixty)
	{
	    secondsEdit->setText("0");
	}
    }
}

// Put angle into DDDMMMSSS.SS
void enterangForm::setAng()
{
    QString s;
    QString t;
    s = s.setNum(degreeSpinBox->value());
    
    // If single digit minutes, need two zeros
    if(minuteSpinBox->value() < 10)
    {
	s.append("00");
    }
    else
    {
	s.append("0");
    }
    
    s.append( t.setNum(minuteSpinBox->value()) );
    
    QString secondsZeros = secondsEdit->text();
    trimZeros( &secondsZeros );
    if( secondsZeros != secondsEdit->text() )
        secondsEdit->setText( secondsZeros );


    // If less than one, need three zeros, single digit seconds, need two zeros
    if( secondsEdit->text().toDouble() < 1)
    {
	s.append("000");
    }
    else if( secondsEdit->text().toDouble() < 10)
    {
	s.append("00");
    }
    else
    {
	s.append("0");
    }

    
    s.append (secondsEdit->text());
    
    ang = s.toDouble();
    
    // If out of range, don't allow
    if(ang > 360000000 || ang < 0)
    {
	QMessageBox::warning (this, "Aborted!","Angle is out of Range", 
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }
    
    emit changesMade(ang);
    this->accept();
}

void enterangForm::getAng()
{
    gctpAng = new double;
    emit getAng( gctpAng );

    if( *gctpAng != 0.0 )
    {
        int secLength = 10,
            minLength = 3;
        QString curValue = "",
                seconds = "",
                minutes = "",
                degrees = "";

        curValue.setNum( (double)*gctpAng, 'f', 6 );

        if( curValue.length() >= (unsigned int)(secLength) - 2)
        {
            seconds = curValue.right( secLength );
            if( ((int)curValue.length() - secLength) >= 0 )
                curValue = curValue.left( curValue.length() - secLength );
            else
                curValue = "";

            if( curValue.length() > 0 )
            {
                minutes = curValue.right( minLength );
                if( ((int)curValue.length() - minLength) >= 0 )
                    curValue = curValue.left( curValue.length() - minLength );
                else
                    curValue = "";
            }
            else
                minutes.setNum( 0 );

            if( curValue.length() > 0 )
                degrees = curValue;
            else
                degrees = "0";

            degreeSpinBox->setValue( degrees.toInt() );
            minuteSpinBox->setValue( minutes.toInt() );
            trimZeros( &seconds );
            secondsEdit->setText( seconds );
        }


    }
    return;
}



void enterangForm::trimZeros( QString *toTrim )
{
     bool foundNonZero = false,
          foundDecimal = false;

     //trims leading zeros from toTrim; stops if it hits any non-'0' character
     while( !foundNonZero && !toTrim->isEmpty() && !toTrim->isNull() )
     {
        if( (*toTrim)[0] == '0' )
     	{
     	    *toTrim = toTrim->right( toTrim->length() - 1 );
     	}
     	else
     	    foundNonZero = true;
     }

     foundNonZero = false;
     foundDecimal = (bool)toTrim->contains('.');

     //trims leading trailing from toTrim; on trims if there is a decimal; stops if it hits a non-'0'
     //or if no decimal is found in toTrim
     while( !foundNonZero && foundDecimal && !toTrim->isEmpty() && !toTrim->isNull() )
     {
        if( ((*toTrim)[(int)(toTrim->length() - 1)] == '0')  || ((*toTrim)[(int)(toTrim->length()-1)] == '.') )
     	{
     	    *toTrim = toTrim->left( toTrim->length() - 1 );
     	}
     	else if( (*toTrim)[(int)(toTrim->length()-1)] == '.' )
     	{
     	    *toTrim = toTrim->left( toTrim->length() - 1 );
     	}
     	else
     	    foundNonZero = true;

        foundDecimal = (bool)toTrim->contains('.');
     }
     return;
}
