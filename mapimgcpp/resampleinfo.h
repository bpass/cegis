// $Id: resampleinfo.h,v 1.1 2005/11/23 00:22:38 mswilliams Exp $


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
   ResampleInfo( const ResampleInfo &src ){copy(src);}
   ~ResampleInfo(){}
   enum ResampleCode{ NullResample, NearestNeighbor, Sum, Mean, Median, Mode, Min, Max };

   void setResampleCode( ResampleCode code );
   bool setResampleCode( const QString &codeName );
   ResampleCode resampleCode() const {return rc;}
   const char * resampleName() const {return rn;}

   void setIsCategorical(bool enable){is_cat = enable;}
   bool isCategorical() const {return is_cat;}

   void setFillValue( double val ){fillval = val;}
   double fillValue() const {return fillval;}

   void setNoDataValue( double val ){noval = val;}
   double noDataValue() const {return noval;}

   void setHasNoDataValue( bool has ){hasNoDataVal = has;}
   bool hasNoDataValue(){return hasNoDataVal;}

   void setIgnoreList( unsigned int size, const IgnoreValue *vals );
   void setIgnoreList( const IgnoreList &slist ){ilist = slist;}
   const IgnoreList &ignoreList() const {return ilist;}
   bool shouldIgnore( IgnoreValue val ) const;

   void copy( const ResampleInfo &src );

   bool noDoubleCounting() const {return rc==Sum;}

   void setCacheLineCount( const int lineCount ) { lineCacheSize = lineCount; }
   int cacheLineCount() const { return lineCacheSize; }

private:
   ResampleCode rc;
   bool is_cat;
   double fillval;
   double noval;
   bool hasNoDataVal;
   IgnoreList ilist;
   int lineCacheSize;

   #define RN_MAXLENGTH 16
   char rn[RN_MAXLENGTH];
};

#endif//RESAMPLEINFO_H
