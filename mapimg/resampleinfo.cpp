// $Id: resampleinfo.cpp,v 1.5 2005/02/13 19:12:30 rbuehler Exp $


#include "resampleinfo.h"
#include <qstringlist.h>


ResampleInfo::ResampleInfo()
{
   rc = ResampleInfo::NullResample;
   fillval = 0.0;
   noval = 0.0;
   ilist.clear();
   rn = NULL;
}


ResampleInfo::~ResampleInfo()
{
   if( rn != NULL )
      delete [] rn;
}


bool ResampleInfo::setResampleCode( QString codeName )
{
   QString str( codeName.lower() );
   str.stripWhiteSpace();
   str.remove(' ');

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


/*const char *ResampleInfo::resampleName() const
{
   if( rn != NULL )
      delete [] rn;

   switch( rc )
   {
   case NearestNeighbor:
      printf( "Nearest neighbor" );
      break;
   case Add:
      rn = new char[]; ...
      return rn;
      break;
   case Mean:
      printf( "Mean" );
      break;
   case Mode:
      printf( "Mode" );
      break;
   case Min:
      printf( "Min" );
      break;
   case Max:
      printf( "Max" );
      break;
   case NullResample:
      printf( "NullResample" );
      break;
   default:
      printf( "Bad resample code!!!" );
      break;
   }
   return NULL;
}*/


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
}

