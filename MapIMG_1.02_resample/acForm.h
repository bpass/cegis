/****************************************************************************
** Form interface generated from reading ui file 'acForm.ui'
**
** Created: Thu Oct 21 09:25:35 2004
**      by: The User Interface Compiler ($Id: acForm.h,v 1.2 2004/10/21 15:15:59 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ACFORM_H
#define ACFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class acForm : public QDialog
{
    Q_OBJECT

public:
    acForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~acForm();

    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* acFormLayout;
    QVBoxLayout* layout11;
    QHBoxLayout* layout6;
    QHBoxLayout* layout7;
    QHBoxLayout* layout8;
    QHBoxLayout* layout9;
    QHBoxLayout* layout26;

protected slots:
    virtual void languageChange();

};

#endif // ACFORM_H
