#include <iostream>
#include "polygon.h"

using namespace std;

PolyGon::PolyGon(PolyGon &p)
{
	shapeType = 3;
	length = p.length;
	recNum = p.recNum;
	numPoints = p.numPoints;
	numParts = p.numParts;
	bounds = p.bounds;
	area = p.area;
	perimeter = p.perimeter;

	parts = new int[numParts];
	for(int i=0; i<numParts; i++)
		parts[i] = p.parts[i];

	points = new api_Point[numPoints];
	for(i=0; i<numPoints; i++)
		points[i] = p.points[i];

}

void PolyGon::operator =(PolyGon &p)
{
	shapeType = 3;
	length = p.length;
	recNum = p.recNum;
	numPoints = p.numPoints;
	numParts = p.numParts;
	bounds = p.bounds;
	area = p.area;
	perimeter = p.perimeter;

	parts = new int[numParts];
	for(int i=0; i<numParts; i++)
		parts[i] = p.parts[i];

	points = new api_Point[numPoints];
	for(i=0; i<numPoints; i++)
		points[i] = p.points[i];
}

PolyGon::~PolyGon()
{
	if( points != NULL )
		delete [] points;
	if( parts != NULL )
		delete [] parts;
}

bool PolyGon::InputPolyGon(FILE* & fp)
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
	parts = new int[numParts];
	points = new api_Point[numPoints];

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

bool PolyGon::WritePolyGon(FILE* & fp)
{
	void* intbuf;
	int tempRecNum = recNum;
	intbuf = (char*)&tempRecNum;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);

	int tempLength = length;
	intbuf = (char*)&tempLength;
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

void PolyGon::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}