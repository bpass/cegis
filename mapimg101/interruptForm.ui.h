// $Id: interruptForm.ui.h,v 1.2 2005/01/06 19:55:34 jtrent Exp $


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
void interruptForm::makeChanges()
{
    bool bFlag;
    
    // Check radius of sphere
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
    emit changesMade(gctparray);
    this->accept();
}

// Constructor
void interruptForm::init()
{
    this->setIcon( qembed_findImage( "usgs" ) );

    gctparray = new double[15];
    Q_CHECK_PTR( gctparray );
    for(int i = 0; i < 15; i++)
    {
	gctparray[i] = 0;
    }
}

void interruptForm::getGCTP()
{
    emit getGCTP( gctparray );

    if( gctparray[0] != 0.0 )
    {
    	QString sphereSize = "";
    	sphereSize.setNum( gctparray[0] );
        sphereEdit->setText( sphereSize );
    }

    return;
}


// Check sphere edit for valid double
void interruptForm::sphereCheck( const QString & check )
{
    bool bFlag = validDouble(check);
    if(!bFlag)
    {
	sphereEdit->setText("6370997.000000");
    }
}
