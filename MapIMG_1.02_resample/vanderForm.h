/****************************************************************************
** Form interface generated from reading ui file 'vanderForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: vanderForm.h,v 1.1 2004/10/20 16:36:41 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VANDERFORM_H
#define VANDERFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class vanderForm : public QDialog
{
    Q_OBJECT

public:
    vanderForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~vanderForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
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
    virtual void sphereCheck( const QString & check );
    virtual void feCheck( const QString & check );
    virtual void fnCheck( const QString & check );
    virtual void sendLon( double * ptr );
    virtual void sendLat( double * ptr );

signals:
    void changesMade(double *ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* vanderFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout78;
    QHBoxLayout* Layout130;
    QHBoxLayout* Layout118;
    QHBoxLayout* layout474;

protected slots:
    virtual void languageChange();

};

#endif // VANDERFORM_H
