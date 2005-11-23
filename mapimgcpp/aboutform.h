// $Id: aboutform.h,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


/****************************************************************************
** Form interface generated from reading ui file 'aboutform.ui'
**
** Created: Thu Jan 6 17:24:08 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;
class QPushButton;

/*
The aboutForm is simply a dialog to provide a disclaimer for any of our
mistakes, some contact info, and some credits for libraries used. It goes away
when the user clicks Ok.
*/
class aboutForm : public QDialog
{
   Q_OBJECT

public:
   aboutForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
   ~aboutForm(){}

   QLabel* logoLabel;
   QLabel* titleLabel;
   QFrame* descFrame;
   QLabel* descLabel1;
   QLabel* descLabel2;
   QLabel* descLabel3;
   QPushButton* okButton;

protected:
   QVBoxLayout* aboutFormLayout;
   QVBoxLayout* descFrameLayout;
   QHBoxLayout* okLayout;
   QSpacerItem* okLeft;
   QSpacerItem* okRight;

protected slots:
   virtual void languageChange();
};

#endif // ABOUTFORM_H
