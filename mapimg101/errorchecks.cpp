// $Id: errorchecks.cpp,v 1.3 2005/01/11 17:11:27 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

///////////////////////////////////////////////////////
// Error checking functions for mapimg               //
//                                                   //
// Created by: S. Posch - 08/2002                    //
// Modified by: J. Trent - 06/2003                   //
//              Adjusted ppointCheck() bounds for    //
//                       gen. vert.                  //
///////////////////////////////////////////////////////
#include <ctype.h>
#include <qstring.h>
#include "errorchecks.h"


bool validDouble(QString s)
{
	char c[200] = "";
    strcpy(c,s);
    
    int i = 0;
    int decimals = 0;
    
    // check Pixel Edit Box for a "value"
    while (i < 200 && c[i] != '\0')
    {
	if(i == 0 && c[i] == '-')
	{
	    i++;
	}
	
		// if digit found, proceed
		else if(isdigit(c[i]))
		{
			i++;
		}
		
		// if decimal found, and only one decimal exists, proceed
		else if(c[i] == '.')
		{
			if(decimals < 1)
			{
				decimals++;
				i++;
			}
			else 
			{
				return false;
			}
		}
	
		// otherwise, delete contents from the edit box
		else
		{
			return false;
		}
    }
	return true;
}


bool fefnCheck(double d)
{
	if(d >= -10000000 && d <= 10000000)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool factorCheck(double d)
{
	if(d >= 0 && d < 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool ppointCheck(double d)
{
	if(d >= -1000000000 && d <= 1000000000)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool shapemCheck(double d)
{
	if(d <= 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool shapenCheck(double d)
{
    if(d >= 2)
    {
	return true;
    }
    else
    {
	return false;
    }
}


bool lratCheck(double d)
{
	if(d >= 0 && d <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool satnumCheck(double d)
{
	if( (d >= 1 && d <= 5) || (d == 7) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool smajminCheck(double d)
{
	if(d >= 0 && d <= 10000000)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool pathCheck(int l, int d)
{
	if(l == 4 || l == 5 || l == 7)
	{
		if(d >= 1 && d <= 233)
		{
			return true;
		}
	}

	else if(l == 1 || l == 2 || l == 3)
	{
		if(d >= 1 && d <= 251)
		{
			return true;
		}
	}

	return false;
}

bool psrevCheck(double d)
{
    if(d >= -10000000 && d <= 10000000)
    {
	return true;
    }
    else
    {
	return false;
    }
}
