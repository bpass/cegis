
#ifndef EXCEPTION_H
#define EXCEPTION_H

class GeneralException {
public:	
	GeneralException(const char* message);
	void printMessage();
	const char* getMessage();
private:
	char m_message[300];
};

#endif

	