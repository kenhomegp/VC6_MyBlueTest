// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyEdit

MyEdit::MyEdit()
{
}

MyEdit::~MyEdit()
{
}

BEGIN_MESSAGE_MAP(MyEdit, CEdit)
	//{{AFX_MSG_MAP(MyEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyEdit message handlers

void MyEdit::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEdit::DoDataExchange(pDX);
}
