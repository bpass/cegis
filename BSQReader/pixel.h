#ifndef PIXEL_H
#define PIXEL_H
#include "point.h"

template <class T>
class Pixel 
{
public:

	Pixel() : m_value(0) {}
	Pixel(const Point& coord, T value) : m_coord(coord), m_value(value) {}

	Point getCoord() const {return m_coord;}
	T getValue() const {return m_value;}
	
	void setValue(T value) {m_value = value;}
	void setCoord(const Point& p) {m_coord = p;}
private:
	T m_value; //the pixel value;
	Point m_coord; //Location of the center of the pixel
};

#endif

