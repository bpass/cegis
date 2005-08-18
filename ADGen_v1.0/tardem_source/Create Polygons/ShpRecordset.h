#if !defined(AFX_SHPRECORDSET_H__5886F923_5BC2_11D3_B234_00A0CC2918CB__INCLUDED_)
#define AFX_SHPRECORDSET_H__5886F923_5BC2_11D3_B234_00A0CC2918CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShpRecordset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ShpRecordset recordset
#include <afx.h>
#include <afxdb.h>

class ShpRecordset : public CRecordset
{
public:
	ShpRecordset(CDatabase* pDatabase = NULL);
	~ShpRecordset();
	DECLARE_DYNAMIC(ShpRecordset)
	
	//sets the table name and directory
	bool	setDBTable(char* dbfile);

	//sets the list of columns and their types, as well as memory allocation
	//for column bindings
	bool	setColumns(CString* names, CString* types, int num);

	//gets the value for the named column at the current record
	void*	columnValue(char* col);

	//goes to a specific record number
	bool	SetRow(int row);
	int		IsColValid(CString column);

	bool	createTable(char* dbfile, CString* names, CString* types, int num);

	//inserts into a created table.  Since the names and types listing have a one-to-one
	//relation, the values must be given this way as well.  
	bool	Insert(CString* vals);

	//overloaded functions
	//similar to SELECT * FROM table WHERE name=val
	bool    whereClause(CString name, CString val);
	bool    whereClause(CString name, CTime val);
	bool    whereClause(CString name, double val);
	bool    whereClause(CString name, int val, bool isPntr);

// Field/Param Data
	//{{AFX_FIELD(ShpRecordset, CRecordset)
	double*	 doubleVals;
	CString* strVals;
	CTime*	 dateVals;
	int*	 intVals;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ShpRecordset)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

private:
	char*		tableDir;
	char*		tableName;

	CString*	colNames;
	CString*    colTypes;
	int*		index;		//offset in the appropriate field binding array
	int			numCols;

	bool		MoveToPNTR(CString name, int pntr);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHPRECORDSET_H__5886F923_5BC2_11D3_B234_00A0CC2918CB__INCLUDED_)
