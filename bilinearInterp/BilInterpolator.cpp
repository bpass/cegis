#include "BilInterpolator.h"
#include "Exception.h"
#include <math.h>

BilInterpolator::BilInterpolator() {
	m_params.height = 0;
	m_params.width = 0;
	m_params.numLayers = 0;
	m_params.pixelWidth = 0;
	m_params.pixelHeight = 0;
	m_params.ulx = 0;
	m_params.uly = 0;
	m_imageFilePtr = NULL;
}

BilInterpolator::BilInterpolator(const char* imageFile, const char* pointsFile, const ImageParameters& params):
m_params(params), m_imageFilePtr(NULL) 

{
	
	if(!imageFile || !pointsFile) {
		std::string error = "null filename given";
		throw(GeneralException(error));
	}
	
	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		std::vector<float> a;
		m_results.push_back(a);
	}

	setImage(imageFile);
	interpolate(pointsFile);
}

BilInterpolator::~BilInterpolator() {
	if(m_imageFilePtr)
		fclose(m_imageFilePtr);
}
void BilInterpolator::setImage(const char* filename) {
	if(!filename) {
		std::string error = "null filename given";
		throw(GeneralException(error));
	}

	if(m_imageFilePtr)
		fclose(m_imageFilePtr);

	m_imageFilePtr = fopen(filename, "rb");
	m_imageFileName = filename;
}

void BilInterpolator::setParams(const ImageParameters& params) {
	m_params = params;
}

void BilInterpolator::interpolate(const char* pointsFile) {
	float result = 0;

	if(!pointsFile) {
		std::string error = "null filename given";
		throw(GeneralException(error));
	}
	
	loadPoints(pointsFile);
	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		for(unsigned int j = 0; j < m_points.size(); j++) {
			result = doInterpolation(m_points[j], i+1);
			m_results[i].push_back(result);
//			emit updateProgress((m_points.size()*(i) + j), (m_points.size()*m_params.numLayers));
		}
	}
}

float BilInterpolator::doInterpolation(const Point& p, int band) {
	if(band < 0) {
		std::string error = "Band < 0";
		throw(GeneralException(error));
	}

	float result = 0.0;
	double fCol = 0;
	double fRow = 0;
	double temp = 0;
	long row = 0;
	long col = 0;
	float fracX = 0;
	float fracY = 0;

	float f00,f10,f01,f11 = 0; //each represents a value in the 2x2 square of pixels 
								//closest to the point of interest.
	float x,y=0; //the position of our point inside of the 2x2 grid.

	double ulx = m_params.ulx;
	double uly = m_params.uly;
	float pxWidth = m_params.pixelWidth;
	float pxHeight = m_params.pixelHeight;
	
	fCol = fabs(fabs(p.x) - fabs(ulx)) / pxWidth;
	fRow = fabs(fabs(p.y) - fabs(uly)) / pxHeight;
	//get row,column coordinate of point
	col = (long)floor(fCol);
	row = (long)floor(fRow);

	//get x and y position within pixels 
	fracX = modf(fCol, &temp);
	fracY = modf(fRow, &temp);

	//convert to %
	fracX *= 100;
	fracY *= 100;

	//value of the pixel that contains the point.
	f00 = getValue(row, col, band);

	//4 cases for where our point lies inside of the pixel,
	//we must rotate our x and y axes accordingly.
	if((fracX <= 50) && (fracY <= 50)) {	
		printf("ul\n");
		f10 = getValue(row-1, col, band);
		f01 = getValue(row, col-1, band);
		f11 = getValue(row-1, col-1, band);

		x = (50 - fracY) / 100;
		y = (50 - fracX) / 100;

		if(x == 0)
			x = 0.01;
		if(y == 0)
			y = 0.01;
	}

	else if((fracX <= 50) && (fracY > 50)) {
		printf("ll\n");
		f01 = getValue(row, col-1, band);
		f10 = getValue(row+1, col, band);
		f11 = getValue(row+1, col-1, band);
	
		x = (fracY - 50) / 100;
		y = (50 - fracX) / 100;

		if(x == 0)
			x = 0.01;
		if(y == 0)
			y = 0.01;
	}

	else if((fracX > 50) && (fracY <= 50)) {
		printf("ur\n");
		f01 = getValue(row, col+1, band);
		f10 = getValue(row-1, col, band);
		f11 = getValue(row-1, col+1, band);
	
		x = (50 - fracY) / 100;
		y = (fracX - 50) / 100;

		if(x == 0)
			x = 0.01;
		if(y == 0)
			y = 0.01;
	}

	else if((fracX > 50) && (fracY > 50)) {
		printf("lr\n");
		f01 = getValue(row, col+1, band);
		f10 = getValue(row+1, col, band);
		f11 = getValue(row+1, col+1, band);
	
		x = (fracY - 50) / 100;
		y = (fracX - 50) / 100;

		if(x == 0)
			x = 0.01;
		if(y == 0)
			y = 0.01;
	}

	//calculate interpolation value.
	result = (f10 - f00)*x + (f01 - f00)*y + (f11 + f00 - f01 - f10)*x*y + f00;
	
	return(result);
}

float BilInterpolator::getValue(unsigned long row, unsigned long col, unsigned int band) {
	unsigned long offset = (m_params.width*(row-1) + (col-1)) * band;
	float value = 0;
	if(!m_imageFilePtr) {
		std::string error = "Image file is null";
		throw(GeneralException(error));
	}

	//seek to value, assume 32 bit data (4 bytes per pixel);
	fseek(m_imageFilePtr, offset*4, SEEK_SET);
	fread((void*)&value, 4, 1, m_imageFilePtr);

	return(value);
}


void BilInterpolator::loadPoints(const char* pointsFilename) {
	std::string* curNumber = NULL;
	
	Point curPoint;
	if(!pointsFilename) {
		std::string error = "null filename";
		throw(GeneralException(error));
	}

	FILE* pointsFile = fopen(pointsFilename, "r");

	if(!pointsFile) {
		std::string error = "Points file is null";
		throw(GeneralException(error));
	}
	char c = '\0';
	while(c != EOF) {
		c = fgetc(pointsFile);
		if(c == EOF)
			break;

		if(curNumber)
			delete curNumber;
		curNumber = new std::string;
		curPoint.x = 0;
		curPoint.y = 0;

		while(c != ',' && (isdigit(c) || c == '.') && c != EOF) {
			curNumber->append(1,c);
			c = fgetc(pointsFile);
		}
		
		curPoint.x = atof(curNumber->c_str());
		
		if(curNumber)
			delete curNumber;
		curNumber = new std::string;

		c = fgetc(pointsFile);
		while((isdigit(c) || c == '.') && c != EOF) {
			curNumber->append(1,c);
			c = fgetc(pointsFile);
			
		}

		curPoint.y = atof(curNumber->c_str());

		m_points.push_back(curPoint);
	}
	if(curNumber)
		delete curNumber;
	fclose(pointsFile);
}

void BilInterpolator::saveResults(const char* outputFilename) {
	if(!outputFilename) {
		std::string error = "Null filename";
		throw(GeneralException(error));
	}

	FILE* outputFile = fopen(outputFilename, "w+");
	if(!outputFile) {
			std::string error = "Error opening output file";
			throw(GeneralException(error));
	}

	fprintf(outputFile, "Interpolation Results for %s:\n\n", m_imageFileName.c_str());
	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		fprintf(outputFile, "Layer %d\n", i+1);
		for(unsigned int j = 0; j < m_points.size(); j++) {
		/*	printf("i : %d  j: %d  x : %f  y : %f  result : %f\n\n"
					,i
					,j
					,m_points[j].x
					,m_points[j].y
					,m_results[i][j]);
					*/
			fprintf(outputFile, "%f,%f  -->  %f\n"
					,m_points[j].x
					,m_points[j].y
					,m_results[i][j]);
		}
	}

	fclose(outputFile);
}


