/****************************************************************************
** Form interface generated from reading ui file 'equirectForm.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: equirectForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EQUIRECTFORM_H
#define EQUIRECTFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class equirectForm : public QDialog
{
    Q_OBJECT

public:
    equirectForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~equirectForm();

    QLabel* sphereText;
    QLineEdit* sphereEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* centMerLabel;
    QPushButton* centMerButton;
    QLabel* trueScaleLabel;
    QPushButton* trueScaleButton;
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
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* equirectFormLayout;
    QHBoxLayout* Layout58;
    QVBoxLayout* Layout151;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout69;
    QHBoxLayout* Layout130;
    QHBoxLayout* Layout135;
    QHBoxLayout* layout104;

protected slots:
    virtual void languageChange();

};

#endif // EQUIRECTFORM_H
