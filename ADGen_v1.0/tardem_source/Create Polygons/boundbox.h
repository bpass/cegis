#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include <stdio.h>
#include <io.h>

class HDR_BoundingBox {
public:
	HDR_BoundingBox(){}
	HDR_BoundingBox( const HDR_BoundingBox & hbox );
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
	SHP_BoundingBox( const SHP_BoundingBox & sbox );
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