/****************************************************************************
** Form interface generated from reading ui file 'intsineForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: intsineForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef INTSINEFORM_H
#define INTSINEFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QComboBox;

class intsineForm : public QDialog
{
    Q_OBJECT

public:
    intsineForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~intsineForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* centMerLabel;
    QPushButton* centMerButton;
    QLabel* zeroAssumed_2;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* nzoneText;
    QSpinBox* nzonesSpin;
    QLabel* rflagText;
    QComboBox* rflagComboBox;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlon();
    virtual void setLon( double l );
    virtual void checkRFlag();
    virtual void sphereCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void zoneCheck();
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* intsineFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout61;
    QHBoxLayout* Layout130;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout142;
    QHBoxLayout* Layout141;
    QHBoxLayout* Layout47;
    QHBoxLayout* layout299;

protected slots:
    virtual void languageChange();

};

#endif // INTSINEFORM_H
