/****************************************************************************
** Form interface generated from reading ui file 'projfileForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: projfileForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PROJFILEFORM_H
#define PROJFILEFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLineEdit;
class QLabel;
class QPushButton;

class projfileForm : public QDialog
{
    Q_OBJECT

public:
    projfileForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~projfileForm();

    QComboBox* pixelComboBox;
    QLineEdit* pixelEdit;
    QLabel* meterLabel;
    QLabel* PixelLabel;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void pixEditCheck();
    virtual void pixelCheck();

signals:
    void changesMade(double pixel);

protected:
    double pixelSize;

    QVBoxLayout* projfileFormLayout;
    QGridLayout* layout3;
    QHBoxLayout* layout359;

protected slots:
    virtual void languageChange();

};

#endif // PROJFILEFORM_H
