// $Id: resampleinfo.cpp,v 1.2 2005/01/31 17:24:07 jtrent Exp $


#include "resampleinfo.h"
#include <qstringlist.h> 

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

   if( str == "nearest neighbor" )
      rc = NearestNeighbor;
   else if( str == "add" )
      rc = Add;
   else if( str == "mean" )
      rc = Mean;
   else if( str == "mode" )
      rc = Mode;
   else if( str == "min" )
      rc = Min;
   else if( str == "max" )
      rc = Max;
   else
      return false;

   return true;
}

void ResampleInfo::copy( const ResampleInfo &src )
{
   fillval = src.fillval;
   noval = src.noval;
   ilist = src.ilist;
}

