# ifndef TOUCHPOINT_H
# define TOUCHPOINT_H

# include "types.h"

class TouchPoint
{	public:

	TouchPoint():numValues(0)
	{	OneFlow = 0;
		TwoFlow = 0;
		ThreeFlow = 0;
		FourFlow = 0;
	};
	TouchPoint( const TouchPoint & tp );
	void printVals();
	void eraseVal( int val, int nodata_value );
	__int16 value[MAXTOUCHING];	
	__int8 numValues;

	unsigned char OneFlow;
	unsigned char TwoFlow;
	unsigned char ThreeFlow;
	unsigned char FourFlow;
};

struct TouchList
{	TouchPoint * touchP;
};

struct myPoint
{	float X;
	float Y;
	bool isJoint;
};

# endif

