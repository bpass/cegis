// $Id: qdmsedit.h,v 1.5 2005/08/17 19:42:34 lwoodard Exp $


#ifndef QDMSEDIT_H
#define QDMSEDIT_H

#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class QDmsEdit : public QWidget
{
   Q_OBJECT
      Q_PROPERTY( int minVal READ minVal WRITE setMinVal )
      Q_PROPERTY( int maxVal READ maxVal WRITE setMaxVal )

public:
   enum Directionality { North, East, South, West, Unspecified };
   QDmsEdit( QWidget* parent = 0, const char* name = 0, Directionality direction = Unspecified );
   ~QDmsEdit(){};

   int minVal() const;
   void setMinVal( const int &newMin );
   int maxVal() const;
   void setMaxVal( const int &newMax );

   double value();
   void setValue( const double );
   void setValue( const int deg, const int min, const double sec );

   Directionality direction() const;
   void setDirection( const Directionality direction );

private:
   bool validate();

   int min;
   int max;

   QLabel *dmsLabel;
   QLineEdit *dEdit;
   QLabel *dLabel;
   QLineEdit *mEdit;
   QLabel *mLabel;
   QLineEdit *sEdit;
   QLabel *sLabel;
   QLabel *directionLabel;
};

#endif
