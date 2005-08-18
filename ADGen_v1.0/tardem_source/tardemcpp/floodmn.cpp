/*  File: floodmn.c 
	Program to provide main for calling flood function in flood.c
	Begining date: 97/8/30
	Publish date: 
	Version 0.1
  
	This program provides the main() function for calling the flood 
	function in flood.c.  This allows use of flood in a stand-alone
	console environment with the flood.c routines linked in as a
	library.  

*/
#include "tardemlib.h"

void main(int argc,char **argv)
{
  char demfile[MAXLN], newfile[MAXLN],flowfile[MAXLN],newflowfile[MAXLN];
  int err,nmain;
  short useflowfile=0;

   if(argc < 2)
    {  
       printf("Usage:\n %s filename [fdrfile]\n",argv[0]);
       printf("The following are appended BEFORE\n");
       printf("the files are opened:\n");
       printf("(no suffix)    Elevation data\n");
       printf("fel    Flooded elevation data\n");
	   printf("optional fdr file of existing stream directions\n");
	   printf("n    appended to existing stream directions during fix\n");
       exit(0);  
    }
    nmain=nameadd(newfile,argv[1],"fel");
	if(argc > 2)
	{
		useflowfile=1;
		sprintf(flowfile,"%s",argv[2]);
		nmain=nameadd(newflowfile,argv[2],"n");
	}

    sprintf(demfile,"%s",argv[1]);

    if(err=flood(demfile, newfile,flowfile,useflowfile,newflowfile) != 0)
        printf("Flood error %d\n",err);
}    
