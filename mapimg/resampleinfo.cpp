// $Id: resampleinfo.cpp,v 1.6 2005/02/13 23:12:48 rbuehler Exp $


#include "resampleinfo.h"
#include <qstringlist.h>


ResampleInfo::ResampleInfo()
{
   rc = ResampleInfo::NullResample;
   fillval = 0.0;
   noval = 0.0;
   ilist.clear();
   strcpy( rn, "NullResample" );
}


ResampleInfo::~ResampleInfo()
{
}


void ResampleInfo::setResampleCode( ResampleCode code )
{
   rc = code;
   switch( rc )
   {
   case NearestNeighbor:
      strcpy( rn, "NearestNeighbor" );
      break;
   case Add:
      strcpy( rn, "Add" );
      break;
   case Mean:
      strcpy( rn, "Mean" );
      break;
   case Mode:
      strcpy( rn, "Mode" );
      break;
   case Min:
      strcpy( rn, "Min" );
      break;
   case Max:
      strcpy( rn, "Max" );
      break;
   default:
      strcpy( rn, "NullResample" );
      break;
   }
}


bool ResampleInfo::setResampleCode( const QString &codeName )
{
   QString str( codeName.lower() );
   str.stripWhiteSpace();
   str.remove(' ');
   strcpy( rn, str.ascii() );

   if( str == "nearestneighbor" )
      rc = ResampleInfo::NearestNeighbor;
   else if( str == "add" )
      rc = ResampleInfo::Add;
   else if( str == "mean" )
      rc = ResampleInfo::Mean;
   else if( str == "mode" )
      rc = ResampleInfo::Mode;
   else if( str == "min" )
      rc = ResampleInfo::Min;
   else if( str == "max" )
      rc = ResampleInfo::Max;
   else
      return false;

   return true;
}


void ResampleInfo::setIgnoreList( unsigned int size, const IgnoreValue *vals )
{
   if( size == 0 || vals == NULL )
      return;

   ilist.clear();
   for( unsigned int i = 0; i < size; ++i )
      ilist.append( (IgnoreValue)vals[i] );
}


bool ResampleInfo::shouldIgnore( IgnoreValue val ) const
{
   if( val == (IgnoreValue)fillval )
      return true;

   if( val == (IgnoreValue)noval )
      return true;

   for( unsigned int i = 0; i < ilist.size(); ++i )
   {
      if( val == (IgnoreValue)ilist[i] )
         return true;
   }

   return false;
}


void ResampleInfo::copy( const ResampleInfo &src )
{
   fillval = src.fillval;
   noval = src.noval;
   ilist = src.ilist;
   rc = src.rc;
   strcpy( rn, src.rn );
}

