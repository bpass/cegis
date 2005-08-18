/* Program to map channel network and save in vector files   */

/* Created by David G Tarboton  */
/* Utah State University       */

/*  This is essentially a C driver program for the fortran programs
    netex and netprop that do all the work       */     

#include "tardemlib.h"
#include <string.h>

void main(int argc, char **argv)
 {
	int method,i,err,contcheck=1;
	float p1,p2,p3,p4;
	char fnprefix[MAXLN];
	float p[4];
	long xr,yr;
   if(argc < 3)
  { 
	 printf("Use:\n");
     printf("netsetup <filename_prefix> -m method parameters -xy outlet_x outlet_y\n");
     printf("Method and parameter definitions\n");
	 printf("1  Constant threshold A >= p[0]\n");
	 printf("2  Area-Slope threshold   A S^p[1] >= p[0]\n");
	 printf("3  Length area threshold  A >= p[0] L^p[1]\n");
	 printf("4  Upward curved grid cells smoothed by p[0], p[1], p[2], Auc >= p[3]\n");
	 printf("5  Grid order threshold  O >= p[0]\n");
	 printf("6  Use existing network fdrn file \n");
	 exit(0);
   }
    sscanf(argv[1],"%s",fnprefix);
	i=2;
	while(argc > i)
	{
	if(strcmp(argv[i],"-m")==0)
	{
		i++;
		sscanf(argv[i++],"%d",&method);
	    if(method > 6 || method < 1)
		{
		 printf("Invalid method.  \nExiting ...\n");
		 exit(0);
		}
    	 if((method ==1 || method == 5) && argc > i)
		    sscanf(argv[i++],"%f",&p[0]);
	
		 else if((method == 2 || method == 3) && argc >i+1)
		 {
			 sscanf(argv[i++],"%f",&p[0]);
			 sscanf(argv[i++],"%f",&p[1]);
		 }
		 else if(method == 4 && argc > i+3)
		 {
			 sscanf(argv[i++],"%f",&p[0]);
			 sscanf(argv[i++],"%f",&p[1]);
			 sscanf(argv[i++],"%f",&p[2]);
			 sscanf(argv[i++],"%f",&p[3]);
		 }
		 else if(method == 6);  // do nothing - no parameters to read
		 else
		 {
			 printf("Incorrect or insufficient command line arguments\n");
			 exit(0);
		 }
	}
	else if(strcmp(argv[i],"-xy")==0)
	{
		i++;
		sscanf(argv[i++],"%ld",&xr);
		sscanf(argv[i++],"%ld",&yr);
	}
	else if(strcmp(argv[i],"-nc")==0)
		{
			i++;
			contcheck=0;
		}
	else
	{
			 printf("Incorrect or insufficient command line arguments\n");
			 exit(0);
	}
  }
p1 = p[0];
p2 = p[1];
p3 = p[2];
p4 = p[3];

	if(err=netsetup(fnprefix, method,p1,p2,p3,p4,xr,yr,contcheck) != 0)
   printf("Netsetup Error %d\n",err);
}

