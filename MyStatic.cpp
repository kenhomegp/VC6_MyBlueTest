// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "MyStatic.h"
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RGB_BLACK	RGB(0x00, 0x00, 0x00)
#define RGB_WHITE	RGB(0xFF, 0xFF, 0xFF)
#define RGB_RED	RGB(255,0,0)
#define RGB_GREEN	RGB(0,255,0)
#define RGB_YELLOW	RGB(255,255,0)
#define RGB_BLUE RGB(0,0,255)
#define RGB_GRAY RGB(128,128,128)

/////////////////////////////////////////////////////////////////////////////
// MyStatic

MyStatic::MyStatic()
{
	m_BackColor = RGB(0,255,0);
	m_pBrush = new CBrush(RGB_WHITE);
	m_crfg = RGB(0,0,255);
}

MyStatic::~MyStatic()
{
    delete m_pBrush;
}


BEGIN_MESSAGE_MAP(MyStatic, CStatic)
	//{{AFX_MSG_MAP(MyStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyStatic message handlers

HBRUSH MyStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	HBRUSH hbr;
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(m_crfg);
    //pDC->SetBkColor(m_BackColor);
    hbr = *m_pBrush;
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void MyStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	unsigned char i = _inp(0x378);
	CString post;
	post.Format("%X" , i);
	//SetDlgItemText(IDC_POST , post);
	SetWindowText("Post Code");
	// Do not call CStatic::OnPaint() for painting messages
}


