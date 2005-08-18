#ifndef MY_POINT_H
#define MY_POINT_H

#include "types.h"

class api_Point {
//constructors and destructors
public:
	api_Point():shapeType(1){}
	api_Point(int num, int len):shapeType(1),length(len),recNum(num){}
	api_Point( const api_Point &p);
	~api_Point();
	
	void operator=(api_Point &p);

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