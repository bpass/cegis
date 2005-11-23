// $Id: qimgpainter.h,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


#ifndef QIMGPAINTER_H
#define QIMGPAINTER_H

/****************************************************************************
** Programmer: Robert Buehler
** File: qImgPainter.h
** Date: 6/29/04
**
** Class definition of the qImgPainter.
** Pass the qImgPainter a file and it will be ready to return a pixmap
** of a requested size.
*****************************************************************************/

#include <qstring.h>
#include <qpixmap.h>
#include <qmap.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qfile.h>
#include <qdatastream.h>

#include "mapimgprogressdialog.h"
#include "rasterinfo.h"

class QImgPainter
{
public:
   //Loads the imgFile
   QImgPainter();
   ~QImgPainter();
   
   void setFile(QString imgFile);

   //returns a pixmap of default size unless otherwise specified
   QPixmap getSample(QSize sampleSize);
   
private:
   void callClear();
   template <class Type>
         void clearColorTable(Type data);
   
   //Create the color table and save it in the void * vColorTable
   void callConstruct(Q_ULLONG offset);
   template <class Type>
         void constructColorTable(Type data, Q_ULLONG offset);
   
   //Draw the pixmap to be returned by getPixmap
   void callPaint();
   template <class Type>
         void paintPixmap(Type data);
   
   QString fileName;
   ulong rows, cols, size;
   QString dataType;
   
   bool fileSet;
   QFile file;
   QDataStream dataStream;
   void * vColorTable;
   QPixmap pixmap;
   double lastRatio;
   bool optimized;
   bool tableAbort;
};


//returns whether the file was set and 
//color table was generated without an abort
QImgPainter::QImgPainter()
{
   vColorTable = NULL;
   lastRatio = 0;
   fileSet = false;
}


////////CALL TEMPLATE FUNCTION TO clearColorTable
QImgPainter::~QImgPainter()
{
   callClear();
}


void QImgPainter::setFile(QString imgFile)
{
   optimized = false;
   callClear();
   lastRatio = 0;
   fileSet = false;
   RasterInfo info;
   if(!info.load( imgFile ))
      return;
   
   fileName = imgFile;
   file.setName( fileName ); 
   
   rows = info.rows();
   cols = info.cols();
   dataType = info.isSigned()?"Signed ":"Unsigned ";
   dataType += QString::number(info.bitCount());
   dataType += " Bit ";
   dataType += info.dataType();

   size = rows * cols;
   pixmap.resize(cols, rows);
   
   if(file.size() < 10000000)
      callConstruct(1);
   else
      callConstruct(0);
   
   fileSet = true;
   return;
}


QPixmap QImgPainter::getSample(QSize sampleSize)
{
   if(!fileSet)
      return QPixmap(2,1);
   
   pixmap.resize(sampleSize);
   callPaint();      
   return pixmap;
}


void QImgPainter::callClear()
{
   if(!vColorTable)
      return;
   
   if( dataType == "Signed 64 Bit IEEE Float" )
   {	double data = 0;
      clearColorTable(data);	}
   else if( dataType == "Signed 32 Bit IEEE Float" )
   {	float data = 0;
      clearColorTable(data);	}
   else if( dataType == "Signed 32 Bit Integer" )
   {	Q_INT32 data = 0;
      clearColorTable(data);	}
   else if( dataType == "Unsigned 32 Bit Integer" )
   {	Q_UINT32 data = 0;
      clearColorTable(data);	}
   else if( dataType == "Signed 16 Bit Integer" )
   {	Q_INT16 data = 0;
      clearColorTable(data);	}
   else if( dataType == "Unsigned 16 Bit Integer" )
   {	Q_UINT16 data = 0;
      clearColorTable(data);	}
   else if( dataType == "Signed 8 Bit Integer" )
   { 	Q_INT8 data = 0;
      clearColorTable(data);	}
   else //(dataType == "Unsigned 8 Bit Integer")
   {  Q_UINT8 data = 0;
      clearColorTable(data);	}
}

template <class Type>
      void QImgPainter::clearColorTable( Type )
{
   QMap<Type, QColor> *colorTable =
         static_cast<QMap<Type, QColor> *>( vColorTable );
   delete colorTable;
   vColorTable = NULL;
}


////////CALL TEMPLATE FUNCTION TO constructColorTable
void QImgPainter::callConstruct(Q_ULLONG offset)
{
   if( dataType == "Signed 64 Bit IEEE Float" )
   {	double data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Signed 32 Bit IEEE Float" )
   {	float data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Signed 32 Bit Integer" )
   {	Q_INT32 data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Unsigned 32 Bit Integer" )
   {	Q_UINT32 data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Signed 16 Bit Integer" )
   {	Q_INT16 data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Unsigned 16 Bit Integer" )
   {	Q_UINT16 data = 0;
      constructColorTable(data, offset);	}
   else if( dataType == "Signed 8 Bit Integer" )
   { 	Q_INT8 data = 0;
      constructColorTable(data, offset);	}
   else //( dataType == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      constructColorTable(data, offset);	}
}


//The colorTable is a QMap<type, QColor>
//				(For every value of "Type" there is one QColor)
//It only stores a value read if that value hasn't been read before
//Once all the values are read it applies a grayscale gradient
//across all of the values with black being the lowest value
template <class Type>
      void QImgPainter::constructColorTable(Type data, Q_ULLONG offset)
{
   tableAbort = false;
   if(vColorTable)
      callClear();
   QMap<Type, QColor> * colorTable = new QMap<Type, QColor>;
   if(offset == 0)
   {
      optimized = true;
      vColorTable = colorTable;
      return;
   }
   
   ////////LOAD IN COLOR TABLE ENTRIES
   MapimgProgressDialog pd( "Reading data entries...",
                      "Abort", pixmap.height(), NULL, NULL,
                      0, "progress", TRUE);
   pd.setMinimumDuration(500);
   Q_ULLONG yOffSet, xOffSet;
   int dataBytes = sizeof(data);
   file.open(IO_ReadOnly);
   dataStream.setDevice(&file);
   for( int y = 0; y < pixmap.height(); y++ )
   {
      if(pd.wasCanceled())
      {
         file.close();
         tableAbort = true;
         pd.setProgress(pd.totalSteps());
         return;
      }
      pd.setProgress(y);
      yOffSet = static_cast<Q_ULLONG>(offset * y) * cols * dataBytes;
      for( int x = 0; x < pixmap.width(); x++ )
      {
         xOffSet = static_cast<Q_ULLONG>(offset * x) * dataBytes;
         file.at(yOffSet + xOffSet);
         dataStream >> data;
         colorTable->insert(data, QColor(0,0,0), false);
      }
   }
   file.close();
   
   ////////APPLY GRADIENT ACCROSS COLORTABLE 
   double f = 0;
   double colorFactor = 0;
   if(colorTable->count() > 1)
      colorFactor = static_cast<double>(255) / (colorTable->count() - 1);
   pd.setProgress(0);
   pd.setTotalSteps(255);
   pd.setLabelText("Calculating color table...");
   typename QMap<Type, QColor>::Iterator it;
   for(it = colorTable->begin(); it != colorTable->end(); ++it)
   {
      if(static_cast<int>(f*10000)%100 == 1)
      {
         if(pd.wasCanceled())
         {
            file.close();
            tableAbort = true;
            pd.setProgress(pd.totalSteps());
            return;
         }
         pd.setProgress(static_cast<int>(f));
      }
      it.data() = QColor((int)f, (int)f, (int)f);
      f += colorFactor;
   }
   vColorTable = colorTable;
   
   pd.setProgress(pd.totalSteps()); //finish off the progress bar
}


////////CALL TEMPLATE FUNCTION TO paintPixmap
void QImgPainter::callPaint()
{
   if( dataType == "Signed 64 Bit IEEE Float" )
   {	double data = 0;
      paintPixmap(data);	}
   else if( dataType == "Signed 32 Bit IEEE Float" )
   {	float data = 0;
      paintPixmap(data);	}
   else if( dataType == "Signed 32 Bit Integer" )
   {	Q_INT32 data = 0;
      paintPixmap(data);	}
   else if( dataType == "Unsigned 32 Bit Integer" )
   {	Q_UINT32 data = 0;
      paintPixmap(data);	}
   else if( dataType == "Signed 16 Bit Integer" )
   {	Q_INT16 data = 0;
      paintPixmap(data);	}
   else if( dataType == "Unsigned 16 Bit Integer" )
   {	Q_UINT16 data = 0;
      paintPixmap(data);	}
   else if( dataType == "Signed 8 Bit Integer" )
   { 	Q_INT8 data = 0;
      paintPixmap(data);	}
   else //( dataType == "Unsigned 8 Bit Integer" )
   {	Q_UINT8 data = 0;
      paintPixmap(data);	}
}


//The QFile is resampled using a nearest neighbor algorithm
//		using the QFile::at() function to offset the QDataStream
template <class Type>
      void QImgPainter::paintPixmap(Type data)
{ 
   ////////RESIZE PIXMAP
   //Resize pixmap to maintain aspect ratio and fit within requested dimensions
   double pixRatio = static_cast<double>( pixmap.height() ) /
                    static_cast<double>( pixmap.width() );
   double fileRatio = static_cast<double>( rows ) /
                     static_cast<double>( cols );
   if( pixRatio > fileRatio )
      pixmap.resize(pixmap.width(), 
                    static_cast<int>(pixmap.width() * fileRatio));
   else if( pixRatio < fileRatio)
      pixmap.resize(static_cast<int>(pixmap.height() / fileRatio),
                    pixmap.height());
   double file_pixRatio = static_cast<double>( rows ) / 
                          static_cast<double>( pixmap.height() );
   
   //file_pixRatio < 1 means that the pixmap is bigger than the file.
   //resampling at less than 1 would result in wasteful pixel doubling
   if(file_pixRatio < 1)
   {
      file_pixRatio = 1;
      pixmap.resize(cols, rows);
   }
   //file_pixRatio == lastRatio means that pixmap is already resampled
   //at the requested size so leave it alone
   if(file_pixRatio == lastRatio)
      return;
   
   lastRatio = file_pixRatio;
   if(optimized)
      callConstruct(static_cast<Q_ULLONG>(file_pixRatio));
   
   if(tableAbort)
   {
      pixmap.fill(Qt::black);
      lastRatio = 0;
      return;
   }
   
   ////////PAINT THE PIXMAP
   //for each pixel in the pixmap
   //	 - read in the corresponding value from the file
   //	 - use the colorTable to lookup what color to use
   //	 - paint the pixel
   //INPUT_COLOR and IMGCOLOR are defined in mapimgpalette.h
   QMap<Type, QColor> *colorTable =
         static_cast<QMap<Type, QColor> *>( vColorTable );
   MapimgProgressDialog pd( "Generating preview...",
                      "Abort", pixmap.height(), &INPUT_COLOR, &IMGCOLOR, 0, "progress", TRUE);
   pd.setMinimumDuration(500);
   Q_ULLONG yOffSet, xOffSet;
   QPainter painter( &pixmap );
   painter.setWindow( 0, 0, pixmap.width(), pixmap.height() );
   int dataBytes = sizeof(data);
   file.open(IO_ReadOnly);
   dataStream.setDevice(&file);
   for( int y = 0; y < pixmap.height(); y++ )
   {
      if(pd.wasCanceled())
      {
         file.close();
         pixmap.fill(Qt::black);
         lastRatio = 0;
         pd.setProgress(pd.totalSteps());
         return;
      }
      pd.setProgress(y);
      yOffSet = static_cast<Q_ULLONG>(file_pixRatio * y) * cols * dataBytes;
      for( int x = 0; x < pixmap.width(); x++ )
      {
         xOffSet = static_cast<Q_ULLONG>(file_pixRatio * x) * dataBytes;
         file.at(yOffSet + xOffSet);
         dataStream >> data;
         painter.setPen( QPen( colorTable->find(data).data() ) );
         painter.drawPoint(x,y);
      }
   }
   file.close();
   
   pd.setProgress(pd.totalSteps());
   
}

#endif
