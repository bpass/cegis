/****************************************************************************
** Form interface generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler ($Id: authorform.h,v 1.2 2005/01/14 22:50:16 rbuehler Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef AUTHORFORM_H
#define AUTHORFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include <qsettings.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;

class authorForm : public QDialog
{
    Q_OBJECT

public:
    authorForm( QSettings *settings = 0, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~authorForm();

    QLabel* textLabel1;
    QLabel* textLabel4;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel5;
    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QLineEdit* lineEdit3;
    QPushButton* pushButton1;

protected:
    QVBoxLayout* authorFormLayout;
    QSpacerItem* spacer5;
    QSpacerItem* spacer1;
    QSpacerItem* spacer3;
    QGridLayout* layout1;
    QHBoxLayout* layout2;
    QSpacerItem* spacer4;

    QSettings *set;

protected slots:
    virtual void languageChange();

};

#endif // AUTHORFORM_H
