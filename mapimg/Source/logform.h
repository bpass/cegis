// $Id: logform.h,v 1.1 2005/07/12 16:21:04 rbuehler Exp $


/****************************************************************************
** Form interface generated from reading ui file 'logForm.ui'
**
** Created: Fri Jan 14 15:41:20 2005
**      by: The User Interface Compiler ($Id: logform.h,v 1.1 2005/07/12 16:21:04 rbuehler Exp $)
****************************************************************************/

#ifndef LOGFORM_H
#define LOGFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTextEdit;

/*
The logFrom is used for showing the user what the output of gctp was
throughout the reprojection.
*/
class logForm : public QDialog
{
   Q_OBJECT

public:
   logForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
   ~logForm();

   QPushButton* saveButton;
   QPushButton* okButton;
   QTextEdit* logViewer;

public slots:
   virtual void saveLog();
   virtual void refreshLog();
   virtual void loadLog();

protected:
   QGridLayout* logFormLayout;
   QHBoxLayout* layout3;
   QSpacerItem* spacer1;

protected slots:
   virtual void languageChange();

};

#endif // LOGFORM_H
