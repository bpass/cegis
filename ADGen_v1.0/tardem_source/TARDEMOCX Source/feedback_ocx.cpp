#include <stdio.h>
#include "lsm.h"
#include "feedback.h"
void feedback(char *message, float per,int ival)
{
	if(ival==FN_FLOOD)
	{
	   ocxCtrl->FloodEvent((short)per);
	}
}