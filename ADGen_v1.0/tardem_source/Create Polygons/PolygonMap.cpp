# include "PolygonMap.h"
# include "FileHeader.h"
# include "myshp.h"
# include "gridio.h"

//This Function will construct a RasterToShape Object
//Setting p_ocx as the control to fire events to
RasterToShape::RasterToShape(  )
{	//ocx = p_ocx;

	cols = 0;
	rows = 0;
	mcols = 0;
	mrows = 0;
	nodata_value = -1;
	xllcenter = 0;
	yllcenter = 0;
	cellsize = 30;
	maxPoints = 0;
}

//This Function will destruct a RasterToShape Object
RasterToShape::~RasterToShape()
{
	for( int y = 0; y < mrows; y++ )
	{	for( int x = 0; x < mcols; x++ )
		{	if( map[ indexMap( x, y ) ].touchP != NULL )
				delete map[ indexMap( x, y ) ].touchP;
		}
	}
	
	if( Flags != NULL )
		delete [] Flags;
	if( map != NULL )
		delete [] map;
}

//This Function will call other functions to create the Shape Files with precedant name outFilePath
//from the ASCII files inFilePath and flowFilePath.
//The dataSource will be checked and used to create the .dbf Shape File
bool RasterToShape::createShape(const char * inFilePath, const char * flowFilePath, const char * outFilePath )
{
	if( inFilePath == NULL || outFilePath == NULL || flowFilePath == NULL )
	{	//ocx->PercentChange( 0, ANYERROR, "Missing Parameter" );
		return false;
	}
	if( strlen(inFilePath) <= 0 || strlen( outFilePath ) <= 0 || strlen( flowFilePath ) <= 0 )
	{//	ocx->PercentChange( 0, ANYERROR, "Missing Parameter" );
		return false;	
	}

	bool deleteFile = true;
	//Delete .shp file
	if( !fileExists( fileName( outFilePath, ".shp" ), deleteFile ) )
	{	if( !deleteFile )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete " );	
			strcat( message, fileName( outFilePath, ".shp") );			
			//ocx->PercentChange( 0, ANYERROR, message);
			return false;
		}
	}
	//Delete .shx file
	if( !fileExists( fileName( outFilePath, ".shx" ), deleteFile ) )
	{	if( !deleteFile )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete " );	
			strcat( message, fileName( outFilePath, ".shx") );			
			//ocx->PercentChange( 0, ANYERROR, message);
			return false;
		}
	}
	//Delete .dbf file
	if( !fileExists( fileName( outFilePath, ".dbf" ), deleteFile ) )
	{	if( !deleteFile )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete " );	
			strcat( message, fileName( outFilePath, ".dbf") );			
			//ocx->PercentChange( 0, ANYERROR, message);
			return false;
		}
	}

	deleteFile = false;
	//Check the inFilePath
/*	if( !fileExists( inFilePath, deleteFile ) )  //  dgt these checks do not work with gridio folders
	{	char * message = new char[MAXBUFFER];
		strcpy( message, inFilePath );
		strcat( message, " Does Not Exist" );
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}
	//Check the flowFilePath
	if( !fileExists( flowFilePath, deleteFile ) )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, flowFilePath );
		strcat( message, " Does Not Exist" );
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}   */
	
	LinkList allPolygons;
	if( !Read( inFilePath ) )
		return false;
	if( !ReadFlowFile( flowFilePath ) )
		return false;
	Calculate( allPolygons );
	if( !Write( allPolygons, fileName( outFilePath, ".shp"), fileName( outFilePath, ".shx" ), fileName( outFilePath, ".dbf" ) ) )
		return false;
	
	return true;
}

//This Function will read and expand the data in the ASCII file rasFile into [] map
//It will also set the needed flags to explore [] map
bool RasterToShape::Read( const char * rasFile )
{
/*	ifstream inf( rasFile );
	if( ! inf )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Could Not Open " );
		strcat( message, rasFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}

	if( !readVariables( inf ) )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Could Not Read File Header of " );
		strcat( message, rasFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}   dgt  */
	int **ww;
	float dx,dy;
	int err;
	char fname[MAXLN];
	sprintf(fname,"%s",rasFile);
	if(err=gridread(fname,(void ***)&ww,RPINTDTYPE,&cols,&rows,&dx,&dy,
        bndbox,&cellsize,&nodata_value,&filetype))
	{
		char * message = new char[MAXBUFFER];
		strcpy( message, "Could Not Read File" );
		strcat( message, rasFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}
	xllcenter=bndbox[0]+cellsize*0.5;
	yllcenter=bndbox[1]+cellsize*0.5;

	mcols = 2*cols + 1;
	mrows = 2*rows + 1;

	long arraySize = mcols * mrows;
	map = new TouchList[ arraySize ];
	memset( map, NULL, sizeof( TouchList )*arraySize );
	Flags = new char[ arraySize ];
	memset( Flags, 0, sizeof( char )*arraySize );

	int value = 0, percent = 0, newpercentage = 0;
	int cellArea = cellsize*cellsize;
	double total = cols * rows;

	for( int y = 0; y < rows; y++ )
	{	for( int x = 0; x < cols; x++ )
		{	
		//inf>>value;
		    value = ww[x][y];    //  dgt   mapping from gridio into what is there
			int mapx = 2*x;
			int mapy = 2*y;

			if( value != nodata_value )
			{	addVal( value, mapx,     mapy );
				addVal( value, mapx + 1, mapy );
				addVal( value, mapx + 2, mapy );
				addVal( value, mapx,     mapy + 1 );
				addVal( value, mapx + 1, mapy + 1 );
				addVal( value, mapx + 2, mapy + 1 );
				addVal( value, mapx,     mapy + 2 );
				addVal( value, mapx + 1, mapy + 2 );
				addVal( value, mapx + 2, mapy + 2 );

				area.Area( value, cellArea );
			}
			else
			{	setHitFlag( mapx,		mapy );
				setHitFlag( mapx + 1,	mapy );
				setHitFlag( mapx + 2,	mapy );
				setHitFlag( mapx,		mapy + 1 );
				setHitFlag( mapx + 1,	mapy + 1 );
				setHitFlag( mapx + 2,	mapy + 1 );
				setHitFlag( mapx,		mapy + 2 );
				setHitFlag( mapx + 1,	mapy + 2 );
				setHitFlag( mapx + 2,	mapy + 2 );			
			}
						
			newpercentage = (double)(x*y)/(total)*100;
			if( newpercentage > percent )
			{	percent = newpercentage;
				//ocx->PercentChange( percent, READ, "Reading and Expanding File" );
			}
		}
	}
    free(ww[0]);  free(ww);
	return true;
}

//This Function will read the ASCII file flowFile and set up direction information needed to explore [] map
bool RasterToShape::ReadFlowFile( const char * flowFile )
{	ifstream inf( flowFile );
/*	if( ! inf )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Could Not Open " );
		strcat( message, flowFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}

	if( !readFlowVariables( inf ) )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Conflicting File Header Values in " );
		strcat( message, flowFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}   */
	short **pp;
	float dx,dy;
	int err;
	char fname[MAXLN];
	sprintf(fname,"%s",flowFile);
	if(err=gridread(fname,(void ***)&pp,RPSHRDTYPE,&cols,&rows,&dx,&dy,
        bndbox,&cellsize,&nodata_value,&filetype))
	{
		char * message = new char[MAXBUFFER];
		strcpy( message, "Could Not Read File" );
		strcat( message, flowFile );		
		//ocx->PercentChange( 0, ANYERROR, message);
		return false;
	}
	xllcenter=bndbox[0]+cellsize*0.5;
	yllcenter=bndbox[1]+cellsize*0.5;


	int value;
	int percent = 0;
	int newpercentage = 0;
	int total = rows * cols;
	for( int y = 0; y < rows; y++ )
	{	for( int x = 0; x < cols; x++ )
		{	
		//inf>>value;
			value = pp[x][y];    //  dgt   mapping from gridio into what is there
			int mapx = 2*x + 1;
			int mapy = 2*y + 1;
			
				if( isTrouble( mapx - 1, mapy - 1 ) )
				{	unsetFlag( BOUNDARY, Flags[ indexMap(mapx - 1, mapy - 1) ] );
					setFlag( TROUBLE, Flags[ indexMap( mapx - 1, mapy - 1 ) ] );		
					setDFlag( (DIRECTION)value, map[ indexMap( mapx - 1, mapy - 1 ) ].touchP->ThreeFlow );
				}
				if( isTrouble( mapx + 1, mapy - 1 ) )
				{	unsetFlag( BOUNDARY, Flags[ indexMap(mapx + 1, mapy - 1 ) ] );
					setFlag( TROUBLE, Flags[ indexMap( mapx + 1, mapy - 1) ] );		
					setDFlag( (DIRECTION)value, map[ indexMap( mapx + 1, mapy - 1 ) ].touchP->FourFlow );
				}
				if( isTrouble( mapx - 1, mapy + 1 ) )
				{	unsetFlag( BOUNDARY, Flags[ indexMap(mapx - 1, mapy + 1 ) ] );
					setFlag( TROUBLE, Flags[ indexMap( mapx - 1, mapy + 1 ) ] );		
					setDFlag( (DIRECTION)value, map[ indexMap( mapx - 1, mapy + 1 ) ].touchP->TwoFlow );
				}
				if( isTrouble( mapx + 1, mapy + 1 ) )
				{	unsetFlag( BOUNDARY, Flags[ indexMap(mapx + 1, mapy + 1 ) ] );
					setFlag( TROUBLE, Flags[ indexMap( mapx + 1, mapy + 1 ) ] );		
					setDFlag( (DIRECTION)value, map[ indexMap( mapx + 1, mapy + 1 ) ].touchP->OneFlow );					
				}
						
			newpercentage = (double)(x*y)/(total)*100;
			if( newpercentage > percent )
			{	percent = newpercentage;
				//ocx->PercentChange( percent, READ, "OverLaying Flow File" );
			}
		}
	}
	free(pp[0]);  free(pp);
	return true;
}

//This Function will call explore() for flagged values in [] map and then save the created Polygon in allPolygons
void RasterToShape::Calculate( LinkList & allPolygons )
{
	int value = nodata_value;
		
	int percent = 0;
	double total = mrows * mcols;
	
	for( int y = 0; y < mrows; y++ )
	{	for( int x = 0; x < mcols; x++ )
		{	
			if( hasFlag( BOUNDARY, Flags[ indexMap( x, y ) ] ) )
			{
				int num = map[ indexMap( x, y ) ].touchP->numValues;
				for( int i = 0; i < num; i++ )
				{	value = map[ indexMap( x, y ) ].touchP->value[i];
					if( value != nodata_value )
					{	int minx = mcols, miny = 0, maxx = 0, maxy = mrows, cnt = 0;
						
							explore( x, y, value, RIGHT, minx, miny, maxx, maxy, cnt, x, y );

							eraseAllVals( minx, miny, maxx, maxy, value );
							if( myPolygon.size() > 0 )
							{	removeAndSmooth();
								addPolygonToList( allPolygons, value, minx, miny, maxx, maxy );
							}							
					}
					myPolygon.clear();
				}				
			}

			int newpercent = (double)(x*y)/total*100;
			if( newpercent > percent )
			{	percent = newpercent;
				//ocx->PercentChange( percent, CALCULATE, "Calculating" );
			}
		}	
	}	
}

//This Function will write the Three Shape Files shapefile, indexfile, and dbfPath from the values in allPolygons
//using the ODBCSource to write dbfPath
bool RasterToShape::Write( LinkList & allPolygons, char * shapefile, char * indexfile, char * dbfPath )
{
	int shpPercent = 50;
	int shxPercent = 15;
	int dbfPercent = 35;	
		
	//WRITE .SHP FILE
	FileHeader head;
	head.setFileCode( FILE_CODE );
	head.setFileLength( fileLength( allPolygons ) );
	head.setVersion( VERSION );
	head.setType( POLYGON );
	findHeaderBoundingBox( head.boundingBox, allPolygons );
	
	int numShapes = numberOfRecords( allPolygons );

	ShapeFile sf;
	sf.setNumShapes( numShapes );
	sf.setList( allPolygons );
	sf.setHeader( head );

	sf.WriteShapeFile( shapefile, numShapes, shpPercent );	
	//END OF WRITE .SHP FILE

	//WRITE .SHX FILE
	FILE * fi = fopen( indexfile, "wb" );
	head.setFileLength( indexFileLength( numShapes ) );
	head.WriteHeader( fi );
		
		void * intbuf;
		int offset = 0;
		int totaloffset = HEADER_BYTES/2;
		int contentLength = 0;
		Node * curPoly = new Node();
		allPolygons.First( curPoly );

	int percent = shpPercent;
	int numWritten = 0;
		
		while( curPoly != NULL )
		{	offset = totaloffset;
			intbuf = (char*)&offset;
			head.SwapEndian((char*)intbuf,sizeof(int));
			fwrite(intbuf, sizeof(char),sizeof(int),fi);

			contentLength = recordLength( *(curPoly->data.polygon) );
			totaloffset += contentLength;
			contentLength -= sizeof(int);	//recordLength -= (2 * #HEADER LENGTH) / 2

			intbuf = (char*)&contentLength;
			head.SwapEndian((char*)intbuf,sizeof(int));
			fwrite(intbuf, sizeof(char),sizeof(int),fi);					

			int newPercent = (double)numWritten++/numShapes*( shpPercent + shxPercent );
			if( newPercent > percent )
			{	percent = newPercent;
				//ocx->PercentChange( percent, WRITE, "Writing .shx File" );
			}

			allPolygons.Next( curPoly );
		}			

	fclose( fi );
	//END OF WRITE .SHX FILE
	
	/*
	int numShapes = numberOfRecords( allPolygons );
	int percent = 0;
	SHPHandle shapeHandle = SHPCreate( shapefile, SHPT_POLYGON );
	if( shapeHandle == NULL )
	{	ocx->PercentChange( 0, ANYERROR, "Could not create .shp && .shx file" );
		return false;
	}
	Node * curPoly = new Node();
	allPolygons.First( curPoly );
	
	double * xPoints = new double[ maxPoints ];
	double * yPoints = new double[ maxPoints ];
	int shapeID = 0;
	int numPoints = 0;
	double x, y;
	SHPObject * object;
	int numWritten = 0;
	while( curPoly != NULL )
	{	shapeID = curPoly->data.polygon->recNum;
		numPoints = curPoly->data.polygon->numPoints;
		
		for( int i = 0; i < numPoints; i++ )
		{	curPoly->data.polygon->points[i].getXY( x, y );
			xPoints[i] = x;
			yPoints[i] = y;
		}		
		object = SHPCreateObject( SHPT_POLYGON, shapeID, 0, NULL, NULL, numPoints, xPoints, yPoints, NULL, NULL );
		SHPWriteObject( shapeHandle, shapeID, object );		
		SHPDestroyObject( object );
			
		int newPercent = (double)numWritten++/numShapes*( shpPercent + shxPercent );
		if( newPercent > percent )
		{	percent = newPercent;
			ocx->PercentChange( percent, WRITE, "Writing .shp and .shx File" );
		}
		allPolygons.Next( curPoly );
	}
	
	SHPClose( shapeHandle );
	*/
			
	//WRITE .DBF FILE	
				//Valid .DBF type = CHAR, NUMERIC, DATE, INT

	DBFHandle dbf = DBFCreate( dbfPath );
	int idField = DBFAddField( dbf, "ID", FTInteger, 9, 0 );
	if( idField == - 1 )
	{	//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
		DBFClose( dbf );
		return false;
	}
	int areaField = DBFAddField( dbf, "AREA", FTDouble, 15, 4 );
	if( areaField == - 1 )
	{	//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
		DBFClose( dbf );
		return false;
	}
	int perimField = DBFAddField( dbf, "PERIMETER", FTDouble, 15, 4 );
	if( perimField == - 1 )
	{	//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
		DBFClose( dbf );
		return false;
	}

	Node * dbfNode;
	allPolygons.First( dbfNode );
	int recordNum = 0;
	while( dbfNode != NULL )
	{	int record = dbfNode->data.polygon->recNum;
		if( !DBFWriteIntegerAttribute( dbf, recordNum, idField, record ) )
		{		//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
				DBFClose( dbf );
				return false;
		}
		double area = dbfNode->data.polygon->area;
		if( !DBFWriteDoubleAttribute( dbf, recordNum, areaField, area ) )
		{		//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
				DBFClose( dbf );
				return false;
		}
		double perimeter = dbfNode->data.polygon->perimeter;
		if( !DBFWriteDoubleAttribute( dbf, recordNum, perimField, perimeter ) )
		{		//ocx->PercentChange( 0, ANYERROR, "Could not create field in .dbf file" );
				DBFClose( dbf );
				return false;
		}
		
		allPolygons.Next( dbfNode );
		int newPercent = (double)recordNum++/numShapes*(  shpPercent + shxPercent + dbfPercent  );
		if( newPercent > percent )
		{	percent = newPercent;
			//ocx->PercentChange( percent, WRITE, "Writing .dbf File" );
		}	
	}		
	
	DBFClose( dbf );	
	return true;
}

//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################

//This Function will read the File Header information of inf
bool RasterToShape::readVariables( istream & inf )
{
	int number_of_variables = 6;
	char * variable_name = new char[MAXBUFFER];
	bool shiftUp = false;
	for( int i = 0; i < number_of_variables; i++ )
	{	inf>>variable_name;
				
		if( !strcmp( variable_name, "ncols" ) )
		{	inf>>cols;
		}
		else if( !strcmp( variable_name, "nrows" ) )
		{	inf>>rows;
		}
		else if( !strcmp( variable_name, "xllcorner" ) )
		{	inf>>xllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "yllcorner" ) )
		{	inf>>yllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "xllcenter" ) )
		{	inf>>xllcenter;
		}
		else if( !strcmp( variable_name, "yllcenter" ) )
		{	inf>>yllcenter;
		}
		else if( !strcmp( variable_name, "cellsize" ) )
		{	inf>>cellsize;
		}
		else if( !strcmp( variable_name, "nodata_value" ) )
		{	inf>>nodata_value;		
		}
		else if( !strcmp( variable_name, "NODATA_value" ) )
		{	inf>>nodata_value;
		}
	}
	if( shiftUp )
	{	xllcenter += cellsize/2;
		yllcenter += cellsize/2;
	}
	
	mcols = (2*cols + 1 );
	mrows = (2*rows + 1 );

	return true;
}

//This Function will read the File Header information of the Flow File inf and compare it to
//values read from the ASCII raster Data File
bool RasterToShape::readFlowVariables( istream & inf )
{
	int number_of_variables = 6;
	char * variable_name = new char[MAXBUFFER];
	bool shiftUp = false;
	int fcols,frows,fnodata_value;
	double fxllcenter,fyllcenter,fcellsize;
	
	for( int i = 0; i < number_of_variables; i++ )
	{	inf>>variable_name;
				
		if( !strcmp( variable_name, "ncols" ) )
		{	inf>>fcols;
		}
		else if( !strcmp( variable_name, "nrows" ) )
		{	inf>>frows;
		}
		else if( !strcmp( variable_name, "xllcorner" ) )
		{	inf>>fxllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "yllcorner" ) )
		{	inf>>fyllcenter;
			shiftUp = true;
		}
		else if( !strcmp( variable_name, "xllcenter" ) )
		{	inf>>fxllcenter;
		}
		else if( !strcmp( variable_name, "yllcenter" ) )
		{	inf>>fyllcenter;
		}
		else if( !strcmp( variable_name, "cellsize" ) )
		{	inf>>fcellsize;
		}
		else if( !strcmp( variable_name, "nodata_value" ) )
		{	inf>>fnodata_value;		
		}
		else if( !strcmp( variable_name, "NODATA_value" ) )
		{	inf>>fnodata_value;
		}
	}
	if( shiftUp )
	{	fxllcenter += fcellsize/2;
		fyllcenter += fcellsize/2;
	}
	
	if( fcols != cols )
		return false;
	if( frows != rows )
		return false;
	if( fxllcenter != xllcenter )
		return false;
	if( fyllcenter != yllcenter )
		return false;
	if( fcellsize != cellsize )
		return false;
	if( fnodata_value != nodata_value )
		return false;

	return true;
}

//This Function will set a flag in [] Flags
void RasterToShape::setHitFlag( int x, int y )
{	
	if( hasFlag( HIT, Flags[ indexMap( x, y ) ] ) )
	{	if( map[ indexMap( x, y ) ].touchP != NULL )
		{	setFlag( BOUNDARY, Flags[ indexMap( x, y )] );
		}
	}
	else
		setFlag( HIT, Flags[ indexMap( x, y )] );
}

//This Function will return a one-dimensional index into [] map from x and y 
int RasterToShape::indexMap( int x, int y )
{	return y*mcols + x;
}

//This Function will unset a FLAG f of c
void RasterToShape::unsetFlag( FLAG f, char & c )
{	if( f & c )
		c = c - f;
}

//This Function will set a FLAG f in c
void RasterToShape::setFlag( FLAG f, char & c )
{	c = c | f;
}

//This Function will check if FLAG f exists in c
bool RasterToShape::hasFlag( FLAG f, char c )
{	return f & c;
}

//This Function will set a DIRECTION d for c
void RasterToShape::setDFlag( DIRECTION d, unsigned char & c )
{	if( d == RIGHT )
	{	c = c | 1;
	}
	else if( d == UPRIGHT )
	{	c = c | 2;
	}
	else if( d == UP )
	{	c = c | 4;
	}
	else if( d == UPLEFT )
	{	c = c | 8;
	}
	else if( d == LEFT )
	{	c = c | 16;
	}
	else if( d == DOWNLEFT )
	{	c = c | 32;
	}
	else if( d == DOWN )
	{	c = c | 64;
	}
	else if( d == DOWNRIGHT )
	{	c = c | 128;
	}
}

//This Function will check if DIRECTION d exists in c
bool RasterToShape::hasDFlag( DIRECTION d, unsigned char c )
{	if( d == RIGHT )
	{	return c & 1;
	}
	else if( d == UPRIGHT )
	{	return c & 2;
	}
	else if( d == UP )
	{	return c & 4;
	}
	else if( d == UPLEFT )
	{	return c & 8;
	}
	else if( d == LEFT )
	{	return c & 16;
	}
	else if( d == DOWNLEFT )
	{	return c & 32;
	}
	else if( d == DOWN )
	{	return c & 64;
	}
	else if( d == DOWNRIGHT )
	{	return c & 128;
	}
	return false;
}

//This Function will unset a DIRECTION d of c
void RasterToShape::unsetDFlag( DIRECTION d, unsigned char & c )
{	
	if( hasDFlag( d, c ) )
	{
		if( d == RIGHT )
		{	c = c - 1;
		}
		else if( d == UPRIGHT )
		{	c = c - 2;
		}
		else if( d == UP )
		{	c = c - 4;
		}
		else if( d == UPLEFT )
		{	c = c - 8;
		}
		else if( d == LEFT )
		{	c = c - 16;
		}
		else if( d == DOWNLEFT )
		{	c = c - 32;
		}
		else if( d == DOWN )
		{	c = c - 64;
		}
		else if( d == DOWNRIGHT )
		{	c = c - 128;
		}
	}
}

//This Function will add the value p_value into [] map
void RasterToShape::addVal( int p_value, int x, int y )
{
	long index = indexMap( x, y );
	
	if( map[ index ].touchP == NULL )
		map[ index ].touchP = new TouchPoint();
	int num = map[ index ].touchP->numValues;
	for( int i = 0; i < num; i++ )
	{	if( map[ index ].touchP->value[i] == p_value )
			return;
	}
	map[ index ].touchP->value[ num++ ] = p_value;
	map[ index ].touchP->numValues++;

	//BOUNDARY && JOINT
	if(  x == 0 || y == 0 )
	{	setFlag( BOUNDARY, Flags[ index ] );
		setFlag( JOINT, Flags[ index ] );
	}		
	else if( x == mcols -1 || y == mrows -1 )
	{	setFlag( BOUNDARY, Flags[ index ] );		
		setFlag( JOINT, Flags[ index ] );
	}
	else if( hasFlag( HIT, Flags[ index ] ) )
		setFlag( BOUNDARY, Flags[ index ] );

	if( num > 2 )
		setFlag( JOINT, Flags[ index ] );
	
	setFlag( HIT, Flags[ index ] );
}

//This Function will check if p_value exists in [] map
bool RasterToShape::hasVal( int x, int y, int p_value )
{	
	if( x < 0 || x >= mcols )
		return false;
	if( y < 0 || y >= mrows )
		return false;
	
	if( map[ indexMap( x, y ) ].touchP == NULL )
		return false;
	int num = map[ indexMap( x, y )].touchP->numValues;
	for( int i = 0; i < num; i++ )
		if( map[ indexMap( x, y )].touchP->value[i] == p_value )
			return true;
	return false;
	
}

//This Function will check the validity of x, y as it would be mapped into [] map
bool RasterToShape::validVal( int x, int y )
{	if( x < 0 || x >= mcols )
		return false;
	if( y < 0 || y >= mrows )
		return false;
	return true;
}

//This Function will erase value from [] map within the given rectangle
void RasterToShape::eraseAllVals( int startX, int startY, int endX, int endY, int value )
{	for( int x = startX; x < endX; x++ )
	{	for( int y = startY; y < endY; y++ )
		{	if( map[ indexMap( x, y ) ].touchP != NULL )
				eraseVal( x, y, value );			
		}
	}
}

//This Function will erase p_value from a specific index of [] map
void RasterToShape::eraseVal( int x, int y, int p_value )
{	
	if( !validVal( x, y ) )
		return;

	TouchPoint * i = map[ indexMap( x, y )].touchP;
	i->eraseVal( p_value, nodata_value );	
}

//This Function will determine if a given index of [] map is a TROUBLE SPOT . . .
//TROUBLE SPOT is a point where multiple path options are available
bool RasterToShape::isTrouble( int x, int y )
{	
	if( !validVal( x, y ) )
		return false;

	if( map[ indexMap( x, y ) ].touchP == NULL )
		return false;

	if( hasFlag( TROUBLE, Flags[ indexMap( x, y ) ] ) )
		return true;

	if( x % 2 != 0 || y % 2 != 0 )
		return false;

	//Is on the Edge
	if( x - 1 < 0 || y - 1 < 0 )
		return false;
	else if( x + 1 >= mcols || y + 1 >= mrows )
		return false;
	//Is in the Middle
	else 
	{	int num = map[ indexMap( x, y ) ].touchP->numValues;
		
		for( int ii = 0; ii < num; ii++ )
		{	int myvalue = map[ indexMap( x, y )].touchP->value[ii];
			
			if( hasVal( x - 1, y - 1, myvalue ) && hasVal( x + 1, y + 1, myvalue ) )
			{	if( !hasVal( x - 1, y + 1, myvalue ) && !hasVal( x + 1, y - 1, myvalue ) )
					return true;			
			}
			else if( hasVal( x - 1, y + 1, myvalue ) && hasVal( x + 1, y - 1, myvalue ) )
			{	if( !hasVal( x - 1, y - 1, myvalue ) && !hasVal( x + 1, y + 1, myvalue ) )
					return true;
			}
		}		
	}	
	
	return false;
}

//This Function will explore [] map for a polygon of value
bool RasterToShape::explore( int x, int y, int value, DIRECTION d, int & minx, int & miny, int & maxx, int & maxy, int cnt, int startx, int starty )
{	
	if( !hasVal( x, y, value ) )
	{	return false;
	}

	//Extents
	if( x < minx )
		minx = x;
	if( x > maxx )
		maxx = x;
	if( y > miny )
		miny = y;
	if( y < maxy )
		maxy = y;

	if( !hasFlag( TROUBLE, Flags[ indexMap( x, y ) ] ) )
	{	//Save Points in myPolygon
		myPoint worldPoint;
		worldPoint.X =  x_arrayIndexToWorldCoordinateX( x );
		worldPoint.Y = y_arrayIndexToWorldCoordinateY( y );
		worldPoint.isJoint = hasFlag( JOINT, Flags[ indexMap( x, y ) ] ); 				
		myPolygon.push_back( worldPoint );
	}
	
	eraseVal( x, y, value );

	//Reset the starting point to have the value and a boundary
	if( cnt++ == 3 )
	{	addVal( value, startx, starty );
		setFlag( BOUNDARY, Flags[ indexMap( startx, starty ) ] );
	}
	//Back to the Beginning
	else if( cnt > 3 && x == startx && y == starty )
		return true;	
	
	//RIGHT
	if( d == RIGHT )
	{
		//CARDINAL DIRECTIONS

		//Right
		if( hasVal( x + 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y ) ] ) )
			return explore( x + 1, y, value, RIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
		//Up
		else if( hasVal( x, y - 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y - 1 ) ] ) )
			return explore( x, y - 1, value, UP, minx, miny, maxx, maxy, cnt, startx, starty );
		//Down
		else if( hasVal( x, y + 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y + 1 ) ] ) )
			return explore( x, y + 1, value, DOWN, minx, miny, maxx, maxy, cnt, startx, starty );
	
		//DIAGONAL DIRECTIONS

		//Right Trouble
		else if( hasVal( x + 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x + 1, y ) ] ) )
		{	
			if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] )  )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNLEFT, map[ indexMap( x + 1, y ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x + 1, y ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x + 1, y ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x + 1, y ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}		
		}		
	}
	//LEFT
	else if( d == LEFT )
	{	
		//CARDINAL DIRECTIONS
		
		//Left
		if( hasVal( x - 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y ) ] ) )
			return explore( x - 1, y, value, LEFT, minx, miny, maxx, maxy, cnt, startx, starty );
		//Up
		else if( hasVal( x, y - 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y - 1 ) ] ) )
			return explore( x, y - 1, value, UP, minx, miny, maxx, maxy, cnt, startx, starty );
		//Down
		else if( hasVal( x, y + 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y + 1 ) ] ) )
			return explore( x, y + 1, value, DOWN, minx, miny, maxx, maxy, cnt, startx, starty );

		//DIAGONAL DIRECTIONS

		//Left Trouble
		else if( hasVal( x - 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x - 1, y ) ] ) )
		{	
			if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x - 1, y ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x - 1, y ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x - 1, y ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x - 1, y ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
	}
	//UP
	else if( d == UP )
	{	
		//CARDINAL DIRECTIONS
		
		//Up
		if( hasVal( x, y - 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y - 1 ) ] ) )
			return explore( x, y - 1, value, UP, minx, miny, maxx, maxy, cnt, startx, starty );		
		//Left
		else if( hasVal( x - 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y ) ] ) )
			return explore( x - 1, y, value, LEFT, minx, miny, maxx, maxy, cnt, startx, starty );		
		//Right
		else if( hasVal( x + 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y ) ] ) )
			return explore( x + 1, y, value, RIGHT, minx, miny, maxx, maxy, cnt, startx, starty );

		//DIAGONAL DIRECTIONS

		//Up Trouble
		else if( hasVal( x, y - 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y - 1 ) ] ) )
		{	
			if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( UPRIGHT, map[ indexMap( x, y - 1 ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNLEFT, map[ indexMap( x, y - 1 ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( UPLEFT, map[ indexMap( x, y - 1 ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y - 1 ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
	}
	//DOWN
	else if( d == DOWN )
	{	
		//CARDINAL DIRECTIONS
		
		//Down
		if( hasVal( x, y + 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y + 1 ) ] ) )
			return explore( x, y + 1, value, DOWN, minx, miny, maxx, maxy, cnt, startx, starty );
		//Left
		else if( hasVal( x - 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y ) ] ) )
			return explore( x - 1, y, value, LEFT, minx, miny, maxx, maxy, cnt, startx, starty );		
		//Right
		else if( hasVal( x + 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y ) ] ) )
			return explore( x + 1, y, value, RIGHT, minx, miny, maxx, maxy, cnt, startx, starty );

		//DIAGONAL DIRECTIONS

		//Down Trouble
		else if( hasVal( x, y + 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y + 1 ) ] ) )
		{
			if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y + 1 ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x, y + 1 ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x, y + 1 ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x, y + 1 ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
	}
	//UPRIGHT
	else if( d == UPRIGHT )
	{	
		//CARDINAL DIRECTIONS
		
		//Up
		if( hasVal( x, y - 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y - 1 ) ] ) )
			return explore( x, y - 1, value, UP, minx, miny, maxx, maxy, cnt, startx, starty );		
		//Right
		else if( hasVal( x + 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y ) ] ) )
			return explore( x + 1, y, value, RIGHT, minx, miny, maxx, maxy, cnt, startx, starty );

		//DIAGONAL DIRECTIONS

		//Up Trouble
		else if( hasVal( x, y - 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y - 1 ) ] ) )
		{	
			if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( UPRIGHT, map[ indexMap( x, y - 1 ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNLEFT, map[ indexMap( x, y - 1 ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( UPLEFT, map[ indexMap( x, y - 1 ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y - 1 ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
		//Right Trouble
		else if( hasVal( x + 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x + 1, y ) ] ) )
		{	
			if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] )  )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNLEFT, map[ indexMap( x + 1, y ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x + 1, y ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x + 1, y ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x + 1, y ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}

		}			
	}
	//DOWNRIGHT
	else if( d == DOWNRIGHT )
	{	
		//CARDINAL DIRECTIONS
		
		//Down
		if( hasVal( x, y + 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y + 1 ) ] ) )
			return explore( x, y + 1, value, DOWN, minx, miny, maxx, maxy, cnt, startx, starty );
		//Right
		else if( hasVal( x + 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y ) ] ) )
			return explore( x + 1, y, value, RIGHT, minx, miny, maxx, maxy, cnt, startx, starty );

		//DIAGONAL DIRECTIONS

		//Down Trouble
		else if( hasVal( x, y + 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y + 1 ) ] ) )
		{
			if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y + 1 ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x, y + 1 ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x, y + 1 ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x, y + 1 ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
		//Right Trouble
		else if( hasVal( x + 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x + 1, y ) ] ) )
		{	
			if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] )  )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNLEFT, map[ indexMap( x + 1, y ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x + 1, y ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x + 1, y ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x + 1, y ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}			
	}
	//UPLEFT
	else if( d == UPLEFT )
	{	
		//CARDINAL DIRECTIONS
		
		//Up
		if( hasVal( x, y - 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y - 1 ) ] ) )
			return explore( x, y - 1, value, UP, minx, miny, maxx, maxy, cnt, startx, starty );		
		//Left
		else if( hasVal( x - 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y ) ] ) )
			return explore( x - 1, y, value, LEFT, minx, miny, maxx, maxy, cnt, startx, starty );	

		//DIAGONAL DIRECTIONS

		//Up Trouble
		else if( hasVal( x, y - 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y - 1 ) ] ) )
		{	
			if( !hasVal( x + 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y - 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( UPRIGHT, map[ indexMap( x, y - 1 ) ].touchP->FourFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNLEFT, map[ indexMap( x, y - 1 ) ].touchP->TwoFlow ) )
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( UPLEFT, map[ indexMap( x, y - 1 ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y - 1 ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y - 1, value, UPRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		
		}
		//Left Trouble
		else if( hasVal( x - 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x - 1, y ) ] ) )
		{	
			if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x - 1, y ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x - 1, y ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x - 1, y ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x - 1, y ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}

		}	
	}
	//DOWNLEFT
	else if( d == DOWNLEFT )
	{	
		//CARDINAL DIRECTIONS
		
		//Down
		if( hasVal( x, y + 1, value) && hasFlag( BOUNDARY, Flags[ indexMap( x, y + 1 ) ] ) )
			return explore( x, y + 1, value, DOWN, minx, miny, maxx, maxy, cnt, startx, starty );
		//Left
		else if( hasVal( x - 1, y, value ) && hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y ) ] ) )
			return explore( x - 1, y, value, LEFT, minx, miny, maxx, maxy, cnt, startx, starty );	

		//DIAGONAL DIRECTIONS

		//Down Trouble
		else if( hasVal( x, y + 1, value ) && hasFlag( TROUBLE, Flags[ indexMap( x, y + 1 ) ] ) )
		{
			if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x + 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x + 1, y + 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x - 1, y, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x, y + 1 ) ].touchP->OneFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x, y + 1 ) ].touchP->ThreeFlow ) )
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x, y + 1 ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x, y + 1 ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x + 1, y + 1, value, DOWNRIGHT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		
		}
		//Left Trouble
		else if( hasVal( x - 1, y, value ) && hasFlag( TROUBLE, Flags[ indexMap( x - 1, y ) ] ) )
		{	
			if( !hasVal( x - 1, y - 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y - 1 ) ] ) )
				return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( !hasVal( x - 1, y + 1, value ) || !hasFlag( BOUNDARY, Flags[ indexMap( x - 1, y + 1 ) ] ) )
				return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			else if( hasVal( x, y + 1, value ) )
			{
				if( hasDFlag( DOWNRIGHT, map[ indexMap( x - 1, y ) ].touchP->OneFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPLEFT, map[ indexMap( x - 1, y ) ].touchP->ThreeFlow ) )
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
			else
			{	
				if( hasDFlag( DOWNLEFT, map[ indexMap( x - 1, y ) ].touchP->TwoFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else if( hasDFlag( UPRIGHT, map[ indexMap( x - 1, y ) ].touchP->FourFlow ) )
					return explore( x - 1, y + 1, value, DOWNLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
				else
					return explore( x - 1, y - 1, value, UPLEFT, minx, miny, maxx, maxy, cnt, startx, starty );
			}
		}
	}
	
	myPolygon.pop_back();
	return false;
}

//This function will convert an x array index into world coordinates
double RasterToShape::x_arrayIndexToWorldCoordinateX( int x )
{	//mcols - x				# reverse arrayIndex
	//cellsize/2					# map is interpolated by cellsize/2
	//xllcenter - cellsize/2		# shift the xllcenter down to center it in map

	return ( x )*( (int)cellsize/2 ) + xllcenter - (int)cellsize/2;
}

//This function will convert a y array index into world coordinates
double RasterToShape::y_arrayIndexToWorldCoordinateY( int y )
{	
	//mrows - y				# reverse arrayIndex
	//cellsize/2					# map is interpolated by cellsize/2
	//yllcenter - cellsize/2		# shift the yllcenter down to center it in map

	return ( mrows - y )*( (int)cellsize/2 ) + yllcenter - cellsize;
}

//This Function will see if fileName exists and if deleteFile == TRUE it will unlink it
bool RasterToShape::fileExists( const char * fileName, bool & deleteFile )
{
	FILE * fileCheck;
	fileCheck = fopen( fileName, "r" );
	if( fileCheck != NULL )
	{	fclose( fileCheck );
		if( deleteFile )
		{	ofstream outf( fileName, ios::out, filebuf::sh_none);
			if( !outf )
			{	deleteFile = false;
				return false;
			}
			outf.close();
			if( _unlink( fileName ) )
			{	
				char * message = new char[MAXBUFFER];
				strcpy( message, "Could Not Delete " );
				strcat( message, fileName );		
				//ocx->PercentChange( 0, ANYERROR, message);
				deleteFile = false;
				return false;
			}
		}
		return true;
	}
	return false;	
}

//This Function will create a fileName from filePath and extension
char * RasterToShape::fileName( const char * filePath, char * extension )
{
	char * newPath = new char[ strlen( filePath ) + strlen( extension ) + 1 ];
	strcpy( newPath, filePath);
	strcat( newPath, extension );

	return newPath;
}

//This Function will save a polygon to the list allPolygons
void RasterToShape::addPolygonToList( LinkList & allPolygons, int PolyGonValue, int minX, int minY, int maxX, int maxY )
{
	//Number of Points in Polygon
	PolyGon * poly = new PolyGon();
	poly->numPoints = myPolygon.size();		

	//Number of Starting Points in Polygon
	poly->numParts = 1;							

	//Indexes to starting Points
	int * index_to_parts = new int[1];			
	index_to_parts[0] = 0;
	poly->parts = index_to_parts;

	//Bounding Box
	double bounds[4];					
	bounds[0] = x_arrayIndexToWorldCoordinateX(minX);
	bounds[1] = y_arrayIndexToWorldCoordinateY(minY);
	bounds[2] = x_arrayIndexToWorldCoordinateX(maxX);
	bounds[3] = y_arrayIndexToWorldCoordinateY(maxY);				
	SHP_BoundingBox polyBounds;
	polyBounds.setBounds( bounds );
	poly->bounds = polyBounds;
								
	if( myPolygon.size() > maxPoints )
		maxPoints = myPolygon.size() + 1;

	//Array of Points
	Point1 * polyPoints = new Point1[ myPolygon.size()];
	for( int path = 0; path < myPolygon.size(); path++ )
	{	polyPoints[path].setXY( myPolygon[path].X, myPolygon[path].Y );
	}
	poly->points = polyPoints;
	poly->recNum = PolyGonValue;

	poly->area = area.GetArea( PolyGonValue );
	poly->perimeter = findPerimeter();
							
	//Attach Polygon to LinkList
	Node * myNode = new Node();
	myNode->data.polygon = poly;
	myNode->Next = NULL;
	allPolygons.InsertItem( myNode );
}

//This Function will find the Length of the associated Index File
int RasterToShape::indexFileLength( int numberRecords )
{	int fileLength = 0;
	fileLength += HEADER_BYTES;							//INDEX FILE HEADER
	fileLength += sizeof(int) * numberRecords;			//RECORD OFFSETS
	fileLength += sizeof(int) * numberRecords;			//RECORD CONTENT LENGTHS

	fileLength /= 2;	//SET LENGTH FROM # of BYTES to # of 16 bit WORDS

	return fileLength;
}

//This Function will find the Length of a Polygon Record on 16 bit WORD boundaries
int RasterToShape::recordLength( PolyGon & poly )
{	int recordLength = 0;
	//###	HEADER LENGTH
	recordLength += sizeof(int);						//RECORD NUMBER
	recordLength += sizeof(int);						//CONTENT LENGTH
	//###	RECORD LENGTH
	recordLength += sizeof(int);						//SHAPE TYPE
	recordLength += 4*sizeof(double);					//BOUNDING BOX
	recordLength += sizeof(int);						//NUMPARTS
	recordLength += sizeof(int);						//NUMPOINTS
	recordLength += sizeof(int)*poly.numParts;			//ARRAY OF PARTS
	recordLength += 2*sizeof(double)*poly.numPoints;	//ARRAY OF POINTS

	recordLength /= 2;		//SET LENGTH FROM # of BYTES to # of 16 bit WORDS
	poly.length = recordLength - sizeof(int);	//recordLength -= (2 * #HEADER LENGTH) / 2
	return recordLength;
}

//This function will find the Header Bounding Box when given a list of Polygons
void RasterToShape::findHeaderBoundingBox( HDR_BoundingBox & hbox, LinkList & allPolygons )
{	
	double box[8];
	box[0] = x_arrayIndexToWorldCoordinateX( mcols );	//xmin
	box[1] = y_arrayIndexToWorldCoordinateY( 0 );		//ymin
	box[2] = xllcenter;									//xmax
	box[3] = yllcenter;									//ymax

	box[4] = 0;
	box[5] = 0;
	box[6] = 0;
	box[7] = 0;		

	Node * curPoly = new Node();
	allPolygons.First( curPoly );	

	double polyBounds[4];
	
	while( curPoly != NULL )
	{
		
		curPoly->data.polygon->bounds.getBounds( polyBounds );

		if( polyBounds[0] < box[0] )		//minX
			box[0] = polyBounds[0];		

		if( polyBounds[1] < box[1] )		//minY
			box[1] = polyBounds[1];		

		if( polyBounds[2] > box[2] )		//maxX
			box[2] = polyBounds[2];

		if( polyBounds[3] > box[3] )		//maxY
			box[3] = polyBounds[3];
		
		allPolygons.Next( curPoly );
	}
	
	hbox.setBounds( box );
}

//This Function will find the Number of Polygon Records in the Link List
int RasterToShape::numberOfRecords( LinkList & allPolygons )
{	int recNum = 0;
	Node * curPoly = new Node();
	allPolygons.First( curPoly );
	while( curPoly != NULL )
	{	recNum++;
		allPolygons.Next( curPoly );
	}	
	return recNum;
}

//This Function will find the Shape File Length on 16 bit WORD boundaries
int RasterToShape::fileLength( LinkList & allPolygons )
{	int fileLength = 0;
	//###	HEADER LENGTH
	fileLength += HEADER_BYTES / 2;
	//###	RECORD LENGTHS
	Node * curPoly = new Node();
	allPolygons.First( curPoly );
	while( curPoly != NULL )
	{	fileLength += recordLength( *(curPoly->data.polygon) );
		allPolygons.Next( curPoly );
	}
	return fileLength;		//# of 16 bit WORDS
}

//This Function will find the Perimeter of the Polygon stored in myPolygon
double RasterToShape::findPerimeter()
{	double perimeter = 0;
	if( myPolygon.size() < 2 )
		return perimeter;
	for( int i = 0; i < myPolygon.size() - 1; i++ )
	{	perimeter += sqrt( pow(myPolygon[i].X - myPolygon[i+1].X,2) + pow(myPolygon[i].Y - myPolygon[i+1].Y,2) );
	}
	return perimeter;
}

//This Function will remove redundant points and also smooth 90 degree angles from the polygon defined in myPolygon
void RasterToShape::removeAndSmooth()
{
	if( cellsize <= 1 )
		return;

	if( myPolygon.size() < 8 )
		return;

	double threshold = 1;

	//Smooth Polygon
	for( int t = 0; t < myPolygon.size() - 2; t++ )
	{	if( !myPolygon[ t + 1 ].isJoint )
		{	if( myPolygon[t].X + threshold > myPolygon[t + 1].X && myPolygon[t].X - threshold < myPolygon[t + 1].X )
			{	if( myPolygon[t + 2].Y + threshold > myPolygon[t + 1].Y && myPolygon[t + 2].Y - threshold < myPolygon[t + 1].Y )
				{	myPolygon.erase( myPolygon.begin() + t + 1 );
				}
			}	
			else if( myPolygon[t].Y + threshold > myPolygon[t + 1].Y && myPolygon[t].Y - threshold < myPolygon[t + 1].Y )
			{	if( myPolygon[t + 2].X + threshold > myPolygon[t + 1].X && myPolygon[t + 2].X - threshold < myPolygon[t + 1].X )
				{	myPolygon.erase( myPolygon.begin() + t + 1 );
				}
			}
		}	
	}
	
	//Smooth Start and End of Polygon
	int size = myPolygon.size();
	if( !myPolygon[0].isJoint )
	{	if( myPolygon[0].X + threshold > myPolygon[size - 2].X && myPolygon[0].X - threshold < myPolygon[size - 2].X )
		{	if( myPolygon[0].Y + threshold > myPolygon[1].Y && myPolygon[0].Y - threshold < myPolygon[1].Y )
			{	myPolygon.erase( myPolygon.begin() );
				myPolygon.erase( myPolygon.begin() + size - 1 );
				myPolygon.push_back( myPolygon[0] );
			}
		}	
		else if( myPolygon[0].Y + threshold > myPolygon[size - 2].Y && myPolygon[t].Y - threshold < myPolygon[size - 1].Y )
		{	if( myPolygon[0].X + threshold > myPolygon[1].X && myPolygon[t + 2].X - threshold < myPolygon[1].X )
			{	myPolygon.erase( myPolygon.begin() );
				myPolygon.erase( myPolygon.begin() + size - 1 );
				myPolygon.push_back( myPolygon[0] );
			}
		}
	}	
		
	//Point Reduction
	for( int i = 0; i < myPolygon.size() - 1; i++ )
	{	if( !myPolygon[ i + 1 ].isJoint )
		{	//HORIZONTAL
			if( myPolygon[i].X + threshold > myPolygon[i + 1].X && myPolygon[i].X - threshold < myPolygon[i + 1].X &&
				myPolygon[i + 1].X + threshold > myPolygon[i + 2].X && myPolygon[i + 1].X - threshold < myPolygon[i + 2].X )
					myPolygon.erase( myPolygon.begin() + i + 1 );
			//VERTICAL
			else if( myPolygon[i].Y + threshold > myPolygon[i + 1].Y && myPolygon[i].Y - threshold < myPolygon[i + 1].Y &&
				myPolygon[i + 1].Y + threshold > myPolygon[i + 2].Y && myPolygon[i + 1].Y - threshold < myPolygon[i + 2].Y )
					myPolygon.erase( myPolygon.begin() + i + 1 );		
			//DIAGONAL
			else
			{
				double deltaxOne = myPolygon[i + 1].X - myPolygon[i].X;
				double deltayOne = myPolygon[i + 1].Y - myPolygon[i].Y;
				double deltaxTwo = myPolygon[i + 1].X - myPolygon[i + 2].X;
				double deltayTwo = myPolygon[i + 1].Y - myPolygon[i + 2].Y;

				if( deltaxOne != 0 && deltaxTwo != 0 )
				{
					double slopeOne = deltayOne/deltaxOne;
					double slopeTwo = deltayTwo/deltaxTwo;
				
					if( slopeOne + threshold > slopeTwo && slopeOne - threshold < slopeTwo )
						myPolygon.erase( myPolygon.begin() + i + 1 );				
				}
			}
		}
	}
}
