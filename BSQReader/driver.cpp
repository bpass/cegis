#include <iostream>
#include "stdafx.h"
#include "bsqreader.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	ImageInfo testInfo(FLOAT32, 1155, 1274, 5);
	BSQReader<float> reader("test.bsq", testInfo);
	Point p;
	Pixel<float> pixel;

	for(int l = 0; l < testInfo.numLayers(); l++)
	{
		for(int i = 0; i < testInfo.height(); i++)
		{
			for(int j = 0; j < testInfo.width(); j++)
			{
				p.setX(j);
				p.setY(i);
				pixel = reader.getPixel(p, l);
				cout << "[" << pixel.getValue() << "] ";
			}
			cout << "\n";
		}
	}

	return(0);
							
}	