#ifndef IMAGEFILE_H
#define IMAGEFILE_H

#include <iostream>

#include <list>
using namespace std;

#include <qstring.h>
#include <qmap.h>
#include <qcolor.h>

struct ImageFile
{
    FILE    *file;
    QString fileName,
            dataType;
    int     rows,
            columns;
};

template <class T>
list<T> loadImageLine( ImageFile fileInfo, int lineNumber, T typeToUse )
{
    list<T> retList;

    if( !fileInfo.file )
	{
        fclose( fileInfo.file );

  	    QMessageBox::critical( 0,
                               "Error",
	                           "An internal error occurred while trying to open "
                               + fileInfo.fileName );
	    return retList;
    }
    if( (lineNumber >= fileInfo.rows) || fseek( fileInfo.file, sizeof(T)*lineNumber*fileInfo.columns, SEEK_SET ) )
    {
        fclose( fileInfo.file );

  	    QMessageBox::critical( 0,
                               "Error",
	                           QString("Line number %1 out of range.\n"
	                           "Line should be between 0 and %2.")
	                           .arg(lineNumber)
                               .arg(fileInfo.rows) );
	    return retList;
    }
    void* element = malloc(fileInfo.columns*sizeof(T));

    fread(element, sizeof(T), fileInfo.columns, fileInfo.file);

    retList.clear();
    for( int index = 0; index < fileInfo.columns; index++ )
    {
          retList.push_back( *((T*)element + index));
    }

    delete element;

    return retList;
}

//file MUST be opened wb
template <class T>
bool saveImageLine( FILE *file, list<T> imageLine )
{
    if( !file )
	{
  	    QMessageBox::critical( 0,
                               "Error",
	                           "An internal error occurred while trying to write to file." );
	    return false;
    }

    list<T>::const_iterator listIter;

    for( listIter = imageLine.begin(); listIter!=imageLine.end(); listIter++ )
    {
         T element = (*listIter);
    	 fwrite( &element, sizeof(T), 1, file);
    }

    return true;
}

template <class T>
bool makeSynthImage( FILE *file, int rows, int columns, T typeToUse )
{
    if( !file )
	{
  	    QMessageBox::critical( 0,
                               "Error",
	                           "An internal error occurred while trying to write to file." );
	    return false;
    }

    T index = 0;
    
    T *data = NULL;
    data = new T[columns];

    for( T rowOffset = 0; rowOffset < rows; rowOffset++ )
    {
        for( T columnOffset = 0; columnOffset < columns; columnOffset++ )
        {
    	    data[columnOffset] = index;
    	    index++;
        }

        fwrite( data, sizeof(T), columns, file);
    }

    delete []data;

    return true;
}

template <class T>
bool compareImages( ImageFile originalImage, ImageFile reprojectedImage, QString outputName, T typeToUse )
{
    int diffCount = 0;

    list<T> originalList;
    list<T> reprojectedList;
    list<T> diffList;
    list<T>::const_iterator origIter;
    list<T>::const_iterator reproIter;
    list<T>::const_iterator diffIter;


    FILE *file;

    file = fopen( outputName.ascii(), "wb" );

    if( !file || !originalImage.file || !reprojectedImage.file )
	{
        fclose( file );
        return false;
    }

    for( int index = 0;
         (index<originalImage.rows) && (index<reprojectedImage.rows);
         index++ )
    {
         originalList.clear();
         reprojectedList.clear();

         originalList = loadImageLine( originalImage, index, typeToUse );
         reprojectedList = loadImageLine( reprojectedImage, index, typeToUse );

         diffCount = compareDataAbs( originalList, reprojectedList, diffList );

#ifdef _JT_DEBUG

         origIter = originalList.begin();
         reproIter = reprojectedList.begin();
         diffIter = diffList.begin();

         for( ; (origIter!=originalList.end()) && (reproIter!=reprojectedList.end()) && (diffIter!=diffList.end()); )
         {
              cout << QString::number((*origIter)).ascii() << "  - "
                   << QString::number((*reproIter)).ascii() << "  = "
                   << QString::number((*diffIter)).ascii() << endl << flush;

              origIter++;
              reproIter++;
              diffIter++;
         }


         cout << "Line:  " << index
              << " - Number of Differences: " << diffCount
              << endl << flush;
#endif
         printf( "Line:  %i - Number of Differences: %i\n", index, diffCount );         fflush(stdout);

         saveImageLine( file, diffList );
    }

    int size = diffList.size();
   	diffList.clear();
    for( index = 0; index < size; index++ )
    {
    	diffList.push_back( 1 );
    }

    for( int rowCount = 0; rowCount < abs( originalImage.rows - reprojectedImage.rows ); rowCount++ )
    {
         saveImageLine( file, diffList );
    }

    fclose( file );
    
    return true;
}

template <class T>
QPixmap* generateDiff( QString fileName, int w, int h, T typeToUse )
{
    list<T> diffList;
    list<T>::const_iterator diffIter;

    int diff_x = 0,
        diff_y = 0;

    QColor identical = Qt::white,
           different = Qt::black,
           tempColor = Qt::blue;

    ImageFile diffImage;
    diffImage.fileName = fileName;
    diffImage.rows = h;
    diffImage.columns = w;

    QPixmap *diffPix = new QPixmap( diffImage.columns, diffImage.rows );
    diffPix->fill( different );

    QPainter diff;
    diff.begin( diffPix );

    diffImage.file = fopen( diffImage.fileName.ascii(), "rb" );

    for( int index = 0; index<diffImage.rows; index++ )
    {
         diffList.clear();
         diffList = loadImageLine( diffImage, index, typeToUse );

         for( diffIter = diffList.begin(); diffIter!=diffList.end(); diffIter++ )
         {
              if( (*diffIter) == 0 )
              {
              	 tempColor = identical;
              }
              else
              {
                 tempColor = different;
              }

              diff.setPen( tempColor );
              diff.setBrush( tempColor );
              diff.drawPoint( diff_x, diff_y );
              diff_x++;
         }
         diff_x = 0;
         diff_y++;
         printf ( "Line %i\n", diff_y ); fflush( stdout );
    }

    fclose( diffImage.file );
    diff.end();

    return diffPix;
}

template <class T>
QPixmap* generateHisto( QString fileName, int w, int h, T typeToUse )
{
    list<T> histoList;
    list<T>::const_iterator histoIter;

    int histo_x = 0,
        histo_y = 0;

    QColor identical = Qt::white,
           different = Qt::black,
           tempColor = Qt::blue;

    ImageFile histoImage;
    histoImage.fileName = fileName;
    histoImage.rows = h;
    histoImage.columns = w;

    QPixmap *histoPix = new QPixmap( histoImage.columns, histoImage.rows );
    histoPix->fill( identical );

    QPainter histo;
    histo.begin( histoPix );
    histo.setPen( different );
    histo.setBrush( different );

    histoImage.file = fopen( histoImage.fileName.ascii(), "rb" );

    for( int index = 0; index<histoImage.rows; index++ )
    {
         histoList.clear();
         histoList = loadImageLine( histoImage, index, typeToUse );

         for( histoIter = histoList.begin(); histoIter!=histoList.end(); histoIter++ )
         {
              if( (*histoIter) != 0 )
              {
                 histo.drawPoint( histo_x, histo_y );
                 histo_x++;
              }
         }
         histo_x = 0;
         histo_y++;
         printf ( "Line %i\n", histo_y ); fflush( stdout );
    }
    fclose( histoImage.file );

    histo.end();
    return histoPix;
}

#endif
