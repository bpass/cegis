/****************************************************************************
** Form interface generated from reading ui file 'hombForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: hombForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HOMBFORM_H
#define HOMBFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class hombForm : public QDialog
{
    Q_OBJECT

public:
    hombForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~hombForm();

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
    QLabel* aziangleLabel;
    QPushButton* aziangleButton;
    QLabel* azmthptLabel;
    QPushButton* azmthptButton;
    QLabel* origLatLabel;
    QPushButton* origLatButton;
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
    virtual void getang();
    virtual void setang( double l );
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void scaleCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual bool valueCheck();
    virtual void sendAng( double * ptr );
    virtual void sendLon( double * ptr );
    virtual void sendLat( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* hombFormLayout;
    QHBoxLayout* Layout19;
    QVBoxLayout* Layout18;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* layout25;
    QHBoxLayout* Layout220;
    QHBoxLayout* Layout175_2;
    QHBoxLayout* Layout175;
    QHBoxLayout* layout250;

protected slots:
    virtual void languageChange();

};

#endif // HOMBFORM_H
