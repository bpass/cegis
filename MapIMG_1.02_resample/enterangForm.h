/****************************************************************************
** Form interface generated from reading ui file 'enterangForm.ui'
**
** Created: Thu Oct 7 08:49:08 2004
**      by: The User Interface Compiler ($Id: enterangForm.h,v 1.1 2004/10/20 16:36:39 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ENTERANGFORM_H
#define ENTERANGFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QSpinBox;
class QLineEdit;
class QPushButton;

class enterangForm : public QDialog
{
    Q_OBJECT

public:
    enterangForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~enterangForm();

    QLabel* titleLabel;
    QLabel* titleLabel2;
    QLabel* degreeText;
    QSpinBox* degreeSpinBox;
    QLabel* minuteText;
    QSpinBox* minuteSpinBox;
    QLabel* secondsText;
    QLineEdit* secondsEdit;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void checkSeconds();
    virtual void setAng();
    virtual void getAng();
    virtual void trimZeros( QString *toTrim);

signals:
    void changesMade(double l);
    void getAng(double * ptr);

protected:
    double * gctpAng;
    double ang;

    QVBoxLayout* enterangFormLayout;
    QVBoxLayout* layout60;
    QHBoxLayout* Layout226;
    QHBoxLayout* Layout227;
    QHBoxLayout* layout59;
    QHBoxLayout* layout57;

protected slots:
    virtual void languageChange();

};

#endif // ENTERANGFORM_H
