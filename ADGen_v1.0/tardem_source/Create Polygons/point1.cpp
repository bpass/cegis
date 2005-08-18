#include <iostream.h>
#include "PolyLine.h"


Point1::Point1( const Point1 &p)
{
	shapeType = 1;
	length = p.length;
	recNum = p.recNum;
	X = p.X;
	Y = p.Y;
}

void Point1::operator =(Point1 &p)
{
	shapeType = 1;
	length = p.length;
	recNum = p.recNum;
	X = p.X;
	Y = p.Y;
}

Point1::~Point1()
{
}

bool Point1::InputPoint(FILE* & fp)
{
	int type;
	fread(&type,sizeof(int),1,fp);
	if(type != shapeType) {
		cout << "Invalid shapeType in record contents of rec num " << recNum << endl;
		return false;
	}

	fread(&X,sizeof(double),1,fp);
	fread(&Y,sizeof(double),1,fp);

	return true;
}

bool Point1::WritePoint(FILE* & fp)
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

void Point1::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}