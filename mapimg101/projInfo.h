// $Id: projInfo.h,v 1.3 2005/01/11 17:11:28 jtrent Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for translating .info file into an object to  //
//       query for name, datum, rows, cols, etc...     //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////


#ifndef PROJINFO_H
#define PROJINFO_H

#include <qstring.h>
#include <qfile.h>

#include "projRowCol.h"
#include "projName.h"
//#include "projZone.h"   UTM Zones not yet implemented
#include "projUnit.h"
#include "projDatum.h"
#include "proj15.h"


class projInfo
{
public:
   projInfo();
   projInfo( const QString fileName );

   bool    loadFromFile( const QString fileName );
   bool    isValid() const { return m_isValid; }

   int     getRows() const { return m_rows; }
   int     getColumns() const { return m_columns; }
   int     getProjNumber() const { return m_projectionNumber; }
   int     maxProjNumber()  { projName m_name; return m_name.maxProjNumber(); }
   QString getProjName() const { return m_projectionName; }
   QString getProjUnit() const { return m_projectionUnit; }
   QString getProjDatum() const { return m_projectionDatum; }
   QString getProjDataType() const { return m_projectionDataType; }
   QString* getProjParameters() const { return (QString*)m_projectionParameters; }
   double* getProjParamValues() const { return (double*)m_parameters; }

   void    setRowColInfo( const QString rowcolInfo );
   void    setProjNumber( const int projNumber );
   void    setProjNumber( const QString projNumber );
   void    setProjUnit( const int unitNumber );
   void    setProjUnit( const QString unitNumber );
   void    setProjDatum( const int datumNumber );
   void    setProjDatum( const QString datumNumber );
   void    setProjParams( const QString paramLine );
   void    setProjDataType( const QString dataType );

   bool    validate();

private:
   void    setRows();
   void    setColumns();
   void    setProjectionName();
   void    setProjectionUnit();
   void    setProjectionDatum();
   void    setProjectionParameters();
   void    setValid( const bool valid );
   void    clearContents();

   QString m_rowcolInfo;
   int     m_projectionNumber,
           m_unitNumber,
           m_datumNumber;
   double  m_parameters[15];

   int     m_rows,
           m_columns;
   QString m_projectionName,
           m_projectionDatum,
           m_projectionUnit,
           m_projectionDataType,
           m_projectionParameters[15];
   bool    m_isValid;
};


#endif
