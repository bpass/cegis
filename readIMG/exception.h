
#ifndef EXCEPT_H
#define EXCEPT_H

//! General exception class.

/*! This class is used to handle generic exceptions,
	that only require a simple error message. This is 
	the base class for other, more specific types of 
	exceptions.
*/
class GeneralException {

public:

	//! Constructor.

	/*! This constructor allows you to create
		a new GeneralException object with a 
		specific error message.
		\param message The error message.
	*/
	GeneralException(const char* message) {
		int limit = 0;
		for(int i = 0; i < 500; i++)
			m_message[i] = '\0';

		if(strlen(message) < 500) 
			limit = strlen(message);
		else
			limit = 500;

		for(int i = 0; i < limit; i++)
			m_message[i] = message[i];
	}

	//! Print the message to stdout.
	virtual void printMessage() {
		printf("%s\n",m_message);
	}

	//! Get a pointer to the message.
	virtual char* getMessage() {
		return(m_message);
	}

protected:

	//! The error message.
	char m_message[500];

};

#endif