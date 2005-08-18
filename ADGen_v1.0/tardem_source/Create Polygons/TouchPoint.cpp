# include <iostream.h>
# include "TouchPoint.h"

TouchPoint::TouchPoint( const TouchPoint & tp )
{	if( this != NULL )
	{	numValues = tp.numValues;
		for( int i = 0; i < numValues; i++ )
		{	value[i] = tp.value[i];
		}
	}
	OneFlow = tp.OneFlow;
	TwoFlow = tp.TwoFlow;
	ThreeFlow = tp.ThreeFlow;
	FourFlow = tp.FourFlow;
}

void TouchPoint::printVals()
{
	for( int i = 0; i < numValues; i++ )
	{	
		cout<<value[i];
	}
}

void TouchPoint::eraseVal( int val, int nodata_value )
{	for( int i = 0; i < numValues; i++ )
	{	if( value[i] == val )
		{	numValues--;
			value[i] = value[ numValues ];
			value[ numValues ] = nodata_value;
			return;
		}
	}
}
