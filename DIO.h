// DIO.h : main header file for the DIO application
//

#if !defined(AFX_DIO_H__72C6D1A8_053C_4F93_9F21_E06559D4A520__INCLUDED_)
#define AFX_DIO_H__72C6D1A8_053C_4F93_9F21_E06559D4A520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDIOApp:
// See DIO.cpp for the implementation of this class
//

class CDIOApp : public CWinApp
{
public:
	HACCEL m_haccel;
	CDIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIOApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIO_H__72C6D1A8_053C_4F93_9F21_E06559D4A520__INCLUDED_)
