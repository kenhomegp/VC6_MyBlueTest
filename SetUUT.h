#if !defined(AFX_SETUUT_H__E6B6A412_E2FA_4FF6_B506_299A21FEB265__INCLUDED_)
#define AFX_SETUUT_H__E6B6A412_E2FA_4FF6_B506_299A21FEB265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetUUT.h : header file
//

#include "INI.h"

/////////////////////////////////////////////////////////////////////////////
// SetUUT dialog

class SetUUT : public CDialog
{
// Construction
public:
	CFileFind findfile;
	CIniReader m_IniReader;
	SetUUT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SetUUT)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetUUT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SetUUT)
	afx_msg void OnDradio1();
	afx_msg void OnDradio2();
	afx_msg void OnDradio3();
	afx_msg void OnDradio4();
	afx_msg void OnDradio5();
	afx_msg void OnDradio6();
	afx_msg void OnEradio1();
	afx_msg void OnEradio2();
	afx_msg void OnEradio3();
	afx_msg void OnEradio4();
	afx_msg void OnEradio5();
	afx_msg void OnEradio6();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUUT_H__E6B6A412_E2FA_4FF6_B506_299A21FEB265__INCLUDED_)
