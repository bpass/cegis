#ifndef BIL_INTERP_H
#define BIL_INTERP_H
#include <vector>
#include "Exception.h"
#include "common.h"

class BilInterpolator{
//	Q_OBJECT
public:
	BilInterpolator();
	BilInterpolator(const char* imageFile, const char* pointsFile, const ImageParameters& params); //throws GeneralException
	~BilInterpolator();

	void setImage(const char* filename);
	void setParams(const ImageParameters& params);
	void saveResults(const char* outputFile); //throws GeneralException
	void interpolate(const char* pointsFile);

//signals:
//	void updateProgress(int progress, int totalSteps);

private:
	ImageParameters m_params;  //Contains all needed parameters for calculations
	std::vector<Point> m_points;  //Input pixels to perform interpolation on
	std::vector< std::vector<float> > m_results; //Interpolated values for each input pixel for each layer
	FILE* m_imageFilePtr; //raster image file
	std::string m_imageFileName;
	float doInterpolation(const Point& p, int band); //Do an interpolation on a pixel and a certain point
															   // in a particular band throws GeneralException
	void loadPoints(const char* pointsFilename); //loads the points to interpolate on.
											 //throws GeneralException
	float getValue(unsigned long row, unsigned long col, unsigned int band); //get the pixel value at (row,col) 
												 //for a particular band in the image file
												 //throws GeneralException
};	

#endif