
#ifndef READ_IMG_H
#define READ_IMG_H
#include <stdio.h>
#include "dictionary.h"
class HFAImage {
public:
	HFAImage(const char* filename);
	~HFAImage();
	
	double getDN(int layer, double x, double y);
	void printDictionaryString();
	void displayDictionary();

private:
	FILE* m_file;
	double** m_currentGrid;
	double m_gridUlx;
	double m_gridUly;
	unsigned long m_dictPtr;
	void getDictString(const char* filename);
	std::string m_dictString;
	Dictionary* m_dict;
	
};
#endif