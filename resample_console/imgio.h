#ifndef IMGIO_H
#define IMGIO_H

#define OK      0
#define INFILE_NAME	1
#define OUTFILE_NAME	2




 
struct IMGINFO 
{
   double pparm[15];		/* 15 GCTP projection parameters */
   double ul_y, ul_x;           /* Upper left projection corner coordinates */
   float pixsize;               /* Image pixel size in meters */
   long nl,ns;                  /* Number of lines and samples in image */
   long sys;			/* Projection system code */
   long zone;			/* Projection zone code */
   long unit;
   long datum;			/* Datum code (spheroid code for now) */
};

   
 
#endif

