/****************************************************************************
** Form interface generated from reading ui file 'enterlatForm.ui'
**
** Created: Thu Oct 7 08:49:08 2004
**      by: The User Interface Compiler ($Id: enterlatForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ENTERLATFORM_H
#define ENTERLATFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QSpinBox;
class QLineEdit;
class QPushButton;

class enterlatForm : public QDialog
{
    Q_OBJECT

public:
    enterlatForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~enterlatForm();

    QLabel* titleLabel;
    QLabel* textLabel2;
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
    virtual void setLat();
    virtual void getLat();
    virtual void trimZeros( QString *toTrim );

signals:
    void changesMade(double l);
    void getLat(double * ptr);

protected:
    double *gctpLat;
    double lat;

    QVBoxLayout* enterlatFormLayout;
    QVBoxLayout* layout81;
    QHBoxLayout* Layout226;
    QHBoxLayout* Layout227;
    QHBoxLayout* layout79;
    QHBoxLayout* layout80;

protected slots:
    virtual void languageChange();

};

#endif // ENTERLATFORM_H
