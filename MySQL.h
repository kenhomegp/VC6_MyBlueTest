#if !defined(AFX_MYSQL_H__94B863AE_110A_44DA_B2C2_258DCC297AB9__INCLUDED_)
#define AFX_MYSQL_H__94B863AE_110A_44DA_B2C2_258DCC297AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySQL.h : header file

#include <mysql.h>
//

/////////////////////////////////////////////////////////////////////////////
// CMySQL dialog

class CMySQL : public CDialog
{
// Construction
public:
	CString Host;
	CString User;
	CString Password;
	void OnDataView();
	void OnStructureView();
	BOOL BuildListView(UINT num_Cols, MYSQL_FIELD *fd);
	void ClearListView();
	BOOL ChangeDB(CString& db_name);
	BOOL GetDatabases();
	CMySQL(CWnd* pParent = NULL);   // standard constructor

	bool IsConnected;
	MYSQL * myData;

// Dialog Data
	//{{AFX_DATA(CMySQL)
	enum { IDD = IDD_MYSQL_DLG };
	CListCtrl	m_lvDbTable;
	CComboBox	m_Tables;
	CComboBox	m_Databases;
	//}}AFX_DATA

	CButton	m_bStructureView;
	CButton	m_bDataView;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySQL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySQL)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDatabases();
	afx_msg void OnSelchangeTables();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSQL_H__94B863AE_110A_44DA_B2C2_258DCC297AB9__INCLUDED_)
