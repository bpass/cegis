// $Id: vanderForm.ui.h,v 1.3 2005/01/11 17:11:28 jtrent Exp $


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
#include <qmessagebox.h>
#include "enterlonForm.h"
#include "enterlatForm.h"
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void vanderForm::makeChanges()
{
    bool bFlag;
    
    // Check radius of sphere value
    bFlag = smajminCheck(sphereEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[0] = sphereEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Sphere Value",
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }
    
    // Check fe value
    bFlag = fefnCheck(feEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[6] = feEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning(this, "Aborted!","Invalid False Easting Value",
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Check fn value
    bFlag = fefnCheck(fnEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[7] = fnEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning(this, "Aborted!","Invalid False Northing Value",
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void vanderForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    centMerButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    origLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for (int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void vanderForm::getGCTP()
{
    emit getGCTP( gctparray );

    if( gctparray[0] != 0.0 )
    {
    	QString sphereSize = "";
    	sphereSize.setNum( gctparray[0] );
        sphereEdit->setText( sphereSize );
    }
    if( gctparray[6] != 0.0 )
    {
    	QString falseEasting = "";
    	falseEasting.setNum( gctparray[6] );
        feEdit->setText( falseEasting );
    }
    if( gctparray[7] != 0.0 )
    {
    	QString falseNorthing = "";
    	falseNorthing.setNum( gctparray[7] );
        fnEdit->setText( falseNorthing );
    }

    return;
}


// Call dialog to get longitude
void vanderForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude
void vanderForm::setLon( double l )
{
     gctparray[4] = l;
}

// Call dialog to get latitude
void vanderForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Set latitude
void vanderForm::setLat( double l )
{
        gctparray[5] = l;
}

// Check sphere edit for valid double
void vanderForm::sphereCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sphereEdit->setText("6370997.000000");
    }
}

// Check fe edit for valid double
void vanderForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void vanderForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}



void vanderForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}


void vanderForm::sendLat( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}
