/****************************************************************************
** Form interface generated from reading ui file 'somaForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: somaForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SOMAFORM_H
#define SOMAFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class somaForm : public QDialog
{
    Q_OBJECT

public:
    somaForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~somaForm();

    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* incangleLabel;
    QPushButton* incangleButton;
    QLabel* psrevLabel;
    QLineEdit* psrevEdit;
    QLabel* lratLabel;
    QLineEdit* lratEdit;
    QLabel* pflagLabel;
    QSpinBox* pflagSpin;
    QLabel* asclonLabel;
    QPushButton* asclonButton;
    QLabel* zeroAssumed;
    QPushButton* okayButton;
    QPushButton* cancelButton;

public slots:
    virtual void makeChanges();
    virtual void init();
    virtual void getGCTP();
    virtual void getlon();
    virtual void getang();
    virtual void setLon( double l );
    virtual void setAng( double l );
    virtual void smajCheck( const QString & check );
    virtual void sminCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void revCheck( const QString & check );
    virtual void ratioCheck( const QString & check );
    virtual void sendAng( double * ptr );
    virtual void sendLon( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* somaFormLayout;
    QVBoxLayout* Layout18;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout518;
    QHBoxLayout* Layout517;
    QHBoxLayout* Layout264;
    QHBoxLayout* Layout261_2;
    QHBoxLayout* Layout267;
    QVBoxLayout* Layout321;
    QHBoxLayout* Layout262;
    QHBoxLayout* layout421;

protected slots:
    virtual void languageChange();

};

#endif // SOMAFORM_H
