// $Id: intsineForm.ui.h,v 1.3 2005/01/11 17:11:27 jtrent Exp $


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
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void intsineForm::makeChanges()
{
    // Make sure justification has been edited
    if(rflagComboBox->currentItem() < 2)
    {
	QMessageBox::information (this, "Aborted!","Please set a Justification",
				         QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }
    
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

    // Set number of zones
    gctparray[8] = nzonesSpin->value();

    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void intsineForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    centMerButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void intsineForm::getGCTP()
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
void intsineForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude
void intsineForm::setLon( double l )
{
    gctparray[4] = l;
}


// Check and set the value of rflag
void intsineForm::checkRFlag()
{
    int index = rflagComboBox->currentItem();
    if(index < 2)
    {
	rflagComboBox->setCurrentItem(0);
    }
    
    switch(index)
    {
    case 2:
	gctparray[10] = 0;
	break;
	
    case 3:
	gctparray[10] = 1;
	break;
	
    case 4:
	gctparray[10] = 2;
	break;
    }
}

// Check sphere edit for valid double
void intsineForm::sphereCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sphereEdit->setText("6370997.000000");
    }
}

// Check fe edit for valid double
void intsineForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void intsineForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

// Check nzones spin for EVEN number
void intsineForm::zoneCheck()
{
    if((nzonesSpin->value() % 2) != 0)
    {
	nzonesSpin->setValue(nzonesSpin->value() - 1);
    }
}


void intsineForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}
