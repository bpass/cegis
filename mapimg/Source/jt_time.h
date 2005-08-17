// $Id: jt_time.h,v 1.2 2005/08/17 19:42:34 lwoodard Exp $


#ifndef JT_TIME_H
#define JT_TIME_H


#include <QDateTime>

/*
This function is used to time the reprojection.
You simply tell it to start timing, and then, when the algorithm is complete,
you tell it to stop and it returns the time spent.
*/
class jt_time
{
public:
   void start() { qTime = new QTime; qTime->start(); return; }
   QString stop()
   {
      QString qRetVal = "0";
      if( qTime != NULL )
      {
         QTime qTimeHolder( 0, 0, 0, 0);
         qTimeHolder = qTimeHolder.addMSecs( qTime->elapsed() );
         qRetVal = qTimeHolder.toString( "hh:mm:ss.zzz" );
         delete qTime;
      }
      return qRetVal;
   }


private:
   QTime *qTime;
};


#endif
