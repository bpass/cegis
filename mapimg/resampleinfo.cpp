// $Id: resampleinfo.cpp,v 1.9 2005/03/04 14:58:35 jtrent Exp $


#include "resampleinfo.h"
#include <qstringlist.h>


ResampleInfo::ResampleInfo()
{
   rc = ResampleInfo::NullResample;
   fillval = 0.0;
   noval = 0.0;
   ilist.clear();
   strncpy( rn, "NullResample", RN_MAXLENGTH );
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
      strncpy( rn, "NearestNeighbor", RN_MAXLENGTH );
      break;
   case Add:
      strncpy( rn, "Add", RN_MAXLENGTH );
      break;
   case Mean:
      strncpy( rn, "Mean", RN_MAXLENGTH );
      break;
   case Median:
      strncpy( rn, "Median", RN_MAXLENGTH );
      break;
   case Mode:
      strncpy( rn, "Mode", RN_MAXLENGTH );
      break;
   case Min:
      strncpy( rn, "Min", RN_MAXLENGTH );
      break;
   case Max:
      strncpy( rn, "Max", RN_MAXLENGTH );
      break;
   default:
      strncpy( rn, "NullResample", RN_MAXLENGTH );
      break;
   }
}


bool ResampleInfo::setResampleCode( const QString &codeName )
{
   QString str( codeName.lower() );
   str.stripWhiteSpace();
   str.remove(' ');
   strncpy( rn, str.ascii(), RN_MAXLENGTH );

   if( str == "nearestneighbor" )
      rc = ResampleInfo::NearestNeighbor;
   else if( str == "add" )
      rc = ResampleInfo::Add;
   else if( str == "mean" )
      rc = ResampleInfo::Mean;
   else if( str == "median" )
      rc = ResampleInfo::Median;
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

bool ResampleInfo::noDoubleCounting() const
{
  bool returnValue = false;
  
  if( rc == ResampleInfo::Add )
      returnValue = true;
      
  return returnValue;
}


void ResampleInfo::copy( const ResampleInfo &src )
{
   fillval = src.fillval;
   noval = src.noval;
   ilist = src.ilist;
   rc = src.rc;
   strncpy( rn, src.rn, RN_MAXLENGTH );
}

