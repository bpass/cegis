/*  File: dinf.c 
	Program to compute dinf flow angles and slopes.
  
	This program provides the main() function for calling the setdir 
	function.  This allows use of dinf in a stand-alone
	console environment and for debugging.

*/
#include "tardemlib.h"            

void main(int argc,char **argv)
{
  char demfile[MAXLN],slopefile[MAXLN],angfile[MAXLN],pfile[MAXLN],flowfile[MAXLN];
  int err,nmain;
  short useflowfile=0;
   if(argc < 2)
    {  
       printf("Usage:\n %s filename [fdrfile]\n",argv[0]);
       printf("The following are appended BEFORE\n");
       printf("the files are opened:\n");
       printf("fel    Pit filled elevation data (input)\n");
       printf("ang    D inf flow directions(output)\n");
       printf("slp    Slopes (Output)\n");
	   printf("p    Single drainage directions (Output)\n");
	   printf("optional fdr file of existing stream directions\n");
       exit(0);  
    }
    nmain=nameadd(demfile,argv[1],"fel");
    nmain=nameadd(angfile,argv[1],"ang");
	nmain=nameadd(slopefile,argv[1],"slp");
    nmain=nameadd(pfile,argv[1],"p");
	if(argc > 2)
	{
		useflowfile=1;
		sprintf(flowfile,"%s",argv[2]);
	}
    if(err=setdir(demfile, angfile, slopefile,flowfile,useflowfile) != 0)
        printf("Setdir error %d\n",err); 
}     
