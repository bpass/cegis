#include "stdafx.h"
#include "ModelMaker.h"
#include "tinyXml.h"

ModelMaker::ModelMaker(const char* inFile, const char* outFile, const char* claFile) {
	
	if(!inFile || !outFile || !claFile)
		throw(GeneralException("Error in ModelMaker constructor: null filename given"));

	m_inFile = new char[strlen(inFile)+1];
	m_outFile = new char[strlen(outFile)+1];
	strcpy(m_inFile, inFile);
	strcpy(m_outFile, outFile);
	m_inFile[strlen(inFile)] = '\0';
	m_outFile[strlen(outFile)] = '\0';

	buildClassInfo(claFile);
}

ModelMaker::~ModelMaker() {
	if(m_inFile)
		delete[] m_inFile;
	if(m_outFile)
		delete[] m_outFile;
}

void ModelMaker::generate(const char* filename) {
	FILE* modelFile = NULL;
	if(!filename)
		throw(GeneralException("Error in ModelMaker::generate(): null filename given."));
	
	if(!m_inFile || !m_outFile)
		throw(GeneralException("Error in ModelMaker::generate(): input or output file is null."));
	
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
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case S8:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case U16:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case S16:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case U32:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case S32:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case FLOAT32:
			fprintf(modelFile, "Float RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;

		case FLOAT64:
			fprintf(modelFile, "Float RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			break;
		default:
			throw(GeneralException("Error in ModelMaker::generate(): invalid data type."));
	}
	
	//print declaration for output file
	fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL THEMATIC 8 BIT UNSIGNED INTEGER \"%s\";\n", m_outFile);

	//generate the conditional statement
	fprintf(modelFile, "r2_out = CONDITIONAL {\n");
	for(int i = 0; i < m_numLayers; i++) {
		for(int j = 0; j < m_classInfo[i].size(); j++) {
			
			//I had to break this up into all of these 
			//separate statements, for some *&#!ed up reason
			//it wouldn't work otherwise
			fprintf(modelFile, "(r1_in(%d) >= ", i);
			fprintf(modelFile, "%.3f", (double)m_classInfo[i][j].low);
			fprintf(modelFile, " AND r1_in(%d) <= ", i);
			fprintf(modelFile, "%.3f)", (double)m_classInfo[i][j].high);
			fprintf(modelFile, "%d", m_classInfo[i][j].outputPixelVal);
			
			//if this is not the last line of the conditional, print
			//a comma.
			if(!((i+1) >= m_numLayers && (j+1) >= m_classInfo[i].size())) 
				fprintf(modelFile, ",");

			fprintf(modelFile, "\n");


		}
	
	}

	fprintf(modelFile, "};\n");
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

	TiXmlElement* curClassInfoE = NULL;
	TiXmlElement* rootE = NULL;
	TiXmlElement* curClassRangeE = NULL;
	TiXmlNode* curClass = NULL;

	TiXmlHandle handle(&doc);
	int numLayers = 0; //total number of layers to process
	int curNumClasses = 0; //number of classes in this layer
	ClassInfo curClassInfo;
	std::vector<ClassInfo> curLayerData;

	//get root tag <classificationReport>
	rootE = handle.FirstChild("classificationReport").Element();
	if(!rootE)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
							"missing or malformed <classificationReport> tag."));

	//get number of layers
	rootE->Attribute("numLayers", &numLayers);
	curClassInfoE = rootE->FirstChild("classificationData")->ToElement();
	if(!curClassInfoE)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
								"missing initial <classificationData> tag."));
	curClassInfoE->Attribute("numClasses", &curNumClasses);
	curClass = curClassInfoE->FirstChild("class");
	if(!curClass)
		throw(GeneralException("Error in ModelMaker::buildClassInfo(): "
								"missing initial <class> tag"));

	//read all <classificationData> tags and each <class> tag under them
	for(int i = 0; i < numLayers; i++) {
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
			curClassRangeE->Attribute("low", &curClassInfo.low);
			curClassRangeE->Attribute("high", &curClassInfo.high);
			curClassInfo.outputPixelVal = j;
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

		
	
	

	