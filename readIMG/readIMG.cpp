
#include "readIMG.h"
#include <stdio.h>
#include <stdlib.h>

HFAImage::HFAImage(const char* filename) {
	m_dictPtr = 0;
	m_currentGrid = NULL;
	m_gridUlx = -1;
	m_gridUly = -1;
	getDictString(filename);
	
	m_dict = new Dictionary(m_dictString);
	
}

HFAImage::~HFAImage() {
	if(m_file)
		fclose(m_file);
	m_file=NULL;
}
void HFAImage::getDictString(const char* filename) {
	FILE* imageFile = fopen(filename, "rb");

	std::string dictString("");
	char curChar = '\0';
	char header_tag[17] = {'\0'};
	unsigned long headerPtr = 0;
	unsigned long dictPtr = 0;
	if(imageFile) {
		if(!fread((void*)header_tag, 16, 1, imageFile))
			printf("Error reading header tag\n");
		if(!fread((void*)&headerPtr, 4, 1, imageFile))
			printf("Error reading header pointer\n");
		if(fseek(imageFile, headerPtr+14, SEEK_SET))
			printf("Error seeking to header\n");
		if(!fread((void*)&dictPtr, 4, 1, imageFile))
			printf("Error reading dictionary pointer\n");
	}
	

	if(imageFile) {
		fseek(imageFile, dictPtr, SEEK_SET);
		while(fread((void*)&curChar, 1, 1, imageFile)) 
			dictString += curChar;
	}
	m_file = imageFile;
	m_dictPtr = dictPtr;
	m_dictString = dictString;
	
}

void HFAImage::printDictionaryString() {
	printf("Dictionary:\n%s", m_dictString.c_str());
}

void HFAImage::displayDictionary() {
	if(m_dict)
		m_dict->display();
}
