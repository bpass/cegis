//Copyright 2003 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/////////////////////////////////////////////////////////
// Class for using projection number to obtain param   //
//       names. For use with projInfo .info parser     //
//                                                     //
// Created by: J. Trent - 07/2003                      //
/////////////////////////////////////////////////////////

#ifndef PROJ15_H
#define PROJ15_H

#include "projName.h"

#include <qstring.h>


class proj15
{
public:
   proj15( double* params = NULL);
   proj15( const int projNumber, double* params = NULL );
   proj15( const QString projectionName, double* params = NULL );
   ~proj15();

   QString* getParameters() { return m_paramNames; }
   QString  getTrueName() const { return m_trueName; }

private:
   QString lonZ( const int element );
   QString latZ( const int element );
   QString semiMajor( const int element );
   QString semiMinor( const int element );
   QString sphere( const int element );
   QString stdPAR();
   QString stdPR1();
   QString stdPR2();
   QString centMer();
   QString originLat();
   QString FE();
   QString FN();
   QString trueScale();
   QString longPol();
   QString factor();
   QString centLon();
   QString centerLat();
   QString height();
   QString long1();
   QString long2();
   QString lat1();
   QString lat2();
   QString aziAng();
   QString azmthPt();
   QString incAng();
   QString ascLong();
   QString psRev();
   QString lRat();
   QString pFlag( const int element );
   QString satNum();
   QString path( const int element );
   QString shapeM();
   QString shapeN();
   QString angle();

   void    setParamNames( const int projNumber );
   void    setParamNames( const QString projName );
   float   getParamValue( const int param );
   void    setParamName( const int param, const QString Name );
   void    setTrueName( const QString Name );
   void    setName( const QString Name );
   void    setNotUsed();
   void    setInvalid();
   void    clearContents();

   QString m_projName,
           m_trueName,
           m_paramNames[15];
   double  *m_paramValues;
};



#endif
