// $Id: mapimgvalidator.h,v 1.7 2005/02/23 18:23:27 jtrent Exp $

#ifndef MAPIMGVALIDATOR_H
#define MAPIMGVALIDATOR_H

#include <qobject.h>
#include <qvalidator.h>
#include <qglobal.h>

#define Q_UINT8_MAX      0xFF
#define Q_UINT8_MIN      0

#define Q_INT8_MAX         127
#define Q_INT8_MIN       (-Q_INT8_MAX -1)

#define Q_UINT16_MAX      0xFFFF
#define Q_UINT16_MIN      0

#define Q_INT16_MAX         32767
#define Q_INT16_MIN       (-Q_INT16_MAX -1)

#define Q_UINT32_MAX      0xFFFFFFFF
#define Q_UINT32_MIN      0

#define Q_INT32_MAX         2147483647
#define Q_INT32_MIN       (-Q_INT32_MAX - 1)

#define Q_UINT64_MAX      0xFFFFFFFFFFFFFFFF
#define Q_UINT64_MIN      0

#if defined (Q_OS_WIN32) || defined (Q_OS_WIN64)
       #define Q_INT64_MAX         9223372036854775807i64
       #define Q_INT64_MIN       (-Q_INT64_MAX - 1i64)
#else
       #define Q_INT64_MAX         9223372036854775807LL
       #define Q_INT64_MIN       (-Q_INT64_MAX - 1LL)
#endif

#define FLOAT32_MAX         3.402823466e+38F
#define FLOAT32_MIN         1.175494351e-38F

#define FLOAT64_MAX         1.7976931348623158e+308
#define FLOAT64_MIN         2.2250738585072014e-308


class QString;


class MapimgValidator : public QValidator
{
   Q_OBJECT
      Q_PROPERTY( double bottom READ bottom WRITE setBottom )
      Q_PROPERTY( double top READ top WRITE setTop )
      Q_PROPERTY( int decimals READ decimals WRITE setDecimals )

public:
   MapimgValidator( QObject* parent, const char* name = 0 );
   MapimgValidator( QString mapimgDataType, QObject* parent, const char* name = 0 );
   MapimgValidator( double bottom, double top, int decimals, QObject* parent, const char* name = 0 );
   ~MapimgValidator();

   QValidator::State validate( QString & input, int & ) const;
   void fixup( QString& input ) const;

   virtual void setRange( double bottom, double top, int decimals = 0 );
   void setDataType( QString mapimgDataType );
   void setBottom( double );
   void setTop( double );
   void setDecimals( int );

   double bottom() const { return b; }
   double top() const { return t; }
   int decimals() const { return d; }

private:
#if defined(Q_DISABLE_COPY)
   MapimgValidator( const MapimgValidator & );
   MapimgValidator& operator=( const MapimgValidator & );
#endif

   double b, t;
   int d;
};


#endif //MAPIMGVALIDATOR_H


