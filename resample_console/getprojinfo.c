/* reads the .proj file that accompanies .img file */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getprojinfo.h"

void get_projInfo(char *name, long *sys, long *zone, long *unit, long *datum, double pparm[])
{
FILE *inptr, *fopen();
char projFile[255];
int i;
double ddum;
int dum;

/* Open .proj file
  -----------------*/
strcpy(projFile, name);
strcat(projFile,".img.info");
inptr = fopen(projFile, "r");
if (!inptr) {
   printf("Error opening file:  %s\n", name);
   exit(-1);
   }

fscanf(inptr,"%d %d",&dum,&dum);	/* Get number lines, samps */
fscanf(inptr,"%d",sys);			/* Projection system code */
fscanf(inptr,"%d",zone);			/* Projection Zone code */
fscanf(inptr,"%d",unit);			/* Projection unit code */
fscanf(inptr,"%d",datum);		/* Projection Datum code */
fscanf(inptr,"%f",&ddum);		/* Image pixel size in meters */
fscanf(inptr,"%lf %lf",&ddum,&ddum);     /* Upleft proj coords */
for(i=0;i<15;i++)fscanf(inptr,"%lf",&pparm[i]); /* 15 gctpc proj parms */

fclose(inptr);
}

