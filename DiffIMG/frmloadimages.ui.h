/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qcolor.h>
#include <qmap.h>
#include <qcombobox.h>

#include "frmdifference.h"
#include "frmdimensions.h"
#include "imagefile.h"

#include "compare.h"

void FrmLoadImages::loadOriginal()
{
    QString *fileName = &originalImage.fileName;

    originalImage.dataType = "";

    if( loadImage( &originalImage ) )
    {
        lblOriginal->setText( QString( "Original Image: " + fileName->right(fileName->length() - fileName->findRev( "/" ) - 1) + "\n\trows: %1  columns: %2  %3")
                                          .arg( originalImage.rows )
                                          .arg( originalImage.columns )
                                          .arg( originalImage.dataType ) );

    }
}


void FrmLoadImages::loadReprojected()
{
    QString *fileName = &reprojectedImage.fileName;

    if( loadImage( &reprojectedImage ) )
    {
        lblReprojected->setText( QString( "Reprojected Image: " + fileName->right(fileName->length() - fileName->findRev( "/" ) - 1) + "\n\trows: %1  columns: %2  %3")
                                          .arg( reprojectedImage.rows )
                                          .arg( reprojectedImage.columns )
                                          .arg( reprojectedImage.dataType ) );
    }
}


void FrmLoadImages::showDifference()
{
    QString bitSelection = originalImage.dataType,
            outputName = "test.txt";

    bool compareSuccess = false;
  
    originalImage.file = fopen( originalImage.fileName.ascii(), "rb" );
    reprojectedImage.file = fopen( reprojectedImage.fileName.ascii(), "rb" );

    if( bitSelection == "Unsigned 8 Bit" )
    {

        UINT8 ucType = 0;

        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        ucType );
    }
    else if( bitSelection == "Signed 8 Bit" )
    {
        INT8 scType = '0';
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        scType );

    }
    else if( bitSelection == "Unsigned 16 Bit" )
    {
    	unsigned short usType = 0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        usType );

    }
    else if( bitSelection == "Signed 16 Bit" )
    {
    	short sType = 0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        sType );

    }
    else if( bitSelection == "Unsigned 32 Bit" )
    {
    	unsigned int uiType = 0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        uiType );

    }
    else if( bitSelection == "Signed 32 Bit" )
    {
    	int iType = 0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        iType );

    }
    else if( bitSelection == "IEEE 32 Bit Float" )
    {
    	float fType = 0.0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        fType );

    }
    else if( bitSelection == "IEEE 64 Bit Float" )
    {
    	double dType = 0.0;
        compareSuccess = compareImages( originalImage,
                                        reprojectedImage,
                                        outputName,
                                        dType );

    }
    else
    {
    	QMessageBox::critical( this, "Error", "Invalid Data Type Selection." );
        return;
    }
    
    fclose( originalImage.file );
    fclose( reprojectedImage.file );

    fflush( stdout );

    if( compareSuccess )
    {
        int w = 0,
            h = 0;


        if( originalImage.rows > reprojectedImage.rows )
    	    h = originalImage.rows;
        else
    	    h = reprojectedImage.rows;

        if( originalImage.columns > reprojectedImage.columns )
    	    w = originalImage.columns;
        else
    	    w = reprojectedImage.columns;


        FrmDifference *difference = new FrmDifference( this );


        difference->paint( w, h, bitSelection );

        difference->exec();
    }
    else
    {
    	QMessageBox::critical( this, "Error", "Compare Failed.\nOutput could note be opened." );
    }
/*
    FILE *bigSynth;
    bigSynth = fopen( "bigSynth.img", "wb" );

    if( !bigSynth )
	{
        fclose( bigSynth );
    }
    else
    {
    	unsigned int synthType;
    	makeSynthImage( bigSynth, 21120, 43200, synthType );
    	fclose( bigSynth );
    }
*/
    return;
}


bool FrmLoadImages::loadImage( ImageFile *image )
{
    bool retVal = false;

    QString fileName = QFileDialog::getOpenFileName( QString::null,
                                                     "Generic Binary Images (*.img)",
                                                     this,
                                                     "SelectImage",
                                                     "Select Image:" );

    if( (!fileName.isNull()) && (!fileName.isEmpty()) )
    {
    	FrmDimensions *dimensions = new FrmDimensions( this );

        dimensions->spinRows->setValue( 180 );
        dimensions->spinColumns->setValue( 360 );
        if( originalImage.dataType != "" )
        {
            dimensions->cmbDataType->setCurrentText( originalImage.dataType );
            dimensions->cmbDataType->setDisabled( true );
        }

    	if( dimensions->exec() )
    	{
    		image->fileName = fileName;
    		image->rows = dimensions->spinRows->value();
    		image->columns = dimensions->spinColumns->value();
            image->dataType = dimensions->cmbDataType->text( dimensions->cmbDataType->currentItem() );

            retVal = true;
    	}
    }


    return retVal;
}
