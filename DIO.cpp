// DIO.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DIO.h"
#include "DIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIOApp

BEGIN_MESSAGE_MAP(CDIOApp, CWinApp)
	//{{AFX_MSG_MAP(CDIOApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIOApp construction

CDIOApp::CDIOApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDIOApp object

CDIOApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDIOApp initialization

BOOL CDIOApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox("AfxSocketInit fail!");
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	AfxInitRichEdit( );

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    
    //HANDLE h;

    //h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //if (h == INVALID_HANDLE_VALUE)
    //{
    //   AfxMessageBox("Couldn't access giveio device");
    //   PostQuitMessage(0);
	//   return FALSE;
    //}

    m_haccel=LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	CDIODlg dlg;

	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CDIOApp::ProcessMessageFilter(int code, LPMSG lpMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_haccel)
    {
        if (::TranslateAccelerator(m_pMainWnd->m_hWnd, m_haccel, lpMsg)) 
            return(TRUE);
    }

	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
