// $Id: resampleinfo.h,v 1.2 2005/01/31 17:24:07 jtrent Exp $


#ifndef RESAMPLEINFO_H
#define RESAMPLEINFO_H

#include <qvaluelist.h>
typedef double IgnoreValue;
typedef QValueList<IgnoreValue> IgnoreList;

class QStringList;

class ResampleInfo
{
public:
   ResampleInfo();
   ResampleInfo( const ResampleInfo &src ){copy(src);}
   ~ResampleInfo();
   enum ResampleCode{ NullResample, NearestNeighbor, Add, Mean, Mode, Min, Max };

   void setResampleCode( ResampleCode code ){rc = code;}
   bool setResampleCode( QString codeName );
   ResampleCode resampleCode(){return rc;}

   void setFillValue( double val ){fillval = val;}
   double fillValue(){return fillval;}

   void setNoDataValue( double val ){noval = val;}
   double noDataValue(){return noval;}

   template<class T>
      void setIgnoreList( unsigned int size, T *vals );
   void setIgnoreList( IgnoreList &slist ){ilist = slist;}
   IgnoreList &ignoreList(){return ilist;}
   template<class T>
      bool shouldIgnore( T val );

   void copy( const ResampleInfo &src );

private:
   ResampleCode rc;
   double fillval;
   double noval;
   IgnoreList ilist;
};

template<class T>
void ResampleInfo::setIgnoreList( unsigned int size, T *vals )
{
   if( size == 0 || vals == NULL )
      return;

   ilist.clear();
   for( unsigned int i = 0; i < size; ++i )
      ilist.append( (IgnoreValue)vals[i] );
}


template<class T>
bool ResampleInfo::shouldIgnore( T val )
{
   if( val == (T)fillval )
      return true;

   if( val == (T)noval )
      return true;

   for( unsigned int i = 0; i < sz; ++i )
   {
      if( val == (T)ilist[i] )
         return true;
   }

   return false;
}

#endif//RESAMPLEINFO_H
