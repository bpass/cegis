
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
#include "DataType.h"

//! ImageClassifier object, performs a Jenks-Caspall classification on generic binary image data.

/*! The ImageClassifier object is responsible for performing the image classification.
	It makes use of the ImageClass object to store all class information, and the 
	ModelMaker object to build the spatial modeler script.
*/
template <class T>
class ImageClassifier {
public:

	//! Contruct an ImageClassifier object using an image file and a certain number of classes.

	/*! This constructor instantiates an ImageClassifier object
		loading the image referenced by filename, numClasses number of
		classes.
		\param filename The image file.
		\param numClasses the number of classes to generate.
		\throw GeneralException 
	*/
	ImageClassifier(const char* filename, int numClasses=5);

	//! Construct an ImageClassifier object with an array of values and a certain number of classes.

	/*! This constructor allows the user to pass the pixel values in in an 
		array rather than loading a generic binary image file.
		\param values The pixel values.
		\param length The length of the pixel values array.
		\param numClasses The number of classes to generate.
		\throw GeneralException
	*/
	ImageClassifier(T values[], int length, int numClasses=5);

	~ImageClassifier();
	
	//! Perform the classification

	/*! This function runs the classification algorithm on
		the current data values.  If the user passes in a
		value for numClasses other than 0, he/she will override
		the number of generated classes that was specified in the
		constructor.
		\param numClasses Number of classes to generate, overrides current value if not 0.
		\throw GeneralException.
	*/
	void classify(int numClasses=0);

	//! Print classification report to stdout
	void printReport();
	
	//! Save results to an XML file.

	/*! This function saves your classification results to an XML file.
		If the file referenced by "filename" allready exists and is 
		properly formatted, the classification results will be appended
		to the file, rather than writing a new file.  This allows you
		to have classification data for multiple image layers (which 
		are each stored in a separate file) in one XML file.
		\param filename The XML output file.
		\param type The DataType of the original image.
		\throw GeneralException.
	*/
	void saveReportXML(const char* filename, DataType type); 

	//! Save a classification report to an HTML file.

	/*! This function allows you to save your classification
		results to an HTML file.
		\param filename HTML output file.
		\throw GeneralException.
	*/
	void saveReportHTML(const char* filename=NULL);

	//! Save a classification report to a text file.

	/*! This function allows you to save your classification
		results to an ASCII text file.
		\param filename Text output file.
		\throw GeneralException.
	*/
	void saveReport(const char* filename=NULL);

	//! Get the ImageClass object referenced by "index".

	/*! This function allows you to get the ImageClass object
		corresponding to the zero-based index of classes being generated.
		\param index The index of the desired class.
		\throw GeneralException
	*/
	ImageClass<T> getClass(unsigned int index);

	//! Set the class color value for class number "index".

	/*! This function allows you to set the RGB color values for 
		a particular class being generated, referenced by "index".
		\param index zero-based index of class to use.
		\param r Red value.
		\param g Green value.
		\param b Blue value.
		\throw GeneralException.
	*/
	void setClassColor(unsigned int index, unsigned int r, unsigned int g, unsigned int b);
	
	//! Set the color for a particular class using a ClassColor object.

	/*! This function allows you to set the RGB color values for 
		a particular class being generated, referenced by "index".
		\param index zero-based index of class to use.
		\param color ClassColor object used to set color.
		\throw GeneralException.
	*/
	void setClassColor(unsigned int index, ClassColor color);

	//! Set the the textual info for a class.

	/*! This function allows you to set any special textual 
		information you wish to about a particular class.
		\param index Zero-based index of class to use.
		\param info String of text to apply to the class.
		\throw GeneralException.
	*/
	void setClassInfo(unsigned int index, const char* info);
private:

	//! Determine if a value exists.

	/*! This function allows you to determine if a particular value
		exists in any of the current ImageClass objects. 
		returns true of the value exists, false otherwise.
		\param val The value to search for.
	*/
	bool valueExists(T val);

	//! Calculate and return the total error of all classes.
	double calcTotalError();

	//! Calculate and return the total error for a vector of ImageClass objects.

	/*! This function calculates and returns the total error for
		a vector of ImageClass objects.
		\param vec The vector of ImageClass objects to use.
	*/
	double calcTotalError(std::vector< ImageClass<T> > vec);

	//! Classes being used in classification.
	std::vector< ImageClass<T> > m_classes;

	//! Array of unique values to classify.
	std::vector<T> m_values;

	//! Number of classes.
	int m_numClasses;

	//! Image file being used.
	char* m_filename;
};

template <class T>
ImageClassifier<T>::ImageClassifier(const char* filename, int numClasses):m_numClasses(numClasses) {
	T curVal; //current value being read
	FILE* imageFile = NULL;
	
	if(numClasses < 2)
		throw(GeneralException("Error in ImageClassifier constructor:"
								"number of classes must be >= 2"));
	if(!filename) 
		throw(GeneralException("Error in ImageClassifier constructor: null filename given"));
		
	imageFile = fopen(filename, "rb");
	if(imageFile) {
		
		//set m_filename to filename
		m_filename = new char[strlen(filename)+1];
		strcpy(m_filename, filename);
		m_filename[strlen(filename)+1] = '\0';

		while(!feof(imageFile)) {
			
			//read the next value from the file and 
			//check to see if we allready have it.
			fread((void*)&curVal, sizeof(T), 1, imageFile);	
			if(!valueExists(curVal))
				m_values.push_back(curVal);
		}
		//sort all unique values
		std::sort(m_values.begin(), m_values.end());
	}
		
	else 
		throw(GeneralException("Error reading image file"));
	
	
	fclose(imageFile);
}

template <class T>
ImageClassifier<T>::ImageClassifier(T values[], int length, int numClasses): m_numClasses(numClasses) {
	if(numClasses < 2)
		throw(GeneralException("Error in ImageClassifier constructor:"
								"number of classes must be >= 2"));
	
	m_values.clear();

	//read all values in array,
	//put every unique value into m_values and then sort it.
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
	typename std::vector<T>::iterator iter;
	iter = std::find(m_values.begin(), m_values.end(), val);

	//if the value was now found return false.
	if(iter == m_values.end())
		return(false);
	else
		return(true);

	/*
	for(unsigned int i = 0; i < m_values.size(); i++) {
		if(m_values[i] == val) 
			return true;
	}
	
	return false;
	*/
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
	int numRemainingValues = 0; //values left to distribute
	int k = 0; //counter
	bool oneGoodIterationDone = false; //did we do at least one iteration
									   //that reduced the error?
	//if numClasses != 0, the user 
	//wants to override the number of classes
	//to be generated as specified in the constructor.
	if(numClasses > 0) 
		m_numClasses = numClasses;

	if(m_numClasses <= 0)
		throw(GeneralException("Error in classify(): number of classes must be greater than zero"));
	
	std::vector< ImageClass<T> > classesTemp;	//temp storage for classes	
	int numValues = m_values.size(); //number of unique values.
	int classSize = (int)(numValues / m_numClasses);//how big each class should be

	//if the number of values does not distribute out evenly among the classes,
	//calculate the number of extra values that we need to place.
	numRemainingValues = numValues - (classSize*m_numClasses);
	
	if(classSize <= 0) 
		throw(GeneralException("Error in classify(): number of classes exceeds number of unique values."));
	
	int curIndex = 0; //current value index.
	double curMeanClass1 = 0;//mean of current class
	double curMeanClass2 = 0;//mean of next class in list
	double diff1 = 0; //distance from mean of second class to boundary value of first class.
	double diff2 = 0; //distance from mean of first class to first value of second class.
	double oldError = 0; //error before new algorithm iteration.
	double newError = 0; //error after latest iteration
	std::string curClassName; //name for current new class.
	ImageClass<T> curClass; //current class being built

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
		curClass.setInitialCapacity(classSize);

		//if we still need to distribute some values across the classes
		if(k < numRemainingValues) 
			curClass++; //increase class capacity by 1;
		
		//fill current class with next chunk of values.
		for(int j = 0; ((j < curClass.getInitialCapacity()) && (curIndex < numValues)); j++) {
			curClass.addValue(m_values[curIndex]);
			curIndex++;
		}
		
		//calculate the mean of the new class and add it to the classes vector.
		curClass.getMean(true);
		classesTemp.push_back(curClass);
		k++;
		
	}
		
	curIndex = 0;
	m_classes = classesTemp;
	while(1) {
		double errorDiff = 0;
		double percentError = 0.0;
		oldError = calcTotalError();
		for(curIndex = 0; curIndex < (m_numClasses-1); curIndex++) {
			curMeanClass1 = classesTemp[curIndex].getMean(true);
			curMeanClass2 = classesTemp[curIndex+1].getMean(true);
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
			oneGoodIterationDone = true;
			m_classes = classesTemp;
			break;
		}
		
		else {
			oneGoodIterationDone = true;
			m_classes = classesTemp;
		}
		
	}
	for(int j = 0; j < m_numClasses; j++)
		m_classes[j].sort();	
}

template <class T>
double ImageClassifier<T>::calcTotalError() {
	double totalError = 0;
	for(int i = 0; i < m_numClasses; i++) {
		totalError += m_classes[i].getError();
	}
	return(totalError);
}

template <class T>
double ImageClassifier<T>::calcTotalError(std::vector< ImageClass<T> > vec) {
	double totalError = 0;
	for(unsigned int i = 0; i < vec.size(); i++) {
		totalError += vec[i].getError();
	}
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
void ImageClassifier<T>::saveReportXML(const char* filename, DataType type ) {
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
		rootE->SetAttribute("dataTypeNumber", type);
		dataE.SetAttribute("layerNumber", 1);

		//set the proper name for the integer data type value
		switch(type) {
		case U8:
			rootE->SetAttribute("dataTypeName", "Unsigned 8-bit");
			break;

		case S8:
			rootE->SetAttribute("dataTypeName", "Signed 8-bit");
			break;

		case U16:
			rootE->SetAttribute("dataTypeName", "Unsigned 16-bit");
			break;

		case S16:
			rootE->SetAttribute("dataTypeName", "Signed 16-bit");
			break;

		case U32:
			rootE->SetAttribute("dataTypeName", "Unsigned 32-bit");
			break;

		case S32:
			rootE->SetAttribute("dataTypeName", "Signed 32-bit");
			break;

		case FLOAT32:
			rootE->SetAttribute("dataTypeName", "32-bit Floating Point");
			break;

		case FLOAT64:
			rootE->SetAttribute("dataTypeName", "64-bit Floating Point");
			break;
		
		default:
			throw(GeneralException("Error in ImageClassifier::saveReportXML():"
								   "invalid data type"));
		}
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
			fprintf(outputFile, "Class %d:\n", i+1);
			fprintf(outputFile, "Low: %.3f\n", (float)m_classes[i][0]);
			fprintf(outputFile, "High: %.3f\n\n", (float)m_classes[i][m_classes[i].getNumValues()-1]);
		}
	}

	else
		throw(GeneralException("Error in ImageClassifier::saveReport(): Error creating output file"));

	fclose(outputFile);
}
#endif
