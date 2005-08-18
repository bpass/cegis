//  Tardem lib.cpp, a combination of all TARDEM library source files into a single file
//  combined aread8.cpp, setdir.cpp, area.cpp, flood.cpp, netsetup.cpp, gridnet.cpp, 
//  subbasinsetup.cpp, depgrd

/* Created by David G Tarboton  */

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

	
#include "lsm.h"
#include "shapefile.h"
#include "feedback.h"

//#include "shapeapi.h"

/* prototypes for functions in AVCalls.c */

void SetWorkingStatus(void);

/*  Prototypes for Numerical Recipe Funcitons Used */

int **imatrix(int,int,int,int);
float **matrix(int,int,int,int);
int *ivector(int,int);
double *dvector(int,int);
void free_imatrix(int**,int,int,int,int);
void free_matrix(float**,int,int,int,int);
void larea(int,int,int);
void free_ivector(int*,int,int);

//Global variables and constants
#define PI2 1.5707963268

float **weight;  /* Global weight array  */
float **areaw;  /*  Global array with weighted results   */
int ccheck;   /*  Global contamination check flag  */
int useww;    /*  Global use weights flag  */
float ndva;   /*Global angle no data value  */
short **src;
int **arr;
float **ltp,**lengd;
double dist[9];
int **gorder, **aa, tresh;
float ndvp;
int iy,jy;
float **dep;
short **aw;
int **rtable; 
float **rprops; 
short **dir;      /* flood, setdir */
short **apool;    /* flood */
float **elev;     /* flood, setdir */
float **slope;		/* setdir */
float **ang;      /* setdir, area */
float **sca;			/* area */

short *ipool, *jpool, *tmp;		/* flood */ 
short *dn, *is, *js;					/* setdir, flood */

int nx, ny;										/* ALL */
int npool, pooln, pstack;			/* flood */
int nis, istack;							/* flood, setdir */
int filetype;									/* all IO */

/*  i1,n1 are the x range when tiling
    i2,n2 are the y range when tiling
    These are global so as to avoid duplication in recursive routines  */
int i1,i2,n1,n2;			/* flood, setdir */

char annot[MAXLN],units[MAXLN];			/* flood, setdir */

float dx, dy;							/* flood, setdir, area */
float emin, et;						/* flood, setdir */
float utme,utmn,skew;			/* setdir */
float aref[10];						/* area */
float ndv;						/* area, setdir */

int nf;				/* flood, setdir */
int dd1[8],dd2[8];				
/* Offset pointers d1 and d2 */
int *d1 = dd1-1;
int *d2 = dd2-1;

/*  Grid bound variables   */
double bndbox[4],csize;
float mval;

// outlet list variables
short nout=0,maxno=0, *diffout;   //  Initialize to 0 for when not used
long *xout, *yout;
short SourceThreshold;



//************************************************************************
/* integer function to compute area recursively */
void darea(int i,int j)
  {                                        
    int in,jn,k,con=0;
      /* con is a flag that signifies possible contaminatin of area
         due to edge effects  */
    if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && dir[j][i] > -1)
                 /* not on boundary  */
    {
      if(arr[j][i]==0)  // not touched yet
      {
        arr[j][i]=1; 
		if(useww == 1)areaw[j][i]=weight[j][i];
        for(k=1; k<=8; k++)
        {  in=i+d1[k];
           jn=j+d2[k];
/* test if neighbor drains towards cell excluding boundaryies */
           if(dir[jn][in]>0 && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
             {
                darea(in,jn);
                if(arr[jn][in] < 0)con = -1;
                else arr[j][i]=arr[j][i]+arr[jn][in];
				if(useww == 1)
				{
					if(areaw[jn][in] <= ndv  || areaw[j][i] <=ndv)
					{  areaw[j][i]=ndv;
					}
					else areaw[j][i]=areaw[j][i]+areaw[jn][in];
				}
             }
           if(dir[jn][in] < 0)con = -1;
        }
        if(con == -1 && ccheck == 1)
		{
			arr[j][i]= -1;
			if(useww == 1)areaw[j][i]=ndv;
		}
      }
    }
	else arr[j][i] = -1;
  }

//************************************************************************

int aread8(char *pfile,char *afile, long x, long y, int doall, 
		   char *wfile, int usew, int contcheck)
 {                
   int i,j,err,row,col; 
   ccheck=contcheck;
   useww=usew;
/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;
     
/*  read pointers */
   err=gridread(pfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
   bndbox,&csize,&ndvp,&filetype);
   if(err != 0)goto ERROR1;

/*  read weights if necessary   */
   if(usew == 1)
   {
      err=gridread(wfile,(void ***)&weight,RPFLTDTYPE,&nx,&ny,&dx,&dy,
      bndbox,&csize,&ndv,&filetype);
      if(err != 0)goto ERROR2;
	  /*  Allocate areaw memory  */
      areaw =   (float **)  matalloc(nx,ny,RPFLTDTYPE);
/* initialize areaw array to ndv */
      for(i=0; i<ny; i++)
	  {
       for(j=0; j<nx; j++)
        {
           areaw[j][i]= ndv;
        }
	  }
   }
/*  Allocate area memory  */
   arr =   (int **)  matalloc(nx,ny,RPINTDTYPE);
/* initialize area array to 0, -1 on boundary */
   for(i=0; i<ny; i++)
   {
      for(j=0; j<nx; j++)
        {
        if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && dir[j][i] > -1)
           arr[j][i]=0;
        else arr[j][i]= -1;
        }
   }
    if(doall == 0)   /*  Only compute area's for designated location  */
   {
		col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Given coords out of area - whole area will be calculated\n");
			row=0; col=0;
		}
	}
	else
	{
		row=0; col=0;
	}
	if(row > 0 && col > 0)
   {
/* call drainage area subroutine for pixel to zero on  */
      darea(row,col);
    }
    else
    {
 
/* call drainage area subroutine for each area */
/* work from middle outwards to avoid deep recursions  */
   for(i=ny/2; i<ny-1; i++)                    
   {  for(j=nx/2; j<nx-1; j++)
        darea(i,j);
      for(j=nx/2-1; j>=1; j--)
        darea(i,j);
   }  
   for(i=ny/2-1; i>=1; i--)                    
   {  for(j=nx/2; j<nx-1; j++)
        darea(i,j);
      for(j=nx/2-1; j>=1; j--)
        darea(i,j);
   } 
   } 
/* write out areas */
	if(usew == 0)
	{
		err=gridwrite(afile, (void **)arr,RPINTDTYPE,nx,ny,dx,dy,
       bndbox,csize, -1., filetype);
	}
	else
	{
		err=gridwrite(afile, (void **)areaw,RPFLTDTYPE,nx,ny,dx,dy,
       bndbox,csize, ndv, filetype);
		free(areaw[0]); free(areaw);
	}
   free(arr[0]);
   free(arr);
   if(usew == 1){
ERROR2:
   free(weight[0]); free(weight);
   }
ERROR1:
   free(dir[0]);
   free(dir);
   return(err);
 }     


//************************************************************************

void   VSLOPE(float E0,float E1, float E2,
              float D1,float D2,float DD,
              float *S,float *A)
{
/*    SUBROUTINE TO RETURN THE SLOPE AND ANGLE ASSOCIATED WITH A DEM PANEL */
float S1,S2,AD;


if(D1!=0)
      S1=(E0-E1)/D1;
if(D2!=0)
      S2=(E1-E2)/D2;

if(S2==0 && S1==0) *A=0;
      else
      *A= (float) atan2(S2,S1);
      AD= (float) atan2(D2,D1);
      if(*A  <   0.)
       {
       *A=0.;
       *S=S1;
       }
      else if(*A > AD)
       {
        *A=AD;
        *S=(E0-E2)/DD;
       }
      else
        *S= (float) sqrt(S1*S1+S2*S2);
 }

//***************************************************************************
/*     SUBROUTINE TO RETURN SLOPE AND ANGLE OF STEEPEST DECENT IF POSITIVE */

void   SET2(int I, int J,float *DXX,float DD)
{

float SK[9];
float ANGLE[9];
float SMAX;
int K;
int KD;

/*    int ID1[]= {NULL,1,2,2,1,1,2,2,1 };
      int ID2[]= {NULL,2,1,1,2,2,1,1,2};
      int I1[] = {NULL,0,-1,-1,0,0,1,1,0 };
      int I2[] = {NULL,-1,-1,-1,-1,1,1,1,1};
      int J1[] = {NULL,1,0,0,-1,-1,0,0,1};
      int J2[] = {NULL,1,1,-1,-1,-1,-1,1,1};
   float  ANGC[]={NULL,0.,1.,1.,2.,2.,3.,3.,4.};
   float  ANGF[]={NULL,1.,-1.,1.,-1.,1.,-1.,1.,-1.}; */
      int ID1[]= {0,1,2,2,1,1,2,2,1 };
/*		int *ID1;  */
      int ID2[]= {0,2,1,1,2,2,1,1,2};
      int I1[] = {0,0,-1,-1,0,0,1,1,0 };
      int I2[] = {0,-1,-1,-1,-1,1,1,1,1};
      int J1[] = {0,1,0,0,-1,-1,0,0,1};
      int J2[] = {0,1,1,-1,-1,-1,-1,1,1};
   float  ANGC[]={0,0.,1.,1.,2.,2.,3.,3.,4.};
   float  ANGF[]={0,1.,-1.,1.,-1.,1.,-1.,1.,-1.};
 /*  ID1=ID1m-1;  */

       for(K=1; K<=8; K++)
       VSLOPE(elev[J][I],
              elev[J+J1[K]][I+I1[K]],
              elev[J+J2[K]][I+I2[K]],
              DXX[ID1[K]],DXX[ID2[K]],DD,&SK[K],&ANGLE[K]);

SMAX=0.;
KD=0;
      ang[J][I]=-1.;  /* USE -1 TO INDICATE DIRECTION NOT YET SET */
      for(K=1; K<=8; K++)
        {
        if(SK[K] >  SMAX)
           {
            SMAX=SK[K];
            KD=K;
           }
         }

      if(KD  > 0)
         ang[J][I]= (float) (ANGC[KD]*PI2+ANGF[KD]*ANGLE[KD]);
      slope[J][I]=SMAX;
}


//************************************************************************

void sloped8(void )
{
	int k,i,j,in,jn;
	float fact[9],ed;
	/*  Direction factors  */
  for(k=1; k<= 8; k++)
    fact[k]= (float) (1./sqrt(d1[k]*dy*d1[k]*dy+d2[k]*d2[k]*dx*dx));

  for(i=i2; i< n2; i++)for(j=i1; j<n1; j++)
  {
    if(dir[j][i] > 0)
	{
		jn=j+d2[dir[j][i]];
		in=i+d1[dir[j][i]];
		ed=elev[j][i]-elev[jn][in];
	    slope[j][i]= ed*fact[dir[j][i]] ;	
	}
	else
		slope[j][i]= -1.;
  }
}

//************************************************************************

void set2(int i,int j,float *fact,float *elev1, float *elev2, int iter, 
		  int **spos, short *s, short useflowfile)
{
/*  This function sets directions based upon secondary elevations for
  assignment of flow directions across flats according to Garbrecht and Martz
  scheme.  There are two possibilities: 
	A.  The neighbor is outside the flat set
	B.  The neighbor is in the flat set.
	In the case of A the elevation of the neighbor is set to 0 for the purposes
	of computing slope.  Since the incremental elevations are all positive there is 
	always a downwards slope to such neighbors, and if the previous elevation 
	increment had 0 slope then a flow direction can be assigned.*/
  float slope,slope2,smax,ed;
  int k,spn,sp,kflat=0;
  short in,jn;
  smax=0.;
  sp=spos[j][i];
  for(k=1; k<=8; k++)
  {
		jn=j+d2[k];
		in=i+d1[k];
		spn=spos[jn][in];
		if(iter <= 1)
		{
			ed=elev[j][i]-elev[jn][in];
		}
		else
		{
			ed=elev1[sp]-elev1[spn];
		}
		slope=fact[k]* ed;
	if(spn < 0 || s[spn] < 0)
	{
		/*  The neighbor is outside the flat set.  */
		ed=0.;
	}
	else
	{
		ed=elev2[spn];
	}
	slope2 =fact[k]*(elev2[sp]-ed);
	if(slope2 > smax && slope >= 0.)  /*  Only if latest iteration slope is 
									  positive and previous iteration slope flat  */		
		{
			smax=slope2;
			dir[j][i]=k;
		}
  }  /*  End of for  */
}

//************************************************************************

void incfall(int n, float *elev1, short *s1,int **spos,  
			 int iter, int *sloc)
{
	/* This routine implements drainage towards lower areas - stage 1 */
	int done=0,donothing,k,ip,ninc,nincold,spn;
	short st=1,i,j,in,jn;
	float ed;
	nincold= -1;
	while(done < 1)
	{
		done=1;
		ninc=0;
		for(ip=0; ip<n; ip++)
		{
/*			if 	adjacent to same level or lower that drains or 
				adjacent to pixel with s1 < st and dir not set
				do nothing  */
			donothing=0;
			j=js[sloc[ip]];
			i=is[sloc[ip]];
			for(k=1; k<=8; k++)
			{
				jn=j+d2[k];
				in=i+d1[k];
				spn=spos[jn][in];
				if(iter <= 1)
				{
					ed=elev[j][i]-elev[jn][in];
				}
				else
				{
					ed=elev1[sloc[ip]]-
						elev1[spn];
				}
				if(ed >= 0. && dir[jn][in] != 0)donothing = 1;  /* If neighbor drains */
				if(spn >= 0)     /* if neighbor is in flat   */
					if(s1[spn] >= 0 && s1[spn] < st   /*  If neighbor is not being  */
						&& dir[jn][in]  == 0)donothing = 1;   /*  Incremented  */
			}
			if(donothing == 0)
			{
				s1[sloc[ip]]++;
				ninc++;
				done=0;
			}
		}   /*  End of loop over all flats  */
		st=st+1;
//		printf("Incfall %d %d \n",ninc,n);
		if(ninc == nincold)
		{
			done = 1;
//			printf("There are pits remaining, direction will not be set\n");
/*  Set the direction of these pits to 9 to flag them   */
			for(ip=0; ip<n; ip++)  /*  loop 2 over all flats  */
			{
/*			if 	adjacent to same level or lower that drains or 
				adjacent to pixel with s1 < st and dir not set
				do nothing  */
				donothing=0;
				j=js[sloc[ip]];
				i=is[sloc[ip]];
				for(k=1; k<=8; k++)
				{
					jn=j+d2[k];
					in=i+d1[k];
					spn=spos[jn][in];
					if(iter <= 1)
					{
						ed=elev[j][i]-elev[jn][in];
					}
					else
					{
						ed=elev1[sloc[ip]]-
							elev1[spn];
					}
					if(ed >= 0. && dir[jn][in] != 0)donothing = 1;  /* If neighbor drains */
					if(spn >= 0)     /* if neighbor is in flat   */
					if(s1[spn] >= 0 && s1[spn] < st   /*  If neighbor is not being  */
						&& dir[jn][in]  == 0)donothing = 1;   /*  Incremented  */
				}
				if(donothing == 0)
				{
				   dir[j][i] = 9;
/*				   printf("%d %d\n",i,j);    */
				}
			}   /*  End of loop 2 over all flats  */
		}
		nincold=ninc;
	}  /*  End of while done loop  */
}

//************************************************************************

void incrise(int n, float *elev1, short *s2,int **spos, int iter, int *sloc)
{
	/*  This routine implements stage 2 drainage away from higher ground 
	dn is used to flag pixels still being incremented */
	int done=0,ip,k,ninc,nincold,spn;
	float ed;
	short i,j,in,jn;
	nincold=0;
	while(done < 1)
	{
		done=1;
		ninc=0; 
		for(ip=0; ip<n; ip++)
		{
			for(k=1; k<=8; k++)
			{
				j=js[sloc[ip]];
				i=is[sloc[ip]];
				jn=j+d2[k];
				in=i+d1[k];
				spn=spos[jn][in];

				if(iter <= 1)
				{
					ed=elev[j][i]-elev[jn][in];
				}
				else
				{
					ed=elev1[sloc[ip]]-
						elev1[spn];
				}
				if(ed < 0.)dn[sloc[ip]]=1;
				if(spn >=0)
					if(s2[spn] > 0)dn[sloc[ip]] = 1;
			}
		}
		for(ip=0; ip<n; ip++)
		{
			s2[sloc[ip]]=s2[sloc[ip]]+dn[sloc[ip]];
			ninc=ninc+dn[sloc[ip]];
			if(dn[sloc[ip]] == 0)done=0;  /*  if still some not being incremented continue 
									looping  */
		}
//		printf("incrise %d %d\n",ninc,n);   
		if(ninc == nincold)done=1;   /*  If there are no new cells incremented  
									 stop - this is the case when a flat has
									 no higher ground around it.  */
		nincold=ninc;
	}
}

//************************************************************************

void flatrout(int n,int *sloc, short *s,
			  int **spos,int iter,float *elev1,float *elev2, float *fact, int ns,
			  short useflowfile)
{
  int ip,nu, *sloc2,ipp;
  float *elev3;

  incfall(n,elev1,s,spos,iter,sloc);
  for(ip=0; ip < n; ip++)
  {
	  elev2[sloc[ip]]=(float)(s[sloc[ip]]);
	  s[sloc[ip]]=0;   /*  Initialize for pass 2  */
  }

  incrise(n,elev1,s,spos,iter,sloc);
  for(ip=0; ip < n; ip++)
  {
	  elev2[sloc[ip]] += (float)(s[sloc[ip]]);
  }

  nu=0;
  for(ip=0; ip < n; ip++)
  {
	  set2(is[sloc[ip]],js[sloc[ip]],fact,elev1,elev2,iter,spos,s,useflowfile);
	  if(dir[js[sloc[ip]]][is[sloc[ip]]] == 0)nu++;
  }
  if(nu > 0)
  { 
/*  Iterate Recursively   */
/*  Now resolve flats following the Procedure of Garbrecht and Martz, Journal
   of Hydrology, 1997.  */
	  iter=iter+1;
//	  printf("Resolving %d Flats, Iteration: %d \n",nu,iter);
      sloc2 = (int *)malloc(sizeof(int) * nu);
      elev3 = (float *)malloc(sizeof(float) *ns);

  if(sloc2 == NULL || elev3 == NULL)
  {
//     printf("Unable to allocate at iteration %d\n",iter);
  }
  /*  Initialize elev3  */
  for(ip=0; ip < ns; ip++)elev3[ip]=0.;
/*  Put unresolved pixels on new stacks - keeping in same positions  */
    ipp=0;
	for(ip=0; ip<n; ip++)
	{
    if(dir[js[sloc[ip]]][is[sloc[ip]]] == 0)
    {
		sloc2[ipp]=sloc[ip];
 	  /*   Initialize the stage 1 array for flat routing   */
		s[sloc[ip]] = 1;
	  ipp++;
//	  if(ipp > nu)printf("PROBLEM - Stack logic\n");
	}
	else
	{
		s[sloc[ip]] = -1;  /*  Used to designate out of remaining flat on 
						   higher iterations   */ 
	}
	dn[sloc[ip]]=0;  /*  Reinitialize for next time round.  */
  }
  flatrout(nu,sloc2,s,spos,iter,elev2,elev3,fact,ns,useflowfile);
  free(sloc2);
  free(elev3);
//  printf("Done iteration %d\nFlats resolved %d\n",iter,n);
  } /*  end if nu > 0  */
}   /*  End flatrout  */

//************************************************************************

void set(int i,int j,float *fact,float mval,short useflowfile)
{
  float slope,smax;
  int k,amax,in,jn,aneigh=-1;
  dir[j][i]=0;  /* This necessary for repeat passes after level raised */
  smax=0.;
  amax=0;
  for(k=1; k<=8; k=k+2)   // examine adjacent cells first
  {
	  in=i+d1[k];
	  jn=j+d2[k];
      if(elev[jn][in] <= mval) dir[j][i] = -1;
      if(dir[j][i] != -1)
	  {
		  slope=fact[k]*(elev[j][i]-elev[jn][in]);
		  if(useflowfile == 1)aneigh=arr[jn][in];
		  if(aneigh > amax && slope >= 0.)  
		  {
			  amax=aneigh;
			  if(abs(dir[jn][in]-k) !=4)
			     dir[j][i]=k;  // Dont set opposing pointers
		  }
		  else if(slope > smax && amax <=0)
		  {
			  smax=slope;
			  dir[j][i]=k;
		  }
	  }
  }
  for(k=2; k<=8; k=k+2)   // examine diagonal cells 
  {                       
	  in=i+d1[k];
	  jn=j+d2[k];
      if(elev[jn][in] <= mval) dir[j][i] = -1;
      if(dir[j][i] != -1)
	  {
		  slope=fact[k]*(elev[j][i]-elev[jn][in]);
		  if(slope > smax && amax <=0)  // still need amax check to prevent crossing
		  {
			  smax=slope;
			  dir[j][i]=k;
		  }
	  }
  }
}


int setdfnoflood(float mval, int ** flowDirection, float fdmval, short useflowfile)
/*  This version is stripped of pit filling  */
{
  int i,j,k,ip, n, iter;
  float fact[9];
  short *s;  /*  variables for flat draining   */
  int **spos, *sloc;
  float *elev2;


/*  Initialize boundaries  */
  for(i=i1; i< n1; i++)
  {
     dir[i][0]= -1;
     dir[i][n2-1]= -1;
  }
  for(i=i2; i< n2; i++)
  {
     dir[0][i]= -1;
     dir[n1-1][i]= -1;
  }
/*  iup=0; */
/*  initialize internal pointers */
  for(i=i2+1; i< n2-1; i++)for(j=i1+1; j<n1-1; j++)
  {
    if(elev[j][i] <= mval)dir[j][i]= -1;
    else dir[j][i] = 0;
  }
/*  Direction factors  */
  for(k=1; k<= 8; k++)
    fact[k]= (float) (1./sqrt(d1[k]*dy*d1[k]*dy+d2[k]*d2[k]*dx*dx));

//  Set stream overlay directions
	if( useflowfile == 1 )
    for(i=i2+1; i< n2-1; i++)
	  for(j=i1+1; j<n1-1; j++)
	  {	
		  {
			  if( flowDirection[j][i] > fdmval )
			  {
		  		dir[j][i] = flowDirection[j][i];
				flowDirection[j][i]=0;
			  }
		  }
	  }

//   Compute contrib area using overlayed directions for direction setting
	arr=flowDirection;
	ccheck=0;   // dont worry about edge contamination
	useww=0;    // dont worry about weights
    for(i=i2+1; i< n2-1; i++)
	  for(j=i1+1; j<n1-1; j++)
	  {	
		  //This allows for a stream overlay
		  if( dir[j][i]>0 )darea(i,j);
	  }

/*  Set positive slope directions   */
   n=0;
   for(i=i2+1; i< n2-1; i++)
	  for(j=i1+1; j<n1-1; j++)
	  {	
		  if( dir[j][i] == 0 )
		  {	
			if(elev[j][i] > mval)
			{
			  set(i,j,fact,mval,useflowfile);
			  if(dir[j][i] == 0)n++;
			}
		  }
	  }
  if(n > 0)
  { 
/*  Now resolve flats following the Procedure of Garbrecht and Martz, Journal
   of Hydrology, 1997.  */

/*  Memory is utilized as follows
is, js, dn, s and elev2 are unidimensional arrays storing information for flats.
sloc is a indirect addressing array for accessing these - used during
recursive iteration
spos is a grid of pointers for accessing these to facilitate finding neighbors

The routine flatrout is recursive and at each recursion allocates a new sloc for
addressing these arrays and a new elev for keeping track of the elevations
for that recursion level.  
  */
	  iter=1;
//	  printf("Resolving %d Flats, Iteration: %d \n",n,iter);
      spos = (int **) matalloc(nx, ny, RPINTDTYPE);
      dn = (short *)malloc(sizeof(short) * n);
      is = (short *)malloc(sizeof(short) * n);
      js = (short *)malloc(sizeof(short) * n);
      s = (short *)malloc(sizeof(short) * n);
	  sloc = (int *)malloc(sizeof(int) * n);
      elev2 = (float *)malloc(sizeof(float) *n);

  if(dn == NULL || is == NULL || js == NULL || s == NULL || 
	  spos == NULL || elev2 == NULL || sloc == NULL)
  {
//     printf("Unable to allocate at iteration %d\n",iter);
  }
/*  Put unresolved pixels on stack  */
   ip=0;
   for(i=i2; i< n2; i++)
	  for(j=i1; j<n1; j++)
  {
    spos[j][i]=-1;   /*  Initialize stack position  */
    if(dir[j][i] == 0)
    {
      is[ip]=i;
      js[ip]=j;
	  dn[ip]=0;
	  sloc[ip]=ip;
	  /*   Initialize the stage 1 array for flat routing   */
	  s[ip] = 1;
	  spos[j][i]=ip;  /*  pointer for back tracking  */
	  ip++;
//	  if(ip > n)printf("PROBLEM - Stack logic\n");
    }
  }
  flatrout(n,sloc,s,spos,iter,elev2,elev2,fact,n,useflowfile);
/*  The direction 9 was used to flag pits.  Set these to 0  */
  for(i=i2; i< n2; i++)
	  for(j=i1; j<n1; j++)
		  if(dir[j][i] == 9)dir[j][i]=0;
  free(spos[0]); free(spos);
  free(elev2);
  free(dn);
  free(is);
  free(js);
  free(s);
  free(sloc);
//  printf("Done iteration %d\nFlats resolved %d\n",iter,n);
  } /*  End if n > 0  */
   return(0);   /*  OK exit from setdir  */
}  /*  End setdfnoflood  */

//************************************************************************
/*

Converted from FORTRAN July 05, 1997  K. Tarbet

C
C---SUBROUTINE TO SET VARIABLE DIRECTIONS.
C    DIRECTIONS MEASURED ANTICLOCKWISE FROM EAST IN RADIANS.
C
*/
void setdf2(void )
{
float FANG[9];
float dxx[3];
int i,j;
float dd;

/*       INITIALISE ANGLES and slopes */

	for (i=0; i<nx; i++)
        {
         ang[i][0]=-2;
         ang[i][ny-1]=-2;
         slope[i][0]=-1;
         slope[i][ny-1]=-1;
         }

       for(i=0; i<ny; i++)
        {
         ang[0][i]=-2;
         ang[nx-1][i]=-2;
         slope[0][i]=-1;
         slope[nx-1][i]=-1;
        }

         FANG[1]=0.;
         FANG[2]=(float)atan2(dy,dx);
         FANG[3]=(float) PI2; 
         FANG[4]=2*FANG[3]-FANG[2];
         FANG[5]=2*FANG[3];
         FANG[6]=2*FANG[3]+FANG[2];
         FANG[7]=3*FANG[3];
         FANG[8]=4*FANG[3]-FANG[2];

/* --INITIALISE INTERNAL POINTERS (-VE POINTER INDICATES OUTSIDE DOMAIN) */

	for(i=1; i<ny-1; i++)
           for(j=1; j<nx-1; j++)
              {
                if(dir[j][i] <  0)   
                                         
                 {
                  ang[j][i]=-2.;
		  slope[j][i]=-1.; /*  !  -1 slope indicates no data */
                 }
                else if(dir[j][i] == 0)
                 {
                  ang[j][i]=-1.;  /*  DGT 9/2/97 since without   */
                  slope[j][i]=0.;  /*  pit filling dir = 0 is possible  */
/*  ang = -1 designates unfilled pit, ang = -2 designates no data.  */                  
                 }
                else
                 ang[j][i]=0.;
               }



/*    TEST ALL INTERNAL ELEVATIONS AND SET SLOPE AND ANGLE */

       dxx[1]=dx;
       dxx[2]=dy;
       dd=(float)sqrt(dx*dx+dy*dy);
	for(i=1; i<ny-1; i++)
    {
    SetWorkingStatus(); 
    for(j=1; j<nx-1; j++)
              {
               if(dir[j][i]>0 )
                   {
                   SET2(i,j,dxx,dd);
                   if(ang[j][i] < -0.5)
                        ang[j][i]=FANG[dir[j][i]];
                   }
              }
    }
 }

//************************************************************************

/*  C version of setdir.c. This does not fill pits, it is intended for use AFTER
    d8.c which fills pits.  Input is a dem file name.  Output is
    slope and dinf angle files.   This version implements Garbrecht flow directions
	for flats.
     
  David G Tarboton
  Utah State University   
     */

/*
This roughly follows the following pseudocode from my review of Garbrecht's paper

Pseudocode to iteratively resolve flow directions

Define arrays E(x), D(x), S1(x), S2(x), I(x) where x refers to grid location.
 where E is original elevation, D is total increment, S1 step 1 increment, 
 S2 step 2 increment, I a flag to indicate whether flow direction has been set.  

Initialize  
do for each x
	read in E(x)
	I(x) = 1
enddo
unresolved = 1
while(unresolved > 0)
	unresolved = 0
	do for each x
		if(I(x) > 0)then      /*  only apply to unresolved pixels  
			calculate slope to each neighbor using E(x) 
			if(max slope > 0)then
				assign flow direction to appropriate neighbor
				I(x) = 0   /*  flag it as resolved   
			else
				unresolved=unresolved + 1
				I(x) = 1   /* this flags the pixel as unresolved  
			endif
		endif
	enddo
	if(unresolved > 0)then
		for each x
			D(x) = S2(x) = S2(x)= 0      /*  reset arrays for iteration  
		for each x with I(x) > 0
			Calculate step 1 increment towards falling terrain, S1(x) 
		for each x with I(x) > 0
			Calculate step 2 increment away from rising terrain, S2(x)
		for each x with I(x) > 0
			D(x) = S1(x) + S2(x)
		for each x
			E(x) = D(x)   
/*  overwrite original elevations since they are no longer needed from now 
on slopes are based on D(x)  
	endif
endwhile
output results
end

  */
int setdir(char *demfile, char *angfile, char *slopefile, char * flowfile, 
			 short useflowfile)
{ 
  int ** flowdirection;
  int err,filetype;
  float fdmval;
  
/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;


  err=gridread(demfile,(void ***)&elev,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&mval,&filetype);
  if(err != 0)goto ERROR2;          

  if( useflowfile == 1 )
	  err=gridread(flowfile,(void ***)&flowdirection,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&fdmval,&filetype);
  if(err != 0)goto ERROR4;
 
/*  allocate  dir and stack arrays  */
  dir = (short **) matalloc(nx, ny, RPSHRDTYPE);

  i1=0; i2=0; n1=nx; n2=ny;  /*  full grid  */ 
  err=setdfnoflood(mval, flowdirection ,fdmval,useflowfile);
   if(err != 0)goto ERROR1;

/*  allocate memory for slope and angle  */
  slope = (float **) matalloc(nx, ny, RPFLTDTYPE);
  ang = (float **) matalloc(nx, ny, RPFLTDTYPE);
  setdf2();
  err = gridwrite(slopefile,(void **)slope,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
  if (err != 0)goto ERROR3;
  err = gridwrite(angfile,(void **)ang,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-2.,filetype);
  if (err != 0)goto ERROR3;

/*  Wrapping up  */
  err=0;
ERROR3:
  free(slope[0]);
  free(slope);
  free(ang[0]);
  free(ang);
ERROR1:
  free(dir[0]);
  free(dir);
ERROR4:
  if(useflowfile == 1)
  {
  free(flowdirection[0]);
  free(flowdirection);
  }
ERROR2:
  free(elev[0]);
  free(elev);
  return(err);
}

//************************************************************************

int setdird8(char *demfile, char *pfile, char *slopefile, char * flowfile, 
			 short useflowfile)
{ 
  int ** flowdirection;

  int err,filetype;
  float mval, fdmval;
  double bndbox[4],csize;


/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

  
  err=gridread(demfile,(void ***)&elev,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&mval,&filetype);
  if(err != 0)goto ERROR2;          

/*  allocate  dir and stack arrays  */
  dir = (short **) matalloc(nx, ny, RPSHRDTYPE);
  
  if( useflowfile == 1 )
	  err=gridread(flowfile,(void ***)&flowdirection,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&fdmval,&filetype);
  if(err != 0)goto ERROR4;
  
  i1=0; i2=0; n1=nx; n2=ny;  /*  full grid  */ 
  err=setdfnoflood(mval, flowdirection, fdmval, useflowfile);
   if(err != 0)goto ERROR1;
  err = gridwrite(pfile,(void **)dir,RPSHRDTYPE,nx,ny,dx,dy,
	  bndbox,csize,-1,filetype);

/*  allocate memory for slope   */
  slope = (float **) matalloc(nx, ny, RPFLTDTYPE);
  sloped8();
  err = gridwrite(slopefile,(void **)slope,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
  if (err != 0)goto ERROR3;

  err=0;
ERROR3:
  free(slope[0]);
  free(slope);
ERROR1:
  free(dir[0]);
  free(dir);
ERROR4:
  if(useflowfile == 1)
  {
  free(flowdirection[0]);
  free(flowdirection);
  }
ERROR2:
  free(elev[0]);
  free(elev);
  return(err);
}

//************************************************************************

float prop(float a,int k)
{
float p=0.;
if(k == 1 && a > PI)a=a-2.*PI;
if(a > aref[k-1] && a < aref[k+1]){
  if( a > aref[k])
       p=(aref[k+1]-a)/(aref[k+1]-aref[k]);
  else 
      p=(a-aref[k-1])/(aref[k]-aref[k-1]);
  }
return(p);
}

//************************************************************************

/* function to compute partial areas recursively */
void dparea(int i,int j)
{
   int in,jn,k,kback,con=0;
    /* con is a flag that signifies possible contaminatin of sca
         due to edge effects  */
   float p;
   if(sca[j][i]==0.0)    /* i.e., if the cell hasn't been looked at yet */
      {
        if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && ang[j][i] > ndva)
       /* i.e. the cell isn't outside domain */
         {
           sca[j][i] = dx;
		   if(useww == 1)areaw[j][i]=weight[j][i];
           for(k=1; k<=8; k++)
           {
             in = i + d1[k];
             jn = j + d2[k];
/* for each neighboring cell, find the proportion of of  outflow */
/* draining back to the cell in question */
             kback = k + 4;
             if(kback>8)kback = kback - 8;
             p=prop(ang[jn][in],kback);
             if(p>0.)
                {
                 dparea(in,jn);
                 if(sca[jn][in] < -0.5)con = -1;
                 sca[j][i] = sca[j][i] + sca[jn][in]*p;
                      if(useww == 1)
                      {
                           if(areaw[jn][in] <= ndv  || areaw[j][i] <=ndv)
                                 {  
                                        areaw[j][i]=ndv;
                                 }
                           else areaw[j][i]=areaw[j][i]+areaw[jn][in]*p;
                      }
                }
             if(ang[jn][in] <= ndva)con = -1;         
            }
           if(con == -1 && ccheck == 1)
		   {
			   sca[j][i]= -1.;
			   if(useww == 1)areaw[j][i]=ndv;
		   }
         }
     }
} 


//************************************************************************

int area(char *pfile, char *afile, long x, long y, int doall, 
		   char *wfile, int usew, int contcheck)
{
   int i,j,err,row,col;
   ccheck=contcheck;
   useww=usew;

/* Define directions */
/*  d1a=dd1-1; d2a=dd2-1;  */
d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

/* read angles */
  err=gridread(pfile,(void ***)&ang,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndva,&filetype);
  if(err != 0)goto ERROR1;           

/*  read weights if necessary   */
   if(usew == 1)
   {
      err=gridread(wfile,(void ***)&weight,RPFLTDTYPE,&nx,&ny,&dx,&dy,
      bndbox,&csize,&ndv,&filetype);
      if(err != 0)goto ERROR2;
	  /*  Allocate areaw memory  */
      areaw =   (float **)  matalloc(nx,ny,RPFLTDTYPE);
/* initialize areaw array to ndv */
      for(i=0; i<ny; i++)
	  {
       for(j=0; j<nx; j++)
        {
           areaw[j][i]= ndv;
        }
	  }
   }

/*  Allocate memory  */
sca = (float **) matalloc(nx, ny, RPFLTDTYPE);

/* Calculate angles for interpolation */                                      
aref[0]= -atan2(dy,dx);
aref[1]=0.;
aref[2]= -aref[0];
aref[3]=0.5*PI;
aref[4]=PI-aref[2];
aref[5]=PI;
aref[6]=PI+aref[2];
aref[7]=1.5*PI;
aref[8]=2.*PI-aref[2];
aref[9]=2.*PI;
/* initialize sca array to 0 */
    for(i=0; i<ny; i++)
       {
       for(j=0; j<nx; j++)
         {
         if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && ang[j][i]> ndva)
            sca[j][i]=0;
         else sca[j][i]= -1;
         }
       }
   if(doall == 0)   /*  Only compute area's for designated location  */
   {
		col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Given coords out of area - whole area will be calculated\n");
			row=0; col=0;
		}
	}
   else
   {
		row=0; col=0;
   }
   if(row > 0 && col > 0)
   {
/* call drainage area subroutine for pixel to zero on  */
      dparea(row,col);
   }
   else
   {

/* Call drainage area subroutine for each cell           */
/* working from the middle out to avoid deep recursions  */ 

/* "lower block" */
      for(i=ny/2; i<ny-1; i++)
      {
        SetWorkingStatus();
         for(j=nx/2; j<nx-1; j++)
           dparea(i,j);
         for(j=nx/2-1; j>=1; j--)
           dparea(i,j);
      }
 /* "upper block" */
      for(i=ny/2-1; i>=1; i--)
      {
        SetWorkingStatus();
         for(j=nx/2; j<nx-1; j++)
           dparea(i,j);
         for(j=nx/2-1; j>=1; j--)
           dparea(i,j);
      }
    }
/*  Write out areas  */  
	if(usew == 0)
	{
        err = gridwrite(afile,(void **)sca,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
	}
	else
	{
		err=gridwrite(afile, (void **)areaw,RPFLTDTYPE,nx,ny,dx,dy,
       bndbox,csize, ndv, filetype);
		free(areaw[0]); free(areaw);
	}
    free(sca[0]);
    free(sca);        
   if(usew == 1){
ERROR2:
   free(weight[0]); free(weight);
   }
ERROR1:
    free(ang[0]);
    free(ang);
    return(err);              
}  

//************************************************************************
                
//  Function to climb recursively flow direction grid for min elevation to burn down
float climb(int i,int j)
{
	float emin,eneigh;
	int in,jn,k;
	emin=elev[j][i];
	for(k=1; k<=8; k++)
	{
		in=i+d1[k];
        jn=j+d2[k];
/* test if neighbor drains towards cell excluding boundaries */
        if(dir[jn][in]>0 && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
		{
			eneigh=climb(in,jn);
			if(eneigh < emin)emin=eneigh;
		}
	}
	elev[j][i]=emin;  // burn down elevation
	return(emin);
}

//************************************************************************

//  Function to burn down flow elevations for given flow directions.
//  This is a placeholder for what will eventually be simplex optimization
int fdsimplex()
{
	int in,jn,i,j,err=0;
	float emin;
	for(i=1; i<ny-1; i++)
		for(j=1; j<nx-1; j++)
		{
			if(dir[j][i] > 0)
			{
				in=i+d1[dir[j][i]];
				jn=j+d2[dir[j][i]];
				if(dir[jn][in] <= 0)  // Neighbour is out
					emin=climb(i,j);
			}
		}
	return(err);
}

//************************************************************************

//  Function to clean given flow directions.
int dirclean(char *newflowfile, int writeflowfile)
{
	int in,jn,i,j,err=0;
	ccheck=0;  // These are globals to remove contamination checking
	useww=0;  //  for the darea function
	for(i=1; i<ny-1; i++)
		for(j=1; j<nx-1; j++)
		{
			if(dir[j][i] > 0)
			{
				in=i+d1[dir[j][i]];
				jn=j+d2[dir[j][i]];
				if(dir[jn][in] <= 0)  // Neighbour is out
					darea(i,j);
			}
		}
	//  At this point contrib area has been evaluated for all grid cells that drain to 
	//	an edge or internal end.  Set direction to no data for the rest.
	for(i=1; i<ny-1; i++)
		for(j=1; j<nx-1; j++)
		{
			if(arr[j][i] <= 0)
			{
				dir[j][i] = 0;   // reinitialize
			}
		}  
	if(writeflowfile == 1)
	{    // Use pool array as placeholder to write directions
	for(i=0; i<ny; i++)
		for(j=0; j<nx; j++)
		{
			if(dir[j][i] >0)apool[j][i]=dir[j][i];
			else apool[j][i] = -1;
		}  
    err = gridwrite(newflowfile,(void **)apool,RPSHRDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1,filetype);
	}
	return(err);
}

//************************************************************************

int addstack(int i, int j)
{
	/*  Routine to add entry to is, js stack, enlarging if necessary   */
	nis=nis+1;
	if(nis >= istack )
      {
        /*  Try enlarging   */
         istack=(int) (istack + nx*ny*.1);
         if(istack > nx*ny)
         {
//           printf("\n is,js stack too large, exiting ...\n");
           return(12);
         }
//         printf("\n Enlarging is,js stack\n");
         is = (short *)realloc(is, sizeof(short) * istack);
         js = (short *)realloc(js, sizeof(short) * istack);
         dn = (short *)realloc(dn, sizeof(short) * istack);
         if(is == NULL || js == NULL || dn == NULL)
        {
//         printf("\n Could not enlarge stack\n");
         return(13);
        }
      }
	is[nis]=i;
	js[nis]=j;
	return(0);
}

//************************************************************************

int vdn(int n)
{
  int ip,k,imin;
  float ed;
  nis=n;
  do
  {
  n=nis;
  nis=0;
  for(ip=1; ip <=n; ip++)dn[ip]=0;
  for(k=1; k<=7; k=k+2)
    for(ip=1; ip<=n; ip++)
    {
       ed=elev[js[ip]][is[ip]]-elev[js[ip]+d2[k]][is[ip]+d1[k]];
       if(ed >= 0. && dir[js[ip]+d2[k]][is[ip]+d1[k]] != 0 && dn[ip] == 0)
         dn[ip]=k;
    }
  for(k=2; k<=8; k=k+2)
    for(ip=1; ip<=n; ip++)
    {
       ed=elev[js[ip]][is[ip]]-elev[js[ip]+d2[k]][is[ip]+d1[k]];
       if(ed >= 0. && dir[js[ip]+d2[k]][is[ip]+d1[k]] != 0 && dn[ip] == 0)
         dn[ip]=k;
    }
  imin=1;  /*  location of point on stack with lowest elevation  */
  for(ip=1; ip <= n; ip++)
  {
     if(dn[ip] > 0)dir[js[ip]][is[ip]]=dn[ip];
     else
     {
        nis++;
        is[nis]=is[ip];
        js[nis]=js[ip];
        if(elev[js[nis]][is[nis]] < elev[js[imin]][is[imin]])imin=nis;
     }
  }
  SetWorkingStatus(); 
  }while(nis < n);
  return(imin);
}   

//************************************************************************

/* function to compute pool recursively and at the same time determine
   the minimum elevation of the edge. */
void pool(int i,int j)

  {
    int in,jn,k;
    if(apool[j][i]<=0)   /* not already part of a pool  */
    {
        if(dir[j][i]!= -1)  /* check only dir since dir was initialized  */
                 /* not on boundary  */
      {
        apool[j][i]=pooln;  /*  apool assigned pool number */
        npool=npool+1;
        if(npool >= pstack)
        {
          if(pstack < (nx*ny))
          {
/*  Try enlarging   */
//            printf("\n Enlarging pool stack\n");
            pstack=(int) (pstack + nx*ny*.1);
            if(pstack > nx*ny)
            {
//              printf("\n Pool stack too large, exiting ...\n");
            }
            ipool = (short *)realloc(ipool, sizeof(short) * pstack);
            jpool = (short *)realloc(jpool, sizeof(short) * pstack);
            if(ipool == NULL || jpool == NULL)
            {
//              printf("\n Cannot reallocate pool stack, exiting ...\n");
            }
          }
          else
          {
//            printf("\n Could not enlarge Pool stack\n");
          }
        }
        ipool[npool]=i;
        jpool[npool]=j;
/*        printf("%d %d Pool %d\n",i,j,pooln);  */
        for(k=1; k<=8; k++)
        {  
           in=i+d1[k];
           jn=j+d2[k];
/* test if neighbor drains towards cell excluding boundaries */
           if((dir[jn][in]>0 && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
              || (dir[jn][in] == 0 && elev[jn][in] >= elev[j][i]))  
               /* so that adjacent flats get included */
             {
				   pool(in,jn);
             }
        }
      }
    }
  }

//************************************************************************

float max2(float e1,float e2)
{ 
  float em; 
  em=e1;
  if(e2 > em)em=e2;
  return(em);
}

//************************************************************************

int setdf(float mval, int **flowdirection, float fdmval, short useflowfile, 
		  char * newflowfile, int writeflowfile)
{
  int i,j,k,n,nflat,ni,ip,imin,err=0,jn,in,np1,nt;
  float fact[9],per=1.;

/*  Initialize boundaries  */
  for(i=i1; i< n1; i++)
  {
     dir[i][i2]= -1;
     dir[i][n2-1]= -1;
  }
  for(i=i2; i< n2; i++)
  {
     dir[i1][i]= -1;
     dir[n1-1][i]= -1;
  }
/*  initialize internal pointers */
  for(i=i2+1; i< n2-1; i++)for(j=i1+1; j<n1-1; j++)
  {
    if(elev[j][i] <= mval)dir[j][i]= -1;
    else dir[j][i] = 0;
  }

//  Set stream overlay directions
  if( useflowfile == 1 )
  {
    for(i=i2+1; i< n2-1; i++)
	  for(j=i1+1; j<n1-1; j++)
	  {	
			  if( flowdirection[j][i] > fdmval && dir[j][i] >=0 )
			  {
		  		dir[j][i] = flowdirection[j][i];
				flowdirection[j][i]=0;
			  }
	  }
	  arr=flowdirection;  // to reuse space for area array for cleaning and in set
	  err=dirclean(newflowfile,writeflowfile);
//   Call function to optimize elevation adjustments based on flow directions already given
	  err = fdsimplex();
	  arr=flowdirection;  // to use in set
  }
/*  printf("Problem Pixels \n");   */
/*  printf("     Flats   Unresolved\n");  */   

/*  Direction factors  */
  for(k=1; k<= 8; k++)
    fact[k]=(float)(1./sqrt(d1[k]*dy*d1[k]*dy+d2[k]*d2[k]*dx*dx));
  
/*  Set positive slope directions - store unresolved on stack */
  nis=0;
  for(i=i2+1; i< n2-1; i++)for(j=i1+1; j<n1-1; j++)
  {
    if(elev[j][i] > mval)set(i,j,fact,mval,useflowfile);
/*  Put unresolved pixels on stack  */
    if(dir[j][i] == 0)
    {
		err=addstack(i,j);
    }
   }
   nflat=nis;
/* routine to drain flats to neighbors  */
   SetWorkingStatus();  
   imin=vdn(nflat);
   n=nis;
 
//   printf("Number of pits to resolve: %d\n",n);  
   np1=n;
   nt=np1*(1-per/100);

/*  initialize apool to zero  */
   for(i=i2; i< n2; i++)for(j=i1; j<n1; j++)
    apool[j][i]=0;

/*  store unresolved stack location in apool for easy deletion  */
  while(nis > 0)   /*  While AA */
  {

    i=is[imin];
    j=js[imin];
	pooln=1;
    npool=0;
    nf = 0;  /*  reset flag to that new min elev is found */
    pool(i,j);  /*  Recursive call on unresolved point with lowest elevation */

/*  Find the pour point of the pool  */
	for(ip=1; ip<=npool; ip++)
	{
		i=ipool[ip];
		j=jpool[ip];
		for(k=1; k <=8; k++)	
        {
			 jn=j+d2[k];
			 in=i+d1[k];
			 if(apool[jn][in] != pooln)  /*  neighbor not in pool  */
			 {
                et=max2(elev[j][i],elev[jn][in]);
                if(nf == 0)  /*  this is the first edge found  */
                {
                  emin=et;
                  nf=1;
                }
                else
                {
					if(emin > et)
					{
						emin = et;
					}
                }
			 }
		}
	}
/*  Fill the pool  */
    for(k=1; k<=npool; k++)
	{
		i=ipool[k];
		j=jpool[k];
      if(elev[j][i] <= emin)
	  {
		  if(dir[j][i] > 0)   /*  Can be in pool, but not flat   */
		  {
			  dir[j][i]=0;
			  /*  Add to stack  */
			  err=addstack(i,j);
		  }
		  for(ip=1; ip <=8; ip++)
		  {
			  jn=j+d2[ip];
			  in=i+d1[ip];
			  if(elev[jn][in] > elev[j][i] && dir[jn][in] > 0)
/*    Only zero direction of neighbors that are higher - because
      lower  or equal may be a pour point in a pit that must not be disrupted  */
			  {
				  dir[jn][in] = 0;
				  err=addstack(in,jn);
			  }
		  }
		  elev[j][i] =emin;

	  }
	  apool[j][i]=0;  /*  Reinitialize for next time round  */
	}

/* reset unresolved stack  */
    ni=0;
    for(ip=1; ip <= nis; ip++)
    {
       set(is[ip],js[ip],fact,mval,useflowfile);  
	   if(dir[js[ip]][is[ip]] == 0)  /* still on stack */
       {
          ni++;
          is[ni]=is[ip];
          js[ni]=js[ip];
       }
     }
	 n=nis;
 	 imin=vdn(ni);
	 if(nis < nt){
		 //printf("Percentage done %f\n",per);
//		 ocxCtrl->FloodEvent((short)per);
		 feedback("Percentage done %f\n",per,FN_FLOOD);
		 per=per+1;
		 nt=np1*(1-per/100);
	 }
  }  /*  end of while AA  */

  return(err);
}

//************************************************************************

int flood(char *demfile, char *newfile, char * flowfile, short useflowfile, 
		  char *newflowfile)
{
  int err, **flowdirection;
  float fdmval;

/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

  err=gridread(demfile,(void ***)&elev,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&mval,&filetype);
  if(err != 0)return(err);

  if( useflowfile == 1 )
	  err=gridread(flowfile,(void ***)&flowdirection,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&fdmval,&filetype);
  if(err != 0)return(err);


/*  allocate  dir and stack arrays  */
  dir = (short **) matalloc(nx, ny, RPSHRDTYPE);
  apool = (short **) matalloc(nx, ny, RPSHRDTYPE);
  istack = (int) (nx * ny * 0.1);
  pstack=istack;
  dn = (short *)malloc(sizeof(short) * istack);
  is = (short *)malloc(sizeof(short) * istack);
  js = (short *)malloc(sizeof(short) * istack);
  ipool = (short *)malloc(sizeof(short) * pstack);
  jpool = (short *)malloc(sizeof(short) * pstack);
  if(dn == NULL || is == NULL || js == NULL || ipool == NULL || jpool == NULL)
  {
//     printf("\nError:  Cannot allocate memory for stacks\n");
     return(11);
  }

  i1=0; i2=0; n1=nx; n2=ny;  /*  full grid  */ 
//  printf("Grid size: %d x %d\n",n1,n2);
  int writeflowfile=1;
  err=setdf(mval,flowdirection, fdmval, useflowfile, newflowfile, writeflowfile);
/*  free stacks  */
  free(dn); free(is); free(js); free(ipool); free(jpool); 
  free(apool[0]); free(apool);  
/*    filetype=0;   0=ASCII   1= ARCVIEW proprietary  */
  err = gridwrite(newfile,(void **)elev,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,mval,filetype);
  if(err != 0)return(err);
  free(elev[0]);
  free(elev);
  free(dir[0]);
  free(dir);
  if(useflowfile == 1)
  {
  free(flowdirection[0]);
  free(flowdirection);
  }
  return(0);  /*  ALL OK return from flood  */
}

//************************************************************************    

float min2(float e1,float e2)
{ 
  float em; 
  em=e1;
  if(e2 < em)em=e2;
  return(em);
}


//************************************************************************
//Function to add outlet to list of potential outlets - keeping in order
int addoutlist(int i,int j,int diff)
{
	long x,y;
	x=j*csize+bndbox[0]+csize/2.;
	y=bndbox[3]-i*csize-csize/2.;
	if(nout < maxno)
	{
		nout=nout+1;
		for(i=nout-1; i>0; i--)
		{
			if(diff > diffout[i-1])
			{
				diffout[i]=diffout[i-1];
				xout[i]=xout[i-1];
				yout[i]=yout[i-1];
			}
			else
			{
				diffout[i]=diff;
				xout[i]=x;
				yout[i]=y;
				return(nout);
			}
		}
		diffout[0]=diff;
		xout[0]=x;
		yout[0]=y;
		return(nout);
	}
	else
	{
		i=nout-1;
		while(i > 0)
		{
			if(diff > diffout[i-1])
			{
				diffout[i]=diffout[i-1];
				xout[i]=xout[i-1];
				yout[i]=yout[i-1];
				i=i-1;
			}
			else
			{
				diffout[i]=diff;
				xout[i]=x;
				yout[i]=y;
				return(nout);
			}
		}
		if(i>=0)
		{
			diffout[i]=diff;
			xout[i]=x;
			yout[i]=y;
			return(nout);
		}
	}
	return(0);  //should only come here if nout is 0 - so return 0.
}


//************************************************************************

/* function to compute area recursively */
void srcarea(int i,int j)
  {                                        
    int in,jn,k,anmax=0,diff,inmax,jnmax,con=0;
	if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && dir[j][i]!= -1)   /* not on boundary  */                  
    {
      if(arr[j][i]< -1)  // not touched yet.
      {
        arr[j][i]= src[j][i];                               
        for(k=1; k<=8; k++)
        {  in=i+d1[k];
           jn=j+d2[k];
/* test if neighbor drains towards cell excluding boundaries */
           if(dir[jn][in]>0 && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
             {
                srcarea(in,jn);
				if(arr[jn][in] < 0)con = -1;
				else
				{
					if(arr[jn][in] > anmax)
					{
						anmax=arr[jn][in];
						inmax=in;
						jnmax=jn;
					}
					arr[j][i]=arr[j][i]+arr[jn][in];
				}
             }
		   if(dir[jn][in] < 0)con = -1;
        }
		if(con == -1  && ccheck == 1)arr[j][i]= -1;
		diff=arr[j][i]-anmax;
		if(diff > SourceThreshold)
			nout=addoutlist(inmax,jnmax,diff);
      }
    }
	else arr[j][i] = -1;
  }

//************************************************************************

int source(char *areafile,char *slopefile,char *plenfile,char *dirfile, 
		   char *srcfile, char *elvfile, char *gordfile, char *scafile,
		   char *fdrfile, int ipar,float *p, double x, double y, int contcheck)
{
  float ndva,ndvs,ndvp,ndvd,emax,ndve,ndvo,wsum;
  float **aa, **plen, **selev, **elev;
  int **gord;
  int row, col, filetype,err,i,j,iomax,jomax,bound,ik,jk,k,itresh;
  int rcgood=1;
  ccheck=contcheck;

/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

/* read grid files */
if(ipar == 1)
   {
	err=gridread(areafile,(void ***)&aa,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndva,&filetype);
   if(err != 0)goto ERROR1;
   }

if(ipar == 2)
{
  err=gridread(scafile,(void ***)&aa,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndva,&filetype);
   if(err != 0)goto ERROR1;
  err=gridread(slopefile,(void ***)&slope,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndvs,&filetype);
  if(err != 0)goto ERROR1; 
}
if(ipar == 3)
{
  err=gridread(scafile,(void ***)&aa,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndva,&filetype);
   if(err != 0)goto ERROR1;
  err=gridread(plenfile,(void ***)&plen,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndvp,&filetype);
  if(err != 0)goto ERROR1; 
}
if(ipar == 4)
{
  err=gridread(elvfile,(void ***)&elev,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndve,&filetype);
  if(err != 0)goto ERROR1;  
}
if(ipar == 5)
{ 
	err=gridread(gordfile,(void ***)&gord,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndvo,&filetype);
  if(err != 0)goto ERROR1; 
}
if(ipar == 6)
{ 
	err=gridread(fdrfile,(void ***)&gord,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndvo,&filetype);
  if(err != 0)goto ERROR1; 
}

if((src = (short **)matalloc(nx,ny, RPSHRDTYPE)) == NULL)
{
  err=7;
//  printf("Could not allocate memory for src\n");
  goto ERROR1;
}  
        

/*  Flag sources  */
for(i=0; i < ny; i++)
  for(j=0; j< nx; j++)
  {
	  src[j][i] = 0;
	  if(ipar == 1)   /*  Area threshold   */
	  {
		  src[j][i]= (aa[j][i] >= p[0]) ? 1 : 0;
	  }
	  else if(ipar == 2)   /*  Slope and area combination   */
	  {
		  if(slope[j][i] > 0.)
			  src[j][i] = (aa[j][i]* pow((double)slope[j][i],(double)p[1]) >= p[0])
			  ? 1: 0;
	  }else if(ipar == 3)  /*  Slope and Length combination   */
	  {
		  if(plen[j][i] > 0.)
			  src[j][i] = (aa[j][i] >= p[0]* pow((double)plen[j][i],(double)p[1])) 
			  ? 1: 0;
	  }
	  else if(ipar == 5)  /*  Grid order threshold  */
		  src[j][i] = (gord[j][i] >= p[0]) ? 1: 0;
	  else if(ipar == 6)  /*  Given flow directions threshold  */
		  src[j][i] = (gord[j][i] > 0) ? 1: 0;
  }
  if(ipar == 4)  /* Peuker and Douglas algorithm  */
  {
/*  Initialize internal cells to 1 for Peuker and Douglas algorithm and smooth  */
if((selev = (float **)matalloc(nx,ny, RPFLTDTYPE)) == NULL)
{
  err=7;
//  printf("Could not allocate memory for selev\n");
  goto ERROR1;
}
  for(i=0; i <ny; i++)
	 for(j=0; j<nx; j++)
	 {
		  if(i == 0 || i == (ny-1) || j == 0 || j == (nx-1) || elev[j][i] <= ndve)
		  {
			  selev[j][i]=elev[j][i];
		  }
		  else
		  {
		    src[j][i] = 1;
		    selev[j][i]=p[0] * elev[j][i];
		    wsum=p[0];
		    if(p[1] > 0.)
              for(k=1; k<=7; k=k+2)
			  {
			    if(elev[j+d2[k]][i+d1[k]] > ndve)
				{
			  	  selev[j][i] += elev[j+d2[k]][i+d1[k]]*p[1];
				  wsum += p[1];
				}				  
			  }
		    if(p[2] > 0.)
              for(k=2; k<=8; k=k+2)
			  {
			    if(elev[j+d2[k]][i+d1[k]] > ndve)
				{
			  	  selev[j][i] += elev[j+d2[k]][i+d1[k]]*p[2];
				  wsum += p[2];
				}				  
			  }
		  }
	  }
    free(elev[0]); free(elev);
	elev = selev;   
	for(i=0; i <ny-1; i++)
	  for(j=0; j<nx-1; j++)
	  {
		  emax= elev[j][i];
          iomax=0;
          jomax=0;           
          bound= 0;  /*  .false.  */
/*  --FIRST PASS FLAG MAX ELEVATION IN GROUP OF FOUR  */
          for(ik=0; ik<2; ik++)
			  for(jk=1-ik; jk < 2; jk++)
			  {
                  if(elev[j+jk][i+ik] > emax)
				  {
                      emax=elev[j+jk][i+ik];
                      iomax=ik;
                      jomax=jk;
				  }
                  if(elev[j+jk][i+ik] <= ndve)
                     bound= 1;  /*  .true.  */
			  }
/*  c---Unflag max pixel */
             src[j+jomax][i+iomax] = 0;
/*  c---Unflag pixels where the group of 4 touches a boundary  */
          if(bound == 1)
		  {
            for(ik=0; ik < 2; ik++)
              for(jk=0; jk< 2; jk++)
			  {
				  src[j+jk][i+ik]=0;
			  }
		  }
/* 		  i.e. unflag flats.  */
            for(ik=0; ik < 2; ik++)
              for(jk=0; jk< 2; jk++)
			  {
                 if(elev[j+jk][i+ik] == emax)src[j+jk][i+ik] = 0;
			  }
	  }
  }
  if(ipar <= 3){free(aa[0]);  free(aa);  }
  if(ipar == 2){free(slope[0]); free(slope);}
  if(ipar == 3){free(plen[0]); free(plen);}
  if(ipar == 4){free(elev[0]); free(elev);}
  if(ipar == 5 || ipar == 6){free(gord[0]); free(gord);}

/*  Now get directions and compute area's  */

  err=gridread(dirfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndvd,&filetype);
  if(err != 0)goto ERROR1; 
  if((arr = (int **)matalloc(nx,ny, RPINTDTYPE)) == NULL)
  {
    err=7;
//    printf("Could not allocate memory for arr\n");
	goto ERROR1;
  }  
for(i=0; i < ny; i++)
  for(j=0; j<nx; j++)
  {
	  arr[j][i] = -2;
  }
  	nout=0;
	itresh=1;
    if(ipar == 4)itresh = p[3];
	col= (int)floor((x-bndbox[0])/csize);
	row= (int)floor((bndbox[3]-y)/csize);
 	if(row >0 && row < ny-1 && col > 0 && col < nx-1)
		{
/* call drainage area subroutine for pixel to zero on  */
			srcarea(row,col);
			if(arr[col][row] < itresh)
			{
				err=9;  //Use flag 9 to indicate no outlet found
				goto ERROR9;
			}
		}
	else
	{
//  Do all pixels
	  for(i=1; i < ny-1; i++)
		for(j=1; j<nx-1; j++)srcarea(i,j);
		rcgood=0;  // no outlet coordinates found
	}

    free(dir[0]); free(dir);

//  Now threshold the src file

	for(i=1; i < ny-1; i++)
		for(j=1; j<nx-1; j++)
		{
			if(arr[j][i] >= itresh)arr[j][i]=1;
			else arr[j][i] = 0;
		}

   err = gridwrite(srcfile,(void **)arr,RPINTDTYPE,nx,ny,dx,dy,
                bndbox,csize,-2.,filetype);
   if(rcgood==0)err=8;  //use this to flag that no outlet was specified.

ERROR9:
   free(src[0]); free(src);
   free(arr[0]); free(arr);

ERROR1:
   return(err);
}
int validateoutlet(long x,long y,char *aread8file, char *srcfile)
{
	int err=0,col,row;
  err=gridread(aread8file,(void ***)&arr,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype);
  if(err !=0)goto ERROR1;
  err=gridread(srcfile,(void ***)&src,RPSHRDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype);
  if(err !=0)goto ERROR1;
	col= (int)floor((x-bndbox[0])/csize);
	row= (int)floor((bndbox[3]-y)/csize);
 	if(row >0 && row < ny-1 && col > 0 && col < nx-1)
	{
		if(arr[col][row] <=0 || src[col][row] <=0)err=1;
	}
	else err = 1;
	free(arr[0]); free(arr);
	free(src[0]); free(src);
ERROR1:
	return(err);
}

//************************************************************************

int netsetup(char *fnprefix, int method,
                         float p1,float p2,float p3,float p4,
                         long xr,long yr, int contcheck)
 {
        int err,itresh,icr,icend, **area,nyn,nxn,nmain;
        short **dir;
        char areafile[MAXLN],pfile[MAXLN],slopefile[MAXLN],plenfile[MAXLN],
                elevfile[MAXLN],gordfile[MAXLN],ordfile[MAXLN],srcfile[MAXLN],
                treefile[MAXLN],coordfile[MAXLN],scafile[MAXLN],fdrfile[MAXLN];
        float p[4];
        FILE *fp;
        p[0]=p1;
        p[1]=p2;
        p[2]=p3;
        p[3]=p4;

        nmain=nameadd(areafile,fnprefix,"ad8");
        nmain=nameadd(scafile,fnprefix,"sca");
        nmain=nameadd(slopefile,fnprefix,"slp"); 
        nmain=nameadd(plenfile,fnprefix,"plen"); 
        nmain=nameadd(pfile,fnprefix,"p");
        nmain=nameadd(elevfile,fnprefix,"fel");
        nmain=nameadd(srcfile,fnprefix,"src");
        nmain=nameadd(ordfile,fnprefix,"ord");
        nmain=nameadd(gordfile,fnprefix,"gord");
		nmain=nameadd(fdrfile,fnprefix,"fdrn");
/*  ASCII  files*/
        strcpy(treefile,"");
        strncat(treefile,fnprefix,nmain);
		strcat(treefile,"tree.dat");
        strcpy(coordfile,"");
        strncat(coordfile,fnprefix,nmain);
		strcat(coordfile,"coord.dat");

	err=source(areafile,slopefile,plenfile,pfile,srcfile,elevfile,
           gordfile,scafile,fdrfile, method,p,(double)xr,(double)yr, contcheck);
   itresh=1;  // Thresholding to 1 done in source
    if(err == 8)feedback("No outlets specified - source file written\nExamine source file to identify outlet",1,FN_NET);
  if(err == 0)   //  Only proceed if no errors - else FORTRAN crashes
  {
/*    read dirfile   */
   err=gridread(pfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);

/*    read srcfile   */
   err=gridread(srcfile,(void ***)&area,RPINTDTYPE,&nxn,&nyn,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);
   if(nxn != nx || nyn != ny)
   {
//	   printf("Incompatible grid sizes\n");
//	   exit(0);
		return 1;
   }

   /*  Call fortran NETEX subroutine  */
    NETEX(dir[0],area[0],treefile,strlen(treefile),coordfile,strlen(coordfile),
		ordfile,strlen(ordfile),&nx,&ny,&itresh,&icr,&icend,&dx,&dy,bndbox,&csize,
		&filetype,&err);   

   err = gridwrite(ordfile,(void**)area,RPINTDTYPE,nx,ny,dx,dy,bndbox,csize,-1,filetype);
   if(err)
	   return false;

   free(area[0]); free(area);
   free(dir[0]); free(dir);
/*    free and reread dir because netex changes them  */
   err=gridread(pfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);

/*    read areafile   */
   err=gridread(areafile,(void ***)&area,RPINTDTYPE,&nxn,&nyn,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);
   if(nxn != nx || nyn != ny)
   {
//	   printf("Incompatible grid sizes\n");
//	   exit(0);
		return 1;
   }
/*    read elevfile   */
   err=gridread(elevfile,(void ***)&elev,RPFLTDTYPE,&nxn,&nyn,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);
   if(nxn != nx || nyn != ny)
   {
//	   printf("Incompatible grid sizes\n");
//	   exit(0);
		return 1;
   }

/*  Call fortran NETPROP subroutine  */
   NETPROP(dir[0],area[0],elev[0],coordfile,strlen(coordfile),&icr,&icend,&dx,
	   &dy,&nx,&ny,bndbox,&csize,&err);  
 
   free(dir[0]); free(dir);
   free(elev[0]); free(elev);
   free(area[0]); free(area);
  }  //  end if associated with err from source
   return(err);
}

//************************************************************************

/* function to compute area recursively */
void d2area(int i,int j)
{
    int in,jn,k,a1,a2;
    float ld;
    if(gorder[j][i] <= 0)
    {
      if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && dir[j][i]!= -1 && aa[j][i]>=tresh)
      /* not on boundary and in area considered  */
      {
        gorder[j][i]=1;
		ltp[j][i]=0.;
		lengd[j][i]=0.;
		a1=0;
		a2=0;
	
        for(k=1; k<=8; k++)
        {  in=i+d1[k];
           jn=j+d2[k];
	   /* test if neighbor drains towards cell excluding boundaries */
           if(dir[jn][in]>=0) 
            {
           if(aa[jn][in]>=tresh && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
             {
                d2area(in,jn);
				if(gorder[jn][in] >= a1)
				{
					a2=a1;
					a1=gorder[jn][in];
				}
				else if (gorder[jn][in] > a2)a2=gorder[jn][in];
				ld=lengd[jn][in]+dist[dir[jn][in]];
                ltp[j][i] += ltp[jn][in] + dist[dir[jn][in]];
				if(ld > lengd[j][i])
				{
                 lengd[j][i]=ld;
                 }
              }
           }
        }
		 if(a2+1 > a1)gorder[j][i]=a2+1;
		 else gorder[j][i]=a1;
      }
    }
}

//************************************************************************

int gridnet(char *pfile,char *plenfile,char *tlenfile,char *gordfile,char *afile,
			long x,long y, int mask, int outlet, int thresh)
 {

   int i,j,row,col,err;
   tresh=thresh;
/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

/*    read pointers  */
   err=gridread(pfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
   bndbox,&csize,&ndvp,&filetype);
   if(mask == 1)
   {
/*   read mask  */
   err=gridread(afile,(void ***)&aa,RPINTDTYPE,&nx,&ny,&dx,&dy,
   bndbox,&csize,&ndv,&filetype);
   if(err != 0)return(err);
   }
   else
   {
	   aa=(int **)matalloc(nx,ny,RPINTDTYPE);
	   for(i=0; i<ny; i++)
		   for(j=0; j<nx; j++)
			   aa[j][i]=0;
   }
//  fprintf(stderr,"...now allocating memory...\n");
/* allocate memory to arrays */
   gorder= (int **)  matalloc(nx,ny,RPINTDTYPE);
   ltp= (float **) matalloc(nx,ny,RPFLTDTYPE);
   lengd= (float **) matalloc(nx,ny,RPFLTDTYPE);

 // fprintf(stderr,"...allocation done...\n");
 // fprintf(stderr,"%d ",tresh,"\n");

/*  Calculate Distances  */
    for(i=1; i<=8; i++){
     dist[i]=sqrt(d1[i]*d1[i]*dy*dy+d2[i]*d2[i]*dx*dx);
   }
/* initialize arrays to 0 */
     for(i=0; i<ny; i++)
     {  for(j=0; j<nx; j++)
        {
          gorder[j][i]= -1;
          ltp[j] [i]= -1;
          lengd[j][i]= -1;
        }
     }
//    fprintf(stderr,"..calcuating areas etc...\n");
	 if(outlet == 1)  /*  Only compute area's for designated location  */
   {
		col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Given coords out of area\n");
			row=0; col=0;
		}
		else
			d2area(row,col);
	}
	 else
	 {
		 for(i=0; i<ny; i++)
			 for(j=0; j<nx; j++)
				 d2area(i,j);
      }
 
//fprintf(stderr,"..finished calcuating areas etc...\n");

err = gridwrite(plenfile,(void **)lengd,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
err = gridwrite(tlenfile,(void **)ltp,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
err = gridwrite(gordfile,(void **)gorder,RPINTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
   free(gorder[0]);
   free(gorder);
   free(aa[0]);
   free(aa);
   free(dir[0]);
   free(dir);
   free(ltp[0]);
   free(ltp);  
   free(lengd[0]);
   free(lengd);
   return(err);
 }


//*************************** cnetwrite   ********************************************
//  Function to write shape file of channel network as read in
/* int cnetwrite(int **cnet,float **coord,int nlmax,int nc, char *shpfile)
{
	shapefile s;
	int i,j,istart,iend;
	double x,y;
	s.create(shpfile,API_SHP_POLYLINE);
	field f("LINKNO",FTInteger,10,0);
	s.insertField(f,0);
	for(i=0; i<nlmax;i++)
	{
		shape * sh = new shp_polyline();;
		istart=cnet[2-1][i];
		iend=cnet[3-1][i];
		for(j=0; j<=iend-istart; j++)
		{
			x=coord[0][j+istart];
			y=coord[1][j+istart];
			api_point p;
			p.setX(x);
			p.setY(y);
			sh ->insertPoint(p,j);
		}
		s.insertShape( sh, i);	
		cell v;
		v.setValue(i);
		s[i]->setCell(v,0);
	}
//	s.save(shpfile);
	s.close(shpfile);
	return(0);
}  */
//    API version
/*int cnetwrite(int **cnet,float **coord,int nlmax,int nc, char *shpfile)
{
	shapefile s;
	int i,j,istart,iend;
	double x,y;
	int shandle = shp_create(shpfile,API_SHP_POLYLINE);
	shp_insertField(shandle,0,"LINKNO",10,0,FTInteger);
	for(i=0; i<nlmax;i++)
	{
		shp_createShape(shandle,i);
		istart=cnet[2-1][i];
		iend=cnet[3-1][i];
		for(j=0; j<=iend-istart; j++)
		{
			x=coord[0][j+istart];
			y=coord[1][j+istart];
			shp_insertPoint(shandle,x,y,i,j);
		}
		shp_setCell(shandle,"",i,0.0,i,0);
	}
	shp_close(shandle,shpfile);
	return(0);
}    */

shapefile shp1;   // Global shapfile

int reachshape(int **cnet,float **coord,int **tree, int ilink, int nb,int *mag, int *order)
{
	shape * sh = new shp_polyline();
	double x,y,length,glength,x1,y1,xlast,ylast,usarea,dsarea,dslast,dl,drop,slope;
	int rstart,rend,istart,iend,j;
	rstart=tree[1][ilink-1];   //  start link in cnet
	rend=tree[0][ilink-1];     //  end link in cnet
	istart=cnet[1][rstart];  //  start coord for first link
	iend=cnet[2][rstart];    //  end coord for first link
	x1=coord[0][istart];
	y1=coord[1][istart];
	length=0.;
	xlast=x1;
	ylast=y1;
	usarea=coord[4][istart];
	dslast=usarea;
	dsarea=usarea;

	for(j=0; j<=iend-istart; j++)  //  add first link
		{
			x=coord[0][j+istart];
			y=coord[1][j+istart];
			dl=sqrt((x-xlast)*(x-xlast)+(y-ylast)*(y-ylast));
			if(dl > 0.)
			{
				length=length+dl;
				xlast=x;  ylast=y;
				dsarea=dslast;   // keeps track of last ds area
				dslast=coord[4][j+istart];
			}
			api_point p(x,y);
			sh ->insertPoint(p,0);

		}
	while(rstart != rend)
	{
		rstart=cnet[3][rstart];  // next link down
		istart=cnet[1][rstart];  //  start coord 
		iend=cnet[2][rstart];    //  end coord 
		for(j=1; j<=iend-istart; j++)  //  add link skipping first point because it is last point of previous link
		{
			x=coord[0][j+istart];
			y=coord[1][j+istart];
			dl=sqrt((x-xlast)*(x-xlast)+(y-ylast)*(y-ylast));
			if(dl > 0.)
			{
				length=length+dl;
				xlast=x;  ylast=y;
				dsarea=dslast;
				dslast=coord[4][j+istart];
			}
			api_point p(x,y);
			sh ->insertPoint(p,0);
		}
	}
	drop=coord[3][cnet[1][rstart]]-coord[3][cnet[2][rend]];
	slope=0.;
	float dsdist=coord[2][iend];
	float usdist=coord[2][istart];
	float middist=(dsdist+usdist)*0.5;
	if(length > 0.)slope=drop/length;
	glength=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	shp1.insertShape( sh, 0);	
	cell v;
	v.setValue(ilink);    shp1[0]->setCell(v,0);
	v.setValue(tree[2][ilink-1]);   shp1[0]->setCell(v,1);
	v.setValue(tree[3][ilink-1]);   shp1[0]->setCell(v,2);
	v.setValue(tree[4][ilink-1]);   shp1[0]->setCell(v,3);
	v.setValue(order[ilink-1]);   shp1[0]->setCell(v,4);
	v.setValue(length);   shp1[0]->setCell(v,5);
	v.setValue(mag[ilink-1]);   shp1[0]->setCell(v,6);
	v.setValue(dsarea);   shp1[0]->setCell(v,7);
	v.setValue(drop);   shp1[0]->setCell(v,8);
	v.setValue(slope);   shp1[0]->setCell(v,9);
	v.setValue(glength);   shp1[0]->setCell(v,10);
	v.setValue(usarea);   shp1[0]->setCell(v,11);
	v.setValue(nb);   shp1[0]->setCell(v,12);
	v.setValue(dsdist); shp1[0]->setCell(v,13);
	v.setValue(usdist); shp1[0]->setCell(v,14);
	v.setValue(middist); shp1[0]->setCell(v,15);
    return(0);
}
/* ****************************** markbasin() ********************************* */

int markbasin(int *nb, int ilink, int **cnet, int **tree, float **coord, int *nrch, 
			  int ordert, int *mag, int *order)
{
	int row,col,iup1,iup2,row1,col1,flag = 0,thisreach,nbp;
	float x,y;
	int icend, icbeg, icarea, ic1, ic2,wlink,elink;  /*  variables for coord positions  */
	
	/*  -1 in all subscripting for fortran compatibility  */
  icend = cnet[3-1][tree[0][ilink-1]];   /*  This is coord of end of last link */
  icbeg = cnet[2-1][tree[1][ilink-1]];   /*  This is coord of beg of first link */
  ic1 = cnet[2-1][tree[0][ilink-1]];  /*  This is coord of beginning of last link */
/*  printf("%d %d %d\n",ic1,icbeg,icend);  */
  if(ic1 < icend)
  {
		*nb = *nb + 1;   /*  Increment basin counter  */
       if(*nb > 1)
	   {
		   icarea=icend-1;
	   }
	   else
	   {
		   icarea=icend;
	   }
	   /*  For anything other than first basin start one grid cell back  */
	  x=coord[1-1][icarea];
	  y=coord[2-1][icarea];
	  	col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Error 1: encountered coordinate outside grid\n");
		}
		larea(row,col, *nb);  /*  Label the region that drains to this pixel  */
  }
  else
  {
	  x=coord[1-1][icend];
	  y=coord[2-1][icend];
	  	col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Error 2: encountered coordinate outside grid\n");
		}
		col1=col;
		row1=row;
	  x=coord[1-1][icbeg];
	  y=coord[2-1][icbeg];
	  	col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Error 3: encountered coordinate outside grid\n");
		}
		if(col == col1 && row == row1)
		{
/*      	  printf("Dummy link\n",icend);  */
		  icarea=icend;
		  flag=1;
		}
		else
		{
/*			printf("Zero length ending segment\n");  */
			*nb = *nb + 1;
			if(*nb == 1)  /*  For first link label from end  */
			{  
				row=row1;
				col=col1;
				icarea=icend;
			}
			else
			{  /*  Trace down tree structure till end of segment  */
				icarea = -1;  /*  flag to designate unassigned  */
				elink = tree[0][ilink-1];
				wlink = tree[1][ilink-1];  /*  First link of segment  */
				while(wlink != elink)
				{
					ic1=cnet[2-1][wlink];  /*  Beginning of working link  */
					ic2=cnet[3-1][wlink];  /*  End of working link  */
                    if(ic2 > ic1)icarea=ic2-1;   /*  This is a link with positive length */
					wlink = cnet[4-1][wlink];  /*  Next link downstream   */
				}
//				if(icarea < 0)printf("Logic error in tree structure\n");
	  x=coord[1-1][icarea];
	  y=coord[2-1][icarea];
	  	col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
//			printf("Error 4: encountered coordinate outside grid\n");
		}
			}
/*  When infinite looking loop is exited rown and coln equal col1 and row1
    indicating the end found.  row and col are the previous point.  */
			larea(row,col, *nb);  /*  Label the region that drains to this pixel  */
		}
  }
  /*  Search for upstream basins   */
	  iup1=tree[4-1][ilink-1]; /*  Pointers to upstream links  */
	  iup2=tree[5-1][ilink-1]; 
		if(iup1 > 0)
		{
/*  There are basins upstream  */
			if(flag == 1)
			{   /*   Dummy 0 length reach   */
				*nrch = *nrch+1;
				thisreach = *nrch;
				//  write shape 
				reachshape(cnet,coord,tree,ilink,0,mag,order);
				rtable[thisreach][1]=thisreach;
				rtable[thisreach][2]=markbasin(nb, iup1, cnet, tree, coord, nrch,ordert,mag,order);
			    rtable[thisreach][3]=markbasin(nb, iup2, cnet, tree, coord, nrch,ordert,mag,order);
				/*  Assign properties to dummy reach  */
				rprops[thisreach][1]=0.01;  /*  Slope  */
				rprops[thisreach][2]=coord[5-1][icarea];   /*  Contributing area  */
				rprops[thisreach][3]=0.;   /*  Length  */
				rprops[thisreach][4]=coord[0][icarea];  /*  End X coordinate */
				rprops[thisreach][5]=coord[1][icarea];  /*  End Y coordinate */
			}
			else
			{
				/*  Lower half reach  */
				*nrch = *nrch+1;
				thisreach = *nrch;
				rtable[thisreach][1]= thisreach;
				rtable[thisreach][2]= *nb; 
				rtable[thisreach][3]= *nrch+1;
				rprops[thisreach][3]= (coord[3-1][icbeg]-coord[3-1][icend])/2.;
				rprops[thisreach][2]= coord[5-1][icarea];
				rprops[thisreach][1]= (coord[4-1][icbeg]-coord[4-1][icend])
					/(2.* rprops[thisreach][3]);
				rprops[thisreach][4]=coord[0][icarea];  /*  End X coordinate */
				rprops[thisreach][5]=coord[1][icarea];  /*  End Y coordinate */
				if(ordert == 0)nbp=0;
				else nbp= *nb;
				reachshape(cnet,coord,tree,ilink,nbp,mag,order);
				/*  Upper half reach  */
				*nrch = *nrch +1;
				rtable[thisreach+1][1]= *nrch;
				rtable[thisreach+1][2]=markbasin(nb, iup1, cnet, tree, coord, nrch,ordert,mag,order);
			    rtable[thisreach+1][3]=markbasin(nb, iup2, cnet, tree, coord, nrch,ordert,mag,order);
				rprops[thisreach+1][3]= rprops[thisreach][3];
				rprops[thisreach+1][2]= rprops[thisreach][2];
				rprops[thisreach+1][1]= rprops[thisreach][1];
				rprops[thisreach+1][4]= -9999.;  /*  Dummy reach does not have coord end */
				rprops[thisreach+1][5]= -9999.;  /*  Dummy reach does not have coord end */
			}
		}
		else
		{
			/*  This is an external basin  */
				*nrch = *nrch+1;
				thisreach = *nrch;
				rtable[*nrch][1]= *nrch;
				rtable[*nrch][2]= *nb; 
				rtable[*nrch][3]= 0;
				rprops[thisreach][3]= (coord[3-1][icbeg]-coord[3-1][icend])/2.;
				rprops[thisreach][2]= coord[5-1][icarea];
				rprops[thisreach][1]= (coord[4-1][icbeg]-coord[4-1][icend])
					/(2.* rprops[thisreach][3]);
				rprops[thisreach][4]=coord[0][icarea];  /*  End X coordinate */
				rprops[thisreach][5]=coord[1][icarea];  /*  End Y coordinate */
				if(ordert == 0)nbp=0;
				else nbp= *nb;
				reachshape(cnet,coord,tree,ilink,nbp,mag,order);
		}
		return(thisreach);
}


//*************************** subbasinsetup ******************************************

int subbasinsetup(char *pfile, char *wfile,char *treefile,char *coordfile, 
				  char *shpfile,int ordert)
{
  char dummy[MAXLN];
  int err,nlmax,i,j,nc,nb,filetype,n,nrc,nrch,
	  nhc,thisreach;
  int **cnet, **tree, *source, *mag, *iord;
  float **coord, ndv;

  FILE *fp;
/* define directions */
   d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
   d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

/*  reading Tree file  */
	fp = fopen(treefile,"r");
    if (fp == NULL) return 1; //printf("Unable to open treefile\n");
/*  count length of file  */
	i=0;
	nlmax= -1;
	while(i != EOF)
	{
		i=readline(fp, dummy);
		nlmax++;
	}
/*	printf("%d\n",nlmax);   */
	fclose(fp);
/*  Allocate arrays for use in fortran  */
	cnet = (int **) matalloc(7,nlmax+1,RPINTDTYPE);
	tree = (int **) matalloc(5,nlmax,RPINTDTYPE);
	source = (int *) malloc(sizeof(int)*nlmax);
	mag = (int *) malloc(sizeof(int)*nlmax);
	iord = (int *) malloc(sizeof(int)*nlmax);

	fp = fopen(treefile,"r");
    if (fp == NULL) return 1; //printf("Unable to open treefile\n");
	for(j = 0; j < nlmax; j++)
		for(i=0; i<7; i++)fscanf(fp,"%d",&cnet[i][j]);
	fclose(fp);

/*  Reading coordfile  */
	fp = fopen(coordfile,"r");
    if (fp == NULL) return 1; //printf("Unable to open coordfile\n");
/*  count length of file  */
	i=0;
	nc= 0;  /*  nc ends up actually being one more than the number of coords 
	            retained for consistency with original linkan code */
	while(i != EOF)
	{
		i=readline(fp, dummy);
		nc++;
	}

/*	printf("%d\n",nc);  */
	coord = (float **) matalloc(5,nc,RPFLTDTYPE);
	nc=nc-1;
	fclose(fp);
	fp = fopen(coordfile,"r");
    if (fp == NULL) return 1;//printf("Unable to open coordfile\n");
	for(j = 0; j < nc; j++)
		for(i=0; i<5; i++)fscanf(fp,"%f",&coord[i][j]);
	fclose(fp);
//    err=cnetwrite(cnet,coord,nlmax,nc,shpfile);
//	if(err)return(err);
	n = LINKSOURCE(cnet[0],tree[0],coord[0],mag,iord,source,&nlmax,&nc,&ordert,&err);

	/*  Free memory no longer needed  */
/*	free(mag);
	free(iord);  // used to be commented out
	free(source);   */
    err=gridread(pfile,(void ***)&dir,RPSHRDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype); 
	if((aw = (short **)matalloc(nx,ny, RPSHRDTYPE)) == NULL)
	{	
	 err=7;
//	 printf("Could not allocate memory for src\n");
	 goto ERROR0;
	} 
/*  Initialize aw to 0   */
	for(i=0; i < ny; i++)
	{
		for(j=0; j<nx; j++)
		{
			aw[j][i]=-1;
		}
	}
	nb=0;
	nrc = 2*n;  /* The number that reach counting starts from  */
	nrch = nrc-1;   /* Initialize current reach number  */
	nhc = 5*n-2;  /*  The maximum number of reaches possible in binary tree */
	rtable =  imatrix(nrc,nhc,1,3);   /*  Numerical Recipes matrix allocation  */
	rprops =  matrix(nrc,nhc,1,5); 
//  create shapefile
//	shapefile s;

	shp1.create(shpfile,API_SHP_POLYLINE);
	{field f("DOUT_MID",FTDouble,16,1);  shp1.insertField(f,0);}
	{field f("DOUT_START",FTDouble,16,1);  shp1.insertField(f,0);}
	{field f("DOUT_END",FTDouble,16,1);  shp1.insertField(f,0);}
	{field f("WSNO",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("US_Cont_Area",FTDouble,16,0);  shp1.insertField(f,0);}
	{field f("Straight_Length",FTDouble,16,0);  shp1.insertField(f,0);}
	{field f("Slope",FTDouble,16,12);  shp1.insertField(f,0);}
	{field f("Drop",FTDouble,16,2);  shp1.insertField(f,0);}
	{field f("DS_Cont_Area",FTDouble,16,0);  shp1.insertField(f,0);}
	{field f("Magnitude",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("Length",FTDouble,16,0);  shp1.insertField(f,0);}
	{field f("Order",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("USLINKNO2",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("USLINKNO1",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("DSLINKNO",FTInteger,6,0);  shp1.insertField(f,0);}
	{field f("LINKNO",FTInteger,6,0);  shp1.insertField(f,0);}	
	
	thisreach=markbasin(&nb,1,cnet,tree,coord, &nrch,ordert,mag,iord);  /*  Recursive call to mark basins  */

	shp1.close(shpfile);
//  If ordert = 0 then write a single watershed file
	if(ordert ==0)
	{
		for(i=0; i<ny; i++)
			for(j=0; j<nx; j++)
				if(aw[j][i]>=1)aw[j][i]=1;
	}
	err=gridwrite(wfile, (void **)aw,RPSHRDTYPE,nx,ny,dx,dy,
       bndbox,csize, -1, filetype);

/*	free(aw[0]); free(aw);
	free(cnet[0]);  free(cnet);
	free(tree[0]);  free(tree);
	free(coord[0]);  free(coord);
	free(dir[0]);  free(dir);
	free_imatrix(rtable,nrc,nhc,1,3);   /*  Numerical Recipes matrix free 
	free_matrix(rprops,nrc,nhc,1,5);  */ 
ERROR0:
	return(err);
} 


/*  ***************************** larea()  *************************** */

/* function to label area recursively */
void larea(int i,int j,int lab)
  {                                        
    int in,jn,k;
    if(aw[j][i]!= (short)lab)
    {
      if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && dir[j][i]!= -1)
                 /* not on boundary  */
      {
        aw[j][i]=(short)lab;                               
        for(k=1; k<=8; k++)
        {  in=i+d1[k];
           jn=j+d2[k];
/* test if neighbor drains towards cell excluding boundaries */
           if(dir[jn][in]>=0 && (dir[jn][in]-k==4||dir[jn][in]-k==-4))
             {
                larea(in,jn,lab);
             }
        }
      }
    }
  }

//*******************************deprec**************************************
/* function to compute dependencies recursively.
   This invokes symmetry and works the opposite way round to dparea*/
void deprec(int i,int j)
{
   int in,jn,k;
   float p;
   if(apool[j][i] == 0)
   {
   apool[j][i]=1;  /*  Use this to flag pixel as done   */
/*   printf("%d %d %g\n",i,j,pout);   */
	if(i!=0 && i!=ny-1 && j!=0 && j!=nx-1 && ang[j][i] >= -0.5)
       /* i.e. the cell isn't outside domain */
         { 
/*    add proportion out to dependence  */
           for(k=1; k<=8; k++)
           {
/* Find the proportion draining to each neighboring cell  */
		    p=prop(ang[j][i],k);
            if(p>0.0)  /*  Recurse to downslope neighbor */	
            {
		         in = i + d1[k];
		         jn = j + d2[k];
				 if(apool[jn][in] == 0)
				 {
					 deprec(in,jn);
				 }
	    		 dep[j][i]= dep[j][i]+p*dep[jn][in];
            }
            }
         }
   }
} 

//****************************************dependence***************************
/* Subroutine to compute Dependence map   */
int dependence(int *infi,int *infj,int ninf)
{
   int i,j, k;
/* Define directions */

d1[1]=0; d1[2]= -1; d1[3]= -1; d1[4]= -1; d1[5]=0; d1[6]=1; d1[7]=1; d1[8]=1;
d2[1]=1; d2[2]=1; d2[3]=0; d2[4]= -1; d2[5]= -1; d2[6]= -1; d2[7]=0; d2[8]=1;

/* Calculate angles for interpolation */                                      
aref[0]= -atan2(dy,dx);
aref[1]=0.;
aref[2]= -aref[0];
aref[3]=0.5*PI;
aref[4]=PI-aref[2];
aref[5]=PI;
aref[6]=PI+aref[2];
aref[7]=1.5*PI;
aref[8]=2.*PI-aref[2];
aref[9]=2.*PI;
/*  Initialize dependence to 0  */
    for(i=0; i<ny; i++)
      for(j=0; j<nx; j++)
	  {
         dep[j][i]= 0.;
		 apool[j][i]=0;
	  }

/*  For each pixel in dependence set initialize dep */
    for(k=0; k<ninf; k++)
	{
		dep[infj[k]][infi[k]]=1.;
		apool[infj[k]][infi[k]]=1;   /*  Set this to mark as done and avoid 
									 double counting in case these drain towards 
									 another in the dependence set */
     }

/* "lower block" */
for(i=ny/2; i<ny-1; i++)
   {for(j=nx/2; j<nx-1; j++)
      deprec(i,j);
    for(j=nx/2-1; j>=1; j--)
      deprec(i,j);
   }
 /* "upper block" */
for(i=ny/2-1; i>=1; i--)
   {for(j=nx/2; j<nx-1; j++)
      deprec(i,j);
    for(j=nx/2-1; j>=1; j--)
      deprec(i,j);
   }
	  return(0);
}  

//************************************depgrd**********************************

int depgrd(char *angfile,char *dgfile, char *depfile)
{
   int err,*rows, *cols,n,i,j,**dg;
 
   /* read angles */
    err=gridread(angfile,(void ***)&ang,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype);
    if(err != 0)goto ERROR1;  
  
/* read disturbance grid file  */  
    err=gridread(dgfile,(void ***)&dg,RPINTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype);
  if(err != 0)goto ERROR2;           
/*  Allocate memory  */
	dep = (float **) matalloc(nx, ny, RPFLTDTYPE);
	apool = (short **) matalloc(nx, ny, RPSHRDTYPE);

	n=0;
    for(i=0; i<ny; i++)
      for(j=0; j<nx; j++)
	  {
         if(dg[j][i] >= 1)n=n+1;
	  }
	rows = (int *)malloc(sizeof(int)*(n));
	cols = (int *)malloc(sizeof(int)*(n));
	n=0;
    for(i=0; i<ny; i++)
      for(j=0; j<nx; j++)
	  {
         if(dg[j][i] >= 1)
		 {
			 rows[n]=i;
			 cols[n]=j;
			 n=n+1;
		 }
	  }
	err=dependence(rows,cols,n);
/*    if(err!=0)printf("Dependence error %d\n",err);  */

	/*  Write out areas  */  
    err = gridwrite(depfile,(void **)dep,RPFLTDTYPE,nx,ny,dx,dy,
             bndbox,csize,-1.,filetype);
	free(dep[0]);
    free(dep);   
	free(apool[0]);
	free(apool);
ERROR2:
	free(dg[0]);
	free(dg);
ERROR1:
    free(ang[0]);
    free(ang);
	return(err);
} 


   
   
