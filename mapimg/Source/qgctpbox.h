// $Id: qgctpbox.h,v 1.2 2005/08/05 16:02:00 lwoodard Exp $


#ifndef QGCTPBOX_H
#define QGCTPBOX_H

#include <q3vbox.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qmap.h>

class QDmsEdit;

class QGctpBox : public Q3VBox
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
   QString     name;

   static QMap<QString,double> *holdValues;        
};

#endif
