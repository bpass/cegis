/****************************************************************************
** Form interface generated from reading ui file 'spForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: spForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SPFORM_H
#define SPFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class spForm : public QDialog
{
    Q_OBJECT

public:
    spForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~spForm();


protected:

protected slots:
    virtual void languageChange();

};

#endif // SPFORM_H
