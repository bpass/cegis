#include "stdafx.h"
#include "GeneralException.h"
#include <iostream>
#include <string.h>
using namespace std;

GeneralException::GeneralException(const char* message) {
	

	for(int i = 0; i < 300; i++)
		m_message[i] = '\0';
	
	if(message) {		
		for(unsigned int i = 0; (i < 299 && i < strlen(message)); i++)
			m_message[i] = message[i];
	}
}

void GeneralException::printMessage() {
	cout << m_message << endl;
}

const char* GeneralException::getMessage() {
	return m_message;
}
	