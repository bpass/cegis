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
void lambForm::makeChanges()
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
void lambForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    stdPar1Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    stdPar2Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    centMerButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    origLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void lambForm::getGCTP()
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
void lambForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude
void lambForm::setLon(double l)
{
    gctparray[4] = l;
}

// Call dialog to get latitude 1
void lambForm::getlat1()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat1st(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat1(double)));
    p->exec();
    delete p;
}

// Set latitude 1
void lambForm::setLat1( double l )
{
    gctparray[2] = l;
}

// Call dialog to get latitude 2
void lambForm::getlat2()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat2nd(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat2(double)));
    p->exec();
    delete p;
}

// Set latitude 2
void lambForm::setLat2( double l )
{
    gctparray[3] = l;
}

// Call dialog to get latitude 3
void lambForm::getlat3()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLatOrig(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat3(double)));
    p->exec();
    delete p;
}

// Set latitude 3
void lambForm::setLat3( double l )
{
    gctparray[5] = l;
}

// Check smajor edit for valid double
void lambForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

// Check sminor edit for valid double
void lambForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void lambForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fnedit for valid double
void lambForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

bool lambForm::valueCheck()
{
     if( fabs(gctparray[2] + gctparray[3]) < EPSLN)
     {
	int q = QMessageBox::information (this, "Warning!","Standard parallel values may produce invalid data. Are you sure you wish to continue?",
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


void lambForm::sendLat1st( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[2];

     return;
}


void lambForm::sendLat2nd( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[3];

     return;
}


void lambForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}


void lambForm::sendLatOrig( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}
