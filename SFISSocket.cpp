// SFISSocket.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "SFISSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SFISSocket dialog


SFISSocket::SFISSocket(CWnd* pParent /*=NULL*/)
	: CDialog(SFISSocket::IDD, pParent)
{
	//{{AFX_DATA_INIT(SFISSocket)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void SFISSocket::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SFISSocket)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SFISSocket, CDialog)
	//{{AFX_MSG_MAP(SFISSocket)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SFISSocket message handlers
