/****************************************************************************
** Form interface generated from reading ui file 'oblatedForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: oblatedForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef OBLATEDFORM_H
#define OBLATEDFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class oblatedForm : public QDialog
{
    Q_OBJECT

public:
    oblatedForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~oblatedForm();

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
    QLabel* shapemText;
    QLineEdit* shapemEdit;
    QLabel* shapenText;
    QLineEdit* shapenEdit;
    QLabel* angleText;
    QPushButton* angleButton;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void getlat();
    virtual void getlon();
    virtual void getang();
    virtual void init();
    virtual void getGCTP();
    virtual void setLat( double l );
    virtual void setLon( double l );
    virtual void setAng( double l );
    virtual void sphereCheck( const QString & check );
    virtual void mCheck( const QString & check );
    virtual void nCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLat( double * ptr );
    virtual void sendLon( double * ptr );
    virtual void sendAng( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* oblatedFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout191;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout190;
    QHBoxLayout* Layout131;
    QHBoxLayout* Layout130;
    QVBoxLayout* Layout428;
    QHBoxLayout* Layout36;
    QHBoxLayout* Layout37;
    QHBoxLayout* Layout426;
    QHBoxLayout* layout345;

protected slots:
    virtual void languageChange();

};

#endif // OBLATEDFORM_H
