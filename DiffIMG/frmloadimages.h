/****************************************************************************
** Form interface generated from reading ui file 'frmloadimages.ui'
**
** Created: Thu Sep 16 10:49:44 2004
**      by: The User Interface Compiler ($Id: frmloadimages.h,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMLOADIMAGES_H
#define FRMLOADIMAGES_H

#include <qvariant.h>
#include <qdialog.h>
#include "imagefile.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QGroupBox;
class QLabel;

class FrmLoadImages : public QDialog
{
    Q_OBJECT

public:
    FrmLoadImages( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FrmLoadImages();

    QPushButton* btnShowDifference;
    QPushButton* btnClose;
    QGroupBox* originalGroup;
    QLabel* lblOriginal;
    QPushButton* btnLoadOriginal;
    QPushButton* btnLoadReprojected;
    QLabel* lblReprojected;
    QLabel* lblPositions;

    virtual bool loadImage( ImageFile * image );

public slots:
    virtual void loadOriginal();
    virtual void loadReprojected();
    virtual void showDifference();

protected:
    ImageFile originalImage;
    ImageFile reprojectedImage;

    QGridLayout* FrmLoadImagesLayout;
    QVBoxLayout* layout7;
    QGridLayout* originalGroupLayout;
    QGridLayout* layout11;
    QGridLayout* layout10;

protected slots:
    virtual void languageChange();

};

#endif // FRMLOADIMAGES_H
