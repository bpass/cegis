/* reads the .geoinfo file that accompanies .img file */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getgeoinfo.h"

void get_geoInfo(name, nl, ns, nb, ulx, uly, pixSiz)
char *name;
float *pixSiz;
double *ulx;
double *uly;
long *ns;
long *nl;
long *nb;
{
FILE *inptr, *fopen();
char geoFile[255];
int dum;

/* Open geoInfo file
  -----------------*/
strcpy(geoFile, name);
strcat(geoFile,".img.info");
inptr = fopen(geoFile, "r");
if (!inptr) {
   printf("Error opening geoInfo file:  %s\n", name);
   exit(-1);
   }
fscanf(inptr,"%d %d",nl,ns);	/* Get number lines, samps */
fscanf(inptr,"%d",&dum);			/* Projection system code */
fscanf(inptr,"%d",&dum);			/* Projection Zone code */
fscanf(inptr,"%d",&dum);			/* Projection unit code */
fscanf(inptr,"%d",&dum);		/* Projection Datum code */
fscanf(inptr,"%f",pixSiz);		/* Image pixel size in meters */
fscanf(inptr,"%lf %lf",ulx,uly);     /* Upleft proj coords */

fclose(inptr);
}

