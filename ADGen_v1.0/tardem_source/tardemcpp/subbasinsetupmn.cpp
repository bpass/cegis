/*  Program to set up subbasins file

  David G Tarboton
  Utah State University  
   */
  
#include "tardemlib.h"
#include <string.h>
# include "PolygonMap.h"

int main(int argc, char **argv)
{
  char treefile[MAXLN], coordfile[MAXLN],pfile[MAXLN],wfile[MAXLN],shpfile[MAXLN],
	  shpfilew[MAXLN],temp[MAXLN];
  int err, nmain, ordert;

  if(argc < 3)
  {
     printf("\nTo avoid manual file name entry this program may\n");
	 printf("be used with the following command\n"); 
     printf("subbasinsetup <filename_prefix orderthresh> \n");
     printf("Input existing D8 flow directions file name (ends with p suffix)\n");
	 scanf("%s",pfile);
	 printf("Input existing channel network tree file (ends with tree.dat)\n");
	 scanf("%s",treefile);
	 printf("Input existing channel network coordinates file (ends with coord.dat)\n");
	 scanf("%s",coordfile);
	 printf("Input order threshold for subbasin delineation\n");
	 scanf("%d",&ordert);
	 printf("Input file name for subbasin grid output (ends with w)\n");
	 scanf("%s",wfile);
	 printf("Input file name for channel network shape output (ends with .shp)\n");
	 scanf("%s",shpfile);
	 printf("Input file name for watershed shape output (ends with .shp)\n");
	 scanf("%s",shpfilew);
//  Now strip it of an extension if it is there
	 nmain=nameadd(temp,shpfilew,"");
	 strcpy(shpfilew,"");
	 strncat(shpfilew,temp,nmain);   
  }
  else
  {
	sscanf(argv[2],"%d",&ordert);
	nmain=nameadd(pfile,argv[1],"p"); 
	nmain=nameadd(wfile,argv[1],"w");

	/*  Non grid files  */
	strcpy(treefile,"");
    strncat(treefile,argv[1],nmain);
    strcat(treefile,"tree.dat");
	strcpy(coordfile,"");
	strncat(coordfile,argv[1],nmain);
    strcat(coordfile,"coord.dat");
	strcpy(shpfile,"");
	strncat(shpfile,argv[1],nmain);
    strcat(shpfile,".shp");
	strcpy(shpfilew,"");
	strncat(shpfilew,argv[1],nmain);
    strcat(shpfilew,"w");
  }

if(err=subbasinsetup(pfile,wfile,treefile,coordfile,shpfile,ordert)!= 0)
{
    printf("Subbasinsetup error %d\n",err);
	return(err);
}
	RasterToShape ras_to_shape;
	ras_to_shape.createShape( wfile, pfile, shpfilew);
	return(0);
}