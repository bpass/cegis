// $Id: homaForm.ui.h,v 1.3 2005/01/11 17:11:27 jtrent Exp $


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

#include "cproj.h"
#include <math.h>
#ifndef EPSLN
        #define EPSLN	1.0e-10
#endif

// Make changes to gctp array
void homaForm::makeChanges()
{
    bool bFlag;
    
    // Check sMajor value
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

    // Check scale factor value
    bFlag = factorCheck(factorEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[2] = factorEdit->text().toDouble();
    }
    else
    {
	int q = QMessageBox::information( this, "Are you sure?", "Most usage of Scale Factor is closer to a value of 1\n\n Proceed anyway?\n" ,
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

    if( valueCheck() )
    {
        emit changesMade(gctparray);
        this->accept();
    }
    return;
}

// Constructor
void homaForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    origLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    long1Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    long2Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    lat1Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    lat2Button->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void homaForm::getGCTP()
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


// Call dialog to get latitude
void homaForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLatOrig(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Set latitude
void homaForm::setLat( double l )
{
    gctparray[5] = l;
}

// Call dialog to get longitude 1
void homaForm::getlon1()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon1st(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon1(double)));
    p->exec();
    delete p;
}

// Set longitude 1
void homaForm::setLon1( double l )
{
    gctparray[8] = l;
}

// Call dialog to longitude 2
void homaForm::getlon2()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon2n(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon2(double)));
    p->exec();
    delete p;
}

// Set longitude 2
void homaForm::setLon2( double l )
{
    gctparray[10] = l;
}

// Call dialog to get latitude 1
void homaForm::getlat1()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat1st(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat1(double)));
    p->exec();
    delete p;
}

// Set latitude 1
void homaForm::setLat1(double l)
{
    gctparray[9] = l;
}

// Call dialog to get latitude 2
void homaForm::getlat2()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat2nd(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat2(double)));
    p->exec();
    delete p;
}

// Set latitude 2
void homaForm::setLat2( double l )
{
    gctparray[11] = l;
}

// Check smajor edit for valid double
void homaForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

//; Check sminor edit for valid double
void homaForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check factor edit for valid double
void homaForm::scaleCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	factorEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void homaForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void homaForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

bool homaForm::valueCheck()
{
  float lat_origin = gctparray[5],
        lat1       = gctparray[9],
        lat2       = gctparray[11];


//Condition of GCTPc for initialization
  if (fabs(lat1 - lat2) <= EPSLN)
  {
     int q = QMessageBox::information (this, "Warning!","Specified latitudes may produce invalid data. Are you sure you wish to continue?",
     					 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape, QMessageBox::NoButton);
     switch( q )
     {
       	case QMessageBox::No:
             return false;
             break;
        default:
             break;
    }
  }
 else
   if ((fabs(lat1) <= EPSLN) || (fabs(fabs(lat1) - HALF_PI) <= EPSLN))
   {
     int q = QMessageBox::information (this, "Warning!","Latitude of point 1 may produce invalid data. Are you sure you wish to continue?",
     					 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape, QMessageBox::NoButton);
     switch( q )
     {
       	case QMessageBox::No:
             return false;
             break;
        default:
             break;
     }
    }
   else
   if (fabs(fabs(lat_origin) - HALF_PI) <= EPSLN)
   {
	int q = QMessageBox::information (this, "Warning!","Latitude of projection origin may produce invalid data. Are you sure you wish to continue?",
     					 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape, QMessageBox::NoButton);
     switch( q )
     {
       	case QMessageBox::No:
             return false;
             break;
        default:
             break;
     }
    }
    return true;
}


void homaForm::sendLon1st( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[8];

     return;
}


void homaForm::sendLon2n( double * ptr )
{    //Send longitude for this button
     *ptr = gctparray[10];

     return;
}


void homaForm::sendLat1st( double * ptr )
{
     //Send latitude for this button
     *ptr = gctparray[9];

     return;
}


void homaForm::sendLat2nd( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[11];

     return;
}

void homaForm::sendLatOrig( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}
