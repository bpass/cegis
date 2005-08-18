/*  Program to output create grid of flow directions from shape file  

  David G Tarboton
  Utah State University  
   */
  
//#include "tardemlib.h"
#include "gridio.h"

# include <iostream>
# include <fstream>
# include <afx.h>
using namespace std;

# include "PolyLineToRaster.h"

void main(int argc, char **argv)
{
	CString shapefile,demfile,fdrfile,idcolname,dptrname;
  char demfile1[MAXLN],fdrfile1[MAXLN];
  int nmain;

  if(argc < 3)
  {
     printf("\nUsage:\n");
     printf("streamtogrid <shapefile> <gridfilename_prefix> [pntr_fieldname dpntr_fieldname]\n");
	 printf("(There should be no extension to the shapefile name)\n");
     printf("The arguments pntr_fieldname dpntr_fieldname are the names in the shape file\n");
     printf("of fields uniquely identifying each reach, and pointing to the identifier\n");
     printf("of the downstream reach.  These only need to be specified if they are different\n");
     printf("from 'PNTR_' and 'DPNTR', which are the defaults because they are common in Idaho\n");
     exit(0);		
  }
  else
  {
	 shapefile = (TCHAR*) argv[1];
//	sprintf(shapefile,"%s",argv[1]);
	nmain=nameadd(demfile1,argv[2],""); 
	nmain=nameadd(fdrfile1,argv[2],"fdr");
	demfile = (TCHAR*) demfile1;
	fdrfile = (TCHAR*) fdrfile1;
	if(argc > 4)
	{
		idcolname= (TCHAR*) argv[3];
		dptrname = (TCHAR*) argv[4];
	}
	else
	{
		idcolname = "PNTR_";
		dptrname = "DPNTR";
	}
  }
	PolyLineToRas poly2ras;

	CString streamOutletID = "";

	poly2ras.createFlowFile( shapefile, demfile, fdrfile, idcolname, dptrname, streamOutletID );		

}