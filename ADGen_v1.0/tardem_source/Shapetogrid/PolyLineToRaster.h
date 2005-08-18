# include "Types.h"
# include "myshp.h"
# include "polyline.h"
# include "linklist.h"
# include "StreamJoin.h"
# include "shapefil.h"
# include <deque>

using namespace std;

# ifndef POLYLINETORAS_H
# define POLYLINETORAS_H

static int * Data;
static stream * streamData;

class PolyLineToRas
{
	public:
		PolyLineToRas();
		~PolyLineToRas();
		bool	createFlowFile( CString inFilePath, CString demFile, CString outFilePath, CString idColumnName, CString downPointer, CString streamOutletID );
		long	getStreamOutletX();
		long	getStreamOutletY();

	private:
		bool	createRasterFile( CString inFilePath, CString demFile, CString outFilePath, CString idColumnName );
		void	writeRasterFile( CString outfile );
		void	writeFlowFile( CString outfile, float * demArray );
		int		indexData( int x, int y );
		int		WorldCoordinateXtoX_arrayIndex( double worldX, bool variation = true );
		int		WorldCoordinateYtoY_arrayIndex( double worldY, bool variation = true );
		bool	createFlow( const char * idColumnName, const char * downPointer, const char * dbfPath, float * demArray, const char * streamOutletID );
		double	X_ArrayIndexToWorldCoordinateX( int x );
		double	Y_ArrayIndexToWorldCoordinateY( int y );
		void	exploreStream( int x, int y, int streamID, int segment, int endSegmentX, int endSegmentY, DIRECTION d, bool isMaster, bool & endMarked );
		int		round( double d );
		bool	readFileHeader( istream & inf );
		bool	isValid( int x, int y );
		double	minValue( double one, double two );
		double	maxValue( double one, double two );
		void	remarkCells( int streamID );
		CString getStreamID( const char * dbfPath, const char * idColumnName, int recordNum );
		void	markOutlet( int x, int y, int streamID, int segment, int endSegmentX, int endSegmentY, float * demArray, DIRECTION d );
		bool	findValidDemValue( int x, int y, int streamID, int segment, int & newX, int & newY, int & newSegment, DIRECTION d, float * demArray );

	private:
		//CPolyLineToRasterOCXCtrl * OCXControl;
	
		deque<StreamJoin> Joins;
		deque<stream> doubleCellMap;

		//  DEM grid header variables
		int		ncols;
		int		nrows;
		double	cellsize;
		int	nodata_value;
		float float_nodata_value;
		double	xllcenter;
		double	yllcenter;
		double  xllright;
		double  ylltop;
		double  bndbox[4];
		int filetype;

		ShapeFile shape;
		CString sOutID;

		long streamOutletX;
		long streamOutletY;
		int outletEndSegment;
};

# endif
