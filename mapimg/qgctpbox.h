// $Id: qgctpbox.h,v 1.5 2005/04/08 21:51:30 rbuehler Exp $


#ifndef QGCTPBOX_H
#define QGCTPBOX_H

#include <qvbox.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qmap.h>

class QDmsEdit;

class QGctpBox : public QVBox
{
   Q_OBJECT

public:
   QGctpBox( QWidget* parent = 0, const char* name = 0 );
   ~QGctpBox();

   void setGctpName( const QString & );
   QString output();

   double value();
   void setValue( const double );

public slots:
   void setDisabled( bool disable );

private:
   void disappear();
   void initializeHoldValues();

   QLabel      *label;

   QLineEdit   *lineEdit;
   QSpinBox    *spinBox;
   QDmsEdit    *dmsEdit;

   QWidget     *activeEdit;

   static QMap<QString,double> holdValues;        
};

#endif
