/****************************************************************************
** Form interface generated from reading ui file 'homaForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: homaForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HOMAFORM_H
#define HOMAFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class homaForm : public QDialog
{
    Q_OBJECT

public:
    homaForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~homaForm();

    QLabel* factorLabel;
    QLineEdit* factorEdit;
    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* origLatLabel;
    QPushButton* origLatButton;
    QLabel* long1Label;
    QPushButton* long1Button;
    QLabel* long2Label;
    QPushButton* long2Button;
    QLabel* lat1Label;
    QPushButton* lat1Button;
    QLabel* lat2Label;
    QPushButton* lat2Button;
    QLabel* zeroAssumed;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlat();
    virtual void setLat( double l );
    virtual void getlon1();
    virtual void setLon1( double l );
    virtual void getlon2();
    virtual void setLon2( double l );
    virtual void getlat1();
    virtual void setLat1( double l );
    virtual void getlat2();
    virtual void setLat2( double l );
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void scaleCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual bool valueCheck();
    virtual void sendLon1st( double * ptr );
    virtual void sendLon2n( double * ptr );
    virtual void sendLat1st( double * ptr );
    virtual void sendLat2nd( double * ptr );
    virtual void sendLatOrig( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* homaFormLayout;
    QHBoxLayout* Layout19;
    QVBoxLayout* Layout18;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout79;
    QHBoxLayout* Layout72;
    QHBoxLayout* Layout72_2;
    QHBoxLayout* Layout72_3;
    QHBoxLayout* Layout72_4;
    QHBoxLayout* Layout72_5;
    QHBoxLayout* layout208;

protected slots:
    virtual void languageChange();

};

#endif // HOMAFORM_H
