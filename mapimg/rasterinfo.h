// $Id: rasterinfo.h,v 1.1 2005/01/14 16:18:50 rbuehler Exp $


#ifndef RASTERINFO_H
#define RASTERINFO_H

#include <qstring.h>

class RasterInfo
{
public:
   //Constuctors and Destructor
   RasterInfo();
   RasterInfo( QString &imgFileName );
   RasterInfo( RasterInfo &src );
   ~RasterInfo();

   //Files
   bool setFileName( QString &imgFileName );
   QString imgFileName(){return fileName + ".img";}
   QString xmlFileName(){return fileName + ".xml";}

   //Author
   bool setAuthor( QString &name, QString &company, QString &email );
   QString author(){return aName;}
   QString company(){return aCompany;}
   QString email(){return aEmail;}

   //Area
   bool setArea( double ul_X, double ul_Y, int rows, int cols );
   double ul_X(){return ulx;}
   double ul_Y(){return uly;}
   int rows(){return row;}
   int cols(){return col;}

   //Pixel Description
   bool setPixelDescription( QString &dataType, double pixelSize, double fillValue );
   bool setPixelDescription( bool isSigned, int bitsCount, QString &type, double pixelSize, double fillValue );
   bool setDataType( QString &dataType );
   bool setDataType( bool isSigned, int bitsCount, QString &type );
   bool setPixelSize( double pixelSize );
   bool setFillValue( double fillValue );
   QString dataType(){return datatype;}
   bool isSigned(){return signd;}
   int bitCount(){return bits;}
   QString type(){return datatype;}
   double pixelSize(){return pixsize;}
   double fillValue(){return fillval;}

   //Projection
   bool setProjection( int projNumber, int zoneNumber = 62, int datumNumber = 19, int unitNumber = 2 );
   bool setProjectionNumber( int projNumber );
   bool setZoneNumber( int zoneNumber );
   int projectionNumber(){return projcode;}
   int zoneNumber(){return zonecode;}
   int datumNumber(){return datumcode;}
   int unitNumber(){return unitcode;}

   //GCTP Parameters
   bool setGctpParam(int i, double value);
   double gctpParam(int i){return ((i>=0)&&(i<15))?gctpParams[i]:0.0;}
   double *allGctpParams(){return gctpParams;}

   //I/O
   bool load( QString &imgFileName = QString::null );
   bool save( QString &imgFileName = QString::null );
   bool remove();

   //Checks
   bool fakeIt();
   bool ready();
   bool notReady();
   
private:
   void defaults();
   void copy( RasterInfo &src );
   bool parseFileName();
   void loadInfo();
   void loadXml();

   QString  fileName;

   QString  aName;
   QString  aCompany;
   QString  aEmail;

   double   ulx;
   double   uly;
   int      row;
   int      col;

   bool     signd;
   int      bits;
   QString  datatype;
   double   pixsize;
   double   fillval;

   long     projcode;
   long     zonecode;
   long     datumcode;
   long     unitcode;

   double*  gctpParams;   
};

#endif//RASTERINFO_H