// $Id: hombForm.ui.h,v 1.3 2005/01/11 17:11:27 jtrent Exp $


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
#include "enterangForm.h"
#include "errorchecks.h"

#include "qembed_images.h"

#include "window_flags.h"
#include "cproj.h"
#include <math.h>
#ifndef EPSLN
        #define EPSLN	1.0e-10
#endif

// Make changes to gctp array
void hombForm::makeChanges()
{
    bool bFlag;
    
    //  Check sMajor value
    bFlag = smajminCheck(smajorEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[0] = smajorEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning(this, "Aborted!","Invalid SMajor Value",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }
    
    // Check sMinor value
    bFlag = smajminCheck(sminorEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[1] = sminorEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid SMinor Value",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }
    
    // Check factor value
    bFlag = factorCheck(factorEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[2] = factorEdit->text().toDouble();
    }
    else
    {
	// Ask if value is correct
	int q = QMessageBox::information( this, "Are you sure?", "Most usage of Scale Factor is closer to a value of 1" ,
				          "&Yes", "&No", QString::null, 1, 1 );
	if(q == 0)
	{
	    gctparray[2] = factorEdit->text().toDouble();
	}
	else
	{
	    return;
	}
    }
    
    // Check fe value
    bFlag = fefnCheck(feEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[6] = feEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid False Easting Value",
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
	QMessageBox::warning (this, "Aborted!","Invalid False Northing Value",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Set element 12 to 1 (Required for HOMB)
    gctparray[12] = 1;

    if( valueCheck() )
    {
        emit changesMade(gctparray);
        this->accept();
    }
    return;
}

// Constructor
void hombForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    aziangleButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    azmthptButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    origLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void hombForm::getGCTP()
{
    emit getGCTP( gctparray );

    if( gctparray[0] != 0.0 )
    {
    	QString semiMajor = "";
    	semiMajor.setNum( gctparray[0] );
        smajorEdit->setText( semiMajor );
    }
    if( gctparray[1] != 0.0 )
    {
    	QString semiMinor = "";
    	semiMinor.setNum( gctparray[1] );
        sminorEdit->setText( semiMinor );
    }
    if( gctparray[2] != 0.0 )
    {
    	QString scaleFactor = "";
    	scaleFactor.setNum( gctparray[2] );
        factorEdit->setText( scaleFactor );
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
void hombForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude
void hombForm::setLon( double l )
{
    gctparray[4] = l;
}

// Call dialog to get latitude
void hombForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Set latitude
void hombForm::setLat( double l )
{
    gctparray[5] = l;
}

// Call dialog to get azi angle
void hombForm::getang()
{
    enterangForm *p = new enterangForm( this, "enterangForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getAng(double*)), this, SLOT(sendAng(double*)) );
    p->getAng();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setang(double)));
    p->exec();
    delete p;
}

// Set azi angle
void hombForm::setang( double l )
{
    gctparray[3] = l;
}

// Check smajor edit for valid double
void hombForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

// Check sminor edit for valid double
void hombForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check factor edit for valid double
void hombForm::scaleCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	factorEdit->setText("0.000000");
    }
}

// Check fe for valid double
void hombForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn for valid double
void hombForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

bool hombForm::valueCheck()
{
 float lat_origin = gctparray[5];

   if ((fabs(lat_origin) <= EPSLN) && (fabs(fabs(lat_origin) - HALF_PI) <= EPSLN))
   {
    	int q = QMessageBox::information (this, "Warning!","Latitude of projection origin may produce invalid data. Are you sure you wish to continue?",
     					 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape, QMessageBox::NoButton);
        switch( q )
        {
        	case QMessageBox::No:
                     return false;
                     break;
                default:
                     return true;
                     break;
        }
   }
 return true;
}


void hombForm::sendAng( double * ptr )
{
    //Send angle for this button
     *ptr = gctparray[3];

     return;
}


void hombForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}


void hombForm::sendLat( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}
