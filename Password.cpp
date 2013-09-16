// Password.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "Password.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassword dialog


CPassword::CPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassword)
	m_Password = 000000;
	//}}AFX_DATA_INIT
}


void CPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassword)
	DDX_Text(pDX, IDC_EDIT1, m_Password);
	DDV_MinMaxInt(pDX, m_Password, 0, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassword, CDialog)
	//{{AFX_MSG_MAP(CPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassword message handlers


