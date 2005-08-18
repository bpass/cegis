# include "TardemCtl.h"

# ifndef STREAMBURN_H
# define STREAMBURN_H

class StreamBurn
{	
	public:
		StreamBurn( CTardemCtrl * p_ocx );
		~StreamBurn();
		void burnExplore( int x, int y, int height );
		bool isHead( int x, int y );
		int indexFlow( int x, int y );
		bool burn( const char * flowFile, const char * demFile, const char * outFile );
		void writeBurn( const char * outFile );
	private:		
		CTardemCtrl * ocx;
		int * flow;
		int * dem;
		int numberCols;
		int numberRows;
		double xllcenter;
		double yllcenter;
		double cellsize;
		int nodata_value;
};

# endif