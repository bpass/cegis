// $Id: projfileForm.ui.h,v 1.3 2005/01/11 17:11:28 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include <ctype.h>
#include "window_flags.h"

#include "qembed_images.h"

void projfileForm::makeChanges()
{
    emit changesMade(pixelSize);
}

// Function to check for a proper double in the pixel edit box
void projfileForm::pixEditCheck()
{
    QString pixstring = pixelEdit->text();
    char cpixstring[200] = "";
    strcpy(cpixstring,pixstring);
    
    int i = 0;
    int decimals = 0;
    
    // check Pixel Edit Box for a "value"
    while (i < 200 && cpixstring[i] != '\0')
    {
		// if digit found, proceed
		if(isdigit(cpixstring[i]))
		{
			i++;
		}
		
		// if decimal found, and only one decimal exists, proceed
		else if(cpixstring[i] == '.')
		{
			if(decimals < 1)
			{
				decimals++;
				i++;
			}
			else 
			{
				pixelEdit->setText("");
				break;
			}
		}
	
		// otherwise, delete contents from the edit box
		else
		{
			pixelEdit->setText("");
			break;	    
		}
    }
}

// Function to check the pixel combo box and assign the appropriate value
void projfileForm::pixelCheck()
{   
    double fivedeg = 555974.548395;		// pixel size for 5 degrees
    double onedeg = 111194.909679;		// pixel size for 1 degree
    double thirtymin = 55597.4548395;		// pixel size for 30 minutes
    double fivemin = 9266.24247325;		// pixel size for 5 minutes
    double thirtysec = 926.624247325;		// pixel size for 30 arc seconds
    
/*    double fivedeg = 555974.6097005;		// pixel size for 5 degrees
    double onedeg = 111194.9219401;		// pixel size for 1 degree
    double thirtymin = 55597.46097005;		// pixel size for 30 minutes
    double fivemin = 9266.243495;			// pixel size for 5 minutes
    double thirtysec = 926.6243495;		// pixel size for 30 arc seconds
*/
    
    // get current item in pixel combo box, if "---------", set back to original
    int index = pixelComboBox->currentItem();
    if(index < 2)
    {
		pixelEdit->setDisabled(true);
		pixelComboBox->setCurrentItem(0);
    }
    
    // otherwise, set the correct value in pixelSize
    // if other chosen, activate the other box
    else
    {
		switch(index)
		{
		case 2:
			pixelEdit->setDisabled(true);
			pixelSize = fivedeg;
			break;
			
		case 3:
			pixelEdit->setDisabled(true);
			pixelSize = onedeg;
			break;
			
		case 4:
			pixelEdit->setDisabled(true);
			pixelSize = thirtymin;
			break;
			
		case 5:
			pixelEdit->setDisabled(true);
			pixelSize = fivemin;
			break;
			
		case 6:
			pixelEdit->setDisabled(true);
			pixelSize = thirtysec;
			break;
			
		case 7:
			pixelEdit->setEnabled(true);
			break;
		}
    }
}
