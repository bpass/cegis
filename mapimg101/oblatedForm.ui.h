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
#include "enterlatForm.h"
#include "enterlonForm.h"
#include "enterangForm.h"
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void oblatedForm::makeChanges()
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

    // Check shape m value
    bFlag = shapemCheck(shapemEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[2] = shapemEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Shape m Value\nm must be <= 2",
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Check shape n value
    bFlag = shapenCheck(shapenEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[3] = shapenEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Shape n Value\nn must be >= 2",
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

// Call dialog to get latitude
void oblatedForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Call dialog to get longitude
void oblatedForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Call dialog to get angle
void oblatedForm::getang()
{
    enterangForm *p = new enterangForm( this, "enterangForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getAng(double*)), this, SLOT(sendAng(double*)) );
    p->getAng();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setAng(double)));
    p->exec();
    delete p;
}

// Constructor
void oblatedForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    centLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    centLonButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    angleButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void oblatedForm::getGCTP()
{
    emit getGCTP( gctparray );

    if( gctparray[0] != 0.0 )
    {
    	QString sphereSize = "";
    	sphereSize.setNum( gctparray[0] );
        sphereEdit->setText( sphereSize );
    }
    if( gctparray[2] != 0.0 )
    {
    	QString paramM = "";
    	paramM.setNum( gctparray[2] );
        shapemEdit->setText( paramM );
    }
    if( gctparray[3] != 0.0 )
    {
    	QString paramN = "";
    	paramN.setNum( gctparray[3] );
        shapenEdit->setText( paramN );
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

// Set latitude
void oblatedForm::setLat( double l )
{
    gctparray[5] = l;
}

// Set longitude
void oblatedForm::setLon( double l )
{
    gctparray[4] = l;
}

// Set angle
void oblatedForm::setAng( double l )
{
    gctparray[8] = l;
}

// Check sphere edit for valid double
void oblatedForm::sphereCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sphereEdit->setText("6370997.000000");
    }
}

// Check shapem edit for valid double
void oblatedForm::mCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	shapemEdit->setText("0.000000");
    }
}

// Check shapen edit for valid double
void oblatedForm::nCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	shapenEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void oblatedForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void oblatedForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}


void oblatedForm::sendLat( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}


void oblatedForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}


void oblatedForm::sendAng( double * ptr )
{
    //Send angle for this button
     *ptr = gctparray[8];

     return;
}
