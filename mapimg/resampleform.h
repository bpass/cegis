// $Id: resampleform.h,v 1.2 2005/02/03 18:12:18 jtrent Exp $


/****************************************************************************
** Form interface generated from reading ui file 'resampleform.ui'
**
** Created: Thu Jan 20 10:21:37 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef RESAMPLEFORM_H
#define RESAMPLEFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include <qvaluelist.h>
#include "resampleinfo.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QComboBox;
class QLineEdit;
class QPushButton;
class QListBox;
class QListBoxItem;

class ResampleForm : public QDialog
{
   Q_OBJECT

public:
   ResampleForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
   ~ResampleForm();

//   enum ResampleCode{NearestNeighbor,Add,Mean,Mode,Min,Max};

   ResampleInfo info();
   ResampleInfo::ResampleCode resampleCode(){return rcode;}
   IgnoreList &ignoreList(){return ilist;}
   bool wasCanceled(){return canceled;}

protected:
   QGroupBox* resampleBox;
   QComboBox* resampleCombo;
   QGroupBox* ignoreBox;
   QLineEdit* ignoreEdit;
   QPushButton* newButton;
   QPushButton* delButton;
   QListBox* ignoreListBox;
   QPushButton* okButton;
   QPushButton* cancelButton;

   QVBoxLayout* ResampleFormLayout;
   QHBoxLayout* inputLayout;
   QVBoxLayout* resampleBoxLayout;
   QHBoxLayout* ignoreBoxLayout;
   QVBoxLayout* ignoreLayout;
   QSpacerItem* ingoreSpacer;
   QHBoxLayout* okLayout;
   QSpacerItem* okSpacer;

protected slots:
   virtual void languageChange();
   void newVal();
   void delVal();
   void cancel();

private:
   ResampleInfo::ResampleCode rcode;
   IgnoreList ilist;
   bool canceled;
};

#endif // RESAMPLEFORM_H
