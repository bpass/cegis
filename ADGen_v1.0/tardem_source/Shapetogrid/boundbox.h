#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include <stdio.h>
#include <io.h>

class HDR_BoundingBox {
public:
	HDR_BoundingBox(){}
	bool setBounds(double bounds[]);
	void getBounds(double bounds[]);

private:
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
	double Zmin;
	double Zmax;
	double Mmin;
	double Mmax;

};

class SHP_BoundingBox {
public:
	SHP_BoundingBox(){}
	bool setBounds(double bounds[]);
	void getBounds(double bounds[]);
	void operator=(SHP_BoundingBox &box);

private:
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
};

#endif