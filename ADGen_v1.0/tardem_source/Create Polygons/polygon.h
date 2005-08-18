#ifndef POLYGON_H
#define POLYGON_H

#include "types.h"
#include "boundbox.h"
#include "point1.h"

class PolyGon { 
public:
	PolyGon():parts(NULL),points(NULL),shapeType(5){}
	PolyGon(int num, int len):parts(NULL),points(NULL),shapeType(5),recNum(num),length(len){}
	PolyGon(PolyGon &p);
	~PolyGon();

	void operator=(PolyGon &p);

//data members
	SHP_BoundingBox bounds;		//Bounding box
	int				numParts;	//Number of parts
	int				numPoints;	//Total number of points
	int				*parts;		//Index to first point in part
	Point1			*points;	//Points for all parts
	int				shapeType;
	int				length;
	int				recNum;
	double			area;
	double			perimeter;

//data functions
	bool			InputPolyGon(FILE* & fp);
	bool			WritePolyGon(FILE* & fp);
	void			SwapEndian(char* a,int size); 
};

#endif