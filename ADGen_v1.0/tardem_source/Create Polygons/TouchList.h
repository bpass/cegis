# include "types.h"
# ifndef TOUCHLIST_H
# define TOUCHLIST_H

//List of Polygon Indexes touching a Point
//This is just a data-holding class

/*
class TouchList
{	
	public:

	TouchList()
		:isBoundary(false),isTrouble(false),isJoint(false),number_touchPoints(0),direction(NONE){}
	TouchList( const TouchList & tl )
	{	for( int i = 0; i < tl.number_touchPoints; i++ )
			touchPoints[i] = tl.touchPoints[i];
		number_touchPoints = tl.number_touchPoints;
		isBoundary = tl.isBoundary;
		isTrouble = tl.isTrouble;
		isJoint = tl.isJoint;
		direction = tl.direction;
	}
	~TouchList()
	{	delete [] touchPoints;
	}
	int touchPoints[maxTouching];
	int number_touchPoints;
	bool isBoundary;
	bool isTrouble;
	bool isJoint;
	DIRECTION direction;
};
*/

# endif