/****************************************************************************
** Form interface generated from reading ui file 'interruptForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: interruptForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef INTERRUPTFORM_H
#define INTERRUPTFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class interruptForm : public QDialog
{
    Q_OBJECT

public:
    interruptForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~interruptForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* fefnLabel_2;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void sphereCheck( const QString & check );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* interruptFormLayout;
    QVBoxLayout* layout191;
    QHBoxLayout* Layout58;
    QHBoxLayout* layout289;

protected slots:
    virtual void languageChange();

};

#endif // INTERRUPTFORM_H
