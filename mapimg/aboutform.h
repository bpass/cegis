// $Id: aboutform.h,v 1.1 2005/01/14 16:18:48 rbuehler Exp $


/****************************************************************************
** Form interface generated from reading ui file 'aboutform.ui'
**
** Created: Thu Jan 6 17:24:08 2005
**      by: The User Interface Compiler ($Id: aboutform.h,v 1.1 2005/01/14 16:18:48 rbuehler Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;

class aboutForm : public QDialog
{
    Q_OBJECT

public:
    aboutForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~aboutForm();

    QLabel* logoLabel;
    QLabel* titleLabel;
    QFrame* discFrame;
    QLabel* discLabel1;
    QLabel* discLabel2;

public slots:
    void mousePressEvent( QMouseEvent * e );

protected:
    QVBoxLayout* aboutFormLayout;
    QVBoxLayout* discFrameLayout;

protected slots:
    virtual void languageChange();
};

#endif // ABOURFORM_H
