// $Id: mapimgvalidator.h,v 1.4 2005/09/28 20:24:28 lwoodard Exp $

#ifndef MAPIMGVALIDATOR_H
#define MAPIMGVALIDATOR_H

#include <QObject>
#include <QValidator>

#include <qglobal.h>

#include "mapimgdatatypes.h"


class QString;


class MapimgValidator : public QValidator
{
   Q_OBJECT
      Q_PROPERTY( double bottom READ bottom WRITE setBottom )
      Q_PROPERTY( double top READ top WRITE setTop )
      Q_PROPERTY( int decimals READ decimals WRITE setDecimals )

public:
   MapimgValidator( QObject* parent );
   MapimgValidator( QString mapimgDataType, QObject* parent );
   MapimgValidator( QString mapimgDataType, bool allowUndefined, QObject* parent );
   MapimgValidator( double bottom, double top, int decimals, QObject* parent );
   MapimgValidator( double bottom, double top, int decimals, bool allowUndefined, QObject* parent = 0 );
   ~MapimgValidator();

   QValidator::State validate( QString & input, int & ) const;
   void fixup( QString& input ) const;

   virtual void setRange( double bottom, double top, int decimals = 0 );
   void setDataType( QString mapimgDataType, bool quiet = false );
   void setBottom( double bottom );
   void setTop( double top );
   void setDecimals( int decimals );
   void setAllowUndefined( bool allow );

   double bottom() const { return m_bottom; }
   double top() const { return m_top; }
   int decimals() const { return m_decimals; }
   bool allowUndefined() const { return m_allowUndefined; }

private:
#if defined(Q_DISABLE_COPY)
   MapimgValidator( const MapimgValidator & );
   MapimgValidator& operator=( const MapimgValidator & );
#endif

   double m_bottom, m_top;
   int m_decimals;
   bool m_allowUndefined;
};


#endif //MAPIMGVALIDATOR_H


