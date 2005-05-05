// $Id: qgctpbox.cpp,v 1.7 2005/05/05 22:48:04 rbuehler Exp $


#include <qtooltip.h>
#include <qvalidator.h>

#include "qgctpbox.h"
#include "qdmsedit.h"

const uint INFO_PRECISION = 6;

QMap<QString,double> *QGctpBox::holdValues=0;

QGctpBox::QGctpBox( QWidget* parent, const char* name )
   : QVBox( parent, name )
{
   QFrame *line = new QFrame( this );
   line->setMinimumHeight( 5 );
   label = new QLabel( this, "label" );
   lineEdit = new QLineEdit( "0.000000", this, "lineEdit" );
   lineEdit->setValidator( new QDoubleValidator( 0.0, 1000000.0, 6, lineEdit ) );
   spinBox = new QSpinBox( this, "spinBox" );
   spinBox->setValue( 0 );
   dmsEdit = new QDmsEdit( this, "dmsEdit" );
   dmsEdit->setValue( 0 );

   activeEdit = NULL;
   name = QString::null;

   hide();
   initializeHoldValues();
}

QGctpBox::~QGctpBox()
{
   //nuttin' doin'
}

double QGctpBox::value()
{
   if( label->text().startsWith( "Radius" ) && 
      lineEdit->text().toDouble() == 0.0 )
      lineEdit->setText( "6370997.000000" );
   else if( label->text().startsWith( "Scale" ) && 
      lineEdit->text().toDouble() == 0.0 )
      lineEdit->setText( "1.000000" );
   else if( label->text().startsWith( "Landsat Satellite" ) && 
      spinBox->value() == 6 )
      spinBox->setValue( 1 );
   

   if( activeEdit == lineEdit )
      return lineEdit->text().toDouble();
   else if( activeEdit == spinBox )
      return static_cast<double>(spinBox->value());
   else if( activeEdit == dmsEdit )
      return dmsEdit->value();
   else
      return 0;
}

void QGctpBox::setValue( const double val )
{
   if( activeEdit == spinBox )
      spinBox->setValue( static_cast<int>( val ) );
   else if( activeEdit == dmsEdit )
      dmsEdit->setValue( val );
   else //( activeEdit == lineEdit )
      lineEdit->setText( QString::number( val, 'f', 12 ) );
}

void QGctpBox::setDisabled( bool disabled )
{
   lineEdit->setDisabled( disabled );
   spinBox->setDisabled( disabled );
   dmsEdit->setDisabled( disabled );
}

QString QGctpBox::output()
{
   QString cleanUp;
   if( value() == 0.0 )
      return "0.000000";
   else if( activeEdit == spinBox )
      return QString::number( spinBox->value() ) + ".000000";
   else if( activeEdit == lineEdit )
      cleanUp = lineEdit->text();
   else if( activeEdit == dmsEdit )
      cleanUp = QString::number( dmsEdit->value(), 'f', 14 );

   cleanUp.stripWhiteSpace();
   uint newLen = cleanUp.find( '.' ) + INFO_PRECISION + 1;
   if( newLen == INFO_PRECISION ) //No '.' found
      cleanUp.append( ".000000" );
   else if( newLen < cleanUp.length() )
      cleanUp.truncate( newLen );
   else
      cleanUp.append( QString("000000").left( newLen - cleanUp.length() ) );

   if( cleanUp.find( '.' ) == 0 )
      cleanUp.prepend( "0" );

   return cleanUp;
}

void QGctpBox::setGctpName( const QString & gctpName )
{
   if( name != QString::null )
      holdValues->replace( name, value() );

   activeEdit = NULL;
   QToolTip::remove( label );

   if( gctpName == "" )
   {
      hide();
      name = "";
      return;
   }
   else if( gctpName == "NoWay" )
   {
      hide();
      name = "";
      return;
      /*label->setText( "Bad Projection *" );
      QToolTip::add( label, 
         "<b>Bad Projection</b>: This projection either generates "
         "useless data or crashes. It is recommended that you "
         "choose a different one. This issue may be addressed in "
         "Future versions of mapimg." );
      activeEdit = NULL;*/
   }
   else if( gctpName == "Angle" )	
   {
      label->setText( "Oval Rotation Angle:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( 0 );
      dmsEdit->setMaxVal( 360 );
   }
   else if( gctpName == "AscLong" )	
   {
      label->setText( "Longitude of Ascending Orbit: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
      QToolTip::add( label,
         "For Landsat-1,2,3 orbits this parameter should be:\n"
         "128° 52' 12\" + (360/251 * path number)\n\n"
         "For Landsat-4,5 orbits this parameter should be:\n"
         "129° 18' 00\" + (360/233 * path number)");
   }
   else if( gctpName == "AziAng" )
   {
      label->setText( "Azimuth Angle East of North:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( 0 );
      dmsEdit->setMaxVal( 360 );
   }
   else if( gctpName == "AzmthPt" )	
   {
      label->setText( "Longitude where Azimuth Occurs:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
   }
   else if( gctpName == "CenterLat" )	
   {
      label->setText( "Latitude of Projection Center:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
   }
   else if( gctpName == "CentLon" )	
   {
      label->setText( "Longitude of Projection Center:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
   }
   else if( gctpName == "CentMer" )	
   {
      label->setText( "Longitude of Central Meridian:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
   }
   else if( gctpName == "FactorH" )	
   {
      label->setText( "Scale Factor at Center of Projection: *" );
      activeEdit = lineEdit;
      lineEdit->setValidator( new QDoubleValidator( 0.0, 1000000.0, 6, lineEdit ) );
      QToolTip::add( label, "This will default to 1 if left as 0." );
   }
   else if( gctpName == "FactorM" )	
   {
      label->setText( "Scale Factor at Central Meridian: *" );
      activeEdit = lineEdit;
      lineEdit->setValidator( new QDoubleValidator( 0.0, 1000000.0, 6, lineEdit ) );
      QToolTip::add( label, "This will default to 1 if left as 0." );
   }
   else if( gctpName == "FE" )	
   {
      label->setText( "False Easting:" );
      activeEdit = lineEdit;
      lineEdit->setValidator( new QDoubleValidator( -10000000.0, 10000000.0, 6, lineEdit ) );
      QToolTip::add( label, "Value must be between -10000000 and 10000000." );
   }
   else if( gctpName == "FN" )	
   {
      label->setText( "False Northing:" );
      activeEdit = lineEdit;
      lineEdit->setValidator( new QDoubleValidator( -10000000.0, 10000000.0, 6, lineEdit ) );
      QToolTip::add( label, "Value must be between -10000000 and 10000000." );
   }
   else if( gctpName == "Height" )	
   {
      label->setText( "Height of Perspective:" );
      activeEdit = lineEdit;
      lineEdit->setValidator( new QDoubleValidator( -1000000000.0, 1000000000.0, 6, lineEdit ) );
      QToolTip::add( label, "Value must be between -1000000000.0 and 1000000000 meters.<br><br>"
         "<b>Note:</b> Currently, values less than or equal to 0 create unexpected results");
   }
   else if( gctpName == "IncAng" )	
   {
      label->setText( "Inclination of Orbit: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( 0 );
      dmsEdit->setMaxVal( 360 );
      QToolTip::add( label,
         "For Landsat-1,2,3 orbits this parameter should be:\n"
         "99° 05' 31.2\"\n\n"
         "For Landsat-4,5 orbits this parameter should be:\n"
         "98° 12' 00\"");
   }
   else if( gctpName == "Lat/Z" )	
   {
      label->setText( "Latitude within Zone:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
   }
   else if( gctpName == "Lat1" )	
   {
      label->setText( "Latitude of 1st Point on Center Line: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "2nd point." );
   }
   else if( gctpName == "Lat2" )	
   {
      label->setText( "Latitude of 2nd Point on Center Line: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "1st point." );
   }
   else if( gctpName == "Lon/Z" )	
   {
      label->setText( "Longitude within Zone:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
   }
   else if( gctpName == "Long1" )	
   {
      label->setText( "Longitude of 1st Point on Center Line: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "2nd point." );
   }
   else if( gctpName == "Long2" )	
   {
      label->setText( "Longitude of 2nd Point on Center Line: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -180 );
      dmsEdit->setMaxVal( 180 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "1st point." );
   }
   else if( gctpName == "LongPol" )	
   {
      label->setText( "Longitude down below Pole of Map:" );
      activeEdit = dmsEdit;
      QToolTip::remove( this );
      dmsEdit->setMinVal( -360 );
      dmsEdit->setMaxVal( 360 );
   }
   else if( gctpName == "LRat" )	
   {
      label->setText( "Landsat Ratio: *" );
      activeEdit = lineEdit;
      QToolTip::add( label,
         "For Landsat-1,2,3 orbits this parameter should be:\n"
         "0.5201613\n\n"
         "For Landsat-4,5 orbits this parameter should be:\n"
         "0.5201613");
      QToolTip::add( label, "Value must be between 0 and 1." );
      lineEdit->setValidator( new QDoubleValidator( 0.0, 1.0, 6, lineEdit ) );
   }
   else if( gctpName == "OriginLat" )	
   {
      label->setText( "Latitude of Projection Origin:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
   }
   else if( gctpName == "Path" )	
   {
      label->setText( "Landsat Path Number:" );
      activeEdit = spinBox;
      spinBox->setMinValue(1);
      spinBox->setMaxValue(233);
      QToolTip::add( label, "Value must be between 1 and 233." );
   }
   else if( gctpName == "PFlag" )	
   {
      label->setText( "End of Path Flag: *" );
      activeEdit = spinBox;
      QToolTip::add( label,
         "A portion of Landsat rows 1 and 2 may also be seen as parts of rows "
         "246 or 247. To place these locations at rows 246 or 247, set the "
         "end of path flag to 1" );
   }
   else if( gctpName == "PSRev" )	
   {
      label->setText( "Period of Satellite Revolution: *" );
      activeEdit = lineEdit;
      QToolTip::add( label,
         "For Landsat-1,2,3 orbits this parameter should be:\n"
         "103.2669323\n\n"
         "For Landsat-4,5 orbits this parameter should be:\n"
         "98.884119");
   }
   else if( gctpName == "Satnum" )	
   {
      label->setText( "Landsat Satellite Number: *" );
      activeEdit = spinBox;
      spinBox->setMinValue(1);
      spinBox->setMaxValue(7);
      QToolTip::add( label, "Value must be 1, 2, 3, 4, 5, or 7.\n"
         "6 will be changed to 1.");
   }
   else if( gctpName == "Shapem" )	
   {
      label->setText( "Oval Shape Parameter m:" );
      activeEdit = lineEdit;
      QToolTip::add( label, "Value must be between 0 and 2." );
      lineEdit->setValidator( new QDoubleValidator( 0.0, 2.0, 6, lineEdit ) );
   }
   else if( gctpName == "Shapen" )	
   {
      label->setText( "Oval Shape Parameter n:" );
      activeEdit = lineEdit;
      QToolTip::add( label, "Value must be greater than 2." );
      lineEdit->setValidator( new QDoubleValidator( 2.0, 10000000.0, 6, lineEdit ) );
   }
   else if( gctpName == "SMajor" )	
   {
      label->setText( "Semi-Major Axis:" );
      activeEdit = lineEdit;
      QToolTip::add( label, "Value must be between 0 and 10000000m." );
      lineEdit->setValidator( new QDoubleValidator( 0.0, 100000000.0, 6, lineEdit ) );
   }
   else if( gctpName == "SMinor" )	
   {
      label->setText( "Semi-Minor Axis:" );
      activeEdit = lineEdit;
      QToolTip::add( label, "Value must be between 0 and 10000000m." );
      lineEdit->setValidator( new QDoubleValidator( 0.0, 10000000.0, 6, lineEdit ) );
   }
   else if( gctpName == "Sphere" )	
   {
      label->setText( "Radius of the Reference Sphere: *" );
      activeEdit = lineEdit;
      QToolTip::add( label, "This will default to 6370997 if left as 0." );
      lineEdit->setValidator( new QDoubleValidator( 0.0, 10000000.0, 6, lineEdit ) );
   }
   else if( gctpName == "STDPAR" )	
   {
      label->setText( "Latitude of Standard Parallel:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
   }
   else if( gctpName == "STDPR1" )	
   {
      label->setText( "Latitude of 1st Standard Parallel: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "2nd Standard Parallel." );
   }
   else if( gctpName == "STDPR2" )	
   {
      label->setText( "Latitude of 2nd Standard Parallel: *" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
      QToolTip::add( label, "mapimg2 will not work if this is equal to "
         "1st Standard Parallel." );
   }
   else if( gctpName == "TrueScale" )	
   {
      label->setText( "Latitude of True Scale:" );
      activeEdit = dmsEdit;
      dmsEdit->setMinVal( -90 );
      dmsEdit->setMaxVal( 90 );
   }
   else
      hide();

   if( gctpName == "one" )
   {
      activeEdit = spinBox;
      spinBox->setValue( 1 );
   }
   else if( gctpName == "zero" )
   {
      activeEdit = spinBox;
      spinBox->setValue( 0 );
   }
   else
   {
      lineEdit->hide();
      spinBox->hide();
      dmsEdit->hide();
      if(activeEdit != NULL)
         activeEdit->show();

      name = gctpName;
      setValue( holdValues->find(name).data() );
      show();
   }
}

void QGctpBox::initializeHoldValues()
{
   if( holdValues != NULL )
      return;

   holdValues = new QMap<QString,double>();
   holdValues->insert("Angle",      0);
   holdValues->insert("AscLong",    0);
   holdValues->insert("AziAng",     0);
   holdValues->insert("AzmthPt",    0);
   holdValues->insert("CenterLat",  0);
   holdValues->insert("CentLon",    0);
   holdValues->insert("CentMer",    0);
   holdValues->insert("FactorH",    1);
   holdValues->insert("FactorM",    0);
   holdValues->insert("FE",         0);
   holdValues->insert("FN",         0);
   holdValues->insert("Height",     0);
   holdValues->insert("IncAng",     0);
   holdValues->insert("Lat/Z",      0);
   holdValues->insert("Lat1",       0);
   holdValues->insert("Lat2",       0);
   holdValues->insert("Lon/Z",      0);
   holdValues->insert("Long1",      0);
   holdValues->insert("Long2",      0);
   holdValues->insert("LongPol",    0);
   holdValues->insert("LRat",       0.5201613);
   holdValues->insert("OriginLat",  0);
   holdValues->insert("Path",       1);
   holdValues->insert("PFlag",      0);
   holdValues->insert("PSRev",      1);
   holdValues->insert("Satnum",     1);
   holdValues->insert("Shapem",     0);
   holdValues->insert("Shapen",     2);
   holdValues->insert("SMajor",     0);
   holdValues->insert("SMinor",     0);
   holdValues->insert("Sphere",     6370997);
   holdValues->insert("STDPAR",     0);
   holdValues->insert("STDPR1",     0);
   holdValues->insert("STDPR2",     20000000);
   holdValues->insert("TrueScale",  1);
   holdValues->insert("NoWay",      0);
   holdValues->insert("one",        1);
   holdValues->insert("zero",       0);
}
