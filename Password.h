#if !defined(AFX_PASSWORD_H__46648035_DD7F_49B4_9254_5CB69E88A4FD__INCLUDED_)
#define AFX_PASSWORD_H__46648035_DD7F_49B4_9254_5CB69E88A4FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Password.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassword dialog

class CPassword : public CDialog
{
// Construction
public:
	CPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPassword)
	enum { IDD = IDD_PASSWORD };
	int		m_Password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassword)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORD_H__46648035_DD7F_49B4_9254_5CB69E88A4FD__INCLUDED_)
