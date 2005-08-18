
/* lsm.h
   Header file for C functions in SINMAP and TARDEM
     
  David G Tarboton and Craig Goodwin
  Utah State University   

*/

/*  Copyright (C) 2000  David Tarboton, Utah State University

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
version 2, 1991 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

A copy of the full GNU General Public License is included in file 
gpl.html. This is also available at:
http://www.gnu.org/copyleft/gpl.html
or from:
The Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
Boston, MA  02111-1307, USA.

If you wish to use or incorporate this program (or parts of it) into 
other software that does not meet the GNU General Public License 
conditions contact the author to request permission.
David G. Tarboton                                                      
Utah State University 
8200 Old Main Hill 
Logan, UT 84322-8200 
USA 
http://www.engineering.usu.edu/dtarb/ 
email:  dtarb@cc.usu.edu 
*/

/*  Toggle between these depending upon platform  */
/*  #define WIN  */
#define WIN

#include "stdafx.h"
#include "TardemCtl.h"
#include "gridio.h"
#include "feedback.h"

/*	Defines */
#define PI2 1.5707963268

/* Functions in tardem.dll that are callable from outside */
/*
#define DllExport __declspec( dllexport )   

DllExport int flood(char *demfile, char *newfile);
DllExport int setdir(char *demfile, char *angfile, char *slopefile); 
DllExport int setdird8(char *demfile, char *pointfile, char *slopefile);             
DllExport int area(char *pfile, char *afile, double x, double y, int doall, 
		   char *wfile, int usew, int contcheck);              
DllExport int aread8(char *pfile, char *afile, double x, double y, int doall, 
		   char *wfile, int usew, int contcheck);
DllExport int gridnet(char *pfile,char *plenfile,char *tlenfile,char *gordfile,char *afile,
			double x,double y, int mask, int outlet, int thresh);
DllExport int netsetup(char *areafile, char *slopefile,char *plenfile,char *pfile,
			 char *srcfile,char *elevfile, char *gordfile,char *treefile, 
			 char *coordfile, char *ordfile, char *scafile, int method,
			 float *p, double xr,double yr);
//DllExport void arclinks(char *coordfile,char *treefile,char *outfile, int *ilink, float *amin);
//DllExport void arcstreams(char *coordfile,char *treefile,char *outfile, int *ilink, float *amin);  
DllExport int subbasinsetup(char *pfile, char *wfile,char *treefile,char *coordfile,int ordert);
DllExport int depgrd(char *angfile,char *dgfile, char *depfile);
DllExport void MaxAreaD8Coords(char* areafile, long *x, long *y); 
*/

int flood(char *demfile, char *newfile, char * flowfile, short useflowfile, char *newflowfile);

int setdir(char *demfile, char *angfile, char *slopefile, char * flowfile, 
			 short useflowfile); 
int setdird8(char *demfile, char *pointfile, char *slopefile, char * flowFile,
			 short useflowfile);             
int area(char *pfile, char *afile, long x, long y, int doall, 
         char *wfile, int usew, int contcheck);              
int aread8(char *pfile, char *afile, long x, long y, int doall, 
		   char *wfile, int usew, int contcheck);
int gridnet(char *pfile,char *plenfile,char *tlenfile,char *gordfile,char *afile,
            long x,long y, int mask, int outlet, int thresh);
int netsetup(char *demfile, int method,
			 float p1,float p2,float p3,float p4, long xr,long yr, int contcheck);
int subbasinsetup(char *pfile, char *wfile,char *treefile,char *coordfile,
				  char *shpfile, int ordert); 
int depgrd(char *angfile,char *dgfile, char *depfile);
void MaxAreaD8Coords(char* areafile, long *x, long *y); 
void setCtrl(CTardemCtrl* ctrl);
int	source(char *areafile,char *slopefile,char *plenfile,char *dirfile, 
		   char *srcfile, char *elvfile, char *gordfile, char *scafile,
		   char *fdrfile, int ipar,float *p, double x, double y, int contcheck);
int validateoutlet(long x,long y,char *aread8file, char *srcfile);

//Fortran subroutines
extern "C" {
__declspec(dllimport) void __stdcall NETEX(short*,int*,char*,int,char*,int,char*,int,int*,int*,int*,int*,int*,float*,float*,double*,double*,int*,int*);
__declspec(dllimport) void __stdcall NETPROP(short*,int*,float*,char*,int,int*,int*,float*,float*,int*,int*,double*,double*,int*);
__declspec(dllimport) void __stdcall ARCLINKS(char*,int,char*,int,char*,int,int*,float*,int*);
__declspec(dllimport) void __stdcall ARCSTREAMS(char*,int,char*,int,char*,int,int*,float*,int*);
__declspec(dllimport) void __stdcall ASFGRID(float*,float*,float*,float*,int*,int*,char*,int,char*,int,int*,int*,float*,float*,int*);
__declspec(dllimport) void __stdcall LINKAN(char*,int,char*,int,char*,int,int*,float*,int*);
__declspec(dllimport) void __stdcall STREAMAN(char*,int,char*,int,char*,int,int*,float*,int*);
__declspec(dllimport) int __stdcall LINKSOURCE(int*,int*,float*,int*,int*,int*,int*,int*,int*,int*);
}

/* prototypes - t********************/

/* used in flood.c */
int		vdn(int n);					/* also used in setdir */
void	pool(int i,int j);
void	set(int i,int j,float *fact,float mval, short useflowfile);		/* also used in setdir */
float	min2(float e1,float e2);
float	max2(float e1,float e2);

/* used in setdir.c */
int		setdfnoflood(float mval, int ** flowDirection, float fdmval, short useflowfile);
void	SET2(int I, int J,float *DXX,float DD);
void	setdf2(void );
void	VSLOPE(float E0,float E1, float E2,
             float D1,float D2,float DD,
             float *S,float *A);
/* int		setdir(char *demfile, char *angfile, char *slopefil);        */      

/* used in area.c */
float	prop(float,int);
void	dparea(int,int);

/*  used in aread8.c  */
void darea(int,int);

void feedback(char *message, float per,int ival);

/*  Global Variables  **************************************************/
extern CTardemCtrl *ocxCtrl;  //used for events

extern short **dir;      /* flood, setdir */
extern short **apool;    /* flood */
extern float **elev;     /* flood, setdir */
extern float **slope;		/* setdir */
extern float **ang;      /* setdir, area */
extern float **sca;			/* area */

extern short *ipool, *jpool, *tmp;		/* flood */ 
extern short *dn, *is, *js;					/* setdir, flood */

extern int nx, ny;										/* flood, setdir, area */
extern int npool, pooln, pstack;			/* flood */
extern int nis, istack;							/* flood, setdir */
extern int filetype;									/* flood */

/*  i1,n1 are the x range when tiling
    i2,n2 are the y range when tiling
    These are global so as to avoid duplication in recursive routines  */
extern int i1,i2,n1,n2;			/* flood, setdir */

extern char annot[MAXLN],units[MAXLN];			/* flood, setdir */

extern float dx, dy;							/* flood, setdir, area */
extern float emin, et;						/* flood, setdir */
extern float utme,utmn,skew;			/* setdir */
extern float aref[10];						/* area */
extern float ndv;								/* area, setdir */

extern int nf;				/* flood, setdir */
extern int dd1[8],dd2[8];			
extern int *d1;
extern int *d2;

/* prototypes for functions in AVCalls.c */

void SetWorkingStatus(void);

/*  Grid bound variables   */
extern double bndbox[4],csize;
extern float mval;

