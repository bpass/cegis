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
#include "errorchecks.h"
#include "window_flags.h"

#include "qembed_images.h"

// Make changes to gctp array
void sombForm::makeChanges()
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

    // Check satnum value
    bFlag = satnumCheck(satnumSpin->value());
    if(bFlag)
    {
	gctparray[2] = satnumSpin->value();
    }
    else
    {
	QMessageBox::warning (this, "Aborted!","Invalid Satellite Number",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Check path value
   bFlag = pathCheck(satnumSpin->value(), pathSpin->value());
   if(bFlag)
   {
        gctparray[3] = pathSpin->value();
   }
   else
   {
	QMessageBox::warning (this, "Aborted!","Invalid Path Number",
					 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	return;
    }

    // Set value 12 to 1 (required for SOM B option)
    gctparray[12] = 1;

    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void sombForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void sombForm::getGCTP()
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
    	satnumSpin->setValue( gctparray[2] );
    }
    if( gctparray[3] != 0.0 )
    {
        pathSpin->setValue( gctparray[3] );
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


// Check smajor edit for valid double
void sombForm::smajCheck( const QString & check )
{
   bool bFlag = validDouble(check);
   if(!bFlag)
   {
       smajorEdit->setText("0.000000");
   }
}

// Check sminor edit for valid double
void sombForm::sminCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sminorEdit->setText("0.000000");
    }
}

// Check fe edit for valid double
void sombForm::feCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	feEdit->setText("0.000000");
    }
}

// Check fn edit for valid double
void sombForm::fnCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	fnEdit->setText("0.000000");
    }
}

// Update satellite path tooltip
void sombForm::updateToolTip()
{
     int satNum = satnumSpin->value();
     QToolTip::remove( pathSpin );

     if( (satNum >= 1) && (satNum <= 3) )
     {
     	//path must be >= 1 && <= 251
        QToolTip::add( pathSpin, tr( "<B>Satellite Path:</B> value must be between 1 and 251." ) );
     }
     else if( (satNum == 4) || (satNum == 5) || (satNum == 7) )
     {
     	//path must be >= 1 && <= 233
        QToolTip::add( pathSpin, tr( "<B>Satellite Path:</B> value must be between 1 and 233." ) );
     }
     else
     {
     	//invalid sastellite number
	QToolTip::add( pathSpin, tr( "<B>Satellite Path:</B> satellite number is invalid. Please check value." ) );
     }
}
