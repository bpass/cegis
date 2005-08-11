// $Id: resampleform.h,v 1.3 2005/08/11 20:21:51 lwoodard Exp $


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
#include <q3valuelist.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "resampleinfo.h"
#include "rasterinfo.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QComboBox;
class QLabel;
class Q3ButtonGroup;
class QRadioButton;
class QLineEdit;
class QPushButton;
class Q3ListBox;
class Q3ListBoxItem;
class QEvent;
class QSlider;
class Q3HBox;

class ResampleForm : public QDialog
{
   Q_OBJECT

public:
   ResampleForm( RasterInfo input, RasterInfo output, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
   ~ResampleForm();

   ResampleInfo info();
   IgnoreList &ignoreList(){return ilist;}
   bool wasCanceled(){return canceled;}

protected:
   QGroupBox* resampleBox;
   QComboBox* resampleCombo;
   QLabel* catconLabel;
 //****  Q3ButtonGroup* catconButtonGroup;
/****/QGroupBox* catconButtonGroup;
   QRadioButton* catRadio;
   QRadioButton* conRadio;

   QGroupBox* ignoreBox;
   QLineEdit* ignoreEdit;
   QPushButton* newButton;
   QPushButton* delButton;
   Q3ListBox* ignoreListBox;
   QPushButton* okButton;
   QPushButton* cancelButton;
   QLabel* ignoreLabel;

   QVBoxLayout* ResampleFormLayout;
   QHBoxLayout* categoricalLayout;
   QVBoxLayout* inputLayout;
   QVBoxLayout* resampleBoxLayout;
   QHBoxLayout* ignoreBoxLayout;
   QVBoxLayout* ignoreLayout;
   QSpacerItem* ingoreSpacer;
   QHBoxLayout* okLayout;
   QSpacerItem* okSpacer;

   QGroupBox* memoryBox;
   QSlider* memoryAllocation;
   QPushButton* memoryResetButton;
   QHBoxLayout* memoryLabelResetLayout;
   QVBoxLayout* memoryBoxLayout;
   QLabel* memoryLabel;

   bool eventFilter( QObject* object, QEvent* event );

protected slots:
   virtual void languageChange();
   void rcodeChanged( const QString &rcode );
   void newVal();
   void delVal();
   void cancel();
   void reject();
   void ignoreListKeyPress( QKeyEvent* e );
   void updateMemoryAllocation();
   void resetMemory();

private:
   IgnoreList ilist;
   bool canceled;
   int defaultMemory;

   float bytesPerRow;
};

#endif // RESAMPLEFORM_H
