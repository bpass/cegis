/****************************************************************************
** Form interface generated from reading ui file 'logForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: logForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef LOGFORM_H
#define LOGFORM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QTextEdit;

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
    virtual void init();
    virtual void refreshLog();
    virtual void loadLog();

protected:
    QGridLayout* logFormLayout;
    QHBoxLayout* layout3;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // LOGFORM_H
