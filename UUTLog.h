#if !defined(AFX_UUTLOG_H__A0832164_B275_4B36_8E35_35ECD813AEF5__INCLUDED_)
#define AFX_UUTLOG_H__A0832164_B275_4B36_8E35_35ECD813AEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UUTLog.h : header file
//

#pragma once
#include "ModelessDialogTracker.h"
#include "INI.h"	// Added by ClassView

#include "ColorCtrl.h"
#include "FontCtrl.h"
#include "XListCtrl.h"
#include "ni488.h"
#include "nidaqex.h"
#include "decl-32.h"
#include "DIODlg.h"

/////////////////////////////////////////////////////////////////////////////
// UUTLog dialog

class UUTLog : public CDialog , ModelessDialogHelper
{
	DECLARE_DYNAMIC(UUTLog)
// Construction
public:
	BOOL ChkLog();
	BOOL PingTest1();
	void InitialSFIS6();
	void InitialSFIS5();
	void InitialSFIS4();
	void InitialSFIS3();
	void InitialSFIS2();
	static UINT SFISSocket(LPVOID pParam);
	CString m_szIP;
	CString m_szPort;
	BOOL ConnectSFIS();
	BOOL InitWinSock1();
	void InitialSFIS1();
	CTime time;
	unsigned long STUUT1;
	unsigned long STUUT2;
	unsigned long STUUT3;
	unsigned long STUUT4;
	unsigned long STUUT5;
	unsigned long STUUT6;
	CWinThread *m_Thread1;
	CWinThread *m_Thread2;
	CWinThread *m_Thread3;
	CWinThread *m_Thread4;
	CWinThread *m_Thread5;
	CWinThread *m_Thread6;
	void ShowFail();
	void ShowPass();
	void TestLog();
	void InitialUUT6();
	void InitialUUT5();
	void InitialUUT4();
	void InitialUUT3();
	void InitialUUT2();
	void InitialUUT1();
	CFileFind FindFile;
	void StartUUT6();
	void StartUUT5();
	CDIODlg* _this;
	CIniReader m_IniReader;
	CFile m_NewFile;
	void Setenv();
	void Show();
	CString m_Str;
	void IniDlgGUI();
	CString m_GUID;
	CString m_Mac2;
	CString m_Bar2;
	void StartUUT4();
	void StartUUT3();
	void StartUUT2();
	int m_UUTNumber;
	COLORREF rgbText;
	COLORREF rgbBkgnd;
	BOOL m_InitialState;
	void StartUUT1();
	CRichEditCtrl	m_DispLog;

    struct   _STRUCT_THREAD
	{
		HWND m_statichwnd;
		HWND m_hwnd;
		int  m_UUTNumber;
		BOOL m_Run;
		UUTLog* _this;
	}m_FileCtrl;

	static BOOL ActiveUUTLog;
	static BOOL ActiveUUTLog1;
	static BOOL ActiveUUTLog2;
	static BOOL ActiveUUTLog3;
	static BOOL ActiveUUTLog4;
	static BOOL ActiveUUTLog5;
	static BOOL ActiveUUTLog6;
	static UINT FileShareingFun(LPVOID pParam);
	void OnTest();
	CString str;
	UUTLog(ModelessDialogTracker& tracker);   // standard constructor
	BOOL Create(UINT nID, CWnd* pWnd) 
	{ return CDialog::Create(nID,pWnd); }
	void PostNcDestroy()
	{ delete this; }
	void OnCancel()
	{ DestroyWindow(); }

	virtual ~UUTLog();

    CColorCtrl <CItalicCtrl<CStatic> > m_Static1;
	CColorCtrl <CItalicCtrl<CStatic> > m_Static2;
	CColorCtrl <CItalicCtrl<CStatic> > m_Static3;
	CColorCtrl<CItalicCtrl<CStatic> > m_UUTName;
	CColorCtrl<CItalicCtrl<CButton> > m_UUTExit;
	CColorCtrl<CItalicCtrl<CButton> > m_UUTStart;
	CColorCtrl<CItalicCtrl<CButton> > m_UUTReTest;
	CFontCtrl <CColorCtrl<CStatic> > m_UUTString;

	//virtual ~UUTLog();
// Dialog Data
	//{{AFX_DATA(UUTLog)
	enum { IDD = IDD_LOG };
	CStatic	m_UUTTime;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UUTLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(UUTLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnUutStart();
	afx_msg void OnUutReTest();
	afx_msg void OnUutExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	afx_msg void OnButton1();
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UUTLOG_H__A0832164_B275_4B36_8E35_35ECD813AEF5__INCLUDED_)
