
#ifndef BSQREAD_H
#define BSQREAD_H

#include <string>
#include <fstream>
#include "pixel.h"
#include "imageinfo.h"
#include "fileexception.h"

template <class T>
class BSQReader 
{
public:

	BSQReader() {}
			
	BSQReader(std::string filename, const ImageInfo& fileInfo)
	{
		m_imageInfo = fileInfo;
		openFile(filename);
	}
	
	~BSQReader() 
	{
		m_fileStream.close();
	}

	void setFileInfo(const ImageInfo& info)
	{
		m_imageFileInfo = info;
	}

	void setFileInfo(const std::string& image, const std::string& header) 
	{
		buildFileInfo(image, header);
	}

	Pixel<T> getPixel(const Point& p, const int layer);

	ImageInfo getFileInfo() {return m_imageInfo;}

private:

	//data members
	ImageInfo m_imageInfo;
	std::ifstream m_fileStream;

	//open the image file, throw exception if an error occurs
	void openFile(const std::string filename)
	{
		m_fileStream.open(filename.c_str(), ios::binary);
		if(m_fileStream.fail())
			throw FileException(__LINE__, __FILE__);
	}
};

#include "bsqreader.hpp"

#endif