/****************************************************************************
** Form interface generated from reading ui file 'pcForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: pcForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PCFORM_H
#define PCFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class pcForm : public QDialog
{
    Q_OBJECT

public:
    pcForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~pcForm();

    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* centMerLabel_2;
    QPushButton* centMerButton_2;
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
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLat( double * ptr );
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* pcFormLayout;
    QVBoxLayout* Layout248;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* layout251;
    QVBoxLayout* Layout86;
    QHBoxLayout* Layout130;
    QHBoxLayout* Layout118;
    QHBoxLayout* layout355;

protected slots:
    virtual void languageChange();

};

#endif // PCFORM_H
