#ifndef POLYLINE_H
#define POLYLINE_H

#include "types.h"
#include "boundbox.h"
#include "point1.h"

class PolyLine { 
public:
	PolyLine():parts(NULL),points(NULL),shapeType(3){}
	PolyLine(int num, int len):parts(NULL),points(NULL),shapeType(3),recNum(num),length(len){}
	PolyLine(PolyLine &line);
	~PolyLine();

	void operator=(PolyLine &line);

//data members
	SHP_BoundingBox bounds;		//Bounding box
	int				numParts;	//Number of parts
	int				numPoints;	//Total number of points
	int				*parts;		//Index to first point in part
	Point1			*points;	//Points for all parts
	int				shapeType;
	int				length;
	int				recNum;

//member functions
	bool			InputPolyLine(FILE* & fp);
	bool			WritePolyLine(FILE* & fp);
	void			SwapEndian(char* a,int size); 
};

#endif