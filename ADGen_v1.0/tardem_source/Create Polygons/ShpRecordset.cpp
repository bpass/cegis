// ShpRecordset.cpp : implementation file
//

#include "ShpRecordset.h"

const char*  DATASOURCE = "DSN=rf3db";


#include <iostream.h>
#include <direct.h>

/////////////////////////////////////////////////////////////////////////////
// ShpRecordset

IMPLEMENT_DYNAMIC(ShpRecordset, CRecordset)

ShpRecordset::ShpRecordset(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ShpRecordset)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;

	numCols = 0;
	tableDir = NULL;
	tableName = NULL;
	dateVals=NULL;
	strVals=NULL;
	intVals=NULL;
	doubleVals=NULL;
	index=NULL;
}

ShpRecordset::~ShpRecordset()
{
	delete tableName;
	delete tableDir;
	delete dateVals;
	delete strVals;
	delete intVals;
	delete doubleVals;
	delete index;
}

//virtual function
//loads the Data Source
CString ShpRecordset::GetDefaultConnect()
{
	
	::chdir(tableDir);
	CString connect("ODBC;");
	connect += CString(DATASOURCE);
	return _T(connect);
}

//virtual function
//loads the dbf table
CString ShpRecordset::GetDefaultSQL()
{
	return _T(tableName);
}

//virtual function
//bind the columns to the appropriate variable type
//the memory should already have been allocated
void ShpRecordset::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ShpRecordset)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	//}}AFX_FIELD_MAP
	CString col;
	LPCSTR name;
	int len;

	for(int i=0; i<numCols; i++) {

		//get name and put inside []
		col = colNames[i];
		col.Insert(0,"[");
		len = col.GetLength();
		col.Insert(len,"]");		
		name = (LPCSTR)col;

		//Doubles, int, strings, dates are only supported and this check has already been made
		if(colTypes[i] == CString("Numeric")) 
			RFX_Double(pFX, _T(name),doubleVals[index[i]]);
		else if(colTypes[i] == CString("Int")) 
			RFX_Int(pFX, _T(name), intVals[index[i]]);
		else if(colTypes[i] == CString("Char")) 
			RFX_Text(pFX, _T(name), strVals[index[i]]);
		else 
			RFX_Date(pFX, _T(name), dateVals[index[i]]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ShpRecordset diagnostics

//set the table name and table directory
bool ShpRecordset::setDBTable(char* dbfile)
{
	//check that no other tables are already associated
	if(IsOpen())
		return false;

	int len = strlen(dbfile);
	char* mystr = new char[len];
	strcpy(mystr,dbfile);

	//check for .dbf extention
	char* tmp = mystr+(len-4);
	if(strcmp(tmp,".dbf")!=0) {
		cout << "Invalid database file.  You must use the dbf format" << endl;
		return false;
	}

	//trim extention
	len -= 4;
	mystr[len] = '\0';
	
	//find index to file name
	int i=len-1;
	while(mystr[i] != '\\') 
		i--;

	tmp = mystr+(i+1);
	mystr[i]='\0';
	
	//store table name and directory and handle memory allocation
	len = strlen(tmp);
	if(tableName)
		delete tableName;

	tableName = new char[len+1];
	for(i=0;i<len;i++)
		tableName[i] = tmp[i];
	tableName[len]='\0';

	len = strlen(mystr);
	tableDir = new char[len+1];
	for(i=0;i<len;i++)
		tableDir[i] = mystr[i];
	tableDir[len]='\0';

	return true;
}

//set the column name and column types data
//there is a one-to-one correspondence to these 
//ie:  the first column name listed (names[0]) has the first type listed (types[0])
bool ShpRecordset::setColumns(CString* names, CString* types, int num)
{
	numCols = num;
	if(numCols <= 0) 
		return false;
	else
		m_nFields = numCols;

	colNames = names;
	colTypes = types;
	index = new int[numCols];

	int numStrings=0, numDoubles=0, numDates=0, numInts=0;
	for(int i=0; i<numCols; i++) {
		if(colTypes[i] == CString("Char")) {
			index[i] = numStrings;
			numStrings++;
		}
		else if(colTypes[i] == CString("Numeric")) {
			index[i] = numDoubles;
			numDoubles++;
		}
		else if(colTypes[i] == CString("Date")) {
			index[i] = numDates;
			numDates++;
		}
		else if(colTypes[i] == CString("Int")) {
			index[i] = numInts;
			numInts++;
		}
		else
			return false;
	}

	//allocate space for variable binding to columns
	
	if(numDoubles > 0)
		doubleVals = new double[numDoubles];
	else
		doubleVals = NULL;
	if(numStrings > 0)
		strVals = new CString[numStrings];
	else
		strVals = NULL;
	if(numDates > 0)
		dateVals = new CTime[numDates];
	else
		dateVals = NULL;
	if(numInts > 0)
		intVals = new int[numInts];
	else
		intVals = NULL;

	return true;
}

//returns the value of any column at the current record position
//Obviosly the caller of this function will know the return type and can cast it as needed
void* ShpRecordset::columnValue(char* col)
{
	int i = IsColValid(CString(col));
	if(i<0) 
		return NULL;

	int loc = index[i];

	if(colTypes[i] == CString("Numeric")) {
		return &doubleVals[loc];
	}
	else if(colTypes[i] == CString("Char")) {
		return &strVals[loc];
	}
	else if(colTypes[i] == CString("Int")) {
		return &intVals[loc];
	}
	else {
		return &dateVals[loc];
	}
}
				
//replicates what an sql where clause does
//SELECT * FROM table WHERE name=val (for strings)
bool ShpRecordset::whereClause(CString name, CString val)
{
	int i = IsColValid(name);
	int loc = index[i];

	if(colTypes[i] != CString("Char"))
		return false;

	this->MoveFirst();
	while(!IsEOF()) {
		if(val == strVals[loc])
			return true;
		MoveNext();
	}

	return false;
}

//replicates what an sql where clause does
//SELECT * FROM table WHERE name=val (for dates)
bool ShpRecordset::whereClause(CString name, CTime val)
{
	int i = IsColValid(name);
	int loc = index[i];

	if(colTypes[i] != CString("Date"))
		return false;

	this->MoveFirst();
	while(!IsEOF()) {
		if(val == dateVals[loc])
			return true;
		MoveNext();
	}

	return false;
}

//replicates what an sql where clause does
//SELECT * FROM table WHERE name=val (for real numbers)
bool ShpRecordset::whereClause(CString name, double val)
{
	int i = IsColValid(name);
	int loc = index[i];

	if(colTypes[i] != CString("Numeric"))
		return false;

	this->MoveFirst();
	while(!IsEOF()) {
		if(val == doubleVals[loc])
			return true;
		MoveNext();
	}

	return false;
}

//replicates what an sql where clause does
//SELECT * FROM table WHERE name=val (for ints)
//Since this is written mainly to access rf3 databases where PNTR_ is a PRIMARY KEY an 
//  more efficient algorithm was written specifically for this
bool ShpRecordset::whereClause(CString name, int val, bool isPntr)
{
	//check if looking for the PRIMARY KEY PNTR_
	if(isPntr)
		return MoveToPNTR(name, val);

	int i = IsColValid(name);
	int loc = index[i];			//offset in the intVals array

	if(colTypes[i] != CString("Int"))
		return false;

	this->MoveFirst();
	while(!IsEOF()) {
		if(val == intVals[loc])
			return true;
		MoveNext();
	}

	return false;
}

		
/**********  Efficient, but DB specific (WHERE PNTR_ = X) ************************/
bool ShpRecordset::MoveToPNTR(CString name, int pntr)
{
	int i = IsColValid(name);
	int loc = index[i];

	if(colTypes[i] != CString("Int"))
		return false;

	//first try the obvious
	SetAbsolutePosition((long)pntr);

	//test if found
	if(intVals[loc] == pntr)
		return true;

	//check all spots within diff (unless diff is > 10)
	int diff = pntr - intVals[loc];
	while( diff != 0 && diff < 10 && diff > -10) {
		if(diff > 0)
			MoveNext();
		else 
			MovePrev();

		if(intVals[loc] == pntr)
			return true;
		diff--;
	}

	//if still not found check every record
	MoveFirst();
	while(!IsEOF()) {
		if(intVals[loc] == pntr)
			return true;
		MoveNext();
	}
		
	return false;
}



int ShpRecordset::IsColValid(CString column)
{
	for(int i=0; i<numCols; i++) {
		if(colNames[i] == column)
			return i;
	}

	return -1;
}

//goes to a specified record number of the table
bool ShpRecordset::SetRow(int row)
{	
	
	try
	{
		SetAbsolutePosition((long)row);	
	}
	catch(CDBException* e)
	{
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION );
		return false;
	}
	
	return true;
}


bool ShpRecordset::createTable(char* dbfile, CString* names, CString* types,int num) 
{
	const char* Char = "CHAR";
	const char* Num  = "NUMERIC";
	const char* Date = "DATE";
	const char* Int = "INT";

	//set tableName and tableDir
	if(!setDBTable(dbfile))
		return false;

	//allocate space to bind colums and set member var's
	if(!setColumns(names,types,num))
		return false;
	 
	//estimate length of sql statement
	int len = strlen("CREATE TABLE ");
	len += strlen(tableDir);
	len += strlen(tableName);
	for(int i=0; i<numCols; i++) {
		len += colNames[i].GetLength();  // add name length
		len += 12;						// len of type and size ie: Char(10),	
	}

	char* sql_create = new char[len];

	//create sql statement
	len=0;
	len = sprintf(sql_create,"CREATE TABLE %s\\%s (",tableDir,tableName);
	LPCSTR field;
	for(i=0; i<numCols; i++) {
		field = (LPCSTR)colNames[i];
		len += sprintf(sql_create+len,"%s ",field);

		if(colTypes[i] == CString("Char")) 
			len += sprintf(sql_create+len,"%s,",Char);
		else if(colTypes[i] == CString("Numeric"))
			len += sprintf(sql_create+len,"%s,",Num);
		else if(colTypes[i] == CString("Date"))
			len += sprintf(sql_create+len,"%s,",Date);
		else if(colTypes[i] == CString("Int"))
			len += sprintf(sql_create+len,"%s,",Int);
		else
			return false;
			
	}
	sql_create[len-1] = ')';

	//now execute sql statement
	CDatabase db;
	db.OpenEx(DATASOURCE);
	try {
		db.ExecuteSQL(sql_create);
	}
	catch(CDBException* e) {
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION);
	}
	
	db.Close();
	delete sql_create;
	
	return true;	
}

bool ShpRecordset::Insert(CString* vals)
{	
	//connect to database
	CDatabase db;
	db.OpenEx(DATASOURCE);

	//estimate length of sql statement
	int len = strlen("INSERT INTO ");
	len += strlen(" INTO ");
	len += strlen(tableDir);
	len += strlen(tableName);
	for(int i=0; i<numCols; i++) {
		len += colNames[i].GetLength();  // add name length
		len += 12;						// value	
	}

	char* sql_insert = new char[len];

	//create sql statement
	len=0;
	len = sprintf(sql_insert,"INSERT INTO %s\\%s (",tableDir,tableName);
	LPCSTR field;
	for(i=0; i<numCols; i++) {
		field = (LPCSTR)colNames[i];
		len += sprintf(sql_insert+len,"%s,",field);
	}
	sql_insert[len-1] = ')';

	len += sprintf(sql_insert+len," VALUES (");
	LPCSTR tmp;
	for(i=0; i<numCols; i++) {
		tmp = (LPCSTR)vals[i];
		if(colTypes[i] == CString("Char")) {
			len += sprintf(sql_insert+len,"'%s',",tmp);
		}
		else if(colTypes[i] == CString("Numeric")) {
			len += sprintf(sql_insert+len,"%s,",tmp);
		}
		else if(colTypes[i] == CString("Date")) {
			len += sprintf(sql_insert+len,"%s,",tmp);
		}
		else if(colTypes[i] == CString("Int")) {
			len += sprintf(sql_insert+len,"%s,",tmp);
		}	
	}
	sql_insert[len-1] = ')';

	try {
		db.ExecuteSQL(sql_insert);
	}
	catch(CDBException* e) {
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION);
		return false;
	}

	delete sql_insert;
	return true;	
}

//I was having a lot of trouble using this code because the table was 
//always locked so the Update command always returned an error.
//I do think this would be a better way of handling Insertions, but the 
//above algorithm works fine
/*********  Insert rows through record set ***************
	try {
		AddNew();
	}
	catch(CDBException* e) {
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION);
		return false;
	}

	int numStrings=0,numDates=0,numDoubles=0,numInts=0;
	LPCSTR tmp;
	for(int i=0; i<numCols; i++) 
	{
		if(colTypes[i] == CString("Char")) {
			strVals[numStrings] = vals[i];
			numStrings++;
		}
		else if(colTypes[i] == CString("Numeric")) {
			tmp = (LPCSTR)vals[i];
			doubleVals[numDoubles] = (double)atol(tmp);
			numDoubles++;
		}
		else if(colTypes[i] == CString("Date")) {
			dateVals[numDates] = CTime();
			numDates++;
		}
		else if(colTypes[i] == CString("Int")) {
			tmp = (LPCSTR)vals[i];
			intVals[numInts] = atoi(tmp);
			numInts++;
		}
	}	
		
	char* tmpp, path[50];
	tmpp = ::getcwd(path,50);
	if(strcmp(tableDir,path)==0) 
		::chdir("C:\\WINDOWS");

	tmpp = ::getcwd(path,50);

	try {
		bool update = CanUpdate();
		bool append = CanAppend();
		Update();
	}
	catch(CDBException* e) {
		AfxMessageBox(e->m_strError,MB_ICONEXCLAMATION);
		return false;
	}
*****************************************************************/
