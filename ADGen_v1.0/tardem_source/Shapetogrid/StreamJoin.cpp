# include "StreamJoin.h"
# include "types.h"

StreamJoin::StreamJoin( int p_x, int p_y )
{	numStreams = 0;
	x = p_x;
	y = p_y;
}

StreamJoin::StreamJoin( const StreamJoin & sj )
{	numStreams = sj.numStreams;
	for( int i = 0; i < numStreams; i++ )
		streams[i] = sj.streams[i];
	x = sj.x;
	y = sj.y;
}

StreamJoin StreamJoin::operator=( const StreamJoin & sj )
{	
	numStreams = sj.numStreams;
	for( int i = 0; i < numStreams; i++ )
		streams[i] = sj.streams[i];
	x = sj.x;
	y = sj.y;
	return * this;
}

StreamJoin::~StreamJoin()
{
}

int StreamJoin::getNumStreams()
{	return numStreams;
}

stream StreamJoin::getStream( int index )
{	if( index >= 0 && index < numStreams )
		return streams[index];
	
	stream s;
	s.streamID = -1;
	s.segmentNumber = -1;
	return s;
}

int StreamJoin::getIndex( stream s )
{	
	for( int i = 0; i < numStreams; i++ )
	{	if( streams[i].streamID == s.streamID )
			return i;
	}
	return -1;
}

int StreamJoin::getX()
{	return x;
}
int StreamJoin::getY()
{	return y;
}

void StreamJoin::addStream( stream s )
{	if( numStreams >= MAXSTREAMS )
		return;
	for( int i = 0; i < numStreams; i++ )
	{	if( streams[i].streamID == s.streamID )
			return;
	}
	
	streams[ numStreams++ ] = s;
}

void StreamJoin::deleteStream( stream s )
{	for( int i = 0; i < numStreams; i++ )
	{	if( streams[i].streamID == s.streamID )
		{	for( int j = i; j < numStreams; j++ )
			{	streams[j] = streams[j+1];
			}
			numStreams--;
			return;
		}
	}
}

void StreamJoin::setSegment( stream s, int segment )
{	for( int i = 0; i < numStreams; i++ )
	{	if( streams[i].streamID == s.streamID )
		{	streams[i].segmentNumber = segment;
			return;
		}
	}
}

bool StreamJoin::streamExists( stream s )
{	for( int i = 0; i < numStreams; i++ )
	{	if( streams[i].streamID == s.streamID )
			return true;
	}
	return false;
}

//Database Stuff

//This function will return the Main Stream 
stream StreamJoin::getMasterStream( const char * idColumnName, const char * downPointer, const char * dbfPath )
{	
	for( int i = 0; i < numStreams; i++ )
	{	bool masterStream = false;
		CString AstreamPNTR = getStreamID( dbfPath, idColumnName, streams[i].streamID );
		if( AstreamPNTR.GetLength() > 0 )
		{	for( int j = 0; j < numStreams; j++ )
			{	CString BstreamPNTR = getStreamID( dbfPath, idColumnName, streams[j].streamID );
				if( AstreamPNTR == getDownstream( BstreamPNTR, idColumnName, downPointer, dbfPath ) )
					masterStream = true;
			}
		}
		if( masterStream )
			return streams[i];
	}
	stream s;
	s.streamID = -1;
	s.segmentNumber = -1;
	return s;
}

//This function will return the downstream PNTR of a stream
CString StreamJoin::getDownstream( CString sID, const char * idColumnName, const char * downPointer, const char * dbfPath )
{
	DBFHandle dbf = DBFOpen( dbfPath, "rb" );

	char * fieldName = new char[MAXBUFFER * 2];
	int dpfieldNumber = -1;
	int fieldNumber = -1;
	int numberFields = DBFGetFieldCount( dbf );
	for( int i = 0; i < numberFields; i++ )
	{	
		DBFGetFieldInfo( dbf, i, fieldName, NULL, NULL );
		if( !strcmp( fieldName, idColumnName ) )
			fieldNumber = i;			
		else if( !strcmp( fieldName, downPointer ) )
			dpfieldNumber = i;
		
	}
	if( fieldNumber < 0 || dpfieldNumber < 0 )
	{	DBFClose( dbf );	
		return "";
	}
	int totalNumberRecords = DBFGetRecordCount( dbf );
	
	for( int j = 0; j < totalNumberRecords; j++ )
	{	
		CString streamID = DBFReadStringAttribute( dbf, j, fieldNumber );
		CString downstream = DBFReadStringAttribute( dbf, j, dpfieldNumber );
		if( streamID == sID )
		{	DBFClose( dbf );
			return downstream;
		}
	}		
	
	DBFClose( dbf );	
	return "";
}

CString StreamJoin::getStreamID( const char * dbfPath, const char * idColumnName, int recordNum )
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
