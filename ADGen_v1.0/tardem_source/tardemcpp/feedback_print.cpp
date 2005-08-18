#include <stdio.h>
#include "feedback.h"
void feedback(char *message, float per,int ival)
{
	if(ival==FN_FLOOD)
	{
	   printf("Flood %s %f\n",message,per);
	}
	if(ival==FN_NET)
	{
	   printf("Netsetup %s\n",message,per);
	}
}