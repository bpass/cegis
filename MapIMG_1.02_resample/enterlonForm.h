/****************************************************************************
** Form interface generated from reading ui file 'enterlonForm.ui'
**
** Created: Thu Oct 7 08:49:08 2004
**      by: The User Interface Compiler ($Id: enterlonForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ENTERLONFORM_H
#define ENTERLONFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QSpinBox;
class QLineEdit;
class QPushButton;

class enterlonForm : public QDialog
{
    Q_OBJECT

public:
    enterlonForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~enterlonForm();

    QLabel* titleLabel;
    QLabel* textLabel3;
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
    virtual void setLon();
    virtual void getLon();
    virtual void trimZeros( QString *toTrim);

signals:
    void changesMade(double l);
    void getLon(double * ptr);

protected:
    double * gctpLon;
    double lon;

    QVBoxLayout* enterlonFormLayout;
    QVBoxLayout* layout91;
    QHBoxLayout* Layout226;
    QHBoxLayout* Layout227;
    QHBoxLayout* layout90;
    QHBoxLayout* layout89;

protected slots:
    virtual void languageChange();

};

#endif // ENTERLONFORM_H
