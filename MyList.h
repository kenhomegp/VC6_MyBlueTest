#if !defined(AFX_MYLIST_H__C4024840_34F5_479E_8345_3DB448E088F8__INCLUDED_)
#define AFX_MYLIST_H__C4024840_34F5_479E_8345_3DB448E088F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyList window

class MyList : public CListCtrl
{
// Construction
public:
	MyList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyList();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__C4024840_34F5_479E_8345_3DB448E088F8__INCLUDED_)
