/****************************************************************************
** Form interface generated from reading ui file 'psForm.ui'
**
** Created: Thu Oct 7 08:49:10 2004
**      by: The User Interface Compiler ($Id: psForm.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PSFORM_H
#define PSFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class psForm : public QDialog
{
    Q_OBJECT

public:
    psForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~psForm();

    QLabel* smajorLabel;
    QLineEdit* smajorEdit;
    QLabel* sminorLabel;
    QLineEdit* sminorEdit;
    QLabel* feLabel;
    QLineEdit* feEdit;
    QLabel* fnLabel;
    QLineEdit* fnEdit;
    QLabel* fefnLabel;
    QLabel* longPolLabel;
    QPushButton* longPolButton;
    QLabel* trueScaleLabel;
    QPushButton* trueScaleButton;
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
    virtual void sendLon( double * ptr );
    virtual void sendLat( double * ptr );

signals:
    void changesMade(double * ptr);
    void getGCTP(double * ptr);

protected:
    double * gctparray;

    QVBoxLayout* psFormLayout;
    QVBoxLayout* Layout248;
    QHBoxLayout* Layout246;
    QHBoxLayout* Layout247;
    QHBoxLayout* Layout89;
    QHBoxLayout* Layout90;
    QVBoxLayout* Layout82;
    QHBoxLayout* Layout73;
    QHBoxLayout* Layout81;
    QHBoxLayout* layout370;

protected slots:
    virtual void languageChange();

};

#endif // PSFORM_H
