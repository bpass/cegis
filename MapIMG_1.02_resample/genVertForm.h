/****************************************************************************
** Form interface generated from reading ui file 'genVertForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: genVertForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GENVERTFORM_H
#define GENVERTFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class genVertForm : public QDialog
{
    Q_OBJECT

public:
    genVertForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~genVertForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* heightLabel;
    QLineEdit* heightEdit;
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
    virtual void heightCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLon( double * ptr );
    virtual void sendLat( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* genVertFormLayout;
    QVBoxLayout* layout228;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout30;
    QHBoxLayout* Layout104;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout150;
    QHBoxLayout* Layout114;
    QHBoxLayout* Layout131;
    QHBoxLayout* layout194;

protected slots:
    virtual void languageChange();

};

#endif // GENVERTFORM_H
