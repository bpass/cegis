/****************************************************************************
** Form interface generated from reading ui file 'ecaForm.ui'
**
** Created: Thu Oct 7 08:49:08 2004
**      by: The User Interface Compiler ($Id: ecaForm.h,v 1.1 2004/10/20 16:36:39 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ECAFORM_H
#define ECAFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class ecaForm : public QDialog
{
    Q_OBJECT

public:
    ecaForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ecaForm();

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
    virtual void getlat();
    virtual void setLat( double l );
    virtual void getstdpar();
    virtual void setstdPar( double l );
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendStdPar( double * ptr );
    virtual void sendLon( double * ptr );
    virtual void sendLat( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* ecaFormLayout;
    QVBoxLayout* layout51;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* layout50;
    QHBoxLayout* Layout165;
    QHBoxLayout* Layout130;
    QHBoxLayout* Layout118;
    QHBoxLayout* layout52;

protected slots:
    virtual void languageChange();

};

#endif // ECAFORM_H
