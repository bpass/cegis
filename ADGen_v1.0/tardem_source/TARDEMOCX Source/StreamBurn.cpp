# include "stdafx.h"
# include <iostream>
# include <fstream>
# include "StreamBurn.h"
# define MAXBUFFER 255

enum DIRECTION { NONE, RIGHT, RIGHTUP, UP, LEFTUP, LEFT, LEFTDOWN, DOWN, RIGHTDOWN };
enum EVENT_ID { READING = 1, CALCULATING = 2, WRITING = 3, ANYERROR = -999 };

StreamBurn::StreamBurn( CTardemCtrl * p_ocx )
{	ocx = p_ocx;
}

StreamBurn::~StreamBurn()
{	if( flow != NULL )
		delete flow;
	if( dem != NULL )
		delete dem;
}

void StreamBurn::burnExplore( int x, int y, int height )
{	if( x < 0 || y < 0 )
		return;
	else if( x >= numberCols || y >= numberRows )
		return;
	
	if( height == nodata_value )
		height = dem[indexFlow( x, y )];
	else if( dem[ indexFlow( x, y ) ] < height )
		height = dem[indexFlow( x, y )];
	else
		dem[indexFlow( x, y )] = height;

	if( flow[indexFlow( x, y )] == RIGHT )
		burnExplore( x + 1, y, height );
	else if( flow[indexFlow( x, y )] == LEFT )
		burnExplore( x - 1, y, height );
	else if( flow[indexFlow( x, y )] == UP )
		burnExplore( x, y - 1, height );
	else if( flow[indexFlow( x, y )] == DOWN )
		burnExplore( x, y + 1, height );
	else if( flow[indexFlow( x, y )] == RIGHTUP )
		burnExplore( x + 1, y - 1, height );
	else if( flow[indexFlow( x, y )] == RIGHTDOWN )
		burnExplore( x + 1, y + 1, height );
	else if( flow[indexFlow( x, y )] == LEFTUP )
		burnExplore( x - 1, y - 1, height );
	else if( flow[indexFlow( x, y )] == LEFTDOWN )
		burnExplore( x - 1, y + 1, height );

	flow[indexFlow( x, y ) ] = nodata_value;
}

bool StreamBurn::isHead( int x, int y )
{	if( x < 0 || x >= numberCols )
		return false;
	if( y < 0 || y >= numberRows )
		return false;
	if( flow[ indexFlow( x, y ) ] == nodata_value )
		return false;
	
	if( x - 1 >= 0 && flow[ indexFlow( x - 1, y ) ] == RIGHT )
		return false;
	else if( x + 1 < numberCols && flow[ indexFlow( x + 1, y ) ] == LEFT )
		return false;
	else if( y - 1 >= 0 && flow[ indexFlow( x, y - 1 ) ] == DOWN )
		return false;
	else if( y + 1 < numberRows && flow[ indexFlow( x, y + 1 ) ] == UP )
		return false;
	else if( x - 1 >=0 && y - 1 >= 0 && flow[ indexFlow( x - 1, y - 1 ) ] == RIGHTDOWN )
		return false;
	else if( x + 1 < numberCols && y - 1 >= 0 && flow[ indexFlow( x + 1, y - 1 ) ] == LEFTDOWN )
		return false;
	else if( x - 1 >= 0 && y + 1 < numberRows && flow[ indexFlow( x - 1, y + 1 ) ] == RIGHTUP )
		return false;
	else if( x + 1 < numberCols && y + 1 < numberRows && flow[ indexFlow( x + 1, y + 1 ) ] == LEFTUP )
		return false;

	return true;
}

int StreamBurn::indexFlow( int x, int y )
{	return y*numberCols + x;
}

bool StreamBurn::burn( const char * flowFile, const char * demFile, const char * outFile )
{	
	FILE * fileCheck;
	//Check to see if outFile exists
	//Delete them if they do
	fileCheck = fopen( outFile, "r" );
	if( fileCheck != NULL )
	{	fclose( fileCheck );
		if( _unlink( outFile ) )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete " );		
			strcat( message, outFile );
			ocx->BurnEvent( 0, ANYERROR, message);
			return false;
		}
	}

	std::ifstream flowIn( flowFile );
	std::ifstream demIn( demFile );

	if( !flowIn )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Could Find File " );		
		strcat( message, flowFile );
		ocx->BurnEvent( 0, ANYERROR, message);
		return false;
	}
	if( !demIn )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Could Find File " );		
		strcat( message, demFile );
		ocx->BurnEvent( 0, ANYERROR, message);
		return false;
	}

	//Read DEM Header
	int number_of_variables = 6;
	char * variable_name = new char[MAXBUFFER];
	bool shiftUp = false;
	int dncols = 0, dnrows = 0, dnodata_value = 0;
	double dxllcenter = 0, dyllcenter = 0, dcellsize = 0;
	for( int i = 0; i < number_of_variables; i++ )
	{	demIn>>variable_name;
				
		if( !strcmp( variable_name, "ncols" ) )
		{	demIn>>dncols;
		}
		else if( !strcmp( variable_name, "nrows" ) )
		{	demIn>>dnrows;
		}
		else if( !strcmp( variable_name, "xllcorner" ) )
		{	demIn>>dxllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "yllcorner" ) )
		{	demIn>>dyllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "xllcenter" ) )
		{	demIn>>dxllcenter;
		}
		else if( !strcmp( variable_name, "yllcenter" ) )
		{	demIn>>dyllcenter;
		}
		else if( !strcmp( variable_name, "cellsize" ) )
		{	demIn>>dcellsize;
		}
		else if( !strcmp( variable_name, "nodata_value" ) )
		{	demIn>>dnodata_value;		
		}
		else if( !strcmp( variable_name, "NODATA_value" ) )
		{	demIn>>dnodata_value;
		}
	}
	if( shiftUp )
	{	dxllcenter += dcellsize/2;
		dyllcenter += dcellsize/2;
	}	

	//Read FLOW Header
	shiftUp = false;
	int fncols = 0, fnrows = 0, fnodata_value = 0;
	double fxllcenter = 0, fyllcenter = 0, fcellsize = 0;
	for( int j = 0; j < number_of_variables; j++ )
	{	flowIn>>variable_name;
				
		if( !strcmp( variable_name, "ncols" ) )
		{	flowIn>>fncols;
		}
		else if( !strcmp( variable_name, "nrows" ) )
		{	flowIn>>fnrows;
		}
		else if( !strcmp( variable_name, "xllcorner" ) )
		{	flowIn>>fxllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "yllcorner" ) )
		{	flowIn>>fyllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "xllcenter" ) )
		{	flowIn>>fxllcenter;
		}
		else if( !strcmp( variable_name, "yllcenter" ) )
		{	flowIn>>fyllcenter;
		}
		else if( !strcmp( variable_name, "cellsize" ) )
		{	flowIn>>fcellsize;
		}
		else if( !strcmp( variable_name, "nodata_value" ) )
		{	flowIn>>fnodata_value;		
		}
		else if( !strcmp( variable_name, "NODATA_value" ) )
		{	flowIn>>fnodata_value;
		}
	}
	if( shiftUp )
	{	fxllcenter += fcellsize/2;
		fyllcenter += fcellsize/2;
	}	

	//Check for header Discrepancy
	bool discrep = false;
	if( fncols != dncols )
		discrep = true;
	if( fnrows != dnrows )
		discrep = true;
	if( fxllcenter != dxllcenter )
		discrep = true;
	if( fyllcenter != dyllcenter )
		discrep = true;
	if( fcellsize != dcellsize )
		discrep = true;
	if( fnodata_value != dnodata_value )
		discrep = true;

	if( discrep )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Header Discrepancy in " );		
		strcat( message, demFile );
		strcat( message, " and " );
		strcat( message, flowFile );
		ocx->BurnEvent( 0, ANYERROR, message);
		return false;
	}

	numberRows = fnrows;
	numberCols = fncols;
	xllcenter = fxllcenter;
	yllcenter = fyllcenter;
	cellsize = fcellsize;
	nodata_value = fnodata_value;


	flow = new int[ numberRows * numberCols ];
	dem = new int[ numberRows * numberCols ];


	int percent = 0;
	int total = numberRows*numberCols;
	for( int yf = 0; yf < numberRows; yf++ )
	{	for( int xf = 0; xf < numberCols; xf++ )
		{	//Read in Original DEM && ASCII Flow
			flowIn>>flow[ indexFlow( xf, yf ) ];
			demIn>>dem[ indexFlow( xf, yf ) ];

			int newpercent = ((double)yf*xf)/total*100;
			if( newpercent > percent )
			{	percent = newpercent;
				ocx->BurnEvent( percent, READING, "Reading Files" );
			}
		}
	}

	percent = 0;
	for( int yd = 0; yd < numberRows; yd++ )
	{	for( int xd = 0; xd < numberCols; xd++ )
		{	if( isHead( xd, yd ) )
				burnExplore( xd, yd, dem[ indexFlow( xd, yd ) ] );				

			int newpercent = ((double)yd*xd)/total*100;
			if( newpercent > percent )
			{	percent = newpercent;
				ocx->BurnEvent( percent, CALCULATING, "Burning Streams" );
			}
		}
	}

	writeBurn( outFile );
	return true;
}

void StreamBurn::writeBurn( const char * outFile )
{	
	std::ofstream out( outFile );
	out<<"ncols "<<numberCols<<std::endl;
	out<<"nrows "<<numberRows<<std::endl;
	out<<"xllcenter "<<xllcenter<<std::endl;
	out<<"yllcenter "<<yllcenter<<std::endl;
	out<<"cellsize "<<cellsize<<std::endl;
	out<<"nodata_value "<<nodata_value<<std::endl;

	int total = numberRows * numberCols;
	int percent = 0;

	for( int y = 0; y < numberRows; y++ )
	{	for( int x = 0; x < numberCols; x++ )
		{	out<<dem[ indexFlow( x, y ) ]<<" ";

			int newpercent = ((double)y*x)/total*100;
			if( newpercent > percent )
			{	percent = newpercent;
				ocx->BurnEvent( percent, WRITING, "Writing Files" );
			}
		}
		out<<std::endl;
	}
	out.close();
}