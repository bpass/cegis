#ifndef CLASS_COLOR_H
#define CLASS_COLOR_H

class ClassColor {
public:
	ClassColor();
	ClassColor(unsigned int r, unsigned int g, unsigned int b);
	void setColor(unsigned int r, unsigned int g, unsigned int b);
	unsigned int getRed();
	unsigned int getGreen();
	unsigned int getBlue();
private:
	unsigned int m_r;
	unsigned int m_g;
	unsigned int m_b;
};

#endif
