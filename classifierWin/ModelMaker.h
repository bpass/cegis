#ifndef MODEL_M_H
#define MODEL_M_H
#include "string.h"
#include "stdafx.h"
#include "stdio.h"
typedef enum {NONE=-1, U8, S8, U16, S16, U32, S32, FLOAT32, FLOAT64} DataType;
template <class T>
class ModelMaker {
public:
	ModelMaker();
	ModelMaker(const char* inFile, const char* outFile 
			  ,char* claFile, int numClasses, DataType type, int numLayers);
	~ModelMaker();

	void setInputFile(const char* file);
	void setOutputFile(const char* file);
	void setPixelValues(int* pixelValues, int numClasses);
	void setClassInfo(T*** classInfo, int numLayers, int numClasses);
	void setDataType(DataType type);
	void setNumLayers(int num);
	void generate(const char* filename);
private:
    char* m_inFile;
	char* m_outFile;
	int* m_pixelValues;
	int m_numClasses;
	int m_numLayers;
	void buildClassInfo(const char** fileList);
	T*** m_classInfo;
	DataType m_type;
};

template <class T>
ModelMaker<T>::ModelMaker(): m_inFile(NULL), m_outFile(NULL), m_pixelValues(NULL)
,m_numClasses(0), m_type(NONE), m_numLayers(0), m_classInfo(NULL){}

template <class T>
ModelMaker<T>::ModelMaker(const char* inFile, const char* outFile
						  ,char* claFile, int numClasses, DataType type, int numLayers){
	
	if(!inFile || !outFile) 
		throw(GeneralException("Error in ModelMaker constructor: null filename given"));
	if(!pixelValues)
		throw(GeneralException("Error in ModelMaker constructor: null pixelValues given"));
	
	if(!classInfo) 
		throw(GeneralException("Error in ModelMaker constructor: null classInfo given."));

	if(numClasses <= 0 || numLayers <= 0) 
		throw(GeneralException("Error in ModelMaker constructor: number of classes"
								"and number of layers must be > 0"));
	

	m_inFile = new char[strlen(inFile)+1];
	m_outFile = new char[strlen(outFile)+1];

	strcpy(m_inFile, inFile);
	strcpy(m_outFile, outFile);
	m_inFile[strlen(m_inFile)] = '\0';
	m_outFile[strlen(m_outFile)] = '\0';

	m_pixelValues = new int[numClasses];
	for(int i = 0; i < numClasses; i++) 
		m_pixelValues[i] = pixelValues[i];
	
	m_classInfo = new T**[numLayers];
	for(int i = 0; i < numClasses; i++) {
		m_classInfo[i] = new T*[numClasses];
		for(int j = 0; j < numClasses; j++) {
			m_classInfo[i][j] = new T[2];
			m_classInfo[i][j][0] = classInfo[i][j][0];
			m_classInfo[i][j][1] = classInfo[i][j][1];
		}
	}
	m_numClasses = numClasses;
	m_numLayers = numLayers;
	m_type = type;
}

template <class T>
ModelMaker<T>::~ModelMaker() {
	if(m_inFile)
		delete[] m_inFile;
	if(m_outFile)
		delete[] m_outFile;
	if(m_pixelValues)
		delete[] m_pixelValues;
	//deallocate memory for the 3D array
	if(m_classInfo) {
		for(int i = 0; i < m_numLayers; i++) {
			for(int j = 0; j < m_numClasses; j++) {
				if(m_classInfo[i][j])
					delete[] m_classInfo[i][j];
			}
			delete[] m_classInfo[i];
		}

		delete[] m_classInfo;
	}
}

template <class T>
void ModelMaker<T>::setClassInfo(T*** classInfo, int numLayers, int numClasses) {
	if(!classInfo) 
		throw(GeneralException("Error in ModelMaker::setClassInfo(): null classInfo given."));
	
	if(numClasses <= 0) 
		throw(GeneralException("Error in ModelMaker::setClassInfo(): number of classes must be > 0"));

	//deallocate memory for the 3D array
	if(m_classInfo) {
		for(int i = 0; i < m_numLayers; i++) {
			for(int j = 0; j < m_numClasses; j++) {
				if(m_classInfo[i][j])
					delete[] m_classInfo[i][j];
			}
			delete[] m_classInfo[i];
		}
		delete[] m_classInfo;
	}

	m_classInfo = new T**[numLayers];
	for(int i = 0; i < numClasses; i++) {
		m_classInfo[i] = new T*[numClasses];
		for(int j = 0; j < numClasses; j++) {
			m_classInfo[i][j] = new T[2];
			m_classInfo[i][j][0] = classInfo[i][j][0];
			m_classInfo[i][j][1] = classInfo[i][j][1];
		}
	}
	m_numClasses = numClasses;
	m_numLayers = numLayers;
}

template <class T>
void ModelMaker<T>::setNumLayers(int num) {
	if(num <= 0) 
		throw(GeneralException("Error in ModelMaker::setNumLayers(): number of layers must be > 0"));
	
	m_numLayers = num;
}

template <class T>
void ModelMaker<T>::setDataType(DataType type) {
	m_type = type;
}

template <class T>
void ModelMaker<T>::setInputFile(const char* file) {
	if(!file)
		throw(GeneralException("Error in ModelMaker::setInputFile(): null filename given."));

	if(m_inFile)
		delete[] m_inFile;
	
	m_inFile = new char[strlen(file)+1];
	strcpy(m_inFile, file);
	m_inFile[strlen(file)] = '\0';
}

template <class T>
void ModelMaker<T>::setOutputFile(const char* file) {
	if(!file)
		throw(GeneralException("Error in ModelMaker::setOutputFile(): null filename given."));

	if(m_outFile)
		delete[] m_outFile;
	
	m_outFile = new char[strlen(file)+1];
	strcpy(m_outFile, file);
	m_inFile[strlen(file)] = '\0';
}

template <class T>
void ModelMaker<T>::setPixelValues(int* pixelValues, int numClasses) {
	if(!pixelValues)
		throw(GeneralException("Error in ModelMaker::setPixelValues(): null pixel values given."));
	if(numClasses <= 0)
		throw(GeneralException("Error in ModelMaker::setPixelValues(): number of classes must be > 0."));
	
	if(m_pixelValues)
		delete[] m_pixelValues;
	for(int i = 0; i < numClasses; i++) 
		m_pixelValues[i] = pixelValues[i];
	
	m_numClasses = numClasses;
}

template <class T>
void ModelMaker<T>::generate(const char* filename) {
	FILE* modelFile = NULL;
	if(!filename)
		throw(GeneralException("Error in ModelMaker::generate(): null filename given."));
	
	if(!m_inFile || !m_outFile)
		throw(GeneralException("Error in ModelMaker::generate(): input or output file is null."));
	
	if(!m_pixelValues)
		throw(GeneralException("Error in ModelMaker::generate(): pixel values are null."));

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
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 8 BIT UNSIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case S8:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 8 BIT SIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case U16:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 16 BIT UNSIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case S16:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 16 BIT SIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case U32:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 32 BIT UNSIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case S32:
			fprintf(modelFile, "Integer RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Integer RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC 32 BIT SIGNED INTEGER \"%s\";\n", m_outFile);
			break;

		case FLOAT32:
			fprintf(modelFile, "Float RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Float RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC FLOAT SINGLE \"%s\";\n", m_outFile);
			break;

		case FLOAT64:
			fprintf(modelFile, "Float RASTER r1_in FILE OLD NEAREST NEIGHBOR AOI NONE \"%s\";\n", m_inFile);
			fprintf(modelFile, "Float RASTER r2_out FILE DELETE_IF_EXISTING USEALL ATHEMATIC FLOAT DOUBLE \"%s\";\n", m_outFile);
			break;
		default:
			throw(GeneralException("Error in ModelMaker::generate(): invalid data type."));
	}
	
	//generate the conditional statement
	fprintf(modelFile, "r2_out = CONDITIONAL {\n");
	for(int i = 0; i < m_numLayers; i++) {
		for(int j = 0; j < m_numClasses; j++) {
			
			//I had to break this up into all of these 
			//separate statements, for some *&#!ed up reason
			//it wouldn't work otherwise
			fprintf(modelFile, "(r1_in(%d) >= ", i);
			fprintf(modelFile, "%.3f", (float)m_classInfo[i][j][0]);
			fprintf(modelFile, " AND r1_in(%d) <= ", i);
			fprintf(modelFile, "%.3f)", (float)m_classInfo[i][j][1]);
			fprintf(modelFile, "%d", m_pixelValues[j]);
			
			//if this is not the last line of the conditional, print
			//a comma.
			if(!((i+1) >= m_numLayers && (j+1) >= m_numClasses)) 
				fprintf(modelFile, ",");

			fprintf(modelFile, "\n");


		}
	
	}

	fprintf(modelFile, "};\n");
	fprintf(modelFile, "QUIT;");
	fclose(modelFile); 
	
}	
#endif
