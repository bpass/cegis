/****************************************************************************
** Form interface generated from reading ui file 'sineForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: sineForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SINEFORM_H
#define SINEFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class sineForm : public QDialog
{
    Q_OBJECT

public:
    sineForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~sineForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* centMerLabel;
    QPushButton* centMerButton;
    QLabel* zeroAssumed_2;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlon();
    virtual void setLon( double l );
    virtual void sphereCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* sineFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout61;
    QHBoxLayout* Layout130;
    QHBoxLayout* layout388;

protected slots:
    virtual void languageChange();

};

#endif // SINEFORM_H
