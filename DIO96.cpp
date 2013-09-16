// DIO96.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "DIO96.h"
//#include "nidaqex.h"
//#include "decl-32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DIO96 dialog

DIO96::DIO96(CWnd* pParent /*=NULL*/)
	: CDialog(DIO96::IDD, pParent)
{
	//{{AFX_DATA_INIT(DIO96)
	m_Port = 1;
	m_Line = 1;
	m_Data = 1;
	//}}AFX_DATA_INIT
}

void DIO96::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DIO96)
	DDX_Text(pDX, IDC_EDIT1, m_Port);
	DDV_MinMaxInt(pDX, m_Port, 0, 11);
	DDX_Text(pDX, IDC_EDIT4, m_Line);
	DDV_MinMaxInt(pDX, m_Line, 0, 7);
	DDX_Text(pDX, IDC_EDIT5, m_Data);
	DDV_MinMaxInt(pDX, m_Data, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DIO96, CDialog)
	//{{AFX_MSG_MAP(DIO96)
	ON_BN_CLICKED(IDC_READDIO, OnReadPORT)
	ON_BN_CLICKED(IDC_WRITEDIO, OnWritePORT)
	ON_BN_CLICKED(IDC_READDIO2, OnReadLINE)
	ON_BN_CLICKED(IDC_WRITEDIO2, OnWriteLINE)
	ON_BN_CLICKED(IDC_BUTTON1, OnClear)
	ON_BN_CLICKED(IDC_INPORT, InitialDIOinport)
	ON_BN_CLICKED(IDC_OUTPORT, InitialDIOoutport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DIO96 message handlers

void DIO96::OnReadPORT() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    
	//iStatus = DIG_In_Prt(1, m_Port, &iPattern); 

	m_Data = iPattern ; 

	UpdateData(FALSE);
}

void DIO96::OnWritePORT() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	
    //iStatus = DIG_Out_Prt(1, m_Port, m_Data);  
}

void DIO96::OnReadLINE() 
{
	// TODO: Add your control notification handler code here
   
}

void DIO96::OnWriteLINE() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//iStatus = DIG_Out_Line(1, m_Port , m_Line , m_Data );   
}

void DIO96::OnClear() 
{
	// TODO: Add your control notification handler code here
	m_Data = m_Port = m_Line = 0 ;

   	UpdateData(FALSE);
}

void DIO96::InitialDIOinport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

    //status = DIG_Prt_Config (deviceNumber, port, mode, dir)  dir: 1(Outport) 

	//iStatus = DIG_Prt_Config(1 , m_Port , m_Line , 0);
}

void DIO96::InitialDIOoutport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//iStatus = DIG_Prt_Config(1 , m_Port , m_Line , 1);
}
