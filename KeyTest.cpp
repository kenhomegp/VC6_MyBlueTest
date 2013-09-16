// KeyTest.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "KeyTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KeyTest dialog


KeyTest::KeyTest(CWnd* pParent /*=NULL*/)
	: CDialog(KeyTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(KeyTest)
	m_KeyNum = 0;
	//}}AFX_DATA_INIT
}


void KeyTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KeyTest)
	DDX_Text(pDX, IDC_KeyNumber, m_KeyNum);
	DDV_MinMaxInt(pDX, m_KeyNum, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KeyTest, CDialog)
	//{{AFX_MSG_MAP(KeyTest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KeyTest message handlers
