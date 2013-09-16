// Dir.h : main header file for the DIR application
//

#if !defined(AFX_DIR_H__5BC7B287_F166_11D4_9393_0050DABB534C__INCLUDED_)
#define AFX_DIR_H__5BC7B287_F166_11D4_9393_0050DABB534C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDirApp:
// See Dir.cpp for the implementation of this class
//

class CDirApp : public CWinApp
{
public:
	CDirApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDirApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIR_H__5BC7B287_F166_11D4_9393_0050DABB534C__INCLUDED_)
