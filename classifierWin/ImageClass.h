#ifndef IMG_CLASS_H
#define IMG_CLASS_H
#include "GeneralException.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "ClassColor.h"
enum {FRONT=0, BACK};
template <class T>
class ImageClass {
public:	
	ImageClass();
	ImageClass(const char* name, int capacity);
	ImageClass(const ImageClass& ic);
	~ImageClass();
	
	//operators
	T operator[](unsigned int index);
    ImageClass<T>& operator=(const ImageClass<T>& right);
	void operator++(int);
	
	//mutators
	void addValue(T newVal, int position = BACK);
	bool removeValue(int index);
	bool removeValue(typename std::vector<T>::iterator pos);
	void setName(const char* name);
	void setCapacity(int newSize);
	void setColor(ClassColor color);
	void setColor(unsigned int r, unsigned int g, unsigned int b);
	void sort();
	void clear(); //clear current class values and name
	void setInfo(const char* info);

	//const functions
	ClassColor getColor() const;
	double getMean() const;
	double getError() const;
	int getNumValues() const;
	char* getName() const;
	int getCapacity() const;
	void printValues() const;
	T getRange() const;
	const char* getInfo() const;

private:
	std::vector<T> m_values;
	ClassColor m_color;
	int m_capacity;
	char* m_name;
	char* m_info; //Any special info about the class that the user wants 
				  //to add.
};

template <class T>
ImageClass<T>::ImageClass(): m_name(NULL),m_info(NULL), m_capacity(0) {
      m_values.clear();
	  m_color.setColor(0xff, 0xff, 0xff);
}

	
template <class T>
ImageClass<T>::ImageClass(const char* name, int capacity): m_capacity(capacity), m_info(NULL) {
	m_values.clear();
	if(name) {
		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);
		m_name[strlen(name)] = '\0';
	}
	else
		m_name = NULL;
}
template <class T>
ImageClass<T>::ImageClass(const ImageClass<T>& ic) {
	m_values = ic.m_values;
	
	
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

template <class T>
ImageClass<T>::~ImageClass() {
	if(m_name) 
		delete[] m_name;		
	
	if(m_info) 
		delete[] m_info;
}

template <class T>
T ImageClass<T>::operator[](unsigned int index) {
	if(index < m_values.size()) 
		return(m_values[index]);
	else
		throw(GeneralException("Error on ImageClass::operator[], index out of bounds"));
}	

template <class T>
ImageClass<T>& ImageClass<T>::operator=(const ImageClass<T>& right) {
	 m_values = right.m_values;
	if(this != &right) {
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

template <class T>
void ImageClass<T>::operator++(int) {
	m_capacity++;
}

template <class T>
void ImageClass<T>::setCapacity(int newSize) {
	m_capacity = newSize;
}

template <class T>
int ImageClass<T>::getCapacity() const {
	return(m_capacity);
}

template <class T>
ClassColor ImageClass<T>::getColor() const {
	return(m_color);
}
template <class T>
void ImageClass<T>::clear() {
	m_values.clear();
	if(m_name) {
		delete[] m_name;
		m_name = NULL;
	}

	else
		m_name = NULL;

}

template <class T>
bool ImageClass<T>::removeValue(int index) {
	if(index < 0)
		throw(GeneralException("Error on ImageClass::removeValue(int index) --> index < 0"));
				
	typename std::vector<T>::iterator iter = m_values.begin();
        iter += index;

	if(iter != m_values.end()) {
		m_values.erase(iter);
		return(true);
	}
	
	else
		return false;
}

template <class T>
bool ImageClass<T>::removeValue(typename std::vector<T>::iterator iter) {
	if(iter != m_values.end()) {
		m_values.erase(iter);
		return(true);
	}

	else
		return(false);
}

template <class T>
void ImageClass<T>::addValue(T newVal, int position) {

	//normally I would be using std::find() to speed up
	//the search process here, but it was seg-faulting
	//for some weird reason.
	for(unsigned int i = 0; i < m_values.size(); i++) {
		if(m_values[i] == newVal) 
			throw(GeneralException("Error on add value: value allready present in class"));
	}
	
	if(position == BACK)
		m_values.push_back(newVal);
	else 
		m_values.insert(m_values.begin(), newVal);
		
 	return;
}

template <class T>
void ImageClass<T>::setName(const char* name) {
	if(name) {
		if(m_name) 
			delete[] m_name;
		
		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);
		m_name[strlen(name)] = '\0';
	}
	else {
		m_name = NULL;
		throw(GeneralException("Error on set name: attempting to set to a null value"));
	}
}

template <class T>
void ImageClass<T>::setColor(ClassColor color) {
	m_color = color;
}

template <class T>
void ImageClass<T>::setColor(unsigned int r, unsigned int g, unsigned int b) {
	m_color.setColor(r, g, b);
}

template <class T>
void ImageClass<T>::setInfo(const char* info) {
	if(m_info)
		delete[] m_info;

	m_info = new char[strlen(info)+1];
	strcpy(m_info, info);
	m_info[strlen(info)] = '\0';
}

template <class T>
const char* ImageClass<T>::getInfo() const{
	return(m_info);
}

template <class T>
double ImageClass<T>::getMean() const{
	int numVals = m_values.size();
	T sum = 0;
	for(int i = 0; i < numVals; i++) 
		sum += m_values[i];
		
	return((double)(sum / numVals));
}


template <class T>
int ImageClass<T>::getNumValues() const{
	return(m_values.size());
}

template <class T>
char* ImageClass<T>::getName()  const{
	return(m_name);
}

template <class T>
void ImageClass<T>::sort() {
	std::sort(m_values.begin(), m_values.end());
}

template <class T>
double ImageClass<T>::getError() const {
	double mean = getMean();
	double error = 0;
	int numValues = m_values.size();
	
	//error is a sum of the aboslute deviation from the mean
	//for each value
	for(int i = 0; i < numValues; i++) 
		error += fabs(m_values[i] - mean);
	
	return(error);
}

template <class T>
T ImageClass<T>::getRange() const{
	
	if(m_values.empty())
		return(0);
	
	else if(m_values.size() == 1)
		return(m_values[0]);

	else {
		std::sort(m_values.begin(), m_values.end());
		return(m_values[m_values.size()-1] - m_values[0]);
	}
	
}

template <class T>
void ImageClass<T>::printValues() const{
	printf("Values: ");
	for(int i = 0; i < m_values.size(); i++)
		printf("%f,", m_values[i]);

	printf("\n");
}
#endif

