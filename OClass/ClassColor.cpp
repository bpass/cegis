#include "stdafx.h"
#include "ClassColor.h"

ClassColor::ClassColor(): m_r(0x00), m_g(0x00), m_b(0x00)
{
}

ClassColor::ClassColor(unsigned int r, unsigned int g, unsigned int b):
	m_r(r), m_g(g), m_b(b)
{
}

void ClassColor::setColor(unsigned int r, unsigned int g, unsigned int b) {
	m_r = r;
	m_g = g;
	m_b = b;
}

unsigned int ClassColor::getRed() {
	return(m_r);
}

unsigned int ClassColor::getGreen() {
	return(m_g);
}

unsigned int ClassColor::getBlue() {
	return(m_b);
}


