// $Id: utmForm.ui.h,v 1.2 2005/01/06 19:55:35 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

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
#include "enterlonForm.h"
#include "enterlatForm.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void utmForm::makeChanges()
{
    emit changesMade(gctparray, zonecodeSpin->value());
}

// Constructor
void utmForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    latzButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    lonzButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void utmForm::getGCTP()
{
    emit getGCTP( gctparray );

    return;
}

// Call dialog to get longitude
void utmForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude
void utmForm::setLon( double l )
{
    gctparray[0] = l;
}

// Call dialog to get latitude
void utmForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Set latitude
void utmForm::setLat( double l )
{
    gctparray[1] = l;
}


void utmForm::sendLat( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[1];

     return;
}


void utmForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[0];

     return;
}
