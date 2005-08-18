/******************************************************************************
 * File:     FileHeader.h                                                     *
 * Author:   Jason Maestri                                                    *
 * Date:     22-Jun-1999                                                      *
 *                                                                            *
 *****************************************************************************/

/******************************************************************************
 ******************************************************************************
 **                                                                          **
 ** Class: FileHeader                                                        **
 **                                                                          **
 ** Description:                                                             **
 **         This Class contains the data structures to hold the data from    **
 **    Main or Index (.shp or .shx) file of an ESRI Shapefile triplet, and   **
 **    the procedures to manipulate, read and write them.                    **
 **                                                                          **
 **                                                                          **
 ******************************************************************************
 *****************************************************************************/

#ifndef FILEHEADER_H
#define FILEHEADER_H

#include "types.h"
#include "boundbox.h"
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <io.h>

using namespace std;

class FileHeader {
public:
	FileHeader();
	FileHeader( const FileHeader & fh );
	~FileHeader();
	bool			InputHeader(FILE* fp);
	bool			WriteHeader(FILE* fp);
	void			SwapEndian(char* a,int size);
	int				getLength() {return fileLength;}
	int				getType() {return shapeType; }
	void			setType( int p_shape ) { shapeType = p_shape; }
	void			setFileCode( int fcode ) { fileCode = fcode; }
	void			setFileLength( int flength ) { fileLength = flength; }
	void			setVersion( int p_version ) { version = p_version; }

	HDR_BoundingBox boundingBox;

private:
	int             fileCode;
	int             Unused[5];
	int             fileLength;
	int             version;
	int             shapeType;	
};


#endif
