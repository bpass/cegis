#ifndef PARSE_EX_H
#define PARSE_EX_H
#include "exception.h"
//! ParseException class

/*! This class is used to specify errors that occur when
	parsing an Imagine image file dictionary.
*/
class ParseException : public GeneralException {
public:

	//! Construct a ParseException object with a particular error message.

	/*! This constructor allows the user to instantiate a ParseException
		object with a specific error message, and also specify the 
		index into the dictionary string where the parse error occured.
		\param message The error message
		\param dictIndex The index in the dictionary string where the parse error was occurred.
	*/
	ParseException(const char* message, unsigned int dictIndex = 0):
					GeneralException(message)
					,m_index(dictIndex) 
	{
	}

	//! Print the error message
	void printMessage() {
		printf("%s: dictionary index %d\n", m_message, m_index);
	}

	//! Get the index where the error occured.
	unsigned int getIndex() {
		return(m_index);
	}

	
private:
	unsigned int m_index;
};

#endif