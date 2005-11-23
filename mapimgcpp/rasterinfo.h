// $Id: rasterinfo.h,v 1.1 2005/11/23 00:22:38 mswilliams Exp $


#ifndef RASTERINFO_H
#define RASTERINFO_H

#include <qstring.h>

class RasterInfo
{
public:
   //Constuctors and Destructor
   RasterInfo();
   RasterInfo( QString &imgFileName );
   RasterInfo( const RasterInfo &src );
   ~RasterInfo();

   //Files
   bool setFileName( QString &imgFileName );
   QString imgFileName() const {return fileName + ".img";}
   QString xmlFileName() const {return fileName + ".xml";}

   //Author
   bool setAuthor( QString &name, QString &company, QString &email );
   QString author() const {return aName;}
   QString company() const {return aCompany;}
   QString email() const {return aEmail;}

   //Area
   bool setArea( double ul_X, double ul_Y, int rows, int cols );
   double ul_X() const {return ulx;}
   double ul_Y() const {return uly;}
   int rows() const {return row;}
   int cols() const {return col;}

   //Pixel Description
   bool setPixelDescription( const QString &dataType, double pixelSize, double fillValue, double noDataValue );
   bool setPixelDescription( bool isSigned, int bitsCount, const QString &type, double pixelSize, double fillValue, double noDataValue );
   bool setDataType( const QString &dataType );
   bool setDataType( bool isSigned, int bitsCount, const QString &type );
   bool setPixelSize( double pixelSize );
   bool setFillValue( double fillValue );
   bool setNoDataValue( double noDataValue );
   QString dataType() const {return datatype;}
   bool isSigned() const {return signd;}
   int bitCount() const {return bits;}
   QString type() const {return datatype;}
   double pixelSize() const {return pixsize;}
   double fillValue() const {return fillval;}
   double noDataValue() const {return noval;}

   bool hasFillValue() const {return hasFillVal;}
   bool hasNoDataValue() const {return hasNoDataVal;}
   bool setHasFillValue( const bool& hasFill );
   bool setHasNoDataValue( const bool& hasNoData );

   //Projection
   bool setProjection( int projNumber, int zoneNumber = 62, int datumNumber = 19, int unitNumber = 2 );
   bool setProjectionNumber( int projNumber );
   bool setZoneNumber( int zoneNumber );
   bool setUnitNumber( int unitNumber );
   int projectionNumber() const {return projcode;}
   int zoneNumber() const {return zonecode;}
   int datumNumber() const {return datumcode;}
   int unitNumber() const {return unitcode;}

   //GCTP Parameters
   bool setGctpParam(int i, double value);
   double gctpParam(int i) const {return ((i>=0)&&(i<15))?gctpParams[i]:0.0;}
   double *allGctpParams() const {return gctpParams;}

   //I/O
   bool load();
   bool load( QString imgFileName );
   bool save( );
   bool save( QString imgFileName );
   bool remove();

   //Checks
   bool fakeIt();
   bool ready();
   bool notReady();
   
   void copy( const RasterInfo &src );

   double*  gctpParams;   
private:
   void defaults();
   bool parseFileName();
   void loadInfo();
   bool loadXml();

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
   double   noval;
   
   bool     hasFillVal;
   bool     hasNoDataVal;

   long     projcode;
   long     zonecode;
   long     datumcode;
   long     unitcode;

};

#endif//RASTERINFO_H
