#if !defined(AFX_DEBUGDLG_H__EEDD87FF_8C80_41CE_BC6F_28705BD797A0__INCLUDED_)
#define AFX_DEBUGDLG_H__EEDD87FF_8C80_41CE_BC6F_28705BD797A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDlg.h : header file
//
#include "XListCtrl.h"
#include "INI.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// DebugDlg dialog

class DebugDlg : public CDialog
{
// Construction
public:
	int m_item;
	CIniReader m_IniReader;
	void SetINIValue(CString m_keyvalue , CString m_key , CString m_section);
	CString str;
	CString tmp;
    void Boot3();
	void Boot2();
	void Boot1();
	void Boot0();
	int Boot;
	afx_msg void OnLoad();
	void IniFileCtrl();
	CWnd* m_ptr;
	virtual void OnOK();
	CStringArray m_Test ;
	void FillListCtrl(CXListCtrl &list);
	void InitListCtrl(CXListCtrl &list);
	BOOL OnInitDialog();
	DebugDlg(CWnd* pParent = NULL);   // standard constructor
    CXListCtrl	m_ListCtrl;
// Dialog Data
	//{{AFX_DATA(DebugDlg)
	enum { IDD = IDD_DEBUG };
	CString	m_SerialSend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DebugDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DebugDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGDLG_H__EEDD87FF_8C80_41CE_BC6F_28705BD797A0__INCLUDED_)
