#if !defined(AFX_MYSTATIC_H__9A11B7FC_DFB6_47C1_B9D8_4A891AFE6BC0__INCLUDED_)
#define AFX_MYSTATIC_H__9A11B7FC_DFB6_47C1_B9D8_4A891AFE6BC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyStatic window

class MyStatic : public CStatic
{
// Construction
public:
	MyStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_crfg;
	CBrush* m_pBrush;
	COLORREF m_BackColor;
	virtual ~MyStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATIC_H__9A11B7FC_DFB6_47C1_B9D8_4A891AFE6BC0__INCLUDED_)
