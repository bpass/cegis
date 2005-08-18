/*  File: d8.c 
	Program to compute d8 flow directions and slopes.

  
	This program provides the main() function for calling the 
	functions in setdir.c.  

*/
#include "tardemlib.h"

void main(int argc,char **argv)
{
  char demfile[MAXLN], pointfile[MAXLN], slopefile[MAXLN], flowfile[MAXLN];
  int err,nmain;
    short useflowfile=0;

   if(argc < 2)
    {  
       printf("Usage:\n %s filename [fdrfile]\n",argv[0]);
       printf("The following are appended BEFORE\n");
       printf("the files are opened:\n");
       printf("fel    Flooded Elevation data\n");
       printf("p      D8 flow directions\n");
       printf("sd8    D8 slope data\n");
	   printf("optional fdr file of existing stream directions\n");
       exit(0);  
    }
    nmain=nameadd(pointfile,argv[1],"p");
	nmain=nameadd(slopefile,argv[1],"sd8");
    nmain=nameadd(demfile,argv[1],"fel");
	if(argc > 2)
	{
		useflowfile=1;
		nmain=nameadd(flowfile,argv[2],"n");
	}

    if(err=setdird8(demfile, pointfile, slopefile,flowfile,useflowfile) != 0)
        printf("setdird8 error %d\n",err);
}    
