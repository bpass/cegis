
#ifndef IMAGE_CLASSIFIER_H
#define IMAGE_CLASSIFIER_H

#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "GeneralException.h"
#include "ImageClass.h"
#include "tinyxml.h"

template <class T>
class ImageClassifier {
public:
	ImageClassifier(const char* filename, int numClasses=5);
	ImageClassifier(T values[], int length, int numClasses=5);
	~ImageClassifier();
	void classify(int numClasses=0);
	void printReport(); //print report to stdout
	void printReportHTML(const char* filename); //print report in html format to file
	void saveReportXML(const char* filename); //save xml file
	void saveReportHTML(const char* filename=NULL);
	void saveReport(const char* filename=NULL);
	ImageClass<T> getClass(unsigned int index);
	void setClassColor(unsigned int index, unsigned int r, unsigned int g, unsigned int b);
	void setClassColor(unsigned int index, ClassColor color);
	void setClassInfo(unsigned int index, const char* info);
private:
	bool valueExists(T val);
	double calcTotalError();
	double calcTotalError(std::vector< ImageClass<T> > vec);
	std::vector< ImageClass<T> > m_classes;
	std::vector<T> m_values;
	int m_numClasses;
	char* m_filename;
};

template <class T>
ImageClassifier<T>::ImageClassifier(const char* filename, int numClasses): m_numClasses(numClasses) {
	T curVal;
	FILE* imageFile = NULL;
	
	if(!filename) 
		throw(GeneralException("Error in ImageClassifier constructor: null filename given"));
		
	imageFile = fopen(filename, "rb");
	if(imageFile) {

		m_filename = new char[strlen(filename)+1];
		strcpy(m_filename, filename);
		m_filename[strlen(filename)+1] = '\0';

		while(!feof(imageFile)) {
			
			fread((void*)&curVal, sizeof(T), 1, imageFile);
	
			if(!valueExists(curVal))
				m_values.push_back(curVal);
		}
		std::sort(m_values.begin(), m_values.end());
	}
		
	else {
		m_filename = NULL;
		throw(GeneralException("Error reading image file"));
	}
	
	fclose(imageFile);
}

template <class T>
ImageClassifier<T>::ImageClassifier(T values[], int length, int numClasses): m_numClasses(numClasses) {
	m_values.clear();
	for(int i = 0; i < length; i++) {
		if(!valueExists(values[i]))
			m_values.push_back(values[i]);
	}
	
	std::sort(m_values.begin(), m_values.end());

}

template <class T>
ImageClassifier<T>::~ImageClassifier() {
	if(m_filename)
		delete[] m_filename;
}

template <class T>
bool ImageClassifier<T>::valueExists(T val) {
	for(unsigned int i = 0; i < m_values.size(); i++) {
		if(m_values[i] == val) 
			return true;
	}
	
	return false;
}

template <class T>
ImageClass<T> ImageClassifier<T>::getClass(unsigned int index) {
	if(index < m_numClasses)
		return(m_classes[index]);
	else
		throw(GeneralException("Error in ImageClassifier::getClass(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassColor(unsigned int index, unsigned int r, unsigned int g, unsigned int b) {
	if(index < m_numClasses)
		m_classes[index].setColor(r, g, b);
	else
		throw(GeneralException("Error in ImageClassifier::setClassColor(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassColor(unsigned int index, ClassColor color) {
	if(index < m_numClasses)
		m_classes[index].setColor(color);
	else
		throw(GeneralException("Error in ImageClassifier::setClassColor(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassInfo(unsigned int index, const char* info) {
	if(index < m_numClasses) {
		if(info)
			m_classes[i].setInfo(info);
		else
			throw(GeneralException("Error in ImageClassifier::setClassInfo(): attempting to set null info string."));
	}

	else 
		throw(GeneralException("Error in ImageClassifier::setClassInfo(): index out of bounds"));
}

template <class T>
void ImageClassifier<T>::classify(int numClasses) {
	int numRemainingValues = 0;
	int k = 0;
	if(numClasses > 0) 
		m_numClasses = numClasses;

	if(m_numClasses <= 0)
		throw(GeneralException("Error in classify(): number of classes must be greater than zero"));
	
	std::vector< ImageClass<T> > classesTemp;		
	int numValues = m_values.size();
	int classSize = (int)(numValues / m_numClasses);

	//if the number of values does not distribute out evenly among the classes,
	//calculate the number of extra values that 
	numRemainingValues = numValues - (classSize*m_numClasses);
	
	if(classSize <= 0) 
		throw(GeneralException("Error in classify(): number of classes exceeds number of unique values."));
	
	int curIndex = 0;
	double curMeanClass1 = 0;
	double curMeanClass2 = 0;
	double diff1 = 0;
	double diff2 = 0;
	double oldError = 0;
	double newError = 0;
	//double errorLimit = 0.01;
	std::string curClassName;
	ImageClass<T> curClass;
	//build initial classes of size classSize
	for(int i = 0; i < m_numClasses; i++) {
		char* number = new char[7];

		for(int j = 0; j < 7; j++) 
			number[j] = '\0';
		
		sprintf(number, "%d", i+1);
		number[6] = '\0';
		
		curClassName = "";
		curClassName.append("Class ");
		curClassName.append(number);
	    delete[] number;
		curClass.clear();
		curClass.setName(curClassName.c_str());
		curClass.setCapacity(classSize);

		//if we still need to distribute some values across the classes
		if(k < numRemainingValues) 
			curClass++; //increase class capacity by 1;

		for(int j = 0; ((j < curClass.getCapacity()) && (curIndex < numValues)); j++) {
			curClass.addValue(m_values[curIndex]);
			curIndex++;
		}
		
		classesTemp.push_back(curClass);
		k++;
		
	}
		
	m_classes = classesTemp;
	printf("Before Classification\n");
	printReport();
	printf("\nAfter Classification\n");
	//begin classification
	curIndex = 0;
	
	while(1) {
		double errorDiff = 0;
		double percentError = 0.0;
		oldError = calcTotalError();
		for(curIndex = 0; curIndex < (m_numClasses-1); curIndex++) {
			curMeanClass1 = classesTemp[curIndex].getMean();
			curMeanClass2 = classesTemp[curIndex+1].getMean();
			int class1EndpointIndex = classesTemp[curIndex].getNumValues()-1;
			T class1EndpointValue = classesTemp[curIndex][class1EndpointIndex];
			T class2FirstValue = classesTemp[curIndex+1][0];
			
			//difference between class 1 endpoint and class 2 mean
			diff1 = fabs(curMeanClass2 - class1EndpointValue);
			diff2 = fabs(curMeanClass1 - class2FirstValue);
			
			//if first value of class 2 belongs in class 1
			if(diff2 < diff1) {
				classesTemp[curIndex+1].removeValue(0); //removed value from class 2
				classesTemp[curIndex].addValue(class2FirstValue); //insert value into class 1
			
			}
			
			//else if end value of class 1 belongs in class 2
			else if(diff2 > diff1){
				classesTemp[curIndex].removeValue(class1EndpointIndex); //remove value from class 1
				classesTemp[curIndex+1].addValue(class1EndpointValue, FRONT); //add value to class 2
			
			}
			
		}
		
		newError = calcTotalError(classesTemp);
		
		//calc change in error
		errorDiff = newError - oldError;
		
		//if error has actually gone up, break
		//if(errorDiff > 0)
		//	break;
		
		//if error has increased since last iteration, break and
		//discard changes
		if(errorDiff > 0)
			break;
			
		//else save our changes
		else if(errorDiff == 0) {
			m_classes = classesTemp;
			break;
		}
		
		else
			m_classes = classesTemp;
		
	}
	for(int j = 0; j < m_numClasses; j++)
		m_classes[j].sort();	
}

template <class T>
double ImageClassifier<T>::calcTotalError() {
	double totalError = 0;
	for(int i = 0; i < m_numClasses; i++) 
		totalError += m_classes[i].getError();
	
	return(totalError);
}

template <class T>
double ImageClassifier<T>::calcTotalError(std::vector< ImageClass<T> > vec) {
	double totalError = 0;
	for(unsigned int i = 0; i < vec.size(); i++) 
		totalError += vec[i].getError();
	
	return(totalError);
}

template <class T>
void ImageClassifier<T>::printReport() {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");
	
	printf( "Classification Report\n");
	for(int i = 0; i < m_numClasses; i++) {
		int endIndex = m_classes[i].getNumValues()-1;
		printf("%s\tRange: %.3f - %.3f\n", m_classes[i].getName(), (float)m_classes[i][0], (float)m_classes[i][endIndex]);
	}
}

template <class T>
void ImageClassifier<T>::saveReportXML(const char* filename) {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");
	
	if(!filename) 
		throw GeneralException("Errorin ImageClassifier::saveReportXML():\n"
							   "NULL filename given");

	bool append = false;
	TiXmlDocument doc;
	
	if(doc.LoadFile(filename))
		append = true;
	
	TiXmlElement dataE("classificationData");
	TiXmlDeclaration decl("1.0", NULL, "yes");
	dataE.SetAttribute("numClasses", m_numClasses);
	if(m_filename)
		dataE.SetAttribute("imageFile", m_filename);
	else
		dataE.SetAttribute("imageFile", "Not Set");

//	TiXmlComment headerComment;

	TiXmlElement* classE = NULL;
	TiXmlElement* classNameE = NULL;
	TiXmlElement* classRangeE = NULL;
	TiXmlElement* classColorE = NULL;
	TiXmlElement* classCommentsE = NULL;

	TiXmlText classNameText("");
	TiXmlText classColorText("");
	TiXmlText classCommentsText("");

	char lowNum[30] = {'\0'};
	char highNum[30] = {'\0'};
	char hexColorString[8] = {'\0'};
	for(int i = 0; i < m_numClasses; i++) {
		int numValues = m_classes[i].getNumValues();
		ClassColor curColor = m_classes[i].getColor();
		for(int j = 0; j < 30; j++) {
			lowNum[j] = '\0';
			highNum[j] = '\0';
		}

		for(int j = 0; j < 8; j++)
			hexColorString[j] = '\0';

		if(classE)
			delete classE;
		if(classNameE)
			delete classNameE;
		if(classRangeE)
			delete classRangeE;
		if(classColorE)
			delete classColorE;
		if(classCommentsE)
			delete classCommentsE;
		
		classE = new TiXmlElement("class");
		classNameE = new TiXmlElement("name");
		classRangeE = new TiXmlElement("range");
		classColorE = new TiXmlElement("color");
		classCommentsE = new TiXmlElement("comments");
	   

		classNameText.SetValue(m_classes[i].getName());
		
		//set range values
		sprintf(lowNum, "%.3f", (float)m_classes[i][0]);
		sprintf(highNum, "%.3f", (float)m_classes[i][numValues-1]);
		classRangeE->SetAttribute("low", lowNum);
		classRangeE->SetAttribute("high", highNum);
		
		//set color value
		sprintf(hexColorString, "#%X%X%X", curColor.getRed(), curColor.getGreen(), curColor.getBlue());
		classColorText.SetValue(hexColorString);
		
		if(m_classes[i].getInfo()) 
			classCommentsText.SetValue(m_classes[i].getInfo());
		else
			classCommentsText.SetValue("No comments");

		classNameE->InsertEndChild(classNameText);
		classColorE->InsertEndChild(classColorText);
		classCommentsE->InsertEndChild(classCommentsText);
		classE->InsertEndChild(*classNameE);
		classE->InsertEndChild(*classRangeE);
		classE->InsertEndChild(*classColorE);
		classE->InsertEndChild(*classCommentsE);
		

		dataE.InsertEndChild(*classE);
	}

	TiXmlElement* rootE = NULL;

	if(append) {
		int numLayers = 0;
		TiXmlHandle handle(&doc);
		rootE = handle.FirstChild("classificationReport").Element();
		if(!rootE)
			throw(GeneralException("Error in ImageClassifier::saveReportXML():\n"
								   "Existing XML document missing <classificationReport> tag."));
		rootE->Attribute("numLayers", &numLayers);
		rootE->SetAttribute("numLayers", numLayers+1);
		dataE.SetAttribute("layerNumber", numLayers+1);
		rootE->InsertEndChild(dataE);
		doc.SaveFile(filename);
	}
	else {
		rootE = new TiXmlElement("classificationReport");
		rootE->SetAttribute("numLayers", 1);
		dataE.SetAttribute("layerNumber", 1);
		rootE->InsertEndChild(dataE);
		doc.InsertEndChild(decl);
		doc.InsertEndChild(*rootE);
		doc.SaveFile(filename);
	}
}

template <class T>
void ImageClassifier<T>::saveReportHTML(const char* filename) {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");

	FILE* htmlFile = NULL;
	int j = 0;
	int k = 0;
	if(!filename) 
		htmlFile = fopen("classificationReport.html", "w+");
	else
		htmlFile = fopen(filename, "w+");

	if(!htmlFile) 
		throw(GeneralException("Error in saveReportHTML(): error creating output file"));

	if(m_filename)
		fprintf(htmlFile, "<html>\n<head><title>Classification Report for %s</title></head>\n", m_filename);
	else
		fprintf(htmlFile, "<html>\n<head><title>Classification Report</title></head>\n");
	
	fprintf(htmlFile, "<div align=\"center\">\n");
	fprintf(htmlFile, "<h2>Classification Results</h2>\n");
	fprintf(htmlFile, "<body>\n");

	//print out classification data tables, 5 classes at a time.
	while(j < m_numClasses) {
		fprintf(htmlFile, "<table border=\"1\" width=\"150\">\n");
		fprintf(htmlFile, "<tr><td>&nbsp;</td>");
		k = j;
		for(int i = 0; (i < 5 && k < m_numClasses); i++) {
			fprintf(htmlFile, "<th>Class %d</th>", k+1);
			k++;
		}
	
		//prepare for printing low values
		fprintf(htmlFile, "</tr><tr><td><b>Low</b></td>\n");
		k = j;
		for(int i = 0; (i < 5 && k < m_numClasses); i++) {
			fprintf(htmlFile, "<td>%.3f</td>", (float)m_classes[k][0]);
			k++;
		}

		//prepare for printing high values
		fprintf(htmlFile, "</tr><tr><td><b>High</b></td>\n");
		k = j;
		for(int i = 0; (i < 5 && k < m_numClasses); i++) {
			int numValues = m_classes[k].getNumValues();
			fprintf(htmlFile, "<td>%.3f</td>", (float)m_classes[k][numValues-1]);
			k++;
		}
	
		fprintf(htmlFile, "</tr></table><p>");
		j += 5;
	}
	fprintf(htmlFile, "</div></body></html>\n");
	fclose(htmlFile);
}

template <class T>
void ImageClassifier<T>::saveReport(const char* filename) {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");

	FILE* outputFile = NULL;

	if(filename) 
		outputFile = fopen(filename, "w+");
	else
		outputFile = fopen("classificationReport.txt", "w+");

	if(outputFile) {
		if(filename)
			fprintf(outputFile, "Classification Report for \"%s\"\n\n", filename);
		else
			fprintf(outputFile, "Classification Report:\n\n");

		for(int i = 0; i < m_numClasses; i++) {
			fprintf(outputFile, "Class %d:\n", i);
			fprintf(outputFile, "Low: %.3f\n", (float)m_classes[i][0]);
			fprintf(outputFile, "High: %.3f\n\n", (float)m_classes[i][m_classes[i].getNumValues()-1]);
		}
	}

	else
		throw(GeneralException("Error in ImageClassifier::saveReport(): Error creating output file"));

	fclose(outputFile);
}
#endif
