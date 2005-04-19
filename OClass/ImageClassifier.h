
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
	ImageClassifier(const char* filename, size_t imageWidth, size_t imageHeight, size_t numLayers, size_t numClasses=5);


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
	ImageClass<T> getClass(size_t index, size_t layer);

	//! Set the class color value for class number "index".

	/*! This function allows you to set the RGB color values for 
		a particular class being generated, referenced by "index".
		\param index zero-based index of class to use.
		\param r Red value.
		\param g Green value.
		\param b Blue value.
		\throw GeneralException.
	*/
	void setClassColor(size_t index, size_t layer, unsigned int r, unsigned int g, unsigned int b);
	
	//! Set the color for a particular class using a ClassColor object.

	/*! This function allows you to set the RGB color values for 
		a particular class being generated, referenced by "index".
		\param index zero-based index of class to use.
		\param color ClassColor object used to set color.
		\throw GeneralException.
	*/
	void setClassColor(size_t index, size_t layer, ClassColor color);

	//! Set the the textual info for a class.

	/*! This function allows you to set any special textual 
		information you wish to about a particular class.
		\param index Zero-based index of class to use.
		\param info String of text to apply to the class.
		\throw GeneralException.
	*/
	void setClassInfo(size_t index,size_t layer, const char* info);
private:

	//! Determine if a value exists in a particular layer.

	/*! This function allows you to determine if a particular value
		exists in any of the current ImageClass objects in any layer. 
		returns true of the value exists, false otherwise.
		\param val The value to search for.
		\param curLayer what layer to search
	*/
	bool valueExists(T val, size_t curLayer);

	//! Determine if a value exists in a particular vector.

	/*! This function allows you to determine if a particular value
		exists in any of the current ImageClass objects contained in "vec" 
		returns true of the value exists, false otherwise.
		\param val The value to search for.
		\param vec The vector of ImageClass objects to search
	*/
	bool valueExists(T val, const std::vector<T>& vec);

	//! Calculate and return the total error of all classes.
	double calcTotalError(size_t layer);

	//! Calculate and return the total error for a vector of ImageClass objects.

	/*! This function calculates and returns the total error for
		a vector of ImageClass objects.
		\param vec The vector of ImageClass objects to use.
	*/
	double calcTotalError(std::vector< ImageClass<T> > vec);

	//! Classes being used in classification.
	std::vector< std::vector< ImageClass<T> > > m_classes;

	//! Array of unique values to classify.
	std::vector< std::vector<T> > m_values;

	//! Number of classes.
	size_t m_numClasses;

	//! Number of layers in the image file.
	size_t m_numLayers;

	//! Height of the image (in pixels).
	size_t m_imageHeight;

	//! Width of the image (in pixels).
	size_t m_imageWidth;

	//! Size of each layer (in bytes)
	unsigned long m_layerSize;

	//! Image file being used.
	std::string m_filename;

};

template <class T>
ImageClassifier<T>::ImageClassifier(const char* filename, size_t imageWidth, size_t imageHeight, size_t numLayers, size_t numClasses):
	m_numClasses(numClasses), m_numLayers(numLayers), m_imageWidth(imageWidth), m_imageHeight(imageHeight) {
	
		
	unsigned long curByte = 0;
	std::vector<T> temp;
	T curVal; //current value being read
	FILE* imageFile = NULL;
	
	if(numClasses < 2)
		throw(GeneralException("Error in ImageClassifier constructor:"
							   "number of classes must be >= 2"));
	if(numLayers < 1)
		throw(GeneralException("Error in ImageClassifier constructor:"
							   "number of layers must be >= 1"));
	if(!filename) 
		throw(GeneralException("Error in ImageClassifier constructor: null filename given"));
	
	if((m_imageWidth == 0) || (m_imageHeight == 0)) 
		throw(GeneralException("Error in ImageClassifier constructor: image width and height must be > 0"));

	
	m_layerSize =(unsigned long)( m_imageWidth * m_imageHeight * sizeof(T));

	imageFile = fopen(filename, "rb");
	if(imageFile) {
		
		m_filename.append(filename);

		//read in all unique values of each layer
		while(!feof(imageFile)) {

			//if we are at the end of a layer
			if(curByte == (m_layerSize - sizeof(T))) {
				std::sort(temp.begin(), temp.end());
				m_values.push_back(temp);
				temp.clear();
				curByte = 0;
			}

			
			//read the next value from the file and 
			//check to see if we allready have it.
			fread((void*)&curVal, sizeof(T), 1, imageFile);	
			if(!valueExists(curVal, temp))
				temp.push_back(curVal);

			curByte += sizeof(T);
		}
	}
		
	else 
		throw(GeneralException("Error reading image file"));
	
	fclose(imageFile);
}

template <class T>
bool ImageClassifier<T>::valueExists(T val, size_t curLayer) {
	
	//if no layers have been added, or the current layer has not been added yet.
	if((m_values.size() == 0) || (curLayer >= m_values.size()))
		return(false);

	typename std::vector<T>::iterator iter;
	iter = std::find(m_values[curLayer].begin(), m_values[curLayer].end(), val);

	//if the value was not found return false.
	if(iter == m_values[curLayer].end())
		return(false);
	else
		return(true);
}


template <class T>
bool ImageClassifier<T>::valueExists(T val, const std::vector<T>& vec) {
	
	//if no layers have been added, or the current layer has not been added yet.
	if(vec.size() == 0)
		return(false);

	typename std::vector<T>::const_iterator iter;
	iter = std::find(vec.begin(), vec.end(), val);

	//if the value was not found return false.
	if(iter == vec.end())
		return(false);
	else
		return(true);
}

template <class T>
ImageClass<T> ImageClassifier<T>::getClass(size_t index, size_t layer) {
	if(index < m_numClasses)
		return(m_classes[layer][index]);
	else
		throw(GeneralException("Error in ImageClassifier::getClass(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassColor(size_t index, size_t layer, unsigned int r, unsigned int g, unsigned int b) {
	if(index < m_numClasses)
		m_classes[layer][index].setColor(r, g, b);
	else
		throw(GeneralException("Error in ImageClassifier::setClassColor(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassColor(size_t index,size_t layer, ClassColor color) {
	if(index < m_numClasses)
		m_classes[layer][index].setColor(color);
	else
		throw(GeneralException("Error in ImageClassifier::setClassColor(): index out of bounds."));
}

template <class T>
void ImageClassifier<T>::setClassInfo(size_t index, size_t layer, const char* info) {
	if(index < m_numClasses) {
		if(info)
			m_classes[layer][index].setInfo(info);
		else
			throw(GeneralException("Error in ImageClassifier::setClassInfo(): attempting to set null info string."));
	}

	else 
		throw(GeneralException("Error in ImageClassifier::setClassInfo(): index out of bounds"));
}

template <class T>
void ImageClassifier<T>::classify(int numClasses) {
	
	//how many bytes per layer?
	unsigned long layerSize = (unsigned long)(m_imageWidth * m_imageHeight * sizeof(T));

	size_t numRemainingValues = 0; //values left to distribute
	size_t k = 0; //counter
	//if numClasses != 0, the user 
	//wants to override the number of classes
	//to be generated as specified in the constructor.
	if(numClasses > 0) 
		m_numClasses = numClasses;

	if(m_numClasses <= 0)
		throw(GeneralException("Error in classify(): number of classes must be greater than zero"));

	for(size_t l = 0; l < m_numLayers; l++) {
		std::vector< ImageClass<T> > curClasses;
		std::vector< ImageClass<T> > classesTemp;	//temp storage for classes	
		std::vector<double> errorDiffVec; //Store the error changes for each iteration
		std::vector<double>::const_iterator errorDiffIter; //iterate through error diffs.
		size_t numValues = m_values[l].size(); //number of unique values.
		size_t classSize = (size_t)(numValues / m_numClasses);//how big each class should be

		//if the number of values does not distribute out evenly among the classes,
		//calculate the number of extra values that we need to place.
		numRemainingValues = numValues - (classSize*m_numClasses);
	
		if(classSize <= 0) 
			throw(GeneralException("Error in classify(): number of classes exceeds number of unique values."));
	
		size_t curIndex = 0; //current value index.
		double curMeanClass1 = 0;//mean of current class
		double curMeanClass2 = 0;//mean of next class in list
		double diff1 = 0; //distance from mean of second class to boundary value of first class.
		double diff2 = 0; //distance from mean of first class to first value of second class.
		double oldError = 0; //error before new algorithm iteration.
		double newError = 0; //error after latest iteration
		std::string curClassName; //name for current new class.
		ImageClass<T> curClass; //current class being built

		//build initial classes of size classSize
		for(size_t i = 0; i < m_numClasses; i++) {
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
			for(size_t j = 0; ((j < curClass.getInitialCapacity()) && (curIndex < numValues)); j++) {
				curClass.addValue(m_values[l][curIndex]);
				curIndex++;
			}
		
			//calculate the mean of the new class and add it to the classes vector.
			curClass.getMean(true);
			curClass.getError(true);
			classesTemp.push_back(curClass);
			k++;
		
		}
		
		curIndex = 0;
		curClasses = classesTemp;
		while(1) {
			double errorDiff = 0;
			double percentError = 0.0;
			oldError = calcTotalError(classesTemp);
			for(curIndex = 0; curIndex < (m_numClasses-1); curIndex++) {
				curMeanClass1 = classesTemp[curIndex].getMean(true);
				curMeanClass2 = classesTemp[curIndex+1].getMean(true);
				size_t class1EndpointIndex = classesTemp[curIndex].getNumValues()-1;
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
				else if(diff2 > diff1) {
					classesTemp[curIndex].removeValue(class1EndpointIndex); //remove value from class 1
					classesTemp[curIndex+1].addValue(class1EndpointValue, FRONT); //add value to class 2
				}			
			}
		
			newError = calcTotalError(classesTemp);
		
			//calc change in error
			errorDiff = newError - oldError;
			percentError = fabs((newError - oldError))/oldError; //per
			
		//	errorDiffIter = std::find(errorDiffVec.begin(), errorDiffVec.end(), errorDiff)
			
			//if error has increased since last iteration, break and
			//discard changes
			if(errorDiff > 0)
				break;
			
	//		//else save our changes
	//		else if((errorDiff == 0) || (percentError <= 0.01) ) {
	//			curClasses = classesTemp;
	//			break;
	//		}
		
			else 
				curClasses = classesTemp;
			
		}
		
	for(size_t j = 0; j < m_numClasses; j++)
		curClasses[j].sort();	

	 m_classes.push_back(curClasses);
	}
}

template <class T>
double ImageClassifier<T>::calcTotalError(size_t layer) {
	double totalError = 0;
	for(size_t i = 0; i < m_numClasses; i++) {
		totalError += m_classes[layer][i].getError(true);
	}
	return(totalError);
}

template <class T>
double ImageClassifier<T>::calcTotalError(std::vector< ImageClass<T> > vec) {
	double totalError = 0;
	for(size_t i = 0; i < vec.size(); i++) {
		totalError += vec[i].getError(true);
	}
	return(totalError);
}


template <class T>
void ImageClassifier<T>::saveReportXML(const char* filename, DataType type ) {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");
	
	if(!filename) 
		throw GeneralException("Error in ImageClassifier::saveReportXML():\n"
							   "NULL filename given");

	TiXmlDocument doc;
	
	TiXmlElement* dataE = NULL;
	TiXmlDeclaration decl("1.0", NULL, "yes");
	
	TiXmlElement* classE = NULL;
	TiXmlElement* classNameE = NULL;
	TiXmlElement* classRangeE = NULL;
	TiXmlElement* classColorE = NULL;
	TiXmlElement* classCommentsE = NULL;
	TiXmlElement* rootE = NULL;

	TiXmlText classNameText("");
	TiXmlText classColorText("");
	TiXmlText classCommentsText("");

	char lowNum[30] = {'\0'};
	char highNum[30] = {'\0'};
	char hexColorString[8] = {'\0'};

	rootE = new TiXmlElement("classificationReport");
	rootE->SetAttribute("numLayers", (int)m_numLayers);
	rootE->SetAttribute("dataTypeNumber", type);
	

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
			
	for(size_t j = 0; j < m_numLayers; j++) {
		dataE = new TiXmlElement("classificationData");
		dataE->SetAttribute("numClasses", (int)m_numClasses);
		
		if(!m_filename.empty())
				dataE->SetAttribute("imageFile", m_filename.c_str());
		else
				dataE->SetAttribute("imageFile", "Not Set");
		
		dataE->SetAttribute("layerNumber", (int)j+1);
		for(size_t i = 0; i < m_numClasses; i++) {
			size_t numValues = m_classes[j][i].getNumValues();
			ClassColor curColor = m_classes[j][i].getColor();
			for(size_t p = 0; p < 30; p++) {
				lowNum[p] = '\0';
				highNum[p] = '\0';
			}

			for(size_t p = 0; p < 8; p++)
				hexColorString[p] = '\0';

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
	   

			classNameText.SetValue(m_classes[j][i].getName());
		
			//set range values
			sprintf(lowNum, "%.3f", (float)m_classes[j][i][0]);
			sprintf(highNum, "%.3f", (float)m_classes[j][i][numValues-1]);
			classRangeE->SetAttribute("low", lowNum);
			classRangeE->SetAttribute("high", highNum);
		
			//set color value
			sprintf(hexColorString, "#%X%X%X", curColor.getRed(), curColor.getGreen(), curColor.getBlue());
			classColorText.SetValue(hexColorString);
		
			if(m_classes[j][i].getInfo()) 
				classCommentsText.SetValue(m_classes[j][i].getInfo());
			else
				classCommentsText.SetValue("No comments");

			classNameE->InsertEndChild(classNameText);
			classColorE->InsertEndChild(classColorText);
			classCommentsE->InsertEndChild(classCommentsText);
			classE->InsertEndChild(*classNameE);
			classE->InsertEndChild(*classRangeE);
			classE->InsertEndChild(*classColorE);
			classE->InsertEndChild(*classCommentsE);
		

			dataE->InsertEndChild(*classE);
		}
		rootE->InsertEndChild(*dataE);
		delete dataE;
		dataE = NULL;
	}
	doc.InsertEndChild(*rootE);
	doc.SaveFile(filename);

	if(rootE)
		delete rootE;
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
	if(dataE)
		delete dataE;
}

template <class T>
void ImageClassifier<T>::saveReportHTML(const char* filename) {
	if(m_classes.size() == 0) 
		throw GeneralException("Error: Classification not yet performed\n");

	FILE* htmlFile = NULL;
	size_t j = 0;
	size_t k = 0;
	if(!filename) 
		htmlFile = fopen("classificationReport.html", "w+");
	else
		htmlFile = fopen(filename, "w+");

	if(!htmlFile) 
		throw(GeneralException("Error in saveReportHTML(): error creating output file"));

	if(!m_filename.empty())
		fprintf(htmlFile, "<html>\n<head><title>Classification Report for %s</title></head>\n", m_filename.c_str());
	else
		fprintf(htmlFile, "<html>\n<head><title>Classification Report</title></head>\n");
	
	fprintf(htmlFile, "<div align=\"center\">\n");
	fprintf(htmlFile, "<h2>Classification Results</h2>\n");
	fprintf(htmlFile, "<body>\n");

	for(size_t l = 0; l < m_numLayers; l++) {
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
				fprintf(htmlFile, "<td>%.3f</td>", (float)m_classes[l][k][0]);
				k++;
			}

			//prepare for printing high values
			fprintf(htmlFile, "</tr><tr><td><b>High</b></td>\n");
			k = j;
			for(int i = 0; (i < 5 && k < m_numClasses); i++) {
				size_t numValues = m_classes[l][k].getNumValues();
				fprintf(htmlFile, "<td>%.3f</td>", (float)m_classes[l][k][numValues-1]);
				k++;
			}
	
			fprintf(htmlFile, "</tr></table><p>");
			j += 5;
		}
		j=0;
		k=0;
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
		for(size_t j = 0; j < m_numLayers; j++) {
			fprintf(outputFile, "Layer: %d\n", j+1);
			for(size_t i = 0; i < m_numClasses; i++) {
				fprintf(outputFile, "Class %d:\n", i+1);
				fprintf(outputFile, "Low: %.3f\n", (float)m_classes[j][i][0]);
				fprintf(outputFile, "High: %.3f\n\n", (float)m_classes[j][i][m_classes[j][i].getNumValues()-1]);
			}
		}
	}

	else
		throw(GeneralException("Error in ImageClassifier::saveReport(): Error creating output file"));

	fclose(outputFile);
}
#endif
