#ifndef COMMON_H
#define COMMON_H

struct ImageParameters
 {
	unsigned long width; //width of the image in pixels
	unsigned long height; //height of the image in pixels
	double ulx; //upper-left x coordinate (in UTM)
	double uly; //upper-left y coordinate (in UTM)
	float pixelWidth; //width of the pixels
	float pixelHeight; //height of the pixels
	unsigned int numLayers; //number of layers in the image file (must be a BSQ file)
}; 
struct Point{
	double x; //x coordinate of the point (in UTM)
	double y; //y coordinate of the point (in UTM)
};

#endif