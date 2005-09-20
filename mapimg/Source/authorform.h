// $Id: authorform.h,v 1.6 2005/09/28 20:24:28 lwoodard Exp $

//Edited by:lwoodard	date:August 2005	for:qt3 to qt4 porting

/****************************************************************************
** Form interface generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef AUTHORFORM_H
#define AUTHORFORM_H

#include <QVariant>
#include <QDialog>
#include <QSettings>

//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QObject>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QGroupBox;
class QLineEdit;
class QPushButton;

/*
The authorForm will edit the author signature settings saved using QSettings.
It has a description box, edit fields for the name, company, and email, and
an Ok and Cancel button.
*/
class authorForm : public QDialog
{
   Q_OBJECT

public:
   authorForm( QSettings *settings = 0, QWidget* parent = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
   ~authorForm();

   QLabel* descrLabel;
   QGroupBox* inputBox;
   QLabel* emailLabel;
   QLabel* nameLabel;
   QLabel* companyLabel;
   QLabel* infoLabel;
   QLineEdit* nameEdit;
   QLineEdit* companyEdit;
   QLineEdit* emailEdit;
   QPushButton* okButton;
   QPushButton* cancelButton;


protected:
   QVBoxLayout* authorFormLayout;
   QSpacerItem* spacer1;
   QSpacerItem* spacer3;
   QGridLayout* layout1;
   QHBoxLayout* layout2;
   QSpacerItem* spacer4;

   QSettings *set;

protected slots:
   virtual void languageChange();
   void okClicked();
};

#endif // AUTHORFORM_H
