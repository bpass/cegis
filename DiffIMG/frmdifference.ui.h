/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qmessagebox.h>
#include <qimage.h>
#include <qpainter.h>
#include <qfiledialog.h>

#include "imagefile.h"
#include "compare.h"


void FrmDifference::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(
                            QString::null, "Images (*.bmp)",
                            this, "save difference",
                            "File for Difference" );
    if( !fileName.isNull() )
    {
        printf( "%i\n", pixDifference->pixmap()->save( fileName, "BMP", 100 ) ); fflush(stdout);
        printf( "%i\n", pixDifference->pixmap()->save( fileName + ".jpg", "JPEG", 100 ) ); fflush(stdout);
    }
}


void FrmDifference::init()
{

}


void FrmDifference::paint( int w, int h, QString dataType )
{
    if( dataType == "Unsigned 8 Bit" )
    {

        UINT8 ucType = 0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, ucType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, ucType ) );
    }
    else if( dataType == "Signed 8 Bit" )
    {
        INT8 scType = '0';
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, scType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, scType ) );
    }
    else if( dataType == "Unsigned 16 Bit" )
    {
    	unsigned short usType = 0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, usType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, usType ) );
    }
    else if( dataType == "Signed 16 Bit" )
    {
    	short sType = 0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, sType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, sType  ) );
    }
    else if( dataType == "Unsigned 32 Bit" )
    {
    	unsigned int uiType = 0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, uiType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, uiType ) );
    }
    else if( dataType == "Signed 32 Bit" )
    {
    	int iType = 0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, iType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, iType ) );
    }
    else if( dataType == "IEEE 32 Bit Float" )
    {
    	float fType = 0.0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, fType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, fType ) );
    }
    else if( dataType == "IEEE 64 Bit Float" )
    {
    	double dType = 0.0;
        pixDifference->setPixmap( *generateDiff( "test.txt", w, h, dType ) );
        pixHisto->setPixmap( *generateHisto( "test.txt", w, h, dType ) );
    }
    else
    {
    	QMessageBox::critical( this, "Error", "Invalid Data Type Selection." );
        return;
    }


    lblDifference->setText( QString( "Image: None   rows: %1  columns: %2  %3")
                            .arg( h )
                            .arg( w )
                            .arg( dataType ));

    return;
}


void FrmDifference::saveHisto()
{
    QString fileName = QFileDialog::getSaveFileName(
                            QString::null, "Images (*.bmp)",
                            this, "save histogram",
                            "File for Histogram" );
    if( !fileName.isNull() )
    {
       printf( "%i\n", pixHisto->pixmap()->save( fileName, "BMP", 100 ) ); fflush(stdout);
       printf( "%i\n", pixHisto->pixmap()->save( fileName + ".jpg", "JPEG", 100 ) ); fflush(stdout);
    }
}
