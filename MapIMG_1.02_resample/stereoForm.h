/****************************************************************************
** Form interface generated from reading ui file 'stereoForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: stereoForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STEREOFORM_H
#define STEREOFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class stereoForm : public QDialog
{
    Q_OBJECT

public:
    stereoForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~stereoForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* centLatLabel;
    QPushButton* centLatButton;
    QLabel* centLonLabel;
    QPushButton* centLonButton;
    QLabel* zeroAssumed;
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
    virtual void sphereCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLat( double * ptr );
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* stereoFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout150;
    QHBoxLayout* Layout114;
    QHBoxLayout* Layout131;
    QHBoxLayout* layout451;

protected slots:
    virtual void languageChange();

};

#endif // STEREOFORM_H
