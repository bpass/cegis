// $Id: resampleinfo.h,v 1.4 2005/02/13 19:12:30 rbuehler Exp $


#ifndef RESAMPLEINFO_H
#define RESAMPLEINFO_H

#include <qvaluelist.h>

typedef double IgnoreValue;
typedef QValueList<IgnoreValue> IgnoreList;

//class QStringList;

class ResampleInfo
{
public:
   ResampleInfo();
   ResampleInfo( const ResampleInfo &src ){rn=NULL;copy(src);}
   ~ResampleInfo();
   enum ResampleCode{ NullResample, NearestNeighbor, Add, Mean, Mode, Min, Max };

   void setResampleCode( ResampleCode code ){rc = code;}
   bool setResampleCode( QString codeName );
   ResampleCode resampleCode() const {return rc;}
//   const char * resampleName() const;

   void setFillValue( double val ){fillval = val;}
   double fillValue() const {return fillval;}

   void setNoDataValue( double val ){noval = val;}
   double noDataValue() const {return noval;}

   void setIgnoreList( unsigned int size, const IgnoreValue *vals );
   void setIgnoreList( const IgnoreList &slist ){ilist = slist;}
   const IgnoreList &ignoreList() const {return ilist;}
   bool shouldIgnore( IgnoreValue val ) const;

   void copy( const ResampleInfo &src );

private:
   ResampleCode rc;
   double fillval;
   double noval;
   IgnoreList ilist;
   char *rn;
};

#endif//RESAMPLEINFO_H
