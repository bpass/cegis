#ifndef SHAPEFILE_H
#define SHAPEFILE_H

#include <stdio.h>
#include <iostream.h>
#include <io.h>
#include <string.h>
//#include <afxdb.h>
#include "types.h"
#include "FileHeader.h"
#include "linklist.h"


class ShapeFile
{

// Constructor, Destructor
public:
	ShapeFile();
	~ShapeFile();
	
//data types
public:
	//ShpRecordset	dataInfo;

private:
	char*			filename;
	FileHeader		header;
	LinkList		shapeList;
	GeoData*		shapes;
	int				numShapes;
	int				fileLength;	//in bytes
	int             numWords;	//16 bit words
	int             shapeType;
	bool			isList;	
	bool			shallowData;

//member functions
public:
	bool			InputShapeFile(char* file );//, void (*FireTotalBytes)(int), void (*FireBytesLeft)(int));
	bool			WriteShapeFile( char* file, int numPolygons, int maxPercent );
	//bool			DBSetup(char* dbfile, CString* names, CString* types, int num);
	bool			CopyToArray();
	//bool			Insert(CString* vals);
	//bool			CreateTable(char* dbfile, CString* names, CString* types, int num);
	
//get routines
	LinkList		getList();
	int			    getArray(GeoData* &data);
	FileHeader		getHeader() { return header; }
	int				getNumShapes() { return numShapes;}
	int				getNumBytes() { return fileLength; }
	int				getNumWords() { return numWords; }	

//set routines
	void			setNumShapes(int shapes) {numShapes = shapes;}
	void			setHeader(FileHeader &header);
	void			setList(LinkList &list);
	bool			setArray(GeoData* data, int num);

protected:
	void			SwapEndian(char* a,int size);
	bool			setFilename(char* fname);
};


#endif 
