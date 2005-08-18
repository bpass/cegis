#include <iostream>
#include "PolyLine.h"

using namespace std;

api_Point::api_Point( const api_Point &p)
{
	shapeType = 1;
	length = p.length;
	recNum = p.recNum;
	X = p.X;
	Y = p.Y;
}

void api_Point::operator =(api_Point &p)
{
	shapeType = 1;
	length = p.length;
	recNum = p.recNum;
	X = p.X;
	Y = p.Y;
}

api_Point::~api_Point()
{
}

bool api_Point::InputPoint(FILE* & fp)
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

	fread(&X,sizeof(double),1,fp);
	fread(&Y,sizeof(double),1,fp);

	return true;
}

bool api_Point::WritePoint(FILE* & fp)
{
	void* intbuf;
	intbuf = (char*)&recNum;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);

	intbuf = (char*)&length;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);

	fwrite(&shapeType,sizeof(int),1,fp);
	fwrite(&X,sizeof(double),1,fp);
	fwrite(&Y,sizeof(double),1,fp);
	return true;
}

void api_Point::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}