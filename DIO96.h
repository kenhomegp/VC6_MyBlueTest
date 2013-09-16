#if !defined(AFX_DIO96_H__0229A2D1_88A4_4B2D_88BE_024212E7EB79__INCLUDED_)
#define AFX_DIO96_H__0229A2D1_88A4_4B2D_88BE_024212E7EB79__INCLUDED_

#include "nidaq.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIO96.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DIO96 dialog

class DIO96 : public CDialog
{
// Construction
public:
	i32 iPattern;
	i16 iStatus;
	DIO96(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DIO96)
	enum { IDD = IDD_DIO96 };
	int		m_Port;
	int		m_Line;
	int		m_Data;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DIO96)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DIO96)
	afx_msg void OnReadPORT();
	afx_msg void OnWritePORT();
	afx_msg void OnReadLINE();
	afx_msg void OnWriteLINE();
	afx_msg void OnClear();
	afx_msg void InitialDIOinport();
	afx_msg void InitialDIOoutport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIO96_H__0229A2D1_88A4_4B2D_88BE_024212E7EB79__INCLUDED_)
