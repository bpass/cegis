
#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point() : m_x(0), m_y(0) {}

	Point(long _x, long _y) : m_x(_x), m_y(_y) {}
	
	long x() const {return m_x;} 
	
	long y() const {return m_y;}
	
	void setX(long x) {m_x = x;}
	
	void setY(long y) {m_y = y;}

private:
	long m_x;
	long m_y;
};

#endif



	