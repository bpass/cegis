
#ifndef IMAGE_SPACE_H
#define IMAGE_SPACE_H

struct ImageSpace
{
	ImageSpace() : ulx(0.0), uly(0.0), numRows(0), numCols(0), pxWidth(0.0), pxHeight(0.0) {} 
	//space information
	double ulx;
	double uly;
	long numRows;
	long numCols;
	
	//pixel info
	double pxWidth;
	double pxHeight;
};

#endif