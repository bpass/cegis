/****************************************************************************
** Form interface generated from reading ui file 'frmdifference.ui'
**
** Created: Mon Apr 26 08:23:56 2004
**      by: The User Interface Compiler ($Id: frmdifference.h,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMDIFFERENCE_H
#define FRMDIFFERENCE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QPushButton;

class FrmDifference : public QDialog
{
    Q_OBJECT

public:
    FrmDifference( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FrmDifference();

    QGroupBox* differenceGroup;
    QLabel* pixDifference;
    QLabel* lblDifference;
    QGroupBox* histoGroup;
    QLabel* pixHisto;
    QPushButton* btnSaveDifference;
    QPushButton* btnSaveHisto;
    QPushButton* btnClose;

public slots:
    virtual void init();
    virtual void paint( int w, int h, QString dataType );
    virtual void saveHisto();

protected:
    QGridLayout* FrmDifferenceLayout;
    QVBoxLayout* layout8;
    QGridLayout* differenceGroupLayout;
    QHBoxLayout* layout7;
    QGridLayout* histoGroupLayout;
    QVBoxLayout* layout9;

protected slots:
    virtual void languageChange();

    virtual void saveImage();


};

#endif // FRMDIFFERENCE_H
