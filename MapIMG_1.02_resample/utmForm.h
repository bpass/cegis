/****************************************************************************
** Form interface generated from reading ui file 'utmForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: utmForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef UTMFORM_H
#define UTMFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QSpinBox;

class utmForm : public QDialog
{
    Q_OBJECT

public:
    utmForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~utmForm();

    QLabel* latLabel;
    QPushButton* latzButton;
    QLabel* lonLabel;
    QPushButton* lonzButton;
    QLabel* zonecodeLabel;
    QSpinBox* zonecodeSpin;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlon();
    virtual void setLon( double l );
    virtual void getlat();
    virtual void setLat( double l );
    virtual void sendLat( double * ptr );
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr, int val);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* utmFormLayout;
    QVBoxLayout* Layout26;
    QHBoxLayout* Layout44;
    QHBoxLayout* Layout45;
    QHBoxLayout* Layout25;
    QHBoxLayout* layout466;

protected slots:
    virtual void languageChange();

};

#endif // UTMFORM_H
