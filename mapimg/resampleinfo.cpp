// $Id: resampleinfo.cpp,v 1.4 2005/02/10 15:48:20 jtrent Exp $


#include "resampleinfo.h"
#include <qstringlist.h>

void ResampleInfo::setIgnoreList( unsigned int size, IgnoreValue *vals )
{
   if( size == 0 || vals == NULL )
      return;

   ilist.clear();
   for( unsigned int i = 0; i < size; ++i )
      ilist.append( (IgnoreValue)vals[i] );
}


bool ResampleInfo::shouldIgnore( IgnoreValue val )
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


ResampleInfo::ResampleInfo()
{
   rc = ResampleInfo::NullResample;
   fillval = 0.0;
   noval = 0.0;
   ilist.clear();
}

ResampleInfo::~ResampleInfo()
{}

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

void ResampleInfo::copy( const ResampleInfo &src )
{
   fillval = src.fillval;
   noval = src.noval;
   ilist = src.ilist;
   rc = src.rc;
}

