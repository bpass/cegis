#include "myshp.h"


ShapeFile::ShapeFile():filename(NULL),shapes(NULL),numShapes(0),
					   numWords(0),shapeType(0),isList(true),shallowData(true)
{	
}

ShapeFile::~ShapeFile()
{
	delete [] filename;
	delete [] shapes;

	if(isList && !shallowData) {
		Node* tmp= new Node();
		for(int i=0; i<numShapes && !shapeList.IsEmpty(); i++) {
			shapeList.DeleteHead(tmp);
			switch(shapeType) {
			case POINT:
				delete tmp->data.point;
				tmp->data.point = NULL;
				break;
			case POLYLINE:
				delete tmp->data.polyline;
				tmp->data.polyline = NULL;
				break;
			case POLYGON:
				delete tmp->data.polygon;
				tmp->data.polygon = NULL;
				break;
			default:
				cout << "Undefined shape - deletion uncompleted" << endl;
				return;
			}
		}// end for
		delete tmp;
	}
}


/*  This is for Link List implementation *********************/
bool ShapeFile::InputShapeFile(char* file )//, void (*FireTotalBytes)(int), void (*FireBytesLeft)(int))
{
	if(!setFilename(file))
		return false;
	//open shape file
	FILE* fHnd = fopen(filename, "rb");
	if(!fHnd) {
		cout << "Unable to open " << filename << " for reading" << endl;
		return false;
	}

	//get header info
	if(header.InputHeader(fHnd)== false) {
		cout << "Error in reading file header" << endl;
		return false;
	}

	//get header values needed
	numWords = header.getLength();
	fileLength = numWords*2;
	shapeType = header.getType();

	//Let containter know the size of the file
//	FireTotalBytes(fileLength);

	//local var's for reading in shape records
	int		bytesLeft	= fileLength - HEADER_BYTES;  //total length - header length
	int		recordLen	= 0;
	int		recNum		= 0;
	char	intbuf[sizeof(int)];
	Node*	myNode;

	int percent = 0;
	//keep track of number of records
	numShapes = 0;
	while(bytesLeft > 0) 
	{
		//get record number -- BIG ENDIAN	
		fread(intbuf,sizeof(char),sizeof(int),fHnd);
		SwapEndian(intbuf,sizeof(int));
		recNum = *(int*)intbuf;

		//get content length -- BIG ENDIAN
		fread(intbuf,sizeof(char),sizeof(int),fHnd);
		SwapEndian(intbuf,sizeof(int));
		recordLen = *(int*)intbuf;

		//create node for LinkList and get record info
		myNode = new Node();
		switch(shapeType)
		{
			case POINT:	
				myNode->data.point = new Point1(recNum,recordLen);
				if(myNode->data.point->InputPoint(fHnd) == false)
					return false;
				break;
			case POLYLINE:	
				myNode->data.polyline = new PolyLine(recNum,recordLen);
				if(myNode->data.polyline->InputPolyLine(fHnd) == false)
					return false;
				break;
			case POLYGON: 
				myNode->data.polygon = new PolyGon(recNum,recordLen);
				if(myNode->data.polygon->InputPolyGon(fHnd) == false)
					return false;
				break;
			default:
				cout << "Invalid shape Type" << endl;
				return false;
		}
		
		shapeList.InsertItem(myNode);
		bytesLeft -= (recordLen*2 + REC_HEADER_BYTES);
		numShapes++;

		int newpercent = ((double)( fileLength - bytesLeft )/fileLength )*100;
		if( newpercent > percent )
		{	percent = newpercent;
			//OCXControl->PercentChange( percent, READ, "Reading File" );
		}		
	}
	//OCXControl->PercentChange( 100, READ, "Reading File" );
	
	shallowData = false;
	if(fclose(fHnd))
		return false;
	return true;
}

bool ShapeFile::WriteShapeFile( char* file, int numPolygons, int maxPercent )
{
	//change filename to the file about to be created
	if(!setFilename(file))
		return false;

	FILE* fHnd = fopen(filename, "wb");
	if(!fHnd) {
		cout << "Unable to open " << filename << " for writing" << endl;
		return false;
	}

	//write header info
	if(header.WriteHeader(fHnd)== false) {
		cout << "Error in reading file header" << endl;
		return false;
	}

	//get header values needed
	numWords = header.getLength();
	fileLength = numWords*2;
	shapeType = header.getType();

	int percent = 0;
	int numPolygonsWritten = 0;
	if( numShapes <= 0 )
		numShapes = 1;
	//local var's for writing out shape records
	int		bytesLeft	= fileLength - HEADER_BYTES;  //total length - header length
	Node*	myNode = new Node(); 
	shapeList.First( myNode );
	while(myNode != NULL) 
	{
		switch(shapeType)
		{
		case POINT:	
			if(myNode->data.point->WritePoint(fHnd) == false)
				return false;
			break;
		case POLYLINE:	
			if(myNode->data.polyline->WritePolyLine(fHnd) == false)
				return false;
			break;
		case POLYGON: 
			if(myNode->data.polygon->WritePolyGon(fHnd) == false)
				return false;
			break;
		default:
			cout << "Invalid shape Type" << endl;
			return false;
		}
		shapeList.Next( myNode );

		numPolygonsWritten++;
		int newpercent = ((double)numPolygonsWritten/numPolygons)*maxPercent;
		if( newpercent > percent )
		{	percent = newpercent;
			//OCXControl->PercentChange( percent, WRITE, "Writing .shp File" );
		}
	}

	if(fclose(fHnd))
		return false;
	return true;
}

void ShapeFile::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}

bool ShapeFile::setFilename( char* fname)
{
	int len = strlen(fname);
	char* ext = fname+(len-4);
	if(strcmp(ext,".shp")!=0)
		return false;

	filename = new char[len+1];
	strcpy(filename,fname);
	return true;
}

/*
bool ShapeFile::DBSetup(char* dbfile, CString* names, CString* types, int num)
{

	dataInfo.setDBTable(dbfile);
	dataInfo.setColumns(names,types,5);
	
	try
	{
		dataInfo.Open(CRecordset::snapshot);
	}
	catch(CDBException* e)
	{
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION );
		return false;
	}

	return true;
}

bool ShapeFile::Insert(CString* vals)
{
	return dataInfo.Insert(vals);
}

bool ShapeFile::CreateTable(char* dbfile, CString* names, CString* types, int num)
{
	return dataInfo.createTable(dbfile,names,types,num);
}
*/

bool ShapeFile::CopyToArray()
{

	shapes = new GeoData[numShapes];

	Node* myNode= new Node();
	for(int i=0; i<numShapes && !shapeList.IsEmpty(); i++) {
		shapeList.DeleteHead(myNode);
		shapes[i] = myNode->data;
	}
	delete myNode;

	isList = false;
	return true;
}

LinkList ShapeFile::getList()
{
	return shapeList; 
}

int ShapeFile::getArray(GeoData* &data)
{
	if(!isList) {
		data = NULL; 
		return 0;
	}
	else {
		data = shapes;
		return numShapes;
	}
}

void ShapeFile::setHeader(FileHeader &h)
{
	header = h;
	shapeType = header.getType();
	numWords = header.getLength();
	fileLength = numWords*2;
}

void ShapeFile::setList(LinkList &list)
{
	shapeList = list;
}

bool ShapeFile::setArray(GeoData* data, int num)
{
	if(numShapes ==0)
		numShapes = num;
	else if(numShapes != num)
		return false;

	if(shapes)
		delete shapes;

	shapes = new GeoData[numShapes];
	for(int i=0; i<numShapes; i++) 
		shapes[i] = data[i];

	return true;
}



