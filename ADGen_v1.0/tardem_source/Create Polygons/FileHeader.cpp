#include "FileHeader.h"


FileHeader::FileHeader()
{
	for( int i = 0; i < UnusedSize; i++ )
		Unused[i] = UNUSEDVAL;
}

FileHeader::FileHeader( const FileHeader & fh )
{	fileCode = fh.fileCode;
	fileLength = fh.fileLength;
	shapeType = fh.shapeType;
	version = fh.version;
	for( int i = 0; i < 5; i++ )
	{	Unused[i] = fh.Unused[i];
	}
}

FileHeader::~FileHeader()
{
	
}

bool FileHeader::InputHeader(FILE* fp) 
{
	char intbuf[sizeof(int)];

	fread(intbuf,sizeof(char),sizeof(int),fp);
	SwapEndian(intbuf,sizeof(int));
	fileCode = *(int*)intbuf;

	if(fileCode != FILE_CODE) {
		cout << "Invalid file code" << endl;
		return false;
	}

	for(int i=0; i<UnusedSize; i++) {
		fread(intbuf, sizeof(char),sizeof(int),fp);
		SwapEndian(intbuf,sizeof(int));
		Unused[i] = *(int*)intbuf;

		if(Unused[i] != UNUSEDVAL) {
			cout << "Invalid unused value" << endl;
			return false;
		}
	}
	
	fread(intbuf, sizeof(char),sizeof(int),fp);
	SwapEndian(intbuf,sizeof(int));
	fileLength = *(int*)intbuf;

	if(fileLength <= 50) {
		cout << "Bad fileLength value" << endl;
		return false;
	}

	fread(&version, sizeof(int),1,fp);
	if(version != VERSION) {
		cout << "Invalid version value" << endl;
		return false;
	}

	fread(&shapeType, sizeof(int),1,fp);
	
	double mybounds[8];
	fread(mybounds,sizeof(double),8,fp);

	if(boundingBox.setBounds(mybounds)==false) {
		cout << "Invalid bounds values" << endl;
		return false;
	}

	return true;
}

bool FileHeader::WriteHeader(FILE* fp)
{
	void* intbuf;
	intbuf = (char*)&fileCode;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf,sizeof(char),sizeof(int),fp);


	for(int i=0; i<UnusedSize; i++)
	{
		intbuf = (char*)&Unused[i];
		SwapEndian((char*)intbuf,sizeof(int));
		fwrite(intbuf,sizeof(char),sizeof(int),fp);
	}
	
	intbuf = (char*)&fileLength;
	SwapEndian((char*)intbuf,sizeof(int));
	fwrite(intbuf, sizeof(char),sizeof(int),fp);

	fwrite(&version, sizeof(int),1,fp);
	
	fwrite(&shapeType, sizeof(int),1,fp);
	
	double mybounds[8];
	boundingBox.getBounds(mybounds);
	fwrite(mybounds,sizeof(double),8,fp);
	return true;
}

void FileHeader::SwapEndian(char* a,int size) 
{
	char hold;
	for(int i=0; i<size/2; i++) {
		hold = a[i];
		a[i]=a[size-i-1];
		a[size-i-1] = hold;
	}
}
