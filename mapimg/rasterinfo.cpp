// $Id: rasterinfo.cpp,v 1.10 2005/03/15 17:34:42 jtrent Exp $


#include "rasterinfo.h"

#include <qmessagebox.h>
#include <qstringlist.h>
#include <qfile.h>

#include "rasterxml.h"
#include "gctpnames.h"

//Default constructor
// Nulls all private variables.
RasterInfo::RasterInfo()
{
   gctpParams = new double[15];

   defaults();
}

//Load constructor
// Loads the values found in the file whose name and path are 
// given in imgFileName.
RasterInfo::RasterInfo( QString &imgFileName )
{
   gctpParams = new double[15];

   if( !load( imgFileName ) )
      defaults();
}

//Copy constructor
//
RasterInfo::RasterInfo( const RasterInfo &src )
{
   gctpParams = new double[15];

   copy( src );
}

//Destructor
// Deletes all the dynamic memory
RasterInfo::~RasterInfo()
{
   delete [] gctpParams;
}

bool RasterInfo::setFileName( QString &imgFileName )
{
   fileName = imgFileName;

   return parseFileName();
}

bool RasterInfo::setAuthor( QString &name, QString &company, QString &email )
{
   aName = name;
   aCompany = company;
   aEmail = email;

   return true;
}

bool RasterInfo::setArea( double ul_X, double ul_Y, int rows, int cols )
{
   ulx = ul_X;
   uly = ul_Y;
   row = rows;
   col = cols;

   return (row > 0 && col > 0);
}

bool RasterInfo::setPixelDescription( const QString &dataType, double pixelSize, double fillValue, double noDataValue )
{
   return setDataType( dataType ) && setPixelSize( pixelSize ) && setFillValue( fillValue ) && setNoDataValue( noDataValue );
}

bool RasterInfo::setPixelDescription( bool isSigned, int bitsCount, const QString &type, double pixelSize, double fillValue, double noDataValue )
{
   return setDataType( isSigned, bitsCount, type ) && setPixelSize( pixelSize ) && setFillValue( fillValue ) && setNoDataValue( noDataValue );
}

bool RasterInfo::setDataType( const QString &dataType )
{
   //Handles "Unsigned/Signed 8/16/32 Bit Integer"
   //    and "Signed 32/64 Bit IEEE Float"

   if( dataType.contains( "Signed" ) > 0 )
      signd = true;
   else
      signd = false;

   if( dataType.contains( "64" ) > 0 )
      bits = 64;
   else if( dataType.contains( "16" ) > 0 )
      bits = 16;
   else if( dataType.contains( "32" ) > 0 )
      bits = 32;
   else //( dataType.contains( "8" ) > 0 )
      bits = 8;

   if( dataType.contains("IEEE", false) > 0 || dataType.contains("Float", false) > 0 )
   {
      datatype = "IEEE Float";
      signd = true;
   }
   else
      datatype = "Integer";

   return !dataType.isNull() && bits != 0;
}

bool RasterInfo::setDataType( bool isSigned, int bitsCount, const QString &type )
{
   signd = isSigned;

   if( bitsCount % 8 == 0 )
      bits = bitsCount;
   else
      bits = 8;

   if( type == "Float" || type == "IEEE Float" || type == "IEEE" )
      datatype = "IEEE Float";
   else
      datatype = "Integer";

   return bitsCount > 0 && !type.isNull();
}

bool RasterInfo::setPixelSize( double pixelSize )
{
   pixsize = pixelSize;

   return pixsize > 0;
}

bool RasterInfo::setFillValue( double fillValue )
{
   if( fillValue < 0 && !signd )
   {
      fillval = 0;
      return false;
   }

   fillval = fillValue;
   return true;
}

bool RasterInfo::setNoDataValue( double noDataValue )
{
   if( noDataValue < 0 && !signd )
   {
      noval = 0;
      return false;
   }

   noval = noDataValue;
   return true;
}

bool RasterInfo::setProjection( int projNumber, int zoneNumber, int datumNumber, int unitNumber )
{
   datumcode = datumNumber; unitcode = unitNumber;
   return setProjectionNumber( projNumber ) && setZoneNumber( zoneNumber );
}

bool RasterInfo::setProjectionNumber( int projNumber )
{
   if( projcode >= 0 && projcode < 31 )
       projcode = projNumber;

   return projcode == projNumber;
}

bool RasterInfo::setZoneNumber( int zoneNumber )
{
   zonecode = zoneNumber;

   if( projcode != 1 ||
      (zonecode > 60 && zonecode != 62) ||
      ( zonecode < -60 ) || ( zonecode == 0 ) )
      zonecode = 62;

   return zonecode == zoneNumber;
}

bool RasterInfo::setUnitNumber( int unitNumber )
{
   unitcode = unitNumber;
   


   return unitcode == unitNumber;
}

bool RasterInfo::setGctpParam(int i, double value)
{
   if( i >= 0 && i < 15 )
   {
      gctpParams[i] = value;
      return true;
   }

   return false;
}

bool RasterInfo::load()
{
   return load( fileName );
}

bool RasterInfo::save()
{
   return save( fileName );
}

bool RasterInfo::load( QString &imgFileName )
{
   if( !imgFileName.isNull() )
   {
      fileName = imgFileName;
      parseFileName();
   }


   if( QFile::exists( fileName + ".xml" ) )
   {
      return loadXml();
   }
   else if( QFile::exists( fileName + ".img.info" ) )
   {
      loadInfo();
      save();
      QFile::remove( fileName + ".img.info" );
   }
   else
   {
      return false;
   }

   return true;
}

bool RasterInfo::save( QString &imgFileName )
{
   bool returnValue = false;

   if( !imgFileName.isNull() )
   {
      fileName = imgFileName;
      parseFileName();
   }

   try
   {
     RasterXML r;

     r.setAuthorName( aName );
     r.setAuthorCompany( aCompany );
     r.setAuthorEmail( aEmail );

     r.setUlCorner( ulx, uly );
     r.setRows( row );
     r.setCols( col );

     r.setSigned( signd );
     r.setBits( bits );
     r.setDataType( datatype.ascii() );
     r.setPixelSize( pixsize );
     r.setFillValue( fillval );
     r.setNoDataValue( noval );

     r.setProjNumber( projcode );
     r.setProjName( projNames[projcode] );
     r.setZone( zonecode );
     r.setDatumNumber( datumcode );
     r.setDatumName( spheroidNames[datumcode] );
     r.setUnitsNumber( unitcode );
     r.setUnitsName( unitNames[unitcode] );

     char variation = 'a';
     if( projcode == 8 && gctpParams[8] == 1 )
        variation = 'b';
     if( ( projcode == 20 || projcode == 22 ) && gctpParams[12] == 1 )
        variation = 'b';
     QStringList paramNames( gctpNames(projcode, variation) );
     for( int i = 0; i < 15; ++i )
        r.setParam( i, gctpParams[i], nameMeanings(paramNames[i]) );

     returnValue =  r.save( QString( fileName + ".xml" ).ascii() );
   }
   catch( XMLException exception )
   {
     QMessageBox::critical( NULL, "Error", exception.getMessage() );
     returnValue = false;
   }

   return returnValue;
}


bool RasterInfo::parseFileName()
{
   fileName.stripWhiteSpace();

   if( fileName.right(4) == ".img" || fileName.right(4) == ".xml" )
      fileName.truncate( fileName.length() - 4 );

   else if( fileName.right(9) == ".img.info" )
      fileName.truncate( fileName.length() - 9 );

   return !fileName.isNull();
}

void RasterInfo::loadInfo()
{
   QFile *file = new QFile( fileName + ".img.info" );
   file->open( IO_ReadOnly );
   QStringList inFile( QStringList::split( '\n', QString(file->readAll()) ) );
   file->close();
   delete file;
   
   ////////Rows and Columns
   int breakPoint = inFile[0].find( ' ', 0, false );
   row = inFile[0].left( breakPoint ).toInt();
   col = inFile[0].right( inFile[0].length() - breakPoint - 1 ).toInt();
   
   projcode = inFile[1].toInt(); //Projection Number/Name
   zonecode = inFile[2].toInt(); //Zone Code
   unitcode = 2; //Unit Type  NOTE: mapimg currently only supports meters
   datumcode = 19; //Spheroid Name  NOTE: mapimg currently only supports Sphere
   pixsize = inFile[5].toDouble(); //Pixel Size

   ////////UL Latitude and Longitude
   breakPoint = inFile[6].find( ' ', 0, false );
   ulx = inFile[6].left( breakPoint ).toDouble();
   uly = inFile[6].right( inFile[6].length() - breakPoint - 1 ).toDouble();

   ////////15 GCTP Params
   QStringList gctpValues = QStringList::split( " ", inFile[7] );
   for( int i = 0; i < 15; ++i )
      gctpParams[i] = gctpValues[i].toDouble();

   setDataType( inFile[8] ); //Data Type
}

bool RasterInfo::loadXml()
{
   bool returnValue = false;

   try
   {
     RasterXML xml( QString( fileName + ".xml" ).ascii() );
     
     aName = xml.getAuthorName();
     aCompany = xml.getAuthorCompany();
     aEmail = xml.getAuthorEmail();
     
     ulx = xml.getUlx();
     uly = xml.getUly();
     row = xml.getRows();
     col = xml.getCols();
     
     signd = xml.isSigned();
     bits = xml.getBits();
     datatype = xml.getDataType();
     pixsize = xml.getPixelSize();
     fillval = xml.getFillValue();
     noval = xml.getNoDataValue();
    
     projcode = xml.getProjNumber();
     zonecode = xml.getZone();
     datumcode = xml.getDatumNumber();
     unitcode = xml.getUnitsNumber();
    
     for( int i = 0; i < 15; ++i )
        gctpParams[i] = xml.getGCTPParam( i );

     returnValue = true;
   }
   catch( XMLException exception )
   {
     QMessageBox::critical( NULL, "Error", exception.getMessage() );
     returnValue = false;
   }

   return returnValue;
}

bool RasterInfo::remove()
{
   QFile thisfile( fileName );
   if( thisfile.exists() )
      return thisfile.remove();

   return true;
}

bool RasterInfo::ready()
{
   if( !(row > 0 && col > 0) )
      return false;

   if( bits == 0 )
      return false;
   
   if( datatype.isNull() )
      return false;

   if( !(pixsize > 0) )
      return false;

   if( projcode < 0 || projcode > 30 )
      return false;
   if( zonecode <= 0 || ( zonecode > 60 && zonecode != 62 ) )
      return false;

   if( datumcode != 19 || unitcode != 2 )
      return false;

   return true;
}

bool RasterInfo::notReady()
{
   return !ready();
}

void RasterInfo::defaults()
{
   fileName = QString::null;

   aName = QString::null;
   aCompany = QString::null;
   aEmail = QString::null;

   ulx = -1.0;
   uly = -1.0;
   row = 0;
   col = 0;

   signd = false;
   bits = 0;
   datatype = QString::null;
   pixsize = -1.0;
   fillval = -1.0;
   noval = -1.0;

   projcode = -1;
   zonecode = 62;
   datumcode = 19;
   unitcode = 2;

   for( int i = 0; i < 15; ++i )
      gctpParams[i] = 0.0;
}

void RasterInfo::copy( const RasterInfo &src )
{
   fileName = src.fileName;

   aName = src.aName;
   aCompany = src.aCompany;
   aEmail = src.aEmail;

   ulx = src.ulx;
   uly = src.uly;
   row = src.row;
   col = src.col;

   signd = src.signd;
   bits = src.bits;
   datatype = src.datatype;
   pixsize = src.pixsize;
   fillval = src.fillval;
   noval = src.noval;

   projcode = src.projcode;
   zonecode = src.zonecode;
   datumcode = src.datumcode;
   unitcode = src.unitcode;

   for( int i = 0; i < 15; ++i )
      gctpParams[i] = src.gctpParams[i];
}

bool RasterInfo::fakeIt()
{
   datumcode = 19; unitcode = 2;

   if( datatype.isNull() )
   {
      signd = true;
      bits = 8;
      datatype = "Integer";
   }

   if( zonecode <= 0 || zonecode > 60 )
      zonecode = 62;

   return ready();
}
