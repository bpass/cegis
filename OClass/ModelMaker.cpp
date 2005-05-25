#include "stdafx.h"
#include "ModelMaker.h"
#include "tinyXml.h"

ModelMaker::ModelMaker(const char* inFile, const char* outFile, const char* claFile) {
	//can't continue with null filenames
	if(!inFile || !outFile || !claFile)
		throw(GeneralException("Error in ModelMaker constructor: null filename given"));

	m_inFile = new char[strlen(inFile)+1];
	m_outFile = new char[strlen(outFile)+1];
	strcpy(m_inFile, inFile);
	strcpy(m_outFile, outFile);
	m_inFile[strlen(inFile)] = '\0';
	m_outFile[strlen(outFile)] = '\0';

	//swap '\' with '/' 
	swapSlashes(m_inFile);
	swapSlashes(m_outFile);

	//parse the CLA file.
	buildClassInfo(claFile);
}

ModelMaker::~ModelMaker() {

	//deallocate memory 
	//because memory leaks = crashy crashy
	if(m_inFile)
		delete[] m_inFile;
	if(m_outFile)
		delete[] m_outFile;
}

void ModelMaker::generate(const char* filename) {
	FILE* modelFile = NULL;
	char imageVarDataType[8] = {'\0'}; //either "Float" or "Integer"
	if(!filename)
		throw(GeneralException("Error in ModelMaker::generate(): null filename given."));
	
	if(!m_inFile || !m_outFile)
		throw(GeneralException("Error in ModelMaker::generate(): input or output file is null."));
	
	//open new file
	modelFile = fopen(filename, "w+");
	if(!modelFile)
		throw(GeneralException("Error in ModelMaker::generate(): error creating output file."));
	
	//print all standard lines that appear unchanged
	//in every script
	fprintf(modelFile, "SET CELLSIZE MIN;\n");
	fprintf(modelFile, "SET WINDOW UNION;\n");
	fprintf(modelFile, "SET AOI NONE;\n");

	//print RASTER variable declarations for input and output files
	switch(m_type) {
		case U8:
		case S8:
		case U16:
		case S16:
		case U32:
		case S32:
			strcpy(imageVarDataType, "Integer");
			break;
		case FLOAT32:
		case FLOAT64:
			strcpy(imageVarDataType, "Float");
			break;
		default:
			throw(GeneralException("Error in ModelMaker::generate(): invalid data type."));
	}
	
	//input File declaration
	fprintf(modelFile, "%s RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", imageVarDataType, m_inFile);

	//print declaration for output file
	fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL THEMATIC 8 BIT UNSIGNED INTEGER \"%s\";\n", m_outFile);

	for(unsigned int i = 0; i < m_numLayers; i++) {
		//declare the in-memory rasters and generate the conditional statement
		//for this layer
		fprintf(modelFile, "#define mem%d %s( CONDITIONAL {(r1_in(%d) == 0.000)0,", i+1, imageVarDataType, i+1);
		
		for(unsigned int j = 0; j < m_classInfo[i].size(); j++) {
			//if our high and low values are equal generate
			//== conditional for the value.
			if(m_classInfo[i][j].low == m_classInfo[i][j].high) {
				//if the value is not 0, generate condition
				if(m_classInfo[i][j].low != 0) {
					fprintf(modelFile, "(r1_in(%d) >= %.3f)%d"
						,i+1
						,m_classInfo[i][j].low
						,m_classInfo[i][j].outputPixelVal);
					//if this is not the last line of the conditional, print
					//a comma.
					if((j+1) < m_classInfo[i].size()) 
						fprintf(modelFile, ",");
				}
			}
			else {
				//print condition for each class range
				if(m_classInfo[i][j].low != 0)
					fprintf(modelFile, "(r1_in(%d) >= ", i+1);
				else
					fprintf(modelFile, "(r1_in(%d) > ", i+1);

				fprintf(modelFile, "%.3f", (double)m_classInfo[i][j].low);
				fprintf(modelFile, " AND r1_in(%d) <= ", i+1);
				fprintf(modelFile, "%.3f)", (double)m_classInfo[i][j].high);
				fprintf(modelFile, "%d", m_classInfo[i][j].outputPixelVal);
				//if this is not the last line of the conditional, print
				//a comma.
				if((j+1) < m_classInfo[i].size()) 
					fprintf(modelFile, ",");
			}
		}
		fprintf(modelFile, "} )\n");
	}

	//print stacklayers command
	fprintf(modelFile, "r2_out = STACKLAYERS (\n");
	for(unsigned int i = 0; i < m_numLayers; i++) {
		fprintf(modelFile, "$mem%d", i+1);
		if((i+1) < m_numLayers) 
			fprintf(modelFile, ",");
		fprintf(modelFile, "\n");
	}
	fprintf(modelFile, ") ;\n");

	fprintf(modelFile, "QUIT;");
	fclose(modelFile); 
	
}

void ModelMaker::buildClassInfo(const char* claFile) {
	if(!claFile) 
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
							   "null filename given."));
	TiXmlDocument doc;
	if(!doc.LoadFile(claFile))
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
							   "error loading CLA file"));

	TiXmlElement* curClassInfoE = NULL; //element for current <classificationInformation> tag
	TiXmlElement* rootE = NULL; //root document element
	TiXmlElement* curClassRangeE = NULL; //current range for current class
	TiXmlNode* curClass = NULL; //

	TiXmlHandle handle(&doc);
	int numLayers = 0; //total number of layers to process
	int curNumClasses = 0; //number of classes in this layer
	ClassInfo curClassInfo; //Store class info for current class
	std::vector<ClassInfo> curLayerData; //vector to store class data for current layer.

	//get root tag <classificationReport>
	rootE = handle.FirstChild("classificationReport").Element();
	if(!rootE)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
							"missing or malformed <classificationReport> tag."));

	//get number of layers
	rootE->Attribute("numLayers", &numLayers);

	//get First <classificationData> tag
	curClassInfoE = rootE->FirstChild("classificationData")->ToElement();
	if(!curClassInfoE)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
								"missing initial <classificationData> tag."));
	
	//get number of classes for this layer.
	curClassInfoE->Attribute("numClasses", &curNumClasses);

	//get first <class> tag 
	curClass = curClassInfoE->FirstChild("class");
	if(!curClass)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
								"missing initial <class> tag"));

	//read all <classificationData> tags and each <class> tag under them
	for(int i = 0; i < numLayers; i++) {
		//get number of classes for this layer.
		curClassInfoE->Attribute("numClasses", &curNumClasses);

		curLayerData.clear();
		if(!curClassInfoE)
			throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
									"missing <classificationData> tag."));

		curClass = curClassInfoE->FirstChild("class");
		for(int j = 0; j < curNumClasses; j++) {
			if(!curClass)
				throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
									   "missing <class> tag."));
			//get <range> tag for current <class> tag
			curClassRangeE = curClass->FirstChild("range")->ToElement();
			if(!curClassRangeE)
				throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
										"missing <range> tag."));

			//Get low value
			curClassRangeE->Attribute("low", &curClassInfo.low);

			//Get high value
			curClassRangeE->Attribute("high", &curClassInfo.high);

			if((curClassInfo.low == 0) && (curClassInfo.high == 0))
				curClassInfo.outputPixelVal = 0;

			else {
				//Assign new pixel value.
				curClassInfo.outputPixelVal = j+1;
			}

			//add this class info to the current layer vector
			curLayerData.push_back(curClassInfo);

			//move to next <class> tag
			curClass = curClass->NextSibling();		
		}
		m_classInfo.push_back(curLayerData);

		//move to next <classificationData> tag
		curClassInfoE = curClassInfoE->NextSibling()->ToElement();
		
		
		
	}

	//do other member assignments
	m_numLayers = numLayers;
	int dType = 0;
	rootE->Attribute("dataTypeNumber", &dType);
	m_type = (DataType)dType;
}

//if using windows, swap the backslash used 
//for filenames with the forward slash
void ModelMaker::swapSlashes(char* input) {
	if(!input)
		throw(GeneralException("Error in ModelMaker::swapSlashes():"
							   "null filename given."));
	
	size_t len = strlen(input);
	for(unsigned int i = 0; i < len; i++) {
		if(input[i] == '\\')
			input[i] = '/';
	}
}

		
	
	

	