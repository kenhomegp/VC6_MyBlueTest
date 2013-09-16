#if !defined(AFX_SFIS_H__4AF0E2E2_9607_4B95_80DA_FC0472060C6A__INCLUDED_)
#define AFX_SFIS_H__4AF0E2E2_9607_4B95_80DA_FC0472060C6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SFIS.h : header file
//

#include "ModelessDialogTracker.h"
#include "INI.h"
	
/////////////////////////////////////////////////////////////////////////////
// CSFIS dialog

class CSFIS : public CDialog , ModelessDialogHelper
{
	DECLARE_DYNAMIC(CSFIS)
// Construction
public:
	CSFIS(ModelessDialogTracker& tracker);   // standard constructor
	// stuff specific to the modeless dialog. //
	BOOL Create(UINT nID, CWnd* pWnd) 
	{ return CDialog::Create(nID,pWnd); }
	void PostNcDestroy()
	{ delete this; }
	void OnCancel()
	{ DestroyWindow(); }
	virtual ~CSFIS();
// Dialog Data
	//{{AFX_DATA(CSFIS)
	enum { IDD = IDD_SFISDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSFIS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSFIS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFIS_H__4AF0E2E2_9607_4B95_80DA_FC0472060C6A__INCLUDED_)
