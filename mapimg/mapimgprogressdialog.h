#ifndef MAPIMGPROGRESSDIALOG_H
#define MAPIMGPROGRESSDIALOG_H


#include <qprogressdialog.h>
#include <qcolor.h>

#define defaultStepsPerUpdate 10

class MapimgProgressDialog : public QProgressDialog
{
 Q_OBJECT

public:
       MapimgProgressDialog( const QColor* color1 = 0, const QColor* color2 = 0, QWidget* creator = 0, const char* name = 0, bool modal = FALSE, WFlags f = 0 );
       MapimgProgressDialog( const QString& labelText, const QString& cancelButtonText, int totalSteps, const QColor* color1 = 0, const QColor* color2 = 0, QWidget* creator = 0, const char* name = 0, bool modal = FALSE, WFlags f = 0 );
       ~MapimgProgressDialog();

public slots:
       void setProgress( int progress );

private:
        void defaults();

        QColor* colorStart;
        QColor* colorEnd;

        double  deltaRed;
        double  deltaGreen;
        double  deltaBlue;

        double  currentRed;
        double  currentGreen;
        double  currentBlue;

        int     stepsPerUpdate;
};

#endif //MAPIMGPROGRESSDIALOG_H



