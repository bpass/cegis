// $Id: report.c,v 1.4 2005/02/01 16:08:13 jtrent Exp $


//Copyright 1991 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

/*******************************************************************************
NAME                    Projection support routines listed below

PURPOSE:	The following functions are included in REPORT.C	

		INIT:
			Initializes the output device for error messages and
		  	report headings.

		P_ERROR:
			Reports errors to the terminal, a specified file, or
			both.

		PTITLE, RADIUS, RADIUS2, CENLON, CENLONMER, CENLAT, ORIGIN,
		STANPARL, STPARL1, OFFSET, GENRPT, GENRPT_LONG, PBLANK:
			Reports projection parameters to the terminal,
			specified file, or both. 


PROGRAMMER              DATE		REASON
----------              ----		------
D. Steinwand, EROS      July, 1991	Initial development.
T. Mittan		Mar,  1993	Adapted code to new "C" version of
					GCTP library.
S. Nelson		Jun, 1993	Added inline code.
					Added error messages if no filename
					was specified.
S. Nelson		Jan, 1998	Returned OK instead of 0.
J. Trent                Sept, 2003      Now can print to a custom FILE*

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "cproj.h"

#define TRUE 1
#define FALSE 0

static long terminal_p;		/* flag for printing parameters to terminal */
static long terminal_e;		/* flag for printing errors to terminal */
static long file_p;		/* flag for printing parameters to file */
static long file_e;		/* flag for printing errors to file */
static long pFile_p_flag;       /* flag for printing parameters to file pointer */
static long pFile_e_flag;	/* flag for printing errors to file pointer */
static FILE  *fptr_p;
static FILE  *fptr_e;
static FILE  *pFile_p;
static FILE  *pFile_e;
static char parm_file[256];
static char err_file[256];

/* initialize output device
-------------------------*/
long init(ipr,jpr,efile,pfile,p_File,e_File)

long ipr;		/* flag for printing errors (0,1,2, or 3)	*/
long jpr;		/* flag for printing parameters (0,1,2, or 3)	*/
char *efile;		/* name of error file				*/
char *pfile;		/* name of parameter file			*/
FILE *p_File;           /* precreated file pointer for parameters       */
FILE *e_File;           /* precreated file pointer for errors           */
{
if (ipr == 0)
   {
   terminal_e = TRUE;
   file_e = FALSE;
   }
else
if (ipr == 1)
   {
   terminal_e = FALSE;
   if (strlen(efile) == 0)
      {
      return(6);
      }
   file_e = TRUE;
   strncpy(err_file,efile, 256);
   err_file[255] = '\0';
   }
else
if (ipr == 2)
   {
   terminal_e = TRUE;
   if (strlen(efile) == 0)
      {
      file_e = FALSE;
      p_error("Output file name not specified","report-file");
      return(6);
      }
   file_e = TRUE;
   strncpy(err_file,efile, 256);
   err_file[255] = '\0';
   }
else
if (ipr == 3)
   {
   terminal_e = FALSE;
   file_e = FALSE;
   if (e_File == NULL)
      {
      pFile_e_flag = FALSE;
      p_error("Precreated FILE* pointer invalid","report-file");
      return(6);
      }
   pFile_e_flag = TRUE;
   pFile_e = e_File;
   }
else
   {
   terminal_e = FALSE;
   file_e = FALSE;
   }
if (jpr == 0)
   {
   terminal_p = TRUE;
   file_p = FALSE;
   }
else
if (jpr == 1)
   {
   terminal_p = FALSE;
   if (strlen(pfile) == 0)
      {
      return(6);
      }
   file_p = TRUE;
   strncpy(parm_file,pfile, 256);
   parm_file[255] = '\0';
   }
else
if (jpr == 2)
   {
   terminal_p = TRUE;
   if (strlen(pfile) == 0)
      {
      file_p = FALSE;
      p_error("Output file name not specified","report-file");
      return(6);
      }
   file_p = TRUE;
   strncpy(parm_file,pfile, 256);
   parm_file[255] = '\0';
   }
else
if (jpr == 3)
   {
   terminal_p = FALSE;
   file_p = FALSE;
   if (p_File == NULL)
      {
      pFile_p_flag = FALSE;
      p_error("Precreated FILE* pointer invalid","report-file");
      return(6);
      }
   pFile_p_flag = TRUE;
   pFile_p = p_File;
   }
else
   {
   terminal_p = FALSE;
   file_p = FALSE;
   }
return(OK);
}

void close_file()
{
if (fptr_e != NULL)
   fclose(fptr_e);
if (fptr_p != NULL)
   fclose(fptr_p);
}

/* Functions to report projection parameters
  -----------------------------------------*/
void ptitle(A)
  char *A;
      {
      if (terminal_p)
           printf("\n%s PROJECTION PARAMETERS:\n\n",A);
      if (file_p)
	   {
           fptr_p = (FILE *)fopen(parm_file,"a");
           fprintf(fptr_p,"\n%s PROJECTION PARAMETERS:\n\n",A);
	   fclose(fptr_p);
	   }
      if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      	   {
           fprintf(pFile_p,"\n%s PROJECTION PARAMETERS:\n\n",A);
	   }
      }

void radius(A)
  double A;
      {
      if (terminal_p)
         printf("   Radius of Sphere:     %lf meters\n",A);
      if (file_p)
	 {
         fptr_p = (FILE *)fopen(parm_file,"a");
         fprintf(fptr_p,"   Radius of Sphere:     %lf meters\n",A);
	 fclose(fptr_p);
         }
         if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
	 {
         fprintf(pFile_p,"   Radius of Sphere:     %lf meters\n",A);
	 }
      }

void radius2(A,B)
  double A,B;
      {
      if (terminal_p)
         {
         printf("   Semi-Major Axis of Ellipsoid:     %lf meters\n",A);
         printf("   Semi-Minor Axis of Ellipsoid:     %lf meters\n",B);
         }
      if (file_p)
         {
         fptr_p = (FILE *)fopen(parm_file,"a");
         fprintf(fptr_p,"   Semi-Major Axis of Ellipsoid:     %lf meters\n",A);
         fprintf(fptr_p,"   Semi-Minor Axis of Ellipsoid:     %lf meters\n",B);
	 fclose(fptr_p);
         }
      if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
         {
         fprintf(pFile_p,"   Semi-Major Axis of Ellipsoid:     %lf meters\n",A);
         fprintf(pFile_p,"   Semi-Minor Axis of Ellipsoid:     %lf meters\n",B);
         }
      }

void cenlon(A)
  double A;
   {
   if (terminal_p)
       printf("   Longitude of Center:     %lf degrees\n",A*R2D);
   if (file_p)
       {
       fptr_p = (FILE *)fopen(parm_file,"a");
       fprintf(fptr_p,"   Longitude of Center:     %lf degrees\n",A*R2D);
       fclose(fptr_p);
       }
   if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
       {
       fprintf(pFile_p,"   Longitude of Center:     %lf degrees\n",A*R2D);
       }
   }

void cenlonmer(A)
  double A;
   {
   if (terminal_p)
     printf("   Longitude of Central Meridian:     %lf degrees\n",A*R2D);
   if (file_p)
     {
     fptr_p = (FILE *)fopen(parm_file,"a");
    fprintf(fptr_p,"   Longitude of Central Meridian:     %lf degrees\n",A*R2D);
     fclose(fptr_p);
     }
     if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
     {
    fprintf(pFile_p,"   Longitude of Central Meridian:     %lf degrees\n",A*R2D);
     }
   }

void cenlat(A)
  double A;
   {
   if (terminal_p)
      printf("   Latitude  of Center:     %lf degrees\n",A*R2D);
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   Latitude of Center:     %lf degrees\n",A*R2D);
      fclose(fptr_p);
      }
   if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   Latitude of Center:     %lf degrees\n",A*R2D);
     }
   }

void origin(A)
  double A;
   {
   if (terminal_p)
      printf("   Latitude of Origin:     %lf degrees\n",A*R2D);
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   Latitude  of Origin:     %lf degrees\n",A*R2D);
      fclose(fptr_p);
      }
   if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   Latitude  of Origin:     %lf degrees\n",A*R2D);
      }
   }
void stanparl(A,B)
  double A,B;
   {
   if (terminal_p)
      {
      printf("   1st Standard Parallel:     %lf degrees\n",A*R2D);
      printf("   2nd Standard Parallel:     %lf degrees\n",B*R2D);
      }
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   1st Standard Parallel:     %lf degrees\n",A*R2D);
      fprintf(fptr_p,"   2nd Standard Parallel:     %lf degrees\n",B*R2D);
      fclose(fptr_p);
      }
  if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   1st Standard Parallel:     %lf degrees\n",A*R2D);
      fprintf(pFile_p,"   2nd Standard Parallel:     %lf degrees\n",B*R2D);
      }
   }

void stparl1(A)
  double A;
   {
   if (terminal_p)
      {
      printf("   Standard Parallel:     %lf degrees\n",A*R2D);
      }
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   Standard Parallel:     %lf degrees\n",A*R2D);
      fclose(fptr_p);
      }
   if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   Standard Parallel:     %lf degrees\n",A*R2D);
      }
   }

void offsetp(A,B)
  double A,B;
   {
   if (terminal_p)
      {
      printf("   False Easting:      %lf meters \n",A);
      printf("   False Northing:     %lf meters \n",B);
      }
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   False Easting:      %lf meters \n",A);
      fprintf(fptr_p,"   False Northing:     %lf meters \n",B);
      fclose(fptr_p);
      }
  if (pFile_p_flag && pFile_p) //if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   False Easting:      %lf meters \n",A);
      fprintf(pFile_p,"   False Northing:     %lf meters \n",B);
      }
   }

void genrpt(A,S)
  double A; char *S;
   {
   if (terminal_p)
      printf("   %s %lf\n", S, A);
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   %s %lf\n", S, A);
      fclose(fptr_p);
      }
   if (pFile_p_flag && pFile_p) //if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   %s %lf\n", S, A);
      }
   }
void genrpt_long(A,S)
  long A; char *S;
   {
   if (terminal_p)
      printf("   %s %ld\n", S, A);
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"   %s %ld\n", S, A);
      fclose(fptr_p);
      }
  if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"   %s %ld\n", S, A);
      }
   }
void pblank()
   {
   if (terminal_p)
      printf("\n");
   if (file_p)
      {
      fptr_p = (FILE *)fopen(parm_file,"a");
      fprintf(fptr_p,"\n");
      fclose(fptr_p);
      }
   if (pFile_p_flag && pFile_p)//if pFile_p is not a valid open FILE * the parameter will be lost
      {
      fprintf(pFile_p,"\n");
      }
   }

/* Function to report errors
  -------------------------*/
void p_error(what, where)
   char *what;
   char *where;
   {
   if (terminal_e)
      printf("[%s] %s\n",where,what);
   if (file_e)
      {
      fptr_e = (FILE *)fopen(err_file,"a");
      fprintf(fptr_e,"[%s] %s\n",where,what);
      fclose(fptr_e);
      }
   if (pFile_e_flag && pFile_e) //if pFile_e is not a valid open FILE * the error will be lost
      {
      fprintf(pFile_e,"[%s] %s\n",where,what);
      }
   }
