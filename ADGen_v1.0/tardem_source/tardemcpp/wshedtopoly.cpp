#include "gridio.h"
#include <string.h>
# include <iostream.h>
# include <fstream.h>
#include <stdio.h>
# include "PolygonMap.h"

void main(int argc, char **argv)
{
  char wfile[MAXLN],pfile[MAXLN],shapefile[MAXLN];
  int nmain;

  if(argc < 2)
  {
     printf("\nTo avoid manual file name entry this program may\n");
	 printf("be used with the following command\n"); 
     printf("wshedtopoly <filename_prefix> \n");
	 printf("Input existing watershed file\n");
	 scanf("%s",wfile);
	 printf("Input existing flow direction file (ends in p)\n");
	 scanf("%s",pfile);
	 printf("Input name for output shape file (without extension)\n");
	 scanf("%s",shapefile);
  }
  else
  { 
	nmain=nameadd(wfile,argv[1],"w");
	nmain=nameadd(pfile,argv[1],"p");
	strcpy(shapefile,"");
	strncat(shapefile,argv[1],nmain);
    strcat(shapefile,"w");
  }
	RasterToShape ras_to_shape;
	ras_to_shape.createShape( wfile, pfile, shapefile);
}