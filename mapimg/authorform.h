// $Id: authorform.h,v 1.4 2005/02/13 00:27:17 rbuehler Exp $


/****************************************************************************
** Form interface generated from reading ui file 'form1.ui'
**
** Created: Thu Jan 13 17:33:43 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
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
class QGroupBox;
class QLineEdit;
class QPushButton;

class authorForm : public QDialog
{
    Q_OBJECT

public:
    authorForm( QSettings *settings = 0, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~authorForm();

    QLabel* descrLabel;
    QGroupBox* inputBox;
    QLabel* emailLabel;
    QLabel* nameLabel;
    QLabel* companyLabel;
    QLabel* infoLabel;
    QLineEdit* nameEdit;
    QLineEdit* companyEdit;
    QLineEdit* emailEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;

protected:
    QVBoxLayout* authorFormLayout;
    QSpacerItem* spacer1;
    QSpacerItem* spacer3;
    QGridLayout* layout1;
    QHBoxLayout* layout2;
    QSpacerItem* spacer4;

    QSettings *set;

protected slots:
    virtual void languageChange();
    void okClicked();

};

#endif // AUTHORFORM_H
