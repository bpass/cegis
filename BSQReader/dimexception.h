
#ifndef DIM_EX_H
#define DIM_EX_H

#include <string>

//This exception class handles image dimension errors, 
//like a negative width for example.
class DimException 
{
public:
	DimException(int line, const std::string& file) 
	{
		char temp[20] = {'\0'};
		sprintf(temp, "%d", line);
		m_location = "DimException occured at line ";
		m_location += line;
		m_location += " in file " + file + "\n";
	}

	std::string location() {return m_location;}
	
private:
	std::string m_location;
};

#endif