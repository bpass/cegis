/*  areadinf.c. Program to compute area contributing to each Pixel in DEM 
   for cell outflow based on angles.
     
  David G Tarboton
  Utah State University   
  
   */
  

#include "tardemlib.h"
#include <string.h>

void main(int argc,char **argv)
{
   char pfile[MAXLN],afile[MAXLN],wfile[MAXLN];
   int err,nmain, doall=1,usew=0,contcheck=1,i;
   long x=0, y=0;  
   if(argc < 2)
    {  
       printf("Usage:\n %s <filename_prefix> [-xy outlet_x outlet_y] [-wg <weightfile>] [-nc]\n",argv[0]);
       printf("(The optional outlet_x and outlet_y are outlet coordinates\n");
       printf("for the area to be computed.  If they are not given \n");
       printf("the whole file is computed.\n");
	   printf("The optional <weightfile> is the file name for a grid of weights for\n");
	   printf("weighted contributing area evaluation\n");
       printf("The flag -nc overrides edge contamination checking\n");
       printf("The following are appended to the file names\n");
       printf("the files are opened:\n");
       printf("ang    D inf flow directions(Input)\n");
       printf("sca    Specific Catchment Area (Output)\n");
	   printf("wsca   Weighted Dinf areas\n");
       exit(0);  
    }
    i=2;
	while(argc > i)
	{
		if(strcmp(argv[i],"-wg")==0)
		{
			i++;
			if(argc > i)
			{
				sscanf(argv[i],"%s",wfile);
				usew=1;
				i=i+1;
			}
		}
		else if(strcmp(argv[i],"-xy")==0)
		{
			i++;
			if(argc > i+1)
			{
				sscanf(argv[i],"%ld",&x);
				sscanf(argv[i+1],"%ld",&y);
				doall = 0;
				i=i+2;
			}
		}
		else if(strcmp(argv[i],"-nc")==0)
		{
			i++;
			contcheck=0;
		}
		else i++;
	}
    nmain=nameadd(pfile,argv[1],"ang");
	if(usew == 1)
	{
		nmain=nameadd(afile,argv[1],"wsca");
	}
	else
	{
		nmain=nameadd(afile,argv[1],"sca");
	}    
    if(err=area(pfile,afile,x,y,doall,wfile,usew,contcheck) != 0)
        printf("area error %d\n",err);
} 
