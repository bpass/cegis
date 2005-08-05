#include <q3progressdialog.h>
#include <QColor>
#include <QString>

#include "mapimgprogressdialog.h"

MapimgProgressDialog::MapimgProgressDialog( const QColor* color1, const QColor* color2, QWidget* creator, const char* name, bool modal, Qt::WFlags f )
: Q3ProgressDialog( creator, name, modal, f )
{
   defaults();

   colorStart = new QColor( *color1 );
   colorEnd = new QColor( *color2 );

   if( colorStart && colorEnd )
   {
      currentRed = colorStart->red();		//Took away Qt::  
      currentGreen = colorStart->green(); //before color names here 
      currentBlue = colorStart->blue();	//and below

      deltaRed = (colorEnd->red() - currentRed) / 1;
      deltaGreen = (colorEnd->green() - currentGreen) / 1;
      deltaBlue = (colorEnd->blue() - currentBlue) / 1;

      stepsPerUpdate = 100;
   }
}


MapimgProgressDialog::MapimgProgressDialog( const QString& labelText, const QString& cancelButtonText, int totalSteps, const QColor* color1, const QColor* color2, QWidget* creator, const char* name, bool modal, Qt::WFlags f )
: Q3ProgressDialog( labelText, cancelButtonText, totalSteps, creator, name, modal, f )
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
   Q3ProgressDialog::setProgress( progress );

   if( progress == totalSteps() && colorEnd )
   {
      currentRed = colorEnd->red();		//took Qt:: away
      currentGreen = colorEnd->green();	//before color
	  currentBlue = colorEnd->blue();		//names
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
   Q3ProgressDialog::setTotalSteps( totalSteps );

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
