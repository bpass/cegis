/****************************************************************************
** Form interface generated from reading ui file 'lambForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: lambForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef LAMBFORM_H
#define LAMBFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class lambForm : public QDialog
{
    Q_OBJECT

public:
    lambForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~lambForm();

    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* stdPar1Label;
    QPushButton* stdPar1Button;
    QLabel* stdPar2Label;
    QPushButton* stdPar2Button;
    QLabel* centMerLabel;
    QPushButton* centMerButton;
    QLabel* origLatLabel;
    QPushButton* origLatButton;
    QLabel* zeroAssumed_2;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlon();
    virtual void setLon( double l );
    virtual void getlat1();
    virtual void setLat1( double l );
    virtual void getlat2();
    virtual void setLat2( double l );
    virtual void getlat3();
    virtual void setLat3( double l );
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual bool valueCheck();
    virtual void sendLat1st( double * ptr );
    virtual void sendLat2nd( double * ptr );
    virtual void sendLon( double * ptr );
    virtual void sendLatOrig( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* lambFormLayout;
    QVBoxLayout* Layout248;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout245;
    QHBoxLayout* Layout161;
    QHBoxLayout* Layout162;
    QHBoxLayout* Layout130;
    QHBoxLayout* Layout118;
    QHBoxLayout* layout311;

protected slots:
    virtual void languageChange();

};

#endif // LAMBFORM_H
