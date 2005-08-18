#include "boundbox.h"


bool HDR_BoundingBox::setBounds(double bounds[])
{
	Xmin = bounds[0];
	Ymin = bounds[1];
	Xmax = bounds[2];
	Ymax = bounds[3];
	Zmin = bounds[4];
	Zmax = bounds[5];
	Mmin = bounds[6];
	Mmax = bounds[7];
	return true;
}

void HDR_BoundingBox::getBounds(double bounds[])
{
	bounds[0]=Xmin;
	bounds[1]=Ymin;
	bounds[2]=Xmax;
	bounds[3]=Ymax;
	bounds[4]=Zmin;
	bounds[5]=Zmax;
	bounds[6]=Mmin;
	bounds[7]=Mmax;
}

bool SHP_BoundingBox::setBounds(double bounds[])
{
	Xmin = bounds[0];
	Ymin = bounds[1];
	Xmax = bounds[2];
	Ymax = bounds[3];
	return true;
}

void SHP_BoundingBox::getBounds(double bounds[])
{
	bounds[0] = Xmin; 
	bounds[1] = Ymin;
	bounds[2] = Xmax;
	bounds[3] = Ymax;
}

void SHP_BoundingBox::operator=(SHP_BoundingBox &box)
{
	double tmp[4];
	box.getBounds(tmp);

	Xmin = tmp[0];
	Ymin = tmp[1];
	Xmax = tmp[2];
	Ymax = tmp[3];
}