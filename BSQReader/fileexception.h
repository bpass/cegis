#ifndef FILE_EX_H
#define FILE_EX_H

#include <string>

//This exception class handles file-related errors
//such as failure to open or read.
class FileException 
{
public:
	FileException(int line, const std::string& file) 
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