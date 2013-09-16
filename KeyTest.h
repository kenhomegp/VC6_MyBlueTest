#if !defined(AFX_KEYTEST_H__FC0C74AF_E835_4400_956A_0E4046205A7F__INCLUDED_)
#define AFX_KEYTEST_H__FC0C74AF_E835_4400_956A_0E4046205A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KeyTest dialog

class KeyTest : public CDialog
{
// Construction
public:
	KeyTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KeyTest)
	enum { IDD = IDD_KEYTEST };
	int		m_KeyNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KeyTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KeyTest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYTEST_H__FC0C74AF_E835_4400_956A_0E4046205A7F__INCLUDED_)
