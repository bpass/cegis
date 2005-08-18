/****************************************************
 *													*
 *	Programmed By Brian Smith						*
 *  Final Version Completed January 26, 2000		*
 *													*
 *	List of foreseen improvements					*
 *		1.  Use an AVL Tree Data Structure for		*
 *			variable TouchList * map instead of		*
 *			an array.								*
 *		2.  Delete unneeded Single Cell Nodes		*
 *			from AVL Tree when inserting.			*
 *		3.	Replace variable char * Flags by		*
 *			making it a member of a TouchPoint		*
 *													*
 ****************************************************/

# include <iostream.h>
# include <fstream.h>
# include <math.h>
# include <deque>
# include "TouchPoint.h"
# include "types.h"
# include "linklist.h"
# include "areaTree.h"
# include "shapefil.h"


static TouchList * map;
static char * Flags;

class RasterToShape
{	public:
	
		RasterToShape( );
		~RasterToShape();

		bool	createShape( const char * inFilePath, const char * flowFilePath, const char * outFilePath );

	private:

		bool	Read( const char * rasFile );
		bool	ReadFlowFile( const char * flowFile );
		void	Calculate( LinkList & allPolygons );
		bool	Write( LinkList & allPolygons, char * shapefile, char * indexfile, char * dbfPath);
		bool	readVariables( istream & inf );
		bool	readFlowVariables( istream & inf );
		int		indexMap( int x, int y );
		void	unsetFlag( FLAG f, char & c );
		void	setFlag( FLAG f, char & c );
		bool	hasFlag( FLAG f, char c );
		void	setDFlag( DIRECTION f, unsigned char & c );
		void	unsetDFlag( DIRECTION f, unsigned char & c );
		bool	hasDFlag( DIRECTION f, unsigned char c );
		void	addVal( int p_value, int x, int y );
		bool	hasVal( int x, int y, int p_value );
		bool	validVal( int x, int y );
		void	eraseAllVals( int startX, int startY, int endX, int endY, int value );
		void	eraseVal( int x, int y, int p_value );
		bool	isTrouble( int x, int y );
		bool	explore( int x, int y, int value, DIRECTION d, int & minx, int & miny, int & maxx, int & maxy, int cnt, int startx, int starty );
		void	setHitFlag( int x, int y );
		void	removeAndSmooth();

		double	x_arrayIndexToWorldCoordinateX( int x );
		double	y_arrayIndexToWorldCoordinateY( int y );
		bool	fileExists( const char * fileName, bool & deleteFile );
		char *	fileName( const char * filePath, char * extension );
		void	addPolygonToList( LinkList & allPolygons, int PolyGonValue, int minX, int minY, int maxX, int maxY );
		int		indexFileLength( int numberRecords );
		double	findPerimeter();
		int		fileLength( LinkList & allPolygons );
		int		recordLength( PolyGon & poly );
		void	findHeaderBoundingBox( HDR_BoundingBox & hbox, LinkList & allPolygons );
		int		numberOfRecords( LinkList & allPolygons );

	private:
	
		int		cols;
		int		rows;
		int		mcols;
		int		mrows;
//		int		nodata_value;
		float   nodata_value;   //dgt
		double  bndbox[4];   //dgt
		int		filetype;    //dgt
		double	xllcenter;
		double	yllcenter;
		double	cellsize;

		int maxPoints;

		AreaTree		area;
		std::deque<myPoint>	myPolygon;
		//CShapeOCXCtrl * ocx;
};

