#include <qprogressdialog.h>
#include <qcolor.h>
#include <qstring.h>

#include "mapimgprogressdialog.h"

MapimgProgressDialog::MapimgProgressDialog( const QColor* color1, const QColor* color2, QWidget* creator, const char* name, bool modal, WFlags f )
    : QProgressDialog( creator, name, modal, f )
{
   defaults();

   colorStart = new QColor( *color1 );
   colorEnd = new QColor( *color2 );

   if( colorStart && colorEnd )
   {
       currentRed = colorStart->red();
       currentGreen = colorStart->green();
       currentBlue = colorStart->blue();

       deltaRed = (colorEnd->red() - currentRed) / 1;
       deltaGreen = (colorEnd->green() - currentGreen) / 1;
       deltaBlue = (colorEnd->blue() - currentBlue) / 1;


       stepsPerUpdate = 100;
   }
}


MapimgProgressDialog::MapimgProgressDialog( const QString& labelText, const QString& cancelButtonText, int totalSteps, const QColor* color1, const QColor* color2, QWidget* creator, const char* name, bool modal, WFlags f )
    : QProgressDialog( labelText, cancelButtonText, totalSteps, creator, name, modal, f )
{
    defaults();

   int tempSteps = totalSteps;
   if( tempSteps <= 0 )
       tempSteps = 1;

   if( color1 && color2 )
   {
       colorStart = new QColor( *color1 );
       colorEnd = new QColor( *color2 );

       currentRed = colorStart->red();
       currentGreen = colorStart->green();
       currentBlue = colorStart->blue();

       deltaRed = (colorEnd->red() - currentRed) / tempSteps;
       deltaGreen = (colorEnd->green() - currentGreen) / tempSteps;
       deltaBlue = (colorEnd->blue() - currentBlue) / tempSteps;


       stepsPerUpdate = defaultStepsPerUpdate;

       QPalette p( *colorStart );
       p.setColor( QColorGroup::Text, p.color( QPalette::Active, QColorGroup::Text ) );
       setPalette( p );
   }
}

MapimgProgressDialog::~MapimgProgressDialog()
{
   if( colorStart )
       delete colorStart;

   if( colorEnd )
       delete colorEnd;
}

void MapimgProgressDialog::defaults()
{
    colorStart = NULL;
    colorEnd = NULL;

    deltaRed = 0.0;
    deltaGreen = 0.0;
    deltaBlue = 0.0;

    currentRed = 0.0;
    currentGreen = 0.0;
    currentBlue = 0.0;

    stepsPerUpdate = 200;
}

void MapimgProgressDialog::setProgress( int progress )
{
    QProgressDialog::setProgress( progress );

    if( progress == totalSteps() && colorEnd )
    {
        currentRed = colorEnd->red();
        currentGreen = colorEnd->green();
        currentBlue = colorEnd->blue();
    }
    else
    {
        currentRed += deltaRed;
        currentGreen += deltaGreen;
        currentBlue += deltaBlue;
    }

    if( progress%stepsPerUpdate == 0 && colorStart && colorEnd )
    {
        QColor c( (int)currentRed, (int)currentGreen, (int)currentBlue );
        QPalette p( c );
        p.setColor( QColorGroup::Text, p.color( QPalette::Active, QColorGroup::Text ) );
        setPalette( p );
    }
    return;
}

void MapimgProgressDialog::setTotalSteps( int totalSteps )
{
    QProgressDialog::setTotalSteps( totalSteps );
    
   int tempSteps = totalSteps;
   if( tempSteps <= 0 )
       tempSteps = 1;

   if( colorEnd && colorStart )
   {
       deltaRed = (colorEnd->red() - currentRed) / tempSteps;
       deltaGreen = (colorEnd->green() - currentGreen) / tempSteps;
       deltaBlue = (colorEnd->blue() - currentBlue) / tempSteps;

       currentRed = colorStart->red() + deltaRed * tempSteps;
       currentGreen = colorStart->green() + deltaGreen * tempSteps;
       currentBlue = colorStart->blue() + deltaBlue * tempSteps;
   }

    return;
}

void MapimgProgressDialog::setProgress( int progress, int totalSteps )
{
    setTotalSteps( totalSteps );
    setProgress( progress );

    return;
}
