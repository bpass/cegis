
#ifndef EXCEPTION_H
#define EXCEPTION_H

//! General exception object.

/*! This exception object is very simple, it only
	stores a single error message, and provides functions
	to print the message to stdout and to get a pointer
	to the error message.
*/
class GeneralException {
public:

	//! Constructor.

	/*! This constructor instantiates a GeneralException object
		with the passed in error string.
		\param message The error message.
	*/
	GeneralException(const char* message);
	
	//! Print the error message to stdout.
	void printMessage();

	//! Get a pointer to the error message.
	const char* getMessage();
private:

	//! The error message
	char m_message[300];
};

#endif

	