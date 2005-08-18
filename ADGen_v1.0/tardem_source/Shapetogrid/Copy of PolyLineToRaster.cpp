//# include "stdafx.h"
# include "PolyLineToRaster.h"
# include "math.h"
# include <fstream>
# include "gridio.h"

using namespace std;

//This function will construct a PolyLineToRaster Object
PolyLineToRas::PolyLineToRas()
{	//OCXControl = p_OCXControl;
	Data = NULL;
	streamData = NULL;
	
	nodata_value = -1;
	ncols = 0;
	nrows = 0;

	streamOutletX = -1;
	streamOutletY = -1;
}

PolyLineToRas::~PolyLineToRas()
{	if( Data != NULL )
	{	delete [] Data;		
		Data = NULL;
	}
	if( streamData != NULL )
	{	delete [] streamData;		
		streamData = NULL;	
	}
	//if( OCXControl != NULL )
	//	OCXControl = NULL;
}

//This function will create the appropriate filenames and then
//This function will create an ASCII data file from a PolyLine file
bool PolyLineToRas::createRasterFile( CString inFilePath, CString demFile, CString outFilePath, CString idColumnName )
{	if( inFilePath.GetLength() <= 0 || demFile.GetLength() <= 0 || outFilePath.GetLength() <= 0 )
	{	//OCXControl->PercentChange( 0, ANYERROR, "Missing Parameter");
		return false;	
	}
	
	CString infile = inFilePath + (CString)".shp";
	CString dbfile = inFilePath + (CString)".dbf";
	CString outfile = outFilePath;
		
	FILE * fileCheck;
	//Create Filename and Check to see if it exists
	//Delete them if they do
	fileCheck = fopen( outfile, "r" );
	if( fileCheck != NULL )
	{	fclose( fileCheck );
		if( _unlink( outfile ) )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete " );		
			strcat( message, outfile );
			//OCXControl->PercentChange( 0, ANYERROR, message);
			return false;
		}
	}

	//Check the infiles for existince
	fileCheck = fopen( infile, "r" );
	if( fileCheck == NULL )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, infile );		
		strcat( message, " Does not Exist" );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;		
	}
	fclose( fileCheck );

	fileCheck = fopen( dbfile, "r" );
	if( fileCheck == NULL )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, dbfile );		
		strcat( message, " Does not Exist" );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;		
	}
	fclose( fileCheck );

/* //dgt	fileCheck = fopen( demFile, "r" );
	if( fileCheck == NULL )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, demFile );		
		strcat( message, " Does not Exist" );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;
	}
	fclose( fileCheck );   */
	
	LinkList * allPolylines;	
	
	if( !shape.InputShapeFile( infile ) )
	{	char * message = new char[MAXBUFFER];
		strcpy( message, "Error reading from " );		
		strcat( message, infile );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;	
	}
	else
		allPolylines = shape.getList();	
	
	//Use the Dem File to get Mapping Extents
//dgt	ifstream dem( demFile );
//	readFileHeader( dem );
//	dem.close();
    {
	char elevfile[MAXLN];
	float **elev,dx,dy;
	int err;
	sprintf(elevfile,"%s",demFile);

    err=gridread(elevfile,(void ***)&elev,RPFLTDTYPE,&ncols,&nrows,&dx,&dy,
        bndbox,&cellsize,&float_nodata_value,&filetype);
	free(elev[0]); free(elev);  //Inneficient - but could be worse.

	xllcenter=bndbox[0]+cellsize*0.5;
	yllcenter=bndbox[1]+cellsize*0.5;
	ylltop = yllcenter + nrows*cellsize;
	xllright = xllcenter + ncols*cellsize;
	}
	
	Node * myNode;
	allPolylines->First( myNode );
	
	int numLines = 0;
	while( myNode != NULL )
	{	allPolylines->Next( myNode );
		numLines++;
	}	
	
	streamData = new stream[ ncols*nrows ];
	if( !streamData )
	{	//OCXControl->PercentChange( 0, ANYERROR, "Could Not Allocate Memory for Flow Direction");
		return false;	
	}
	Joins.clear();
	for( int y = 0; y < nrows; y++ )
		for( int x = 0; x < ncols; x++ )
		{	
			streamData[ indexData( x, y ) ].streamID = nodata_value;
			streamData[ indexData( x, y ) ].segmentNumber = nodata_value;
			streamData[ indexData( x, y ) ].length = 0.0;
		}	
	
	//Map PolyLines into data Array
		
	allPolylines->First( myNode );

	int percent = 0;
	int lineNum = 0;
	int counter = 0;
	double x1, y1, x2, y2;
	double deltax, deltay, dx, dy;
	int record, streamCount, stepAmount;
	int XbeginIndex, YbeginIndex, XendIndex, YendIndex;
	double slope, yintercept;
	int xindex, yindex;

	while( myNode != NULL )
	{
		if( myNode->data.polyline == NULL || myNode->data.polyline->points == NULL )
		{	allPolylines->Next( myNode );
			continue;
		}
		if( myNode->data.polyline->shapeType != POLYLINE )
		{	allPolylines->Next( myNode );
			continue;
		}

		bool insideDEM = false;		
		record = myNode->data.polyline->recNum;
		streamCount = 0;
		stepAmount = 1;

		XbeginIndex = -1;
		YbeginIndex = -1;
		XendIndex = -1;
		YendIndex = -1;

		for( int i = 0; i < myNode->data.polyline->numPoints - 1; i++ )
		{	
			x1 = myNode->data.polyline->points[i].X;
			y1 = myNode->data.polyline->points[i].Y;
			
			//A two-point line
			if( i <= myNode->data.polyline->numPoints - 2 )
			{	x2 = myNode->data.polyline->points[i+1].X;
				y2 = myNode->data.polyline->points[i+1].Y;
			}
			else
				continue;

			//Make a check to see if they exist in the smaller map Grip
			
			if( y1 > ylltop && y2 > ylltop )
				continue;	
			else if( y1 < yllcenter && y2 < yllcenter )
				continue;
			else if( x1 > xllright && x2 > xllright )
				continue;
			else if( x1 < xllcenter && x2 < xllcenter )
				continue;

			insideDEM = true;
				
			deltax = x1 - x2;
			deltay = y1 - y2;

			dx = deltax;
			dy = deltay;
			if( dx < 0 )
				dx *= - 1;
			if( dy < 0 )
				dy *= -1;
			
			
			//XDominant
			if( dx > dy && dx != 0.0 )
			{	slope = deltay / deltax;
				yintercept = y1 -slope*x1;
				
				if( x1 >= x2 )
				{	
					while( x1 >= x2 )
					{	double y = slope*x1 + yintercept;
						xindex = WorldCoordinateXtoX_arrayIndex( x1 );
						yindex = WorldCoordinateYtoY_arrayIndex( y );		
						
						if( isValid( xindex, yindex ) )
							streamData[ indexData( xindex, yindex ) ].length += stepAmount;
					
						if( XbeginIndex != -1 )
						{							
								if( isValid( xindex, yindex ) )
								{
									XendIndex = xindex;
									YendIndex = yindex;
										
									//Mark new Cells
									if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
									{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
										{	streamData[ indexData( xindex, yindex )].streamID = record;
											streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;											
										}
									}
									
									//Log old cells
									else if( streamData[ indexData( xindex, yindex )].streamID != record )
									{	
										//Satisfies Length??
										if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
										{	
											//Don't save join information
											//if( xindex != XbeginIndex || yindex != YbeginIndex )
											{	stream at;
												at.endSegmentX = xindex;
												at.endSegmentY = yindex;
												at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
												at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
												doubleCellMap.push_back( at );
										
												streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
										
												stream dc;
												dc.endSegmentX = xindex;
												dc.endSegmentY = yindex;
												dc.segmentNumber = streamCount++;
												dc.streamID = record;
												doubleCellMap.push_back( dc );
											}
										}
									}									
		
								}							
						}
						//Beginning of Stream
						else
						{	
								xindex = WorldCoordinateXtoX_arrayIndex( x1, false );
								yindex = WorldCoordinateYtoY_arrayIndex( y, false );

								if( isValid( xindex, yindex ) )
								{	
									XbeginIndex = xindex;
									YbeginIndex = yindex;
									
									streamData[ indexData( xindex, yindex )].streamID = record;
									streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																																												
								}
						}

						x1 -= stepAmount;
						
					}
					//End of Stream Segment
					xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
					yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

					if( isValid( xindex, yindex ) )
					{
						XendIndex = xindex;
						YendIndex = yindex;

						if( streamData[ indexData( xindex, yindex )].streamID != record )
						{	streamData[ indexData( xindex, yindex )].streamID = record;
							streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
						}												
					}
				}
				else
				{	
					while( x1 <= x2 )
					{	double y = slope*x1 + yintercept;
						xindex = WorldCoordinateXtoX_arrayIndex( x1 );
						yindex = WorldCoordinateYtoY_arrayIndex( y );

						if( isValid( xindex, yindex ) )
							streamData[ indexData( xindex, yindex ) ].length += stepAmount;
											
						if( XbeginIndex != - 1 )
						{
								if( isValid( xindex, yindex ) )
								{
									XendIndex = xindex;
									YendIndex = yindex;

									//Mark New Cells
									if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
									{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
										{	streamData[ indexData( xindex, yindex )].streamID = record;
											streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;											
										}
									}

									//Log old Cells
									else if( streamData[ indexData( xindex, yindex )].streamID != record )
									{	
										//Satisfies Length??
										if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
										{
											//Dont' Save Join information
											//if( xindex != XbeginIndex || yindex != YbeginIndex )
											{	stream at;
												at.endSegmentX = xindex;
												at.endSegmentY = yindex;
												at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
												at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
												doubleCellMap.push_back( at );
											
												streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
											
												stream dc;
												dc.endSegmentX = xindex;
												dc.endSegmentY = yindex;
												dc.segmentNumber = streamCount++;
												dc.streamID = record;
												doubleCellMap.push_back( dc );												
											}
										}
									}
									
								}							
						}
						//Beginning of Stream
						else
						{	xindex = WorldCoordinateXtoX_arrayIndex( x1, false );
							yindex = WorldCoordinateYtoY_arrayIndex( y, false );
							
							if( isValid( xindex, yindex ) )
							{	
								XbeginIndex = xindex;
								YbeginIndex = yindex;
								
								streamData[ indexData( xindex, yindex )].streamID = record;
								streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																									
							}
						}

						x1 += stepAmount;
						
					}
					//End of the Stream Segment
					xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
					yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

					if( isValid( xindex, yindex ) )
					{	
						XendIndex = xindex;
						YendIndex = yindex;

						if( streamData[ indexData( xindex, yindex )].streamID != record )
						{	streamData[ indexData( xindex, yindex )].streamID = record;
							streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
						}																	
					}
				}
			}
			//YDominant
			else
			{
				if( dx == 0.0 )
				{
					if( y1 >= y2 )
					{	xindex = WorldCoordinateXtoX_arrayIndex( x1 );					
						while( y1 >= y2 )
						{	yindex = WorldCoordinateYtoY_arrayIndex( y1 );

							if( isValid( xindex, yindex ) )
								streamData[ indexData( xindex, yindex ) ].length += stepAmount;
							
							if( XbeginIndex != -1 )
							{		
									if( isValid( xindex, yindex ) )
									{
										XendIndex = xindex;
										YendIndex = yindex;

										//Mark new information
										if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
										{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;												
											}
										}

										//Log old cells
										else if( streamData[ indexData( xindex, yindex )].streamID != record )
										{
											//Satisfies Length??
											if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	
												//Dont' Save Join information
												//if( xindex != XbeginIndex || yindex != YbeginIndex )
												{	stream at;
													at.endSegmentX = xindex;
													at.endSegmentY = yindex;
													at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
													at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
													doubleCellMap.push_back( at );
												
													streamData[ indexData( xindex, yindex )].streamID = record;
													streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
												
													stream dc;
													dc.endSegmentX = xindex;
													dc.endSegmentY = yindex;
													dc.segmentNumber = streamCount++;
													dc.streamID = record;
													doubleCellMap.push_back( dc );

												}
											}
										}
										
									}								
							}
							//Beginning of Stream
							else
							{	xindex = WorldCoordinateXtoX_arrayIndex( x1, false );
								yindex = WorldCoordinateYtoY_arrayIndex( y1, false );

								if( isValid( xindex, yindex ) )
								{
									XbeginIndex = xindex;
									YbeginIndex = yindex;
									
									streamData[ indexData( xindex, yindex )].streamID = record;
									streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;									
																			
								}
							}

							y1 -= stepAmount;
						}
						//End of Stream Segment
						xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
						yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

						if( isValid( xindex, yindex ) )
						{
							XendIndex = xindex;
							YendIndex = yindex;							

							if( streamData[ indexData( xindex, yindex )].streamID != record )
							{	streamData[ indexData( xindex, yindex )].streamID = record;
								streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
							}															
						}
					}
					else
					{	xindex = WorldCoordinateXtoX_arrayIndex( x1 );
						
						while( y1 <= y2 )
						{	yindex = WorldCoordinateYtoY_arrayIndex( y1 );
							
							if( isValid( xindex, yindex ) )
								streamData[ indexData( xindex, yindex ) ].length += stepAmount;
						
							if( XbeginIndex != -1 )
							{	
									if( isValid( xindex, yindex ) )
									{
										XendIndex = xindex;
										YendIndex = yindex;

										//Mark new information
										if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
										{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;
											
											}
										}

										//Log old cells
										else if( streamData[ indexData( xindex, yindex )].streamID != record )
										{	
											//Satisfies Length??
											if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	
												//Dont' Save Join information
												//if( xindex != XbeginIndex || yindex != YbeginIndex )
												{	stream at;
													at.endSegmentX = xindex;
													at.endSegmentY = yindex;
													at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
													at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
													doubleCellMap.push_back( at );
												
													streamData[ indexData( xindex, yindex )].streamID = record;
													streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
												
													stream dc;
													dc.endSegmentX = xindex;
													dc.endSegmentY = yindex;
													dc.segmentNumber = streamCount++;
													dc.streamID = record;
													doubleCellMap.push_back( dc );

												
												}
											}
										}
										
									}																	
							}
							else
							{	xindex = WorldCoordinateXtoX_arrayIndex( x1, false );
								yindex = WorldCoordinateYtoY_arrayIndex( y1, false );
								
								if( isValid( xindex, yindex ) )
								{
									XbeginIndex = xindex;
									YbeginIndex = yindex;
									
									streamData[ indexData( xindex, yindex )].streamID = record;
									streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																								
								}
							}
						
							y1 += stepAmount;							
						}					
						//End of the Stream Segment
						xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
						yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

						if( isValid( xindex, yindex ) )
						{
							XendIndex = xindex;
							YendIndex = yindex;

							if( streamData[ indexData( xindex, yindex )].streamID != record )
							{	streamData[ indexData( xindex, yindex )].streamID = record;
								streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
							}													
						}
					}
				}
				else
				{	double slope = deltay / deltax;
					double yintercept = y1 -slope*x1;
					
					if( y1 >= y2 )
					{	
						while( y1 >= y2 )
						{	double x = (y1 - yintercept)/slope;
							xindex = WorldCoordinateXtoX_arrayIndex( x );
							yindex = WorldCoordinateYtoY_arrayIndex( y1 );
							
							if( isValid( xindex, yindex ) )
								streamData[ indexData( xindex, yindex ) ].length += stepAmount;
						
							if( XbeginIndex != -1 )
							{
							
									if( isValid( xindex, yindex ) )
									{
										XendIndex = xindex;
										YendIndex = yindex;

										//Mark new information
										if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
										{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;
												
											}
										}

										//Log old cells
										else if( streamData[ indexData( xindex, yindex )].streamID != record )
										{	
											//Satisfies Length??
											if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	
												//Dont' Save Join information
												//if( xindex != XbeginIndex || yindex != YbeginIndex )
												{
													stream at;
													at.endSegmentX = xindex;
													at.endSegmentY = yindex;
													at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
													at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
													doubleCellMap.push_back( at );
												
													streamData[ indexData( xindex, yindex )].streamID = record;
													streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
												
													stream dc;
													dc.endSegmentX = xindex;
													dc.endSegmentY = yindex;
													dc.segmentNumber = streamCount++;
													dc.streamID = record;
													doubleCellMap.push_back( dc );

												}
											}
										}
									
									}								
							}
							//Beginning of Stream
							else
							{	
									xindex = WorldCoordinateXtoX_arrayIndex( x, false );
									yindex = WorldCoordinateYtoY_arrayIndex( y1, false );

									if( isValid( xindex, yindex ) )
									{
										XbeginIndex = xindex;
										YbeginIndex = yindex;
										
										streamData[ indexData( xindex, yindex )].streamID = record;
										streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																													
									}
							}

							y1 -= stepAmount;							
						}
						//Set the end Point
						xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
						yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

						if( isValid( xindex, yindex ) )
						{
							XendIndex = xindex;
							YendIndex = yindex;							
										
							if( streamData[ indexData( xindex, yindex )].streamID != record )
							{	streamData[ indexData( xindex, yindex )].streamID = record;
								streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
							}
						}
					}
					else
					{	
						while( y1 <= y2 )
						{	double x = ( y1 - yintercept )/slope;
							xindex = WorldCoordinateXtoX_arrayIndex( x );
							yindex = WorldCoordinateYtoY_arrayIndex( y1 );

							if( isValid( xindex, yindex ) )
								streamData[ indexData( xindex, yindex ) ].length += stepAmount;
												
							if( XbeginIndex != - 1 )
							{
									if( isValid( xindex, yindex ) )
									{
										XendIndex = xindex;
										YendIndex = yindex;

										//Mark new information
										if( streamData[ indexData( xindex, yindex )].streamID == nodata_value )
										{	if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	streamData[ indexData( xindex, yindex )].streamID = record;
												streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;
													
											}
										}

										//Log old cells
										else if( streamData[ indexData( xindex, yindex )].streamID != record )
										{	
											//Satisfies Length??
											if( streamData[ indexData( xindex, yindex )].length >= cellsize/2 )
											{	
												//Dont' Save Join information
												//if( xindex != XbeginIndex || yindex != YbeginIndex )
												{
													stream at;
													at.endSegmentX = xindex;
													at.endSegmentY = yindex;
													at.segmentNumber = streamData[ indexData( xindex, yindex ) ].segmentNumber;
													at.streamID = streamData[ indexData( xindex, yindex ) ].streamID;
													doubleCellMap.push_back( at );
												
													streamData[ indexData( xindex, yindex )].streamID = record;
													streamData[ indexData( xindex, yindex )].segmentNumber = streamCount;										
												
													stream dc;
													dc.endSegmentX = xindex;
													dc.endSegmentY = yindex;
													dc.segmentNumber = streamCount++;
													dc.streamID = record;
													doubleCellMap.push_back( dc );													
												}
											}
										}										
									}							
							}
							else
							{	xindex = WorldCoordinateXtoX_arrayIndex( x, false );
								yindex = WorldCoordinateYtoY_arrayIndex( y1, false );

								if( isValid( xindex, yindex ) )
								{															
									XbeginIndex = xindex;
									YbeginIndex = yindex;
									
									streamData[ indexData( xindex, yindex )].streamID = record;
									streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																											
								}								
							}

							y1 += stepAmount;														
						}
						//Set the end Point
						xindex = WorldCoordinateXtoX_arrayIndex( x2, false );
						yindex = WorldCoordinateYtoY_arrayIndex( y2, false );

						if( isValid( xindex, yindex ) )
						{
							XendIndex = xindex;
							YendIndex = yindex;

							if( streamData[ indexData( xindex, yindex )].streamID != record )
							{	streamData[ indexData( xindex, yindex )].streamID = record;
								streamData[ indexData( xindex, yindex )].segmentNumber = streamCount++;																							
							}																				
						}
					}	
				}
			}							
		}
		lineNum++;

		//Flow File specific
		if( insideDEM )
		{	
			//Possibly get the correct validate outlet for Dan!!!!
			CString dbfile = inFilePath + ".dbf";
			if( getStreamID( dbfile, idColumnName, record ) == sOutID )
			{	streamOutletX = XendIndex;
				streamOutletY = YendIndex;
				outletEndSegment = streamCount - 1;
			}

				bool beginMarked = false;
				bool endMarked = false;
				for( int i = 0; i < Joins.size(); i++ )
				{	if( XbeginIndex == Joins[i].getX() && YbeginIndex == Joins[i].getY() )
					{	stream s;
						s.streamID = record;
						s.segmentNumber = 0;
						s.endSegmentX = XendIndex;
						s.endSegmentY = YendIndex;

						if( XbeginIndex >= 0 && XbeginIndex < ncols && YbeginIndex >= 0 && YbeginIndex < nrows )
							Joins[i].addStream( s );
						beginMarked = true;
						if( beginMarked && endMarked )
							break;
					}
					else if( XendIndex == Joins[i].getX() && YendIndex == Joins[i].getY() )
					{	stream s;
						s.streamID = record;
						s.segmentNumber = streamCount - 1;
						s.endSegmentX = XbeginIndex;
						s.endSegmentY = YbeginIndex;
						if( XendIndex >= 0 && XendIndex < ncols && YendIndex >= 0 && YendIndex < nrows )
							Joins[i].addStream( s );
						endMarked = true;
						if( beginMarked && endMarked )
							break;
					}
				}
				if( !beginMarked )
				{	StreamJoin sj( XbeginIndex, YbeginIndex );
					stream s;
					s.streamID = record;
					s.segmentNumber = 0;
					s.endSegmentX = XendIndex;
					s.endSegmentY = YendIndex;
					sj.addStream( s );
					if( XbeginIndex >= 0 && XbeginIndex < ncols && YbeginIndex >= 0 && YbeginIndex < nrows )
						Joins.push_back( sj );
				}
				if( !endMarked )
				{	StreamJoin sj( XendIndex, YendIndex );
					stream s;
					s.streamID = record;
					s.segmentNumber = streamCount - 1;
					s.endSegmentX = XbeginIndex;
					s.endSegmentY = YbeginIndex;
					sj.addStream( s );
					if( XendIndex >= 0 && XendIndex < ncols && YendIndex >= 0 && YendIndex < nrows )
						Joins.push_back( sj );
				}			
		}
		
		int newpercent = (((double)lineNum/numLines)*100);
		if( newpercent > percent )
		{	percent = newpercent;
			//OCXControl->PercentChange( percent, CALCULATE, "Calculating" );
		}		
		
		allPolylines->Next( myNode );
	}

	writeRasterFile( outfile );
	return true;
}

CString PolyLineToRas::getStreamID( const char * dbfPath, const char * idColumnName, int recordNum )
{
	DBFHandle dbf = DBFOpen( dbfPath, "rb" );

	//Index DBF From record 0
	recordNum -= 1;

	char * record = new char[MAXBUFFER];
	itoa( recordNum, record, 10 );

	char * fieldName = new char[MAXBUFFER * 2];
	int fieldNumber = -1;
	for( int i = 0; i < DBFGetFieldCount( dbf ); i++ )
	{	
		DBFGetFieldInfo( dbf, i, fieldName, NULL, NULL );
		if( !strcmp( fieldName, idColumnName ) )
		{	fieldNumber = i;
			break;
		}
	}
	if( fieldNumber < 0 )
	{	DBFClose( dbf );	
		return record;
	}
	int totalNumberRecords = DBFGetRecordCount( dbf );

	if( recordNum >= totalNumberRecords || recordNum < 0 )
	{	DBFClose( dbf );	
		return record;
	}
	CString streamID = DBFReadStringAttribute( dbf, recordNum, fieldNumber );		
	DBFClose( dbf );	
	return streamID;
}

void PolyLineToRas::markOutlet( int x, int y, int streamID, int segment, int endSegmentX, int endSegmentY, int * demArray, DIRECTION d )
{	

	if( x != endSegmentX || y != endSegmentY )
	{	if( x >= 0 && x < ncols && y >= 0 && y < nrows )
		{	if( demArray[ indexData( x, y ) ] > nodata_value )  //dgt changed != to >
				{	streamOutletX = x;
					streamOutletY = y;
					return;
				}	
		}	
	}

	if( d == DOWN )
		segment--;
	else
		segment++;
	
	//Trace Opposite Directions
	if( (x + 1 < ncols && streamData[ indexData( x + 1 , y  ) ].streamID == streamID && streamData[ indexData( x + 1, y ) ].segmentNumber == segment ) )
	{	markOutlet( x + 1, y, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (x + 1 < ncols && y - 1 >= 0 && streamData[ indexData( x + 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y - 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x + 1, y - 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (y - 1 >= 0 && streamData[ indexData( x, y - 1 ) ].streamID == streamID && streamData[ indexData( x, y - 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x, y - 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (x - 1 >= 0 && y - 1 >= 0 && streamData[ indexData( x - 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y - 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x - 1, y - 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (x - 1 >= 0 && streamData[ indexData( x - 1, y ) ].streamID == streamID && streamData[ indexData( x - 1, y ) ].segmentNumber == segment ) )
	{	markOutlet( x - 1, y, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (x - 1 >= 0 && y + 1 < nrows && streamData[ indexData( x - 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y + 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x - 1, y + 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (y + 1 < nrows && streamData[ indexData( x, y + 1 ) ].streamID == streamID && streamData[ indexData( x, y + 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x, y + 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
	else if( (x + 1 < ncols && y + 1 < nrows && streamData[ indexData( x + 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y + 1 ) ].segmentNumber == segment ) )
	{	markOutlet( x + 1, y + 1, streamID, segment, endSegmentX, endSegmentY, demArray, d );
	}
}

bool PolyLineToRas::findValidDemValue( int x, int y, int streamID, int segment, int & newX, int & newY, int & newSegment, DIRECTION d, int * demArray )
{	
	if( x >= 0 && x < ncols && y >= 0 && y < nrows )
	{	if( demArray[ indexData( x, y ) ] > nodata_value )  //dgt changed != to >
			{	newX = x;
				newY = y;
				newSegment = segment;
				return true;
			}	
	
		if( d == DOWN )
			segment--;
		else
			segment++;
		
		//Trace Opposite Directions
		if( (x + 1 < ncols && streamData[ indexData( x + 1 , y  ) ].streamID == streamID && streamData[ indexData( x + 1, y ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x + 1, y, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (x + 1 < ncols && y - 1 >= 0 && streamData[ indexData( x + 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y - 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x + 1, y - 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (y - 1 >= 0 && streamData[ indexData( x, y - 1 ) ].streamID == streamID && streamData[ indexData( x, y - 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x, y - 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (x - 1 >= 0 && y - 1 >= 0 && streamData[ indexData( x - 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y - 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x - 1, y - 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (x - 1 >= 0 && streamData[ indexData( x - 1, y ) ].streamID == streamID && streamData[ indexData( x - 1, y ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x - 1, y, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (x - 1 >= 0 && y + 1 < nrows && streamData[ indexData( x - 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y + 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x - 1, y + 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (y + 1 < nrows && streamData[ indexData( x, y + 1 ) ].streamID == streamID && streamData[ indexData( x, y + 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x, y + 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
		else if( (x + 1 < ncols && y + 1 < nrows && streamData[ indexData( x + 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y + 1 ) ].segmentNumber == segment ) )
		{	return findValidDemValue( x + 1, y + 1, streamID, segment, newX, newY, newSegment, d, demArray );
		}
	}
	return false;
}

//This function will write the ASCII data to a file
void PolyLineToRas::writeRasterFile( CString outfile )
{	
	char fname[MAXLN],fname2[MAXLN];
	float dx=cellsize;
	float dy=cellsize;
	int total = nrows*ncols;
	int ** dir,err;
	dir = (int **) matalloc(ncols, nrows, RPINTDTYPE);
	for( int y = 0; y < nrows; y++ )
	{	for( int x = 0; x < ncols; x++ )
		{	
			if( streamData[ indexData( x, y ) ].length >= cellsize/2 )
				dir[x][y]=streamData[ indexData( x, y ) ].streamID;
			else
				dir[x][y]= -1;
		}
	}
	sprintf(fname,"%s",outfile);
	int nmain=nameadd(fname2,fname,"p");
	err=gridwrite(fname2, (void **)dir,RPINTDTYPE,ncols,nrows,dx,dy,
       bndbox,cellsize, -1, filetype);
	free(dir[0]);  free(dir);
/*  //dgt	
	ofstream outf( outfile );
	//Header
	outf<<"ncols "<<ncols<<endl;
	outf<<"nrows "<<nrows<<endl;
	outf<<"xllcenter "<<xllcenter<<endl;
	outf<<"yllcenter "<<yllcenter<<endl;
	outf<<"cellsize "<<cellsize<<endl;
	outf<<"nodata_value "<<nodata_value<<endl;
	
	//OCXControl->PercentChange( 0, WRITE, "Write File" );	
	int percent = 0;
	int total = nrows*ncols;
	for( int y = 0; y < nrows; y++ )
	{	for( int x = 0; x < ncols; x++ )
		{	
			if( streamData[ indexData( x, y ) ].length >= cellsize/2 )
				outf<<streamData[ indexData( x, y ) ].streamID<<" ";
			else
				outf<<nodata_value<<" ";

			int newpercent = ((double)(y*x)/(total))*100;
			if( newpercent > percent )
			{	percent = newpercent;
				//OCXControl->PercentChange( percent, WRITE, "Write File" );	
			}		
		}	
		outf<<endl;					
	}
		
	outf.close();  */
}

double PolyLineToRas::minValue( double one, double two )
{	if( one < two )
		return one;
	return two;
}

double PolyLineToRas::maxValue( double one, double two )
{	if( one > two )
		return one;
	return two;
}

//This function will return an index into Data
inline int PolyLineToRas::indexData( int x, int y )
{	return y*ncols + x;
}

//This function will convert an x array index into a world coordinate
inline double PolyLineToRas::X_ArrayIndexToWorldCoordinateX( int x )
{	
	return floor(( x )*( cellsize ) + xllcenter);
}

//This function will convert a y array index into a world coordinate
inline double PolyLineToRas::Y_ArrayIndexToWorldCoordinateY( int y )
{	
	return floor(( nrows - y - 1 )*( cellsize ) + yllcenter);
}

inline int PolyLineToRas::round( double d )
{	if( ceil(d) - d <= .5 )
		return (int)ceil(d);
	else
		return (int)floor(d);
}

//This function will return the appropriate index to use for a worldX Coordinate
inline int PolyLineToRas::WorldCoordinateXtoX_arrayIndex( double worldX, bool variation )
{	//worldX - xllcenter			# difference from reference
	//( expression )/cellsize		# map to array index
	int indexCenter = round((worldX - xllcenter)/cellsize);

	if( variation )
	{
		double variationCenter = X_ArrayIndexToWorldCoordinateX( indexCenter ) - worldX;
		double variationLeft = X_ArrayIndexToWorldCoordinateX( indexCenter - 1 ) - worldX;
		double variationRight = X_ArrayIndexToWorldCoordinateX( indexCenter + 1 ) - worldX;

		if( variationCenter < 0 )
			variationCenter *= - 1;
		if( variationLeft < 0 )
			variationLeft *= -1;
		if( variationRight < 0 )
			variationRight *= -1;

		if( variationLeft < variationCenter )
			indexCenter -= 1;
		else if( variationRight < variationCenter )
			indexCenter += 1;
	}		
	
	return indexCenter;
}

//This function will return the appropriate index to use for a worldY Coordinate
inline int PolyLineToRas::WorldCoordinateYtoY_arrayIndex( double worldY, bool variation )
{	
	//nrows - expression			# reverse arrayIndex
	//worldY - yllcenter			# difference from reference
	//( expression )/cellsize		# map to array index
	int indexCenter = nrows - 1 - round((worldY - yllcenter)/cellsize);

	if( variation )
	{	double variationCenter = Y_ArrayIndexToWorldCoordinateY( indexCenter ) - worldY;
		double variationTop = Y_ArrayIndexToWorldCoordinateY( indexCenter -1 ) - worldY;
		double variationBottom = Y_ArrayIndexToWorldCoordinateY( indexCenter + 1 ) - worldY;

		if( variationCenter < 0 )
			variationCenter *= - 1;
		if( variationTop < 0 )
			variationTop *= -1;
		if( variationBottom < 0 )
			variationBottom *= -1;

		if( variationTop < variationCenter )
			indexCenter -= 1;
		else if( variationBottom < variationCenter )
			indexCenter += 1;
	}

	return indexCenter;

}

void PolyLineToRas::exploreStream( int x, int y, int streamID, int segment, int endSegmentX, int endSegmentY, DIRECTION d, bool isMaster, bool & endMarked )
{	
	if( x < 0 || y < 0 )
		return;
	else if( x >= ncols || y >= nrows )
		return;

	streamData[ indexData( x, y ) ].streamID = nodata_value;
	
	if( d == UP )
		segment++;
	else
		segment--;

	if( isMaster )
	{		
		if( (x + 1 < ncols && streamData[ indexData( x + 1 , y  ) ].streamID == streamID && streamData[ indexData( x + 1, y ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = RIGHT;
				exploreStream( x + 1, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x + 1 < ncols && y - 1 >= 0 && streamData[ indexData( x + 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = UPRIGHT;
				exploreStream( x + 1, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}					
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (y - 1 >= 0 && streamData[ indexData( x, y - 1 ) ].streamID == streamID && streamData[ indexData( x, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = UP;
				exploreStream( x, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && y - 1 >= 0 && streamData[ indexData( x - 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = UPLEFT;
				exploreStream( x - 1, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && streamData[ indexData( x - 1, y ) ].streamID == streamID && streamData[ indexData( x - 1, y ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = LEFT;
				exploreStream( x - 1, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && y + 1 < nrows && streamData[ indexData( x - 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = DOWNLEFT;
				exploreStream( x - 1, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (y + 1 < nrows && streamData[ indexData( x, y + 1 ) ].streamID == streamID && streamData[ indexData( x, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = DOWN;
				exploreStream( x, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x + 1 < ncols && y + 1 < nrows && streamData[ indexData( x + 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y ) ] = DOWNRIGHT;
				exploreStream( x + 1, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}

		//Ending Segments
		if( x + 1 == endSegmentX && y == endSegmentY && !endMarked  )
		{	Data[ indexData( x, y ) ] = RIGHT;
			endMarked = true;
			return;			
		}
		else if( x + 1 == endSegmentX && y - 1 == endSegmentY && !endMarked  )
		{	Data[ indexData( x, y ) ] = UPRIGHT;
			endMarked = true;
			return;				
		}
		else if( x == endSegmentX && y - 1 == endSegmentY && !endMarked  )
		{	Data[ indexData( x, y ) ] = UP;
			endMarked = true;
			return;			
		}
		else if( x - 1 == endSegmentX && y - 1 == endSegmentY && !endMarked  )
		{	Data[ indexData( x, y ) ] = UPLEFT;
			endMarked = true;
			return;			
		}
		else if( x - 1 == endSegmentX && y == endSegmentY && !endMarked  )
		{	Data[ indexData( x, y ) ] = LEFT;
			endMarked = true;
			return;			
		}
		else if( x - 1 == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x, y ) ] = DOWNLEFT;
			endMarked = true;
			return;
		}
		else if( x == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x, y ) ] = DOWN;
			endMarked = true;
			return;
		}
		else if( x + 1 == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x, y ) ] = DOWNRIGHT;
			endMarked = true;
			return;
		}

	}
	//Trace Opposite Directions
	else
	{
		if( (x + 1 < ncols && streamData[ indexData( x + 1 , y  ) ].streamID == streamID && streamData[ indexData( x + 1, y ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y ) ].length >= cellsize/2 )
			{	Data[ indexData( x + 1, y ) ] = LEFT;
				exploreStream( x + 1, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x + 1 < ncols && y - 1 >= 0 && streamData[ indexData( x + 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x + 1, y - 1 ) ] = DOWNLEFT;
				exploreStream( x + 1, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (y - 1 >= 0 && streamData[ indexData( x, y - 1 ) ].streamID == streamID && streamData[ indexData( x, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y - 1) ] = DOWN;
				exploreStream( x, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && y - 1 >= 0 && streamData[ indexData( x - 1, y - 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y - 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y - 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x - 1, y - 1 ) ] = DOWNRIGHT;
				exploreStream( x - 1, y - 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && streamData[ indexData( x - 1, y ) ].streamID == streamID && streamData[ indexData( x - 1, y ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y ) ].length >= cellsize/2 )
			{	Data[ indexData( x - 1, y ) ] = RIGHT;
				exploreStream( x - 1, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x - 1 >= 0 && y + 1 < nrows && streamData[ indexData( x - 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x - 1, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x - 1, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x - 1, y + 1 ) ] = UPRIGHT;
				exploreStream( x - 1, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (y + 1 < nrows && streamData[ indexData( x, y + 1 ) ].streamID == streamID && streamData[ indexData( x, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x, y + 1) ] = UP;
				exploreStream( x, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}
		else if( (x + 1 < ncols && y + 1 < nrows && streamData[ indexData( x + 1, y + 1 ) ].streamID == streamID && streamData[ indexData( x + 1, y + 1 ) ].segmentNumber == segment ) )
		{	if( streamData[ indexData( x + 1, y + 1 ) ].length >= cellsize/2 )
			{	Data[ indexData( x + 1, y + 1 ) ] = UPLEFT;
				exploreStream( x + 1, y + 1, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
			}			
			else
				exploreStream( x, y, streamID, segment, endSegmentX, endSegmentY, d, isMaster, endMarked );
		}

		//Ending Segments
		if( x + 1 == endSegmentX && y == endSegmentY && !endMarked ) 
		{	Data[ indexData( x + 1, y ) ] = LEFT;
			endMarked = true;
			return;
		}
		else if( x + 1 == endSegmentX && y - 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x + 1, y - 1 ) ] = DOWNLEFT;
			endMarked = true;
			return;
		}
		else if( x == endSegmentX && y - 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x, y - 1) ] = DOWN;
			endMarked = true;
			return;
		}
		else if( x - 1 == endSegmentX && y - 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x - 1, y - 1 ) ] = DOWNRIGHT;
			endMarked = true;
			return;
		}
		else if( x - 1 == endSegmentX && y == endSegmentY ) 
		{	Data[ indexData( x - 1, y ) ] = RIGHT;
			endMarked = true;
			return;
		}
		else if( x - 1 == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x - 1, y + 1 ) ] = UPRIGHT;
			endMarked = true;
			return;
		}
		else if( x == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x, y + 1) ] = UP;
			endMarked = true;
			return;
		}
		else if( x + 1 == endSegmentX && y + 1 == endSegmentY && !endMarked  ) 
		{	Data[ indexData( x + 1, y + 1 ) ] = UPLEFT;
			endMarked = true;
			return;
		}
	}
}

//This function will create an ASCII Flow Direction file
bool PolyLineToRas::createFlow( const char * idColumnName, const char * downPointer, const char * dbfPath, int * demArray, const char * streamOutletID )
{	
	int percent = 0;
	
	//OCXControl->PercentChange( 1, CALCULATE, "Calculating Flow File");

	
	deque<StreamJoin> singles;
	for( int n = 0; n < Joins.size();)
	{	if( Joins[n].getNumStreams() <= 1 )
		{	singles.push_back( Joins[n] );
			Joins.erase( Joins.begin() + n );
		}
		else
			n++;
	}

	//
	bool outletMarked = false;
	int total = Joins.size() + singles.size();
	percent = 0;
	for( int i = 0; i < Joins.size(); i++ )
	{	
			DIRECTION cntDirection = NONE;
			int masterx = 0;
			int mastery = 0;
			int segment = 0;
			int masterEndX = 0;
			int masterEndY = 0;

			stream master = Joins[i].getMasterStream( idColumnName, downPointer, dbfPath );
			
			for( int j = 0; j < Joins[i].getNumStreams(); j++ )
			{	stream current = Joins[i].getStream( j );
				int x =	Joins[i].getX();
				int y = Joins[i].getY();
				segment = current.segmentNumber;
				int endX = current.endSegmentX;
				int endY = current.endSegmentY;
				
				cntDirection = NONE;
				if( segment == 0 )
					cntDirection = UP;
				else
					cntDirection = DOWN;

				bool isMaster = false;
				if( current.streamID == master.streamID )
				{	masterx = x;
					mastery = y;
					masterEndX = current.endSegmentX;
					masterEndY = current.endSegmentY;
					isMaster = true;
				}
				else if( master.streamID == -1 )
				{
					remarkCells( current.streamID );
					/*
					//Make sure the beg/end is not on a nodata_value
					if( demArray[ indexData( x, y ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;
						
						if( findValidDemValue( x, y, current.streamID, streamData[ indexData( x, y ) ].segmentNumber, newX, newY, newSegment, cntDirection, demArray ) )
						{
							segment = newSegment;
							x = newX;
							y = newY;
						}		
					}
					if( demArray[ indexData( endX, endY ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;

						DIRECTION opposite = cntDirection;
						if( opposite == DOWN )
							opposite = UP;
						else
							opposite = DOWN;
						if( findValidDemValue( endX, endY, current.streamID, streamData[ indexData( endX, endY ) ].segmentNumber, newX, newY, newSegment, opposite, demArray ) )
						{	endX = newX;
							endY = newY;
						}
					}
					*/
								
					if( !outletMarked && Joins[i].getStreamID( dbfPath, idColumnName, current.streamID ) == streamOutletID )
					{	//Get Information from the DEM ... this stream could be screwed up!!
						if( demArray[ indexData( x, y ) ] > demArray[ indexData( endX, endY ) ] )
						{	streamOutletX = endX;
							streamOutletY = endY;
						}
						else
						{	streamOutletX = x;
							streamOutletY = y;
						}
					}
					
					bool endMarked = false;
					if( demArray[ indexData( x, y ) ] >= demArray[ indexData( endX, endY ) ] )
					{	//if( demArray[ indexData( x, y ) ] != nodata_value )
							exploreStream( x, y, current.streamID, segment, endX, endY, cntDirection, true, endMarked );
					}
					else
					{	//if( demArray[ indexData( x, y ) ] != nodata_value )
							exploreStream( x, y, current.streamID, segment, endX, endY, cntDirection, false, endMarked );
					}
				}
				else
				{
					remarkCells( current.streamID );
					/*
					//Make sure the beg/end is not on a nodata_value
					if( demArray[ indexData( x, y ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;
						
						if( findValidDemValue( x, y, current.streamID, streamData[ indexData( x, y ) ].segmentNumber, newX, newY, newSegment, cntDirection, demArray ) )
						{
							segment = newSegment;
							x = newX;
							y = newY;
						}		
					}
					if( demArray[ indexData( endX, endY ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;

						DIRECTION opposite = cntDirection;
						if( opposite == DOWN )
							opposite = UP;
						else
							opposite = DOWN;
						if( findValidDemValue( endX, endY, current.streamID, streamData[ indexData( endX, endY ) ].segmentNumber, newX, newY, newSegment, opposite, demArray ) )
						{	endX = newX;
							endY = newY;
						}
					}
					*/

					if( Joins[i].getStreamID( dbfPath, idColumnName, current.streamID ) == streamOutletID )
					{	if( cntDirection == DOWN )
							markOutlet( streamOutletX, streamOutletY, current.streamID, outletEndSegment, streamOutletX, streamOutletY, demArray, cntDirection );
						else
							markOutlet( x, y, current.streamID, 0, x, y, demArray, cntDirection );							
						
						
						outletMarked = true;
					}
					
					bool endMarked = false;
					//if( demArray[ indexData( x, y ) ] != nodata_value )
						exploreStream( x, y, current.streamID, segment, endX, endY, cntDirection, false, endMarked );
				}
			}

			if( master.streamID != -1 )
			{
				cntDirection = NONE;
				segment = master.segmentNumber;
				if( segment == 0 )
					cntDirection = UP;				
				else
					cntDirection = DOWN;
				
				bool endMarked = false;
				remarkCells( master.streamID );
					/*
					//Make sure the beg/end is not on a nodata_value
					if( demArray[ indexData( masterx, mastery ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;
						
						if( findValidDemValue( masterx, mastery, master.streamID, streamData[ indexData( masterx, mastery ) ].segmentNumber, newX, newY, newSegment, cntDirection, demArray ) )
						{
							segment = newSegment;
							masterx = newX;
							mastery = newY;
						}		
					}
					if( demArray[ indexData( masterEndX, masterEndY ) ] == nodata_value )
					{	int newSegment;
						int newX, newY;

						DIRECTION opposite = cntDirection;
						if( opposite == DOWN )
							opposite = UP;
						else
							opposite = DOWN;
						if( findValidDemValue( masterEndX, masterEndY, master.streamID, streamData[ indexData( masterEndX, masterEndY ) ].segmentNumber, newX, newY, newSegment, cntDirection, demArray ) )
						{	masterEndX = newX;
							masterEndY = newY;
						}
					}
					*/

				if( Joins[i].getStreamID( dbfPath, idColumnName, master.streamID ) == streamOutletID )
				{	
					if( cntDirection == DOWN )
						markOutlet( streamOutletX, streamOutletY, master.streamID, outletEndSegment, streamOutletX, streamOutletY, demArray, cntDirection );
					else
						markOutlet(	masterEndX, masterEndY, master.streamID, 0, masterEndX, masterEndY, demArray, cntDirection );
					
					outletMarked = true;
				}
				//if( demArray[ indexData( masterx, mastery ) ] != nodata_value )
					exploreStream( masterx, mastery, master.streamID, segment, masterEndX, masterEndY, cntDirection, true, endMarked );
			}
			
		int newpercent = (double)i/total*100;
		if( newpercent > percent )
		{	percent = newpercent;
			//OCXControl->PercentChange( percent, CALCULATE, "Calculating Flow File");
		}
	}

	for( int g = 0; g < singles.size(); g++ )
	{
		DIRECTION cntDirection = NONE;
		stream current = singles[g].getStream( 0 );
		int x = singles[g].getX();
		int y = singles[g].getY();
		int segment = current.segmentNumber;
		int endX = current.endSegmentX;
		int endY = current.endSegmentY;
		if( segment == 0 )
			cntDirection = UP;
		else
			cntDirection = DOWN;
		
		remarkCells( current.streamID );
			/*
			//Make sure the beg/end is not on a nodata_value
			if( demArray[ indexData( x, y ) ] == nodata_value )
			{	int newSegment;
				int newX, newY;
						
				if( findValidDemValue( x, y, current.streamID, streamData[ indexData( x, y ) ].segmentNumber, newX, newY, newSegment, cntDirection, demArray ) )
				{
					segment = newSegment;
					x = newX;
					y = newY;
				}		
			}
			if( demArray[ indexData( endX, endY ) ] == nodata_value )
			{	int newSegment;
				int newX, newY;

				DIRECTION opposite = cntDirection;
				if( opposite == DOWN )
					opposite = UP;
				else
					opposite = DOWN;
				if( findValidDemValue( endX, endY, current.streamID, streamData[ indexData( endX, endY ) ].segmentNumber, newX, newY, newSegment, opposite, demArray ) )
				{	endX = newX;
					endY = newY;
				}
			}
			*/
					
		if( !outletMarked && singles[i].getStreamID( dbfPath, idColumnName, current.streamID ) == streamOutletID )
		{	//Get Information from the DEM ... this stream could be screwed up!!
			if( demArray[ indexData( x, y ) ] > demArray[ indexData( endX, endY ) ] )
			{	streamOutletX = endX;
				streamOutletY = endY;
			}
			else
			{	streamOutletX = x;
				streamOutletY = y;
			}
		}
		
		bool endMarked = false;
		if( demArray[ indexData( x, y ) ] >= demArray[ indexData( endX, endY ) ] )
		{	
			//if( demArray[ indexData( x, y ) ] != nodata_value )
				exploreStream( x, y, current.streamID, segment, endX, endY, cntDirection, true, endMarked );
		}
		else
		{	//if( demArray[ indexData( x, y ) ] != nodata_value )
				exploreStream( x, y, current.streamID, segment, endX, endY, cntDirection, false, endMarked );
		}

		int newpercent = (double)(g + Joins.size())/total*100;
		if( newpercent > percent )
		{	percent = newpercent;
			//OCXControl->PercentChange( percent, CALCULATE, "Calculating Flow File");
		}
	}		

	return true;
}

void PolyLineToRas::remarkCells( int streamID )
{	
	
	for( int i = 0; i < doubleCellMap.size(); i++ )
	{	if( streamID == doubleCellMap[i].streamID )
		{	
			int x = doubleCellMap[i].endSegmentX;
			int y = doubleCellMap[i].endSegmentY;
			int segment = doubleCellMap[i].segmentNumber;

			streamData[ indexData( doubleCellMap[i].endSegmentX, doubleCellMap[i].endSegmentY ) ].streamID = streamID;
			streamData[ indexData( doubleCellMap[i].endSegmentX, doubleCellMap[i].endSegmentY ) ].segmentNumber = doubleCellMap[i].segmentNumber;
		}
	}
	
}

//This function will create the appropriate filenames and call createRasterFile() and createFlow()
bool PolyLineToRas::createFlowFile( CString inFilePath, CString demFile, CString outFilePath, CString idColumnName, CString downPointer, CString streamOutletID )
{	
	sOutID = streamOutletID;	

	CString outfile = outFilePath; //dgt + "fdr.asc";
	
	FILE * fileCheck;
	//Create Filename and Check to see if it exists
	//Delete them if they do
/*//dgt	fileCheck = fopen( outfile, "r" );
	if( fileCheck != NULL )
	{	fclose( fileCheck );
		if( _unlink( outfile ) )
		{	char * message = new char[MAXBUFFER];
			strcpy( message, "Could not delete ");		
			strcat( message, outfile );
			//OCXControl->PercentChange( 0, ANYERROR, message);
			return false;	
		}
	}   */   // File checks do not work for ESRI grid files which are folders

	CString rasOut = outFilePath;  //dgt + ".asc";
	if( !createRasterFile( inFilePath, demFile, rasOut, idColumnName ) )
		return false;

	//This file has been checked for existence in createRasterFile
	
	CString dbfile = inFilePath + ".dbf";

	//Set Data
	Data = new int[ ncols*nrows ];
	if( !Data )
	{	//OCXControl->PercentChange( 0, ANYERROR, "Could Not Allocate Memory for Raster Data");
		return false;	
	}
	for( int y = 0; y < nrows; y++ )
		for( int x = 0; x < ncols; x++ )
			Data[ indexData( x, y ) ] = nodata_value;


	//Read in DEM File
/* //dgt	ifstream dem( demFile );
	if( !dem )
	{	char * message = new char[ MAXBUFFER ];
		strcpy( message, "Could not open " );
		strcat( message, demFile );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;
	}

	readFileHeader( dem );
  */
    int percent = 0;
	int totalSize = nrows * ncols;
	int * demArray = new int[ totalSize ];
	{
	char elevfile[MAXLN];
	float **elev,dx,dy;
	int err;
	sprintf(elevfile,"%s",demFile);

    err=gridread(elevfile,(void ***)&elev,RPFLTDTYPE,&ncols,&nrows,&dx,&dy,
        bndbox,&cellsize,&float_nodata_value,&filetype);
	if(err)
	{	char * message = new char[ MAXBUFFER ];
		strcpy( message, "Could not open " );
		strcat( message, demFile );
		//OCXControl->PercentChange( 0, ANYERROR, message);
		return false;
	}
	xllcenter=bndbox[0]+cellsize*0.5;
	yllcenter=bndbox[1]+cellsize*0.5;
	ylltop = yllcenter + nrows*cellsize;
	xllright = xllcenter + ncols*cellsize;

	for( int s = 0; s < totalSize; s++ )
		demArray[s] = nodata_value;
	for( int yy = 0; yy < nrows; yy++ )
	{	for( int xx = 0; xx < ncols; xx++ )
		{	
		    demArray[ indexData( xx, yy ) ] = elev[xx][yy];
		}
			
			int newpercent = ((double)xx*yy)/totalSize*100;
			if( newpercent > percent )
			{	percent = newpercent;
				//OCXControl->PercentChange( percent, READ, "Reading Dem File" );
			}
	}   
    free(elev[0]);  free(elev);
	}

	if( createFlow( idColumnName, downPointer, dbfile, demArray, streamOutletID ) )
		writeFlowFile( outfile, demArray );
	else
		return false;

	Joins.clear();

	return true;
}

//This function will write the ASCII streamData to a file
void PolyLineToRas::writeFlowFile( CString outfile, int * demArray )
{	
	char fname[MAXLN];
	float dx=cellsize;
	float dy=cellsize;
	int total = nrows*ncols;
	int err;
	short ** dir;
	dir = (short **) matalloc(ncols, nrows, RPSHRDTYPE);
	for( int y = 0; y < nrows; y++ )
	{	for( int x = 0; x < ncols; x++ )
		{	
			if( Data[ indexData( x, y ) ] > 0)
			dir[x][y]=Data[ indexData( x, y ) ];
			else dir[x][y] = -1;
		}
	}
	sprintf(fname,"%s",outfile);
	err=gridwrite(fname, (void **)dir,RPSHRDTYPE,ncols,nrows,dx,dy,
       bndbox,cellsize, -1, filetype);
	free(dir[0]); free(dir);
/*	ofstream outf( outfile );
	//Header
	outf<<"ncols "<<ncols<<endl;
	outf<<"nrows "<<nrows<<endl;
	outf<<"xllcenter "<<xllcenter<<endl;
	outf<<"yllcenter "<<yllcenter<<endl;
	outf<<"cellsize "<<cellsize<<endl;
	outf<<"nodata_value "<<nodata_value<<endl;
	
	//OCXControl->PercentChange( 0, WRITE, "Write File" );	
	int percent = 0;    
	int total = nrows*ncols;
	for( int y = 0; y < nrows; y++ )
	{	for( int x = 0; x < ncols; x++ )
		{	//outf<<streamData[ indexData( x, y ) ].segmentNumber<<" ";
			//outf<<streamData[ indexData( x, y ) ].length<<" ";
			//if( demArray[ indexData( x, y ) ] != nodata_value )
			outf<<Data[ indexData( x, y ) ]<<" ";
			//else
			//	outf<<nodata_value<<" ";

			int newpercent = ((double)(y*x)/(total))*100;
			if( newpercent > percent )
			{	percent = newpercent;
				//OCXControl->PercentChange( percent, WRITE, "Write File" );	
			}		
		}	
		outf<<endl;					
	}
		
	outf.close();  */
}

//This Function will read the File Header information of inf
bool PolyLineToRas::readFileHeader( istream & inf )
{
	int number_of_variables = 6;
	char * variable_name = new char[MAXBUFFER];
	bool shiftUp = false;
	for( int i = 0; i < number_of_variables; i++ )
	{	inf>>variable_name;
				
		if( !strcmp( variable_name, "ncols" ) )
		{	inf>>ncols;
		}
		else if( !strcmp( variable_name, "nrows" ) )
		{	inf>>nrows;
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
	
	ylltop = yllcenter + nrows*cellsize;
	xllright = xllcenter + ncols*cellsize;

	return true;
}

bool PolyLineToRas::isValid( int x, int y )
{	if( x >= 0 && y >= 0 && x < ncols && y < nrows )
		return true;
	else
		return false;
}

long PolyLineToRas::getStreamOutletX()
{	return (long)X_ArrayIndexToWorldCoordinateX( streamOutletX );				
}

long PolyLineToRas::getStreamOutletY()
{	return (long)Y_ArrayIndexToWorldCoordinateY( streamOutletY );;
}
