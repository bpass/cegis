#ifndef MODEL_M_H
#define MODEL_M_H
#include "stdafx.h"
#include "DataType.h"
#include "GeneralException.h"
#include <string.h>
#include <stdio.h>
#include <vector>
typedef struct {
	double low;
	double high;
	int outputPixelVal;
} ClassInfo;

class ModelMaker {
public:
	ModelMaker(const char* inFile, const char* outFile, const char* claFile);
	~ModelMaker();

	//build the SML script file
	void generate(const char* filename);
private:
    char* m_inFile; //original imagine input file
	char* m_outFile; //desired imaging output file
	unsigned int m_numLayers; //number of layers in the image
	std::vector< std::vector<ClassInfo> > m_classInfo; //Class Informatin for all
													   //classes in all layers.
	DataType m_type; //image data type
	void swapSlashes(char* filename);
	void buildClassInfo(const char* claFile); //parse .cla XML file
};

#endif 

