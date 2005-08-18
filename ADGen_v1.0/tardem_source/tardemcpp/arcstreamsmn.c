/* Program import stream channel network into arcview   */

/* Created by David G Tarboton  */
/* Utah State University       */


#include "tardemlib.h"
#include <string.h>

void main(int argc, char **argv)
 {
   char coordfile[MAXLN],treefile[MAXLN],outfile[MAXLN],ans[MAXLN],outfile2[MAXLN];
   int avrun=0,idel=0,i,nmain,ilink=0;
   float amin=0.;

  if(argc < 2)
  {
     printf("\nTo avoid manual input this program may\n");
	 printf("be used with the following command\n"); 
     printf("\narcstreams <filename_prefix> [-i (to call import)] \n");
	 printf("  [-d (to delete intermediate .e00 file) -l link_no (for subnetwork)\n");
	 printf("   -am min_area (for subnetwork)]\n\n");
	 printf("Input existing Channel Network link file name (ends with tree.dat)\n");
	 scanf("%s",treefile);
	 printf("Input existing Channel Network coordinate file name (ends with coord.dat)\n");
	 scanf("%s",coordfile);
	 printf("Input file name for Arcview import (ends with .e00)\n");
	 scanf("%s",outfile);
	 printf("Do you want to run ArcView import function (y[es]/n[o])\n");
	 scanf("%s",&ans);
	 if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0)
	 {
		avrun=1;
		printf("Input Arcview feature file name to use\n");
		scanf("%s",outfile2);
		printf("Do you want to delete intermediate .e00 file after\n");
		printf("ArcView import function has run(y[es]/n[o])\n");
		scanf("%s",&ans);
		if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0)
		 idel=1;
	 }
	 printf("Do you want to Select a subnetwork (y[es]/n[o])\n");
	 scanf("%s",&ans);
	 if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0)
	 {
		printf("Input link number of subnetwork outlet link (0 selects whole network)\n");
		scanf("%d",&ilink);
		printf("Input subnetwork contrib area threshold (0 selects whole network)\n");
		printf("Nonzero input here is only meaningfull if it is larger than support\n");
		printf("area threshold originally used to define network\n");
		scanf("%f",&amin);
	 }

  }
  else
  {
    i=2;
	while(argc > i)
	{
		if(strcmp(argv[i],"-i")==0)
		{
			i++;
			avrun=1;
		}
		else if(strcmp(argv[i],"-d")==0)
		{
			i++;
			idel=1;
		}
		else if(strcmp(argv[i],"-l")==0)
		{
			i++;
			sscanf(argv[i++],"%d",&ilink);
		}
		else if(strcmp(argv[i],"-am")==0)
		{
			i++;
			sscanf(argv[i++],"%f",&amin);
		}
		else i++;
	}
	nmain=nameadd(ans,argv[1],"st");  /* purpose of this call is to get nmain  */
    strcpy(treefile,"");
    strncat(treefile,argv[1],nmain);
    strcat(treefile,"tree.dat");
	strcpy(coordfile,"");
	strncat(coordfile,argv[1],nmain);
    strcat(coordfile,"coord.dat");
	strcpy(outfile,"");
	strncat(outfile,argv[1],nmain);
    strcat(outfile,"st.e00");
	strcpy(outfile2,"");
	strncat(outfile2,argv[1],nmain);
    strcat(outfile2,"st");
  }

	arcstreams(coordfile,treefile,outfile,&ilink,&amin);
	if(avrun)
	{
		sprintf(ans,"import71 %s %s",outfile,outfile2);
		system(ans);
	}
	if(idel)
	{
		sprintf(ans,"del %s",outfile);
		system(ans);
	}
}

