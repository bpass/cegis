#ifndef CLASS_COLOR_H
#define CLASS_COLOR_H

//! This class stores RGB color values for use in ImageClass objects.

/*! This class allows the user to store RGB color values to associate
	with a particular ImageClass object.
*/
class ClassColor {
public:
	//! Default constructor.
	ClassColor();

	//! Instantiate ClassColor object with RGB values
	
	/*! This constructor allows the user to instantiate a new 
		ClassColor object while passing it its inital RGB values.
		\param r The red value.
		\param g The green value.
		\param b The blue value.
	*/
	ClassColor(unsigned int r, unsigned int g, unsigned int b);
	
	//! Set the color values.

	/*! This function allows the user to set the current color values.
		\param r The red value.
		\param g The green value.
		\param b The blue value.
	*/
	void setColor(unsigned int r, unsigned int g, unsigned int b);

	//! Get the current red value.
	unsigned int getRed();

	//!Get the current green value.
	unsigned int getGreen();

	//!Get the current blue value
	unsigned int getBlue();
private:

	//! The red value.
	unsigned int m_r;

	//! The green value
	unsigned int m_g;

	//! The blue value
	unsigned int m_b;
};

#endif
