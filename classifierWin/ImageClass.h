#ifndef IMG_CLASS_H
#define IMG_CLASS_H
#include "GeneralException.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "ClassColor.h"

//!Enum Value to indicate where in the class value list to insert new values.
enum {
	FRONT=0, /*!< Enum Value FRONT. */ 
	BACK /*!< Enum Value BACK. */ 
}; 

//! An object that stores pixel values pertaining to a certain class.

/*! The ImageClass object is used to store each pixel value associated
	with a particular class for an image.  ImageClass also stores other
	information about the class including its name, size, color(for possible 
	future uses), and any particular textual information the user wishes to
	include about the class. The ImageClassifier object uses ImageClass objects
	to perform its Jenks-Caspall classifications on generic binary images.
*/
template <class T>
class ImageClass {
public:
	//! Default Constructor.
	ImageClass();

	//! Construct an ImageClass object with a given name and capacity.
	ImageClass(const char* name, int capacity);

	//! Copy Constructor.
	ImageClass(const ImageClass& ic);

	//! Destructor.
	~ImageClass();
	
	//! Insert a value into the class.

	/*! This function allows you to insert a new value
		into the class.  The default insert position is at
		the back of the value list, but the user may also
		specify FRONT to insert at the front of the list.
		By default the mean value of the class is calculated
		each time a new value is added, but this can be disabled by 
		passing false as the third parameter. Only unique values can
		be inserted into a class, duplicates are not allowed.
		\param newVal The value to be inserted.
		\param position Where to place the new value, in the front or back of the list.
		\throw GeneralException 
	*/
	void addValue(T newVal, int position = BACK);

	//! Remove a value from the class

	/*! This function allows you to remove a value from 
		the image class located at a particular index.
		\param index the zero-based index of the value to be removed.
		\throw GeneralException 
	*/
	void removeValue(int index);

	//! Remove a value from the class

	/*! This function allows you to remove a value from 
		the image class located at a particular iterator position.
		\param pos An iterator pointing to the position of the item to be removed.
		\throw GeneralException 
	*/
	void removeValue(typename std::vector<T>::iterator pos);

	//! Set the name of the class.

	/*! This function allows you to specify a name for the class.
		\param name The name string to assign to the class.
		\throw GeneralException 
	*/
	void setName(const char* name);

	//! Set the initial capacity of the class.

	/*! This function sets the initial capacity of the class.
		This value is used by the classification algorithm
		to determine the initial distribution of values before
		the actual algorithm is executed.
		\param newSize The initial number of values to be stored in this class.
	*/
	void setInitialCapacity(int newSize);

	//! Set the color value for the class.
	
	/*! This function sets the color value for the class.
		Currently this is not used by anything, it only exists
		for future uses.
		\param color The new color for the class.
	*/
	void setColor(ClassColor color);

	//! Set the color value for the class.

	/*! This function sets the color value for the class.
		Currently this is not used by anything, it only exists
		for future uses.
		\param r The red value.
		\param g The green value.
		\param b The blue value.
	*/
	void setColor(unsigned int r, unsigned int g, unsigned int b);

	//! Sort the class values in ascending order.
	void sort();

	//! Reset all class values.
	void clear(); 

	//! Set special textual information for the class.

	/*! This function sets special textual information that
		the user might want to include about this class, such
		as what it represents.
		\param info The string containing special information about the class.
		\throw GeneralException 
	*/
	void setInfo(const char* info);

	//! Get the current color of the class.
	ClassColor getColor() const;

	//! Get the mean value of the class.

	/*! Returns the mean value of the class.
		If the user passes true to this function, the
		mean will be recalculated, otherwise the stored value
		will be returned.
		\param recalc Tells the function whether or not to recalculate the mean.
	*/
	double getMean(bool recalc = false);

	//! Get the current error value for the class.

	/*! Returns the error value of the class.
		If the user passes true to this function, the
		error will be recalculated, otherwise the stored value
		will be returned.
		\param recalc Tells the function whether or not to recalculate the mean.
	*/
	double getError(bool recalc = false);

	//! Get the number of values in the class.
	int getNumValues() const;

	//! Get the name of the class, NULL if not set.
	char* getName() const;

	//!Get the initial capacity of the class.
	int getInitialCapacity() const;

	//! Print class values to stdout.
	void printValues() const;

	//! Get the range for the class.
	double getRange();

	//!Get the textual information for the class.
	const char* getInfo() const;

	//! Returns a particular pixel value for the class.

	/*! This operator returns the value located at the index
		passed in.
		\param index The zero-based index of the value.
		\throw GeneralException
	*/
	T operator[](unsigned int index);
    
	//! Assignment operator.

	/*! \param right The ImageClass object to assign values from.
	*/
	ImageClass<T>& operator=(const ImageClass<T>& right);
	
	//! Increase the initial capacity of the class.

	/*! The purpose of this operator is to increase the 
		maximum initial capaciy of the class. This is by ImageClassifier
		when doing the initial distribution of pixel values among the
		classes before the actual classification algorithm is run.	
	*/
	void operator++(int);
	
	
private:
	//! Vector of class values.
	std::vector<T> m_values; 

	//! Color setting for class.
	ClassColor m_color;

	//! Initial Class Capacity
	int m_initialCapacity;

	//! Name of the class.
	char* m_name;

	//! Any special info about the class that the user wants to add.
	char* m_info; 

	//! Holds the current mean value of the class.
	double m_mean;

	//! Holds the current error value for the class.
	double m_error;
	
	//! Calculates the mean of the class.
	void calcMean();

	//! Calculates the error of the class.
	void calcError();
};

//Default constructor
template <class T>
ImageClass<T>::ImageClass(): m_color(ClassColor(0xff, 0xff, 0xff)) 
							,m_initialCapacity(0)
							,m_name(NULL)
							,m_info(NULL)
							,m_mean(0)
							,m_error(0)
{
      m_values.clear();
}

//Non Default Constructor
template <class T>
ImageClass<T>::ImageClass(const char* name, int capacity): 
							 m_color(ClassColor(0xff, 0xff, 0xff))
							,m_initialCapacity(capacity)
							,m_name(NULL)
							,m_info(NULL)
							,m_mean(0)
							,m_error(0)
{
	m_values.clear();
	if(name) {
		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);
		m_name[strlen(name)] = '\0';
	}
	
}

//Copy Constructor
template <class T>
ImageClass<T>::ImageClass(const ImageClass<T>& ic) {
	m_values = ic.m_values;
	m_color = ic.m_color;
	m_mean = ic.m_mean;
	m_error = ic.m_error;
	if(ic.m_name) {
		m_name = new char[strlen(ic.m_name)+1];
		strcpy(m_name, ic.m_name);
		m_name[strlen(ic.m_name)] = '\0';
	}

	else
		m_name = NULL;

	if(ic.m_info) {
		m_info = new char[strlen(ic.m_info)+1];
		strcpy(m_info, ic.m_info);
		m_info[strlen(ic.m_info)] = '\0';
	}

	else 
		m_info = NULL;
}

//Destructor
template <class T>
ImageClass<T>::~ImageClass() {
	if(m_name) 
		delete[] m_name;		
	
	if(m_info) 
		delete[] m_info;
}

//Index operator
template <class T>
T ImageClass<T>::operator[](unsigned int index) {
	if(index < m_values.size()) 
		return(m_values[index]);
	else
		throw(GeneralException("Error on ImageClass::operator[], index out of bounds"));
}	

//Assignment operator
template <class T>
ImageClass<T>& ImageClass<T>::operator=(const ImageClass<T>& right) {
	if(this != &right) {
		m_values = right.m_values;
		m_color = right.m_color;
		if(m_name) {
	        delete[] m_name;
	        m_name = NULL;
		}
		
		if(m_info) {
			delete[] m_info;
			m_info = NULL;
		}

		if(right.m_name) {
			 m_name = new char[strlen(right.m_name)+1];
			 strcpy(m_name, right.m_name);
		 	 m_name[strlen(right.m_name)] = '\0';
		}

		if(right.m_info) {
			 m_info = new char[strlen(right.m_info)+1];
			 strcpy(m_info, right.m_info);
		 	 m_info[strlen(right.m_info)] = '\0';
		}
	 
	}
	return(*this);
}

//Increment Operator
template <class T>
void ImageClass<T>::operator++(int) {
	m_initialCapacity++;
}

//Set initial class capacity
template <class T>
void ImageClass<T>::setInitialCapacity(int newSize) {
	m_initialCapacity = newSize;
}

//Get initial class capacity
template <class T>
int ImageClass<T>::getInitialCapacity() const {
	return(m_initialCapacity);
}

//Get class color 
template <class T>
ClassColor ImageClass<T>::getColor() const {
	return(m_color);
}

//Reset class values
template <class T>
void ImageClass<T>::clear() {
	m_values.clear();
	m_color.setColor(0xff, 0xff, 0xff);
	m_initialCapacity = 0;
	if(m_name) 
		delete[] m_name;
	if(m_info)
		delete[] m_info;
	m_name = NULL;
	m_info = NULL;
}

//Remove value from class at numeric index.
template <class T>
void ImageClass<T>::removeValue(int index) {
	if(index < 0)
		throw(GeneralException("Error on ImageClass::removeValue(): index out of bounds "));
				
	typename std::vector<T>::iterator iter = m_values.begin();
        iter += index;

	if(iter != m_values.end()) 
		m_values.erase(iter);

	else
		throw(GeneralException("Error on ImageClass::removeValue(): index out of bounds "));
	
}

//Remove value from class at iterator position.
template <class T>
void ImageClass<T>::removeValue(typename std::vector<T>::iterator iter) {
	if(iter != m_values.end()) 
		m_values.erase(iter);
	
	else
		throw(GeneralException("Error on ImageClass::removeValue(): iterator out of bounds "));
}

//Add value to class.
template <class T>
void ImageClass<T>::addValue(T newVal, int position) {

	typename std::vector<T>::iterator iter;
	iter = std::find(m_values.begin(), m_values.end(), newVal);
	if(iter != m_values.end())
		throw(GeneralException("Error on add value: value allready present in class"));

	if(position == BACK)
		m_values.push_back(newVal);
	else 
		m_values.insert(m_values.begin(), newVal);
}

//Set the name of the class.
template <class T>
void ImageClass<T>::setName(const char* name) {
	if(name) {
		if(m_name) 
			delete[] m_name;
		
		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);
		m_name[strlen(name)] = '\0';
	}
	else 
		throw(GeneralException("Error on set name: attempting to set to a null value"));
	
}

//Set the color of the class with a ClassColor object
template <class T>
void ImageClass<T>::setColor(ClassColor color) {
	m_color = color;
}

//Set the color of the class with r,g,b values.
template <class T>
void ImageClass<T>::setColor(unsigned int r, unsigned int g, unsigned int b) {
	m_color.setColor(r, g, b);
}

//Set special info for class.
template <class T>
void ImageClass<T>::setInfo(const char* info) {
	if(!info)
		throw(GeneralException("Error on setInfo(): setting a null value."));

	if(m_info)
		delete[] m_info;

	m_info = new char[strlen(info)+1];
	strcpy(m_info, info);
	m_info[strlen(info)] = '\0';
}

//Get special info for class.
template <class T>
const char* ImageClass<T>::getInfo() const{
	return(m_info);
}

//Calculate class mean.
template <class T>
double ImageClass<T>::getMean(bool recalc) {
	if(recalc)
		calcMean();

	return(m_mean);
}

template <class T>
void ImageClass<T>::calcMean() {
	int numVals = m_values.size();
	double sum = 0;
	for(int i = 0; i < numVals; i++) 
		sum += m_values[i];
		
	m_mean = (double)(sum / numVals);
}

//Get the number of class values.
template <class T>
int ImageClass<T>::getNumValues() const{
	return(m_values.size());
}

//Get the name of the class.
template <class T>
char* ImageClass<T>::getName()  const{
	return(m_name);
}

//Sort the class values in ascending order.
template <class T>
void ImageClass<T>::sort() {
	std::sort(m_values.begin(), m_values.end());
}

//get the class error.
template <class T>
double ImageClass<T>::getError(bool recalc) {
	if(recalc)
		calcError();

	return(m_error);
}

template <class T>
void ImageClass<T>::calcError() {
	double mean = m_mean;
	double error = 0;
	int numValues = m_values.size();
	
	//error is a sum of the aboslute deviation from the mean
	//for each value
	for(int i = 0; i < numValues; i++) 
		error += fabs(m_values[i] - mean);
	
	m_error = error;
}


//calculate the range for the class.
template <class T>
double ImageClass<T>::getRange() {
	if(m_values.empty())
		return(0);
	
	else if(m_values.size() == 1)
		return(m_values[0]);

	else {
		std::sort(m_values.begin(), m_values.end());
		return((double)m_values[m_values.size()-1] - m_values[0]);
	}
}



//print class values to stdout.
template <class T>
void ImageClass<T>::printValues() const {
	printf("Values: ");
	for(int i = 0; i < m_values.size(); i++)
		printf("%f,", m_values[i]);

	printf("\n");
}
#endif

