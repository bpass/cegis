#ifndef POINT_H
#define POINT_H

#include "types.h"

class Point1 {
//constructors and destructors
public:
	Point1():shapeType(1){}
	Point1(int num, int len):shapeType(1),length(len),recNum(num){}
	Point1( const Point1 &p);
	~Point1();
	
	void operator=(Point1 &p);

//data types
	double			X;
	double			Y;
	int				shapeType;
	int				length;
	int				recNum;

//member functions
	void			setXY(double x,double y) {X=x;Y=y;}
	void			getXY(double &x, double &y) { x=X;y=Y;}
	bool			InputPoint(FILE* & fp);
	bool			WritePoint(FILE* & fp);
	void			SwapEndian(char* a,int size);

};

#endif