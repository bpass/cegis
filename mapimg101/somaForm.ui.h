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
#include "enterangForm.h"
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void somaForm::makeChanges()
{
     bool bFlag;
    
     // Check sMajor valie
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
    
    // Check ps revolution value
    bFlag = psrevCheck(psrevEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[8] = psrevEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Period of Revolution",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton); 
	return;
    }  
  
    // Check lrat value
    bFlag = lratCheck(lratEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[9] = lratEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Landsat Ratio",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // set pflag value
    gctparray[10] = pflagSpin->value();

    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void somaForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    incangleButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    asclonButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

//Get appropriate GCTP array from MapIMG and setup dialog
void somaForm::getGCTP()
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
    if( gctparray[8] != 0.0 )
    {
    	QString PSRev = "";
    	PSRev.setNum( gctparray[8] );
        psrevEdit->setText( PSRev );
    }
    if( gctparray[9] != 0.0 )
    {
    	QString LRat = "";
    	LRat.setNum( gctparray[9] );
        lratEdit->setText( LRat );
    }
    if( gctparray[10] != 0.0 )
    {
        pflagSpin->setValue( gctparray[10] );
    }

    return;
}

// Call dialog to get longitude
void somaForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Call dialog to get angle
void somaForm::getang()
{
    enterangForm *p = new enterangForm( this, "enterangForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getAng(double*)), this, SLOT(sendAng(double*)) );
    p->getAng();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setAng(double)));
    p->exec();
    delete p;
}

// Set longitude
void somaForm::setLon( double l )
{
    gctparray[4] = l;
}

// Set angle
void somaForm::setAng( double l )
{
    gctparray[3] = l;
}

// Check smajor edit for valid double
void somaForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

// Check sminor edit for valid double
void somaForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void somaForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void somaForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

// Check psrev edit for valid double
void somaForm::revCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	psrevEdit->setText("0.000000");
    }
}

// Check lrat edit for valid double
void somaForm::ratioCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	lratEdit->setText("0.5201613");
    }
}    



void somaForm::sendAng( double * ptr )
{
    //Send angle for this button
     *ptr = gctparray[3];

     return;
}


void somaForm::sendLon( double * ptr )
{
    //Send longitude for this button
     *ptr = gctparray[4];

     return;
}
