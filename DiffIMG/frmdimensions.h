/****************************************************************************
** Form interface generated from reading ui file 'frmdimensions.ui'
**
** Created: Fri Apr 23 12:49:29 2004
**      by: The User Interface Compiler ($Id: frmdimensions.h,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMDIMENSIONS_H
#define FRMDIMENSIONS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QSpinBox;
class QComboBox;
class QPushButton;

class FrmDimensions : public QDialog
{
    Q_OBJECT

public:
    FrmDimensions( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FrmDimensions();

    QLabel* lblImage;
    QLabel* lblRows;
    QSpinBox* spinRows;
    QLabel* lblColumns;
    QSpinBox* spinColumns;
    QLabel* lblDataType;
    QComboBox* cmbDataType;
    QPushButton* btnOk;
    QPushButton* btnCancel;

protected:
    QGridLayout* FrmDimensionsLayout;
    QVBoxLayout* layout12;
    QHBoxLayout* layout14;
    QHBoxLayout* layout11;
    QHBoxLayout* layout10;
    QVBoxLayout* layout19;

protected slots:
    virtual void languageChange();

    virtual void Ok();


};

#endif // FRMDIMENSIONS_H
