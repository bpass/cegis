#ifndef EXCEP_H
#define EXCEP_H

#include <string>
class GeneralException {
public:
	GeneralException(const std::string& message);

	void printMessage();
	std::string getMessage();
private:
	std::string m_message;
};

#endif