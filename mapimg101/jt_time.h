// $Id: jt_time.h,v 1.2 2005/01/06 19:55:34 jtrent Exp $


#ifndef JT_TIME_H
#define JT_TIME_H


#include <qdatetime.h>

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
