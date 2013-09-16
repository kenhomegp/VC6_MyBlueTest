// SetUUT.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "SetUUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetUUT dialog


SetUUT::SetUUT(CWnd* pParent /*=NULL*/)
	: CDialog(SetUUT::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetUUT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SetUUT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetUUT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SetUUT, CDialog)
	//{{AFX_MSG_MAP(SetUUT)
	ON_BN_CLICKED(IDC_DRADIO1, OnDradio1)
	ON_BN_CLICKED(IDC_DRADIO2, OnDradio2)
	ON_BN_CLICKED(IDC_DRADIO3, OnDradio3)
	ON_BN_CLICKED(IDC_DRADIO4, OnDradio4)
	ON_BN_CLICKED(IDC_DRADIO5, OnDradio5)
	ON_BN_CLICKED(IDC_DRADIO6, OnDradio6)
	ON_BN_CLICKED(IDC_ERADIO1, OnEradio1)
	ON_BN_CLICKED(IDC_ERADIO2, OnEradio2)
	ON_BN_CLICKED(IDC_ERADIO3, OnEradio3)
	ON_BN_CLICKED(IDC_ERADIO4, OnEradio4)
	ON_BN_CLICKED(IDC_ERADIO5, OnEradio5)
	ON_BN_CLICKED(IDC_ERADIO6, OnEradio6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetUUT message handlers

void SetUUT::OnDradio1() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT1","UUTSTATE");
		}
	}
}

void SetUUT::OnDradio2() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT2","UUTSTATE");
		}
	}
}

void SetUUT::OnDradio3() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT3","UUTSTATE");
		}
	}
}

void SetUUT::OnDradio4() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT4","UUTSTATE");
		}
	}
}

void SetUUT::OnDradio5() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT5","UUTSTATE");
		}
	}
}

void SetUUT::OnDradio6() 
{
	// TODO: Add your control notification handler code here
	if(!(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0)))	
	{
		if(!(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0)))	
		{
			m_IniReader.setKey("CLOSE","UUT6","UUTSTATE");
		}
	}
}

void SetUUT::OnEradio1() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT1","UUTSTATE");
}

void SetUUT::OnEradio2() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT2","UUTSTATE");
}

void SetUUT::OnEradio3() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT3","UUTSTATE");
}

void SetUUT::OnEradio4() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT4","UUTSTATE");
}

void SetUUT::OnEradio5() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT5","UUTSTATE");
}

void SetUUT::OnEradio6() 
{
	// TODO: Add your control notification handler code here
	m_IniReader.setKey("","UUT6","UUTSTATE");
}

BOOL SetUUT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetWindowPos(&this->wndTopMost,0,0,1024/3,768/2,0);

	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
