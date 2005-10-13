
#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include "dimexception.h"

typedef enum 
{
UNDEFINED = -1,
UNSIGNED_8,
SIGNED_8,
UNSIGNED16,
SIGNED16,
UNSIGNED32,
SIGNED32,
FLOAT32,
FLOAT64
} DataType;


class ImageInfo 
{
public:

	ImageInfo() : m_dType(UNDEFINED), m_width(0), m_height(0), m_numLayers(0) {}

	ImageInfo(DataType dType, 
			    long width, 
				long height, 
				int numLayers) : m_dType(dType), m_width(width), m_height(height),
								 m_numLayers(numLayers) 
	{
		//throw exception for dimensions that
		//don't make sense.
		if(width <= 0 || height <= 0 || numLayers <= 0)
			throw DimException(__LINE__, __FILE__);
		
	}

	void setDataType(DataType dType) {m_dType = dType;}
	void setWidth(int width) {m_width = width;}
	void setHeight(int height) {m_height = height;}
	void setNumLayers(int numLayers) {m_numLayers = numLayers;}

	DataType dataType() const{ return m_dType;}
	long width() const {return m_width;}
	long height() const {return m_height;}
	int numLayers() const {return m_numLayers;}

private:

	DataType m_dType;
	long m_width;
	long m_height;
	int m_numLayers;
};

#endif