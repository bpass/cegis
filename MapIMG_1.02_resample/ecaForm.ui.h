//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
**
**Created By S. Posch - 08/2002
**
*****************************************************************************/
#include <qmessagebox.h>
#include "enterlonForm.h"
#include "enterlatForm.h"
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"
// Make changes to the gctp array
void ecaForm::makeChanges()
{
   bool bFlag;
    
   // Check the value of sMajor
    bFlag = smajminCheck(smajorEdit->text().toDouble());
    if(bFlag)
    {
	gctparray[0] = smajorEdit->text().toDouble();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid SMajor Value",
			                       QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Check the value of sMinor
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

    // Check the value of fe
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
    
    // Check the value of fn
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

    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void ecaForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );
    stdPar1Button->setPixmap( qembed_findImage( "vwp_icon" ) );
    centMerButton->setPixmap( qembed_findImage( "vwp_icon" ) );
    origLatButton->setPixmap( qembed_findImage( "vwp_icon" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void ecaForm::getGCTP()
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
void ecaForm::getlon()
{
    enterlonForm *p = new enterlonForm( this, "enterlonForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLon(double*)), this, SLOT(sendLon(double*)) );
    p->getLon();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLon(double)));
    p->exec();
    delete p;
}

// Set longitude value
void ecaForm::setLon( double l )
{
    gctparray[4] = l;
}

// Call dialog to get latitude
void ecaForm::getlat()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendLat(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setLat(double)));
    p->exec();
    delete p;
}

// Set latitude value
void ecaForm::setLat( double l )
{
    gctparray[5] = l;
}

// Call dialog to get lat of standard parallel
void ecaForm::getstdpar()
{
    enterlatForm *p = new enterlatForm( this, "enterlatForm", true, WINDOW_FLAGS);
    connect(p, SIGNAL(getLat(double*)), this, SLOT(sendStdPar(double*)) );
    p->getLat();
    connect(p, SIGNAL(changesMade(double)), this, SLOT(setstdPar(double)));
    p->exec();
    delete p;
}

// Set latitude of standard parallel
void ecaForm::setstdPar( double l )
{
    gctparray[2] = l;
}

// Check smajor edit for valid double
void ecaForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

// Check sminor edit for valid double
void ecaForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void ecaForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void ecaForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}



void ecaForm::sendStdPar( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[2];

     return;
}


void ecaForm::sendLon( double * ptr )
{
  //Send longitude for this button
     *ptr = gctparray[4];

     return;
}


void ecaForm::sendLat( double * ptr )
{
    //Send latitude for this button
     *ptr = gctparray[5];

     return;
}
