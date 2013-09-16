#if !defined(AFX_SERIALTHREAD_H__93EB90A3_6265_437E_9DE0_11D51EA52AD0__INCLUDED_)
#define AFX_SERIALTHREAD_H__93EB90A3_6265_437E_9DE0_11D51EA52AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// SerialThread thread
class CDIODlg;

class SerialThread : public CWinThread
{
	DECLARE_DYNCREATE(SerialThread)
protected:
	SerialThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	CFile FileCtrl2;
	CString m_PINFO;
	CString m_SerialNumber;
	CString m_Header;
	CStdioFile m_File;
	CDIODlg* ptrDlg;
	void setOwner(CDIODlg* m_ptrDialog);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SerialThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~SerialThread();

	// Generated message map functions
	//{{AFX_MSG(SerialThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALTHREAD_H__93EB90A3_6265_437E_9DE0_11D51EA52AD0__INCLUDED_)
