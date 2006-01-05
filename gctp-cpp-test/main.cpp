#include "GCTPCPPTester.h"
#include "GCTPTester.h"
#include <RasterXML.h>

int main() 
{
	GCTPTester cTest;
	GCTPCPPTester cppTest;
	ImageSpace imgSpace;
	ProjInfo inProjInfo;
	ProjInfo outProjInfo;

	RasterXML inConfig;
	RasterXML outConfig;
	inConfig.loadFile("images\\equirectangular.xml");
	outConfig.loadFile("images\\mollweide.xml");
	
	imgSpace.numCols = inConfig.getCols();
	imgSpace.numRows = inConfig.getRows();
	imgSpace.pxHeight = inConfig.getPixelSize();
	imgSpace.pxWidth = imgSpace.pxHeight;
	imgSpace.ulx = inConfig.getUlx();
	imgSpace.uly = inConfig.getUly();

	inProjInfo.projDatum = inConfig.getDatumNumber();
	outProjInfo.projDatum = outConfig.getDatumNumber();
	inProjInfo.projNumber = inConfig.getProjNumber();
	outProjInfo.projNumber = outConfig.getProjNumber();
	


	for(int i = 0; i < 15; i++)
	{
		inProjInfo.projParams[i] = inConfig.getGCTPParam(i);
		outProjInfo.projParams[i] = outConfig.getGCTPParam(i);
	}


	
}