// $Id: qdmsedit.h,v 1.1 2005/01/14 16:18:50 rbuehler Exp $


#ifndef QDMSEDIT_H
#define QDMSEDIT_H

#include <qhbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>

class QDmsEdit : public QHBox
{
   Q_OBJECT
   Q_PROPERTY( int minVal READ minVal WRITE setMinVal )
   Q_PROPERTY( int maxVal READ maxVal WRITE setMaxVal )
   
public:
   QDmsEdit( QWidget* parent = 0, const char* name = 0);
   ~QDmsEdit(){};
   
   int minVal() const;
   void setMinVal( const int &newMin );
   int maxVal() const;
   void setMaxVal( const int &newMax );

   double value();
   void setValue( const double );
   void setValue( const int deg, const int min, const double sec );
   
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
};
         
#endif
