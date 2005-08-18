#include <iostream>
#include "polyline.h"

using namespace std;

PolyLine::PolyLine(PolyLine &line)
{
	shapeType = 3;
	length = line.length;
	recNum = line.recNum;
	numPoints = line.numPoints;
	numParts = line.numParts;
	bounds = line.bounds;

	parts = new int[numParts];
	for(int i=0; i<numParts; i++)
		parts[i] = line.parts[i];

	points = new api_Point[numPoints];
	for(i=0; i<numPoints; i++)
		points[i] = line.points[i];	

}

void PolyLine::operator =(PolyLine &line)
{
	shapeType = 3;
	length = line.length;
	recNum = line.recNum;
	numPoints = line.numPoints;
	numParts = line.numParts;
	bounds = line.bounds;

	parts = new int[numParts];
	for(int i=0; i<numParts; i++)
		parts[i] = line.parts[i];

	points = new api_Point[numPoints];
	for(i=0; i<numPoints; i++)
		points[i] = line.points[i];	
}

PolyLine::~PolyLine()
{	
	if( parts != NULL )
		delete [] parts;
	if( points != NULL )
		delete [] points;	
}

bool PolyLine::InputPolyLine(FILE* & fp)
{
	int type;
	fread(&type,sizeof(int),1,fp);
	if(type != shapeType) {
		cout << "Shape type does not match in record " << recNum << endl;
		if( type != NULLSHAPE )
			return false;
		else
			return true;
	}
	
	//input Bounding Box
	double mybounds[4];
	fread(mybounds,sizeof(double),4,fp);
	if(bounds.setBounds(mybounds)==false) 
		return false;

	//input Number of Parts and Number of Points
	fread(&numParts,sizeof(int),1,fp);
	fread(&numPoints,sizeof(int),1,fp);
	
	//Allocate space for numparts and numpoints
	if( numParts <= 0 && numPoints <= 0 )
		return true;

	parts = new int[numParts + 1];
	points = new api_Point[numPoints + 1];

	//Input parts
	fread(parts,sizeof(int),numParts,fp);

	//Input points
	double x,y;
	for(int i=0;i<numPoints;i++) {
		fread(&x,sizeof(double),1,fp);
		fread(&y,sizeof(double),1,fp);
		points[i].setXY(x,y);
	}

	return true;
}

bool PolyLine::WritePolyLine(FILE* & fp)
{
	void* intbuf;
	intbuf = (char*)&recNum;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);

	intbuf = (char*)&length;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);

	fwrite(&shapeType,sizeof(int),1,fp);
	
	double mybounds[4];
	bounds.getBounds(mybounds);
	fwrite(mybounds,sizeof(double),4,fp);
	
	fwrite(&numParts,sizeof(int),1,fp);
	fwrite(&numPoints,sizeof(int),1,fp);
	
	fwrite(parts,sizeof(int),numParts,fp);

	//Input points
	double x,y;
	for(int i=0;i<numPoints;i++) {
		points[i].getXY(x,y);
		fwrite(&x,sizeof(double),1,fp);
		fwrite(&y,sizeof(double),1,fp);
	}

	return true;
}

void PolyLine::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}