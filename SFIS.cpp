// SFIS.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "SFIS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSFIS dialog
IMPLEMENT_DYNAMIC(CSFIS, CDialog)

CSFIS::CSFIS(ModelessDialogTracker& tracker)
	: ModelessDialogHelper(tracker, *this)
{
	//{{AFX_DATA_INIT(CSFIS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSFIS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSFIS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSFIS, CDialog)
	//{{AFX_MSG_MAP(CSFIS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSFIS message handlers

CSFIS::~CSFIS()
{

}