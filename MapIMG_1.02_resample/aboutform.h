/****************************************************************************
** Form interface generated from reading ui file 'aboutform.ui'
**
** Created: Thu Oct 7 08:49:08 2004
**      by: The User Interface Compiler ($Id: aboutform.h,v 1.1 2004/10/20 16:36:39 jtrent Exp $)
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
class QLabel;

class aboutForm : public QDialog
{
    Q_OBJECT

public:
    aboutForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~aboutForm();

    QLabel* pixmapLogo;
    QLabel* lblUSGS;
    QLabel* lblVersion;
    QLabel* lblDisclaim;
    QLabel* lblDisclaim_2;

public slots:
    virtual void init();
    virtual void mousePressEvent( QMouseEvent * e );

protected:
    QGridLayout* aboutFormLayout;
    QVBoxLayout* layout6;
    QVBoxLayout* layout5;
    QHBoxLayout* layout4;
    QVBoxLayout* layout1;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // ABOUTFORM_H
