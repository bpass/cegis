// $Id: qgctpbox.h,v 1.6 2005/08/29 16:48:14 lwoodard Exp $


#ifndef QGCTPBOX_H
#define QGCTPBOX_H

#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QMap>

class QDmsEdit;

class QGctpBox : public QWidget 
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