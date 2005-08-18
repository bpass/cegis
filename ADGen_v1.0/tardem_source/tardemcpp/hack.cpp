/*  Program to output area and main path length by links, for Hack's law

  David G Tarboton
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
  
#include "tardemlib.h"
#include "lsm.h"
#include <string.h>

int hack(char *plenfile,char *treefile,char *coordfile,char *hackfile)
{
	FILE *fp;
	int i,j,nlmax, **cnet,nc,err,ic1,ic2,ic,col,row;
	float **plen,**coord,x,y;
	char dummy[MAXLN];
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
/*  Allocate arrays   */
	cnet = (int **) matalloc(7,nlmax+1,RPINTDTYPE);

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
    err=gridread(plenfile,(void ***)&plen,RPFLTDTYPE,&nx,&ny,&dx,&dy,
             bndbox,&csize,&ndv,&filetype);
	
	fp=fopen(hackfile,"w");
	fprintf(fp,"area length\n");
	for(i=0; i<nlmax; i++)
	{
		ic1=cnet[1][i];
		ic2=cnet[2][i];
		ic=ic2;    // end coordinate
		if(ic2>ic1)ic=ic2-1;
		x=coord[0][ic];
		y=coord[1][ic];
		col= (int)floor((x-bndbox[0])/csize);
		row= (int)floor((bndbox[3]-y)/csize);
		if(row <0 || row > ny || col < 0 || col > nx)
		{
			err=1;
			return(err);
		}
		fprintf(fp,"%f %f\n",coord[4][ic],plen[col][row]);
	}
	fclose(fp);
	return(0);
}

int main(int argc, char **argv)
{
  char treefile[MAXLN], coordfile[MAXLN],plenfile[MAXLN],hackfile[MAXLN];
  int err, nmain, ordert;

  if(argc < 2)
  {
     printf("\nTo avoid manual file name entry this program may\n");
	 printf("be used with the following command\n"); 
     printf("hack <filename_prefix> \n");
	 printf("Input existing channel network tree file (ends with tree.dat)\n");
	 scanf("%s",treefile);
	 printf("Input existing channel network coordinates file (ends with coord.dat)\n");
	 scanf("%s",coordfile);
	 printf("Input file name for mainstream path length grid (ends with plen)\n");
	 scanf("%s",plenfile);
	 printf("Input file name for hack output (ends with hack.dat)\n");
	 scanf("%s",hackfile);
  }
  else
  {
	nmain=nameadd(plenfile,argv[1],"plen"); 

	/*  Parameter files do not have extensions - They are ASCII  */
	strcpy(treefile,"");
    strncat(treefile,argv[1],nmain);
    strcat(treefile,"tree.dat");
	strcpy(coordfile,"");
	strncat(coordfile,argv[1],nmain);
    strcat(coordfile,"coord.dat");
	strcpy(hackfile,"");
	strncat(hackfile,argv[1],nmain);
    strcat(hackfile,"hack.dat");
  }

if(err=hack(plenfile,treefile,coordfile,hackfile)!= 0)
        printf("Hack error %d\n",err);
}