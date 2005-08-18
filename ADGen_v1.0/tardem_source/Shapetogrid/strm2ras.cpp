# include <iostream>
# include <fstream>
# include <afx.h>
using namespace std;

# include "PolyLineToRaster.h"


void main()
{
	PolyLineToRas poly2ras;
	
	CString inFilePath = "c:\\windows\\desktop\\bear_river_ID\\nmboise";
	CString demFile = "c:\\windows\\desktop\\bear_river_ID\\bear_river_ID.asc";
	CString outFilePath = "c:\\windows\\desktop\\stream";
	CString idColumnName = "PNTR_";
	CString downPointer = "DPNTR";
	CString streamOutletID = "";

	poly2ras.createFlowFile( inFilePath, demFile, outFilePath, idColumnName, downPointer, streamOutletID );		
}