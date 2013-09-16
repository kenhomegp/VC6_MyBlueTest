#if !defined(AFX_SFISSOCKET_H__96E4B2F7_B02A_40E5_97A9_5735EBC57B8A__INCLUDED_)
#define AFX_SFISSOCKET_H__96E4B2F7_B02A_40E5_97A9_5735EBC57B8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SFISSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SFISSocket dialog

class SFISSocket : public CDialog
{
// Construction
public:
	SFISSocket(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SFISSocket)
	enum { IDD = IDD_SFISDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SFISSocket)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SFISSocket)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFISSOCKET_H__96E4B2F7_B02A_40E5_97A9_5735EBC57B8A__INCLUDED_)
