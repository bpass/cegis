//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating .info file into an object to  //
//       query for name, datum, rows, cols, etc...     //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#include "projInfo.h"

#include "projName.h"
#include "projUnit.h"
#include "projDatum.h"
#include "proj15.h"

#include <qstring.h>
#include <qfile.h>

projInfo::projInfo()
{
     clearContents();
}

projInfo::projInfo( const QString fileName )
{
     clearContents();
     loadFromFile( fileName );
}

bool projInfo::loadFromFile( const QString fileName )
{
     bool    bReturnValue = false;
     int     paramCount = 0;
     QString currentLine = "";

     //initial pass to check if file has valid number of fields and spacing

     QFile   theFile( fileName );
     theFile.open( IO_ReadOnly | IO_Translate );

     while( !theFile.atEnd() )
     {
         if( theFile.readLine( currentLine, 1000 ) >= 0  )
         {
                if( paramCount == 0 )
                {
                    //check for errors in Row/Column line
                    if( currentLine.contains( ' ' ) == 1 )
                        bReturnValue = true;
                    else
                        bReturnValue = false;
                }
                else if( paramCount >= 1 && paramCount <= 5 )
                {
                    //check for errors in projection, zone, unit, datum, and pixel size line
                    if( currentLine.contains( ' ' ) == 0 )
                        bReturnValue = true;
                    else
                        bReturnValue = false;
                }
                else if( paramCount == 6 )
                {
                    //check for errors in pixel width and height line
                    if( currentLine.contains( ' ' ) == 1 )
                        bReturnValue = true;
                    else
                        bReturnValue = false;
                }
                else if( paramCount == 7 )
                {
                    //check for errors in 15 parameter line
                    if( currentLine.contains( ' ' ) == 14 || currentLine.contains( ' ' ) == 15 )
                        bReturnValue = true;
                    else
                        bReturnValue = false;
                }
         	paramCount++;
         }
     }
     theFile.close();

     theFile.open( IO_ReadOnly | IO_Translate );
     paramCount = 0;

     while( !theFile.atEnd() )
     {
         if( theFile.readLine( currentLine, 1000 ) >= 0  )
         {
                if( paramCount == 0 )
                    setRowColInfo( currentLine );
                else if( paramCount == 1 )
                    setProjNumber( currentLine );
                else if( paramCount == 3 )
                    setProjUnit( currentLine );
                else if( paramCount == 4 )
                    setProjDatum( currentLine );
                else if( paramCount == 7 )
                    setProjParams( currentLine );
                else if( paramCount == 8 )
                    setProjDataType( currentLine );

         	paramCount++;
         }
     }

     theFile.close();

     validate();

     return bReturnValue;
}


void projInfo::setRowColInfo( const QString rowcolInfo )
{
     m_rowcolInfo = rowcolInfo;
     setRows();
     setColumns();
     return;
}

void projInfo::setProjNumber( const int projNumber )
{ 
     m_projectionNumber = projNumber; 
     setProjectionName(); 
     return;
}

void projInfo::setProjNumber( const QString projNumber )
{ 
     setProjNumber( projNumber.toInt() );
     return;
}

void projInfo::setProjUnit( const int unitNumber )
{ 
     m_unitNumber = unitNumber; 
     setProjectionUnit(); 
     return;
}

void projInfo::setProjUnit( const QString unitNumber )
{
     setProjUnit( unitNumber.toInt() );
     return;
}

void projInfo::setProjDatum( const int datumNumber )
{
     m_datumNumber = datumNumber;
     setProjectionDatum();
     return;
}

void projInfo::setProjDatum( const QString datumNumber )
{
     setProjDatum( datumNumber.toInt() );
     return;
}

void projInfo::setProjDataType( const QString dataType )
{
 QString useType = dataType.simplifyWhiteSpace();
     if( ( useType == "Unsigned 8 Bit" ) || ( useType == "Signed 8 Bit" ) ||
         ( useType == "Unsigned 16 Bit" ) || ( useType == "Signed 16 Bit" ) ||
         ( useType == "Unsigned 32 Bit" ) || ( useType == "Signed 32 Bit" ) ||
         ( useType == "IEEE 32 Bit Float" ) || ( useType == "IEEE 64 Bit Float" ) )
         {
           m_projectionDataType = useType;
         }
     else
         m_projectionDataType = "Unknown";
     return;
}

void projInfo::setRows()
{
     m_rows = projRowCol::projectionRow( m_rowcolInfo );
     return;
}

void projInfo::setColumns()
{
     m_columns = projRowCol::projectionCol( m_rowcolInfo );
     return;
}

void projInfo::setProjectionName()
{
     projName m_name;
     m_projectionName = m_name.projectionName( m_projectionNumber );
     return;
}

void projInfo::setProjectionUnit()
{
     projUnit m_unit;
     m_projectionUnit = m_unit.projectionUnit( m_unitNumber );
     return;
}

void projInfo::setProjectionDatum()
{
     projDatum m_datum;
     m_projectionDatum = m_datum.projectionDatum( m_datumNumber );
     return;
}

void projInfo::setProjectionParameters()
{
     proj15 m_15( getProjNumber() );
     QString *m_pParam = m_15.getParameters();

     for( int index = 0; index < 15; index++ )
          m_projectionParameters[index] = m_pParam[index];

     m_projectionName = m_15.getTrueName();
     return;
}

void projInfo::clearContents()
{
     m_rowcolInfo = "";
     m_projectionNumber = -1;
     m_unitNumber = -1;
     m_datumNumber = -1;

     m_rows = -1;
     m_columns = -1;
     m_projectionName = "";
     m_projectionDatum = "";
     m_projectionUnit = "";
     m_projectionDataType = "Unknown";
     m_isValid = false;

     if( m_projectionParameters != NULL )
         for( int index = 0; index < 15; index++ )
              m_projectionParameters[index] = "";


     return;
}

void projInfo::setValid( const bool valid )
{
  m_isValid = valid;
  return;
}

bool projInfo::validate()
{
   if( ( getRows() > 0 ) &&
       ( getColumns() > 0 ) &&
       ( getProjNumber() >= 0 ) &&
       ( getProjNumber() <= maxProjNumber() ) &&
       ( getProjName() != "" ) &&
       ( getProjUnit() != "" ) &&
       ( getProjDatum() != "" ) )
        setValid( true );
  else
        setValid( false );

  return isValid();
}

void projInfo::setProjParams( const QString paramLine )
{
     int offset = 0;
     int index = 0;
     QString workingSet = paramLine;

     for( index = 0; index < 15; index++ )
          m_parameters[index] = 0.0;

     for( index = 0; index < 15; index++ )
     {
     	  offset = workingSet.find( ' ' );
     	  if( offset != -1 )
     	  {
               m_parameters[index] = workingSet.left( offset ).toDouble();
               workingSet = workingSet.right( workingSet.length() - offset - 1 );
          }
     }

     setProjectionParameters();
}
