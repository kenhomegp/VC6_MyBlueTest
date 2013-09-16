#if !defined(AFX_MYEDIT_H__69FAB971_8BA7_485A_9226_AF23C34DCA05__INCLUDED_)
#define AFX_MYEDIT_H__69FAB971_8BA7_485A_9226_AF23C34DCA05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyEdit window

class MyEdit : public CEdit
{
// Construction
public:
	MyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__69FAB971_8BA7_485A_9226_AF23C34DCA05__INCLUDED_)
