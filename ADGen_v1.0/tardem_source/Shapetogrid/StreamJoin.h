# ifndef STREAMJOIN_H
# define STREAMJOIN_H
# include "shapefil.h"
# include <afx.h>

class stream
{	
	public:
	stream()
	{}
	int streamID;
	int segmentNumber;
	float length;
	int endSegmentX;
	int endSegmentY;	
};

class StreamJoin
{
	public:
		StreamJoin( int p_x = 0, int p_y = 0 );
		StreamJoin( const StreamJoin & sj );
		~StreamJoin();
		StreamJoin operator=( const StreamJoin & sj );
		void addStream( stream s );
		void deleteStream( stream s );
		stream getMasterStream( const char * idColumnName, const char * downPointer, const char * dbfPath );
		int getNumStreams();
		stream getStream( int index );
		int getX();
		int getY();
		bool streamExists( stream s );
		int getIndex( stream s );
		void setSegment( stream s, int segment );
		CString getStreamID( const char * dbfPath, const char * idColumnName, int recordNum );
		
	
	private:
		CString getDownstream( CString sID, const char * idColumnName, const char * downPointer, const char * dbfPath );
		stream streams[3];
		int numStreams;
		int x;
		int y;
};

# endif