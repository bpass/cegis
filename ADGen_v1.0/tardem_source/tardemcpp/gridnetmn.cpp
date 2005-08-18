/* Program to compute longest path length and grid network order 
   for a DEM grid  */

/* Created by David G Tarboton  */
/* Utah State University       */

/*  The modified code calculates the length of the longest stream.        */
/*  The code also computes the longest lengths of all sub basins in one   */
/*  go,so the area file and the ld file can be used to examine basin      */
/*  geomorphologic characteristics, such as, Hack's Law.                  */     


#include "tardemlib.h"
#include <string.h>

void main(int argc, char **argv)
 {
   char pfile[MAXLN],afile[MAXLN],tlenfile[MAXLN],plenfile[MAXLN];
	char ans[MAXLN],gordfile[MAXLN];

   int err,mask=0,outlet=0,nmain,tresh,i;
     long x=0, y=0;

  if(argc < 2)
  {
     printf("\nTo avoid manual file name entry this program may\n");
	 printf("be used with the following command\n"); 
     printf("gridnet <filename_prefix> [-m mask_file mask_threshold] [-xy outlet_x outlet_y]\n");
     printf("Input existing D8 flow directions file name (ends with p suffix)\n");
	 scanf("%s",pfile);
	 printf("Do you want to use a mask file (y[es]/n[o])\n");
	 scanf("%s",&ans);
	 if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0)
	 {
		 printf("Input maskfilename\n");
		 scanf("%s",afile);
		 printf("Input integer mask threshold (for >= test)\n");
		 scanf("%d",&tresh);
		 mask=1;
	 }
	 else
	 {
		 tresh=0;
	 }
	 printf("Do you want to input outlet coordinates ((y[es]/n[o])\n");
	 scanf("%s",&ans);
	 if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0)
	 {
		 printf("Input outlet_x, outlet_y\n");
		 scanf("%lf%lf",&x,&y);
		 outlet=1;
	 }
	 printf("Input file names for output\n");
	 printf("Input file name for path length grid\n");
	 scanf("%s",plenfile);
	 printf("Input file name for total length grid\n");
	 scanf("%s",tlenfile);
 	 printf("Input file name for grid (Strahler) order\n");
	 scanf("%s",gordfile);
  }
  else
  {
	nmain=nameadd(pfile,argv[1],"p");
	nmain=nameadd(plenfile,argv[1],"plen"); 
	nmain=nameadd(tlenfile,argv[1],"tlen"); 
	nmain=nameadd(gordfile,argv[1],"gord");
    i=2;
	while(argc > i)
	{
		if(strcmp(argv[i],"-m")==0)
		{
			i++;
			if(argc > i+1)
			{
				sscanf(argv[i],"%s",afile);
				sscanf(argv[i+1],"%d",&tresh);
				mask=1;
				i=i+2;
			}
		}
		else if(strcmp(argv[i],"-xy")==0)
		{
			i++;
			if(argc > i+1)
			{
				sscanf(argv[i],"%ld",&x);
				sscanf(argv[i+1],"%ld",&y);
				outlet=1;
				i=i+2;
			}
		}
		else i++;
	}
  }
  if(err=gridnet(pfile,plenfile,tlenfile,gordfile,afile,x,y,mask,outlet,tresh) != 0)
  printf("Gridnet error %d\n",err);
}