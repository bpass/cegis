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
		destruct();
		throw(GeneralException(error));
	}

	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		std::vector<double> a;
		m_results.push_back(a);
	}

	setImage(imageFile);
	interpolate(pointsFile);
}

BilInterpolator::~BilInterpolator() {
	destruct();
}

void BilInterpolator::destruct() {
	if(m_imageFilePtr)
		fclose(m_imageFilePtr);
}

void BilInterpolator::setImage(const char* filename) {
	if(!filename) {
		std::string error = "null filename given";
		destruct();
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
	double result = 0;

	if(!pointsFile) {
		std::string error = "null filename given";
		destruct();
		throw(GeneralException(error));
	}

	loadPoints(pointsFile);
	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		for(unsigned int j = 0; j < m_points.size(); j++) {
			result = doInterpolation(m_points[j], i);
			m_results[i].push_back(result);
			//			emit updateProgress((m_points.size()*(i) + j), (m_points.size()*m_params.numLayers));
		}
	}
}

double BilInterpolator::doInterpolation(const Point& p, int band) {
	if(band < 0) {
		std::string error = "Band < 0";
		destruct();
		throw(GeneralException(error));
	}

	double result = 0.0;
	double fCol = 0;
	double fRow = 0;
	double temp = 0;
	unsigned long row = 0;
	unsigned long col = 0;
	double fracX = 0;
	double fracY = 0;

	float f00,f10,f01,f11 = 0; //each represents a value in the 2x2 square of pixels 
	//closest to the point of interest.
	double x,y=0; //the position of our point inside of the 2x2 grid.

	double ulx = m_params.ulx;
	double uly = m_params.uly;
	float pxWidth = m_params.pixelWidth;
	float pxHeight = m_params.pixelHeight;

	fCol = fabs(fabs(p.x) - fabs(ulx)) / pxWidth;
	fRow = fabs(fabs(p.y) - fabs(uly)) / pxHeight;
	//get row,column coordinate of point
	col = (long)floor(fCol);
	row = (long)floor(fRow);
	if(col > m_params.width) {
		std::string error("Column out of bounds");
		throw(GeneralException(error));
	}

	if(row > m_params.height) {
		std::string error("Column out of bounds");
		throw(GeneralException(error));
	}

	//get x and y position within pixels 
	fracX = modf(fCol, &temp);
	fracY = modf(fRow, &temp);

	//convert to %
	fracX *= 100;
	fracY *= 100;

	//value of the pixel that contains the point.



	if((fracX <= 50) && (fracY <= 50)) {	
		printf("lr\n");
		f00 = getValue(row, col, band);
		f01 = getValue(row+1, col, band);
		f10 = getValue(row, col+1, band);
		f11 = getValue(row+1, col+1, band);


		x = (fracX + 50) / 200;
		y = (fracY + 50) / 200;
		printf("Point:  %f,%f\ncol=%d\nrow=%d\nfracX=%f\nfracY=%f\n", p.x, p.y, col, row, fracX, fracY);
		printf("x=%f\ny=%f\nf00=%f\nf01=%f\nf10=%f\nf11=%f\n", x, y, f00, f01, f10, f11);



	}

	else if((fracX <= 50) && (fracY > 50)) {
		printf("ur\n");
		row += 1;

		f00 = getValue(row, col, band);
		f01 = getValue(row-1, col, band);
		f10 = getValue(row, col+1, band);
		f11 = getValue(row-1, col+1, band);


		x = (fracX+50) / 200;
		y =  (100-(fracY-50)) / 200;
		printf("Point:  %f,%f\ncol=%d\nrow=%d\nfracX=%f\nfracY=%f\n", p.x, p.y, col, row, fracX, fracY);
		printf("x=%f\ny=%f\nf00=%f\nf01=%f\nf10=%f\nf11=%f\n", x, y, f00, f01, f10, f11);


	}

	else if((fracX > 50) && (fracY <= 50)) {
		printf("ll\n");

		col+= 1;

		f00 = getValue(row, col, band);
		f01 = getValue(row+1, col, band);
		f10 = getValue(row, col-1, band);
		f11 = getValue(row+1, col-1, band);

		x = (100 - (fracX - 50)) / 200;
		y = (fracY + 50)/200;

		printf("Point:  %f,%f\ncol=%d\nrow=%d\nfracX=%f\nfracY=%f\n", p.x, p.y, col, row, fracX, fracY);
		printf("x=%f\ny=%f\nf00=%f\nf01=%f\nf10=%f\nf11=%f\n", x, y, f00, f01, f10, f11);

	}

	else if((fracX > 50) && (fracY > 50)) {
		printf("ul\n");

		col += 1;
		row += 1;
		f00 = getValue(row, col, band);
		f01 = getValue(row-1, col, band);
		f10 = getValue(row, col-1, band);
		f11 = getValue(row-1, col-1, band);

		x = (100 - (fracX-50)) / 200;
		y = (100 - (fracY - 50)) / 200;
		printf("Point:  %f,%f\ncol=%d\nrow=%d\nfracX=%f\nfracY=%f\n", p.x, p.y, col, row, fracX, fracY);
		printf("x=%f\ny=%f\nf00=%f\nf01=%f\nf10=%f\nf11=%f\n", x, y, f00, f01, f10, f11);
	}

	//calculate interpolation value.
	result = (f10 - f00)*x + (f01 - f00)*y + (f11 + f00 - f01 - f10)*x*y + f00;

	return(result);
}

float BilInterpolator::getValue(unsigned long row, unsigned long col, unsigned int band) {
	//pixel offset within the file
	unsigned long offset = (m_params.width*(row) + (col)) + (m_params.width*m_params.height*band);
	float value = 0;
	if(!m_imageFilePtr) {
		std::string error = "Image file is null";
		destruct();
		throw(GeneralException(error));
	}


	//	fflush(m_imageFilePtr);
	printf("byte offset: %d\n", offset*4);

	//seek to value starting at beginning of file, assume 32 bit data (4 bytes per pixel);
	fseek(m_imageFilePtr, offset*4, SEEK_SET);
	fread((void*)&value, 4, 1, m_imageFilePtr);

	return(value);
}


void BilInterpolator::loadPoints(const char* pointsFilename) {
	std::string* curNumber = NULL;

	Point curPoint;
	if(!pointsFilename) {
		std::string error = "null filename";
		destruct();
		throw(GeneralException(error));
	}

	FILE* pointsFile = fopen(pointsFilename, "r");

	if(!pointsFile) {
		std::string error = "Points file is null";
		destruct();
		throw(GeneralException(error));
	}
	char c = '\0';
	while(c != EOF) {
		bool decimal = false; //have we found a decimal point yet?
		bool plus = false;//have we seen a plus sign?
		bool minus = false;//have we seen a minus sign?
		c = fgetc(pointsFile);
		if(c == EOF)
			break;

		if(curNumber)
			delete curNumber;
		curNumber = new std::string;
		curPoint.x = 0;
		curPoint.y = 0;

		while(!isdigit(c) && c != '.' && c != '-' && c != '+' && c != EOF)
			c = fgetc(pointsFile);

		while((isdigit(c) || c == '.' || c == '-' || c == '+') && c != EOF) {
			if(c == '.') {
				if(decimal) {
					std::string error("Duplicate decimal points found in points file");
					destruct();
					throw(GeneralException(error));
				}
				decimal = true;
			}

			//if we see a decimal point, check that we have not seen one before
			if(c == '-') {
				if(minus) {
					std::string error("Duplicate minus signs found in points file");
					destruct();
					throw(GeneralException(error));
				}
				minus = true;
			}

			//if we see a decimal point, check that we have not seen one before
			if(c == '+') {
				if(plus) {
					std::string error("Duplicate plus signs found in points file");
					destruct();
					throw(GeneralException(error));
				}
				plus = true;
			}
			curNumber->append(1,c);
			c = fgetc(pointsFile);
		}

		curPoint.x = atof(curNumber->c_str());

		if(curNumber)
			delete curNumber;

		decimal = false;
		plus = false;
		minus = false;
		curNumber = new std::string;

		c = fgetc(pointsFile);

		while(!isdigit(c) && c != '.' && c != '-' && c != '+' && c != EOF)
			c = fgetc(pointsFile);

		while((isdigit(c) || c == '.' || c == '-' || c == '+') && c != EOF) {

			//if we see a decimal point, check that we have not seen one before
			if(c == '.') {
				if(decimal) {
					std::string error("Duplicate decimal points found in points file");
					destruct();
					throw(GeneralException(error));
				}
				decimal = true;
			}

			//if we see a decimal point, check that we have not seen one before
			if(c == '-') {
				if(minus) {
					std::string error("Duplicate minus signs found in points file");
					destruct();
					throw(GeneralException(error));
				}
				minus = true;
			}

			//if we see a decimal point, check that we have not seen one before
			if(c == '+') {
				if(plus) {
					std::string error("Duplicate plus signs found in points file");
					destruct();
					throw(GeneralException(error));
				}
				plus = true;
			}

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
	char* csvFileName = NULL;
	if(!outputFilename) {
		std::string error = "Null filename";
		destruct();
		throw(GeneralException(error));
	}

	csvFileName = new char[strlen(outputFilename)+5];
	strcpy(csvFileName, outputFilename);
	strcat(csvFileName, ".csv\0");
	FILE* outputFile = fopen(csvFileName, "w+");
	if(!outputFile) {
		std::string error = "Error opening output file";
		destruct();
		throw(GeneralException(error));
	}

	fprintf(outputFile, "Interpolation Results for %s:\n\n", m_imageFileName.c_str());
	fprintf(outputFile, "X, ,Y, ,Layer, ,Result\n");
	for(unsigned int i = 0; i < m_params.numLayers; i++) {
		for(unsigned int j = 0; j < m_points.size(); j++) {

			fprintf(outputFile, "     %f,      ,     %f,     ,%d,     ,     %f\n"
				,m_points[j].x
				,m_points[j].y
				,i+1
				,m_results[i][j]);
		}
	}

	fclose(outputFile);
	delete[] csvFileName;
}


