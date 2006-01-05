#ifndef NULL_POINT_EX_H
#define NULL_POINT_EX_H

#include <string>

class NullPointerException
{
public:

	NullPointerException(int line, const std::string& file)
	{
		char lineString[20] = {'\0'};
		sprintf(lineString, "%d", line);

		m_location = "NullPointerException at line ";
		m_location += lineString;
		m_location += " in " + file;
	}
		
	std::string location() { return(m_location); }

private:

	std::string m_location;
};

#endif