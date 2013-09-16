#if !defined(AFX_BARCODE_H__7771261F_0359_4E00_8857_75E36C8BEFD7__INCLUDED_)
#define AFX_BARCODE_H__7771261F_0359_4E00_8857_75E36C8BEFD7__INCLUDED_

#include "INI.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Barcode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarcode dialog

class CBarcode : public CDialog
{
// Construction
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL CheckMac();
	CIniReader m_IniReader;
	BOOL CheckBarcode();
	BOOL m_CheckMac;
	BOOL m_CheckBar;
	void Test();
	CFileFind FFile;
	void Restore();
	CString str;
	CBarcode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcode)
	enum { IDD = IDD_BARMAC };
	CString	m_Bar;
	CString	m_Mac;
	CString	m_GUID;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcode)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcode)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODE_H__7771261F_0359_4E00_8857_75E36C8BEFD7__INCLUDED_)
