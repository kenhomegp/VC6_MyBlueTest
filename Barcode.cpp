// Barcode.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "Barcode.h"
#include "DIODlg.h"
#include "INI.h"	// Added by ClassView
#include "Imports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RGB_MYCOLOR RGB(217 , 255, 217)

/////////////////////////////////////////////////////////////////////////////
// CBarcode dialog

CBarcode::CBarcode(CWnd* pParent /*=NULL*/)
	: CDialog(CBarcode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcode)
	m_Bar = _T("");
	m_Mac = _T("");
	m_GUID = _T("");
	//}}AFX_DATA_INIT
}

void CBarcode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcode)
	DDX_Text(pDX, IDC_EDIT1, m_Bar);
	DDV_MaxChars(pDX, m_Bar, 22);
	DDX_Text(pDX, IDC_EDIT2, m_Mac);
	DDV_MaxChars(pDX, m_Mac, 15);
	DDX_Text(pDX, IDC_EDIT3, m_GUID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBarcode, CDialog)
	//{{AFX_MSG_MAP(CBarcode)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCLEAR, OnClear)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDD_RESTORE, Restore)
	ON_COMMAND(IDD_TEST, Test)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarcode message handlers

void CBarcode::OnTimer(UINT nIDEvent) //Generate setbar.bat and mac.bat
{
    if(nIDEvent == 3)//Get Barcode
	{
		UpdateData(TRUE);
			
		if(m_Bar != "" && (m_Bar.GetLength() == 19))
		{
			//CEdit *m_Edt2 = (CEdit*)GetDlgItem(IDC_EDIT2);
			//GotoDlgCtrl(m_Edt2);
			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

			KillTimer(3);
			SetTimer(6 , 100, 0);
		}			
	 }
     else if(nIDEvent == 4)//Get Maccode
	 {
		UpdateData(TRUE);
		if(m_Mac != "" && (m_Mac.GetLength() == 12))
		{
			CEdit *m_Edt1 = (CEdit*)GetDlgItem(IDC_EDIT1);
			GotoDlgCtrl(m_Edt1);
			GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);

			KillTimer(4);
			//SetTimer(5, 100, 0);
			SetTimer(3, 100, 0);
			//m_Mac = m_Mac.Right(12);
		}
	 }
	 else if(nIDEvent == 5)//Get GUID
	 {
		UpdateData(TRUE);
        if(m_GUID != "" && (m_GUID.GetLength() == 16))
		{
			GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);

			KillTimer(5);
			SetTimer(6, 100, 0);
		}
	 }
	 else if(nIDEvent == 6)
	 {
		::SetCursorPos(310,550);
		KillTimer(6);
	 }
}

BOOL CBarcode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect;
	
	// TODO: Add extra initialization here

	CButton *pBtn;

	int width = GetSystemMetrics(SM_CXSCREEN); 
	int height = GetSystemMetrics(SM_CYSCREEN);

	m_Bar = "";
	m_Mac = "";
	m_GUID = "";

    //SetTimer(3, 100, 0);
	SetTimer(4, 100, 0);

	m_CheckBar = TRUE;
	m_CheckMac = FALSE;

	GetClientRect(&rect);

	pBtn = (CButton*)GetDlgItem(IDCANCEL);
	pBtn->EnableWindow(FALSE);

	//SetWindowPos( 0 , (512-425) , (384-225) , 850 , 450 , SWP_SHOWWINDOW);
	//SetWindowPos(&wndTopMost,(512-425) , (384-225) , 850 , 450 , SWP_SHOWWINDOW);
	//SetWindowPos(&wndTopMost,(512-425) , (384-225) , 845 , 400 , SWP_SHOWWINDOW);
	SetWindowPos(&wndTopMost,(width-rect.right)/2 , (height-rect.bottom)/2 , 845 , 400 , SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarcode::OnClear() 
{
	// TODO: Add your control notification handler code here
    
	//DeleteFile("d:\\usinote\\mac.bat");  
	//DeleteFile("d:\\usinote\\setbar.bat");

    //
	//GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);

	//SetDlgItemText(IDC_EDIT1 , " ");
	//SetDlgItemText(IDC_EDIT2 , " ");

    //CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_EDIT1);
	//GotoDlgCtrl(m_Edt);
	//SetTimer(3, 100, 0);

    CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_EDIT1);

	KillTimer(3);
	KillTimer(4);
	KillTimer(5);
	KillTimer(6);

	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);

	GotoDlgCtrl(m_Edt);

	//AfxMessageBox("Clear!");

	SetDlgItemText(IDC_EDIT1 , "");
	//SetDlgItemText(IDC_EDIT2 , "");

	m_Bar = "";
	//m_Mac = "";
	//m_GUID = "";

	SetTimer(4, 100, 0);
}

void CBarcode::Restore()
{
    // Read Barcode and Maccode from File

	/*
	if(FFile.FindFile("d:\\usinote\\setbar.bat" , 0))
	{
	    if(FFile.FindFile("d:\\usinote\\mac.bat" , 0))
		{
   			char Bar[22];

			CFile setbar , mac;

			setbar.Open("d:\\usinote\\setbar.bat" ,CFile::modeRead );
			mac.Open("d:\\usinote\\mac.bat" ,CFile::modeRead );

			setbar.Seek(12,CFile::begin);
			setbar.Read(Bar , 22);

			m_Bar = Bar;
			m_Bar = m_Bar.Left(22);

			SetDlgItemText(IDC_EDIT1 , m_Bar);

			setbar.Close();

			mac.Seek(8,CFile::begin);
			mac.Read(Bar , 12);

			m_Mac = Bar;
			m_Mac = m_Mac.Left(12);

			SetDlgItemText(IDC_EDIT2 , m_Mac);

			mac.Close();
		}
		else
			MessageBox("mac.bat not found!" , "" , MB_ICONERROR);
	}
	else
		MessageBox("setbar.bat not found!" , "" , MB_ICONERROR);

    */

}

void CBarcode::OnOK()   // Check setbar.bat and mac.bat is exist!
{
	// TODO: Add extra validation here

	//if(!(CheckBarcode()))
	//{
	//	AfxMessageBox("Invalid Barcode!");
	//	OnClear();
	//	return;
	//}
	
	//if(!(CheckMac()))
	//{
	//	AfxMessageBox("Invalid MAC Address !");
	//	OnClear();
	//	return;
	//}

	//CFile setbar , mac;

	//setbar.Open("d:\\usinote\\setbar.bat" ,CFile::modeCreate | CFile::modeReadWrite );
	//mac.Open("d:\\usinote\\mac.bat" ,CFile::modeCreate | CFile::modeReadWrite );

    //setbar.Write("set BARCODE="+m_Bar, 34);

	//mac.Write("set MAC="+m_Mac.Right(12) , 20);

	//setbar.Close();

	//mac.Close();

	//if(FFile.FindFile("d:\\usinote\\setbar.bat" , 0))
	//{	
	    //if(FFile.FindFile("d:\\usinote\\mac.bat" , 0))
		//{
        //    CDialog::OnOK();
		//}
		//else
		//	OnCancel(); 
	//}
	//else
 			//OnClear();  

	//m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	//m_IniReader.setKey(m_Bar , "Bar Code ID" , "UUT");
	//m_IniReader.setKey(m_Mac , "Lan1 Mac" , "UUT");
	//m_IniReader.setKey(m_GUID , "IEEE1394 Mac" , "UUT");

	/*
    //str = m_IniReader.getKeyValue( "SupportUUT" , "DEBUG");
	str = m_IniReader.getKeyValue( "UUTMode" , "DEBUG");

	if(str.Compare("Enable") == 0)
	{
		str = m_IniReader.getKeyValue( "SupportUUT" , "DEBUG");
		if(str.Compare("1") == 0)
		{
			m_IniReader.setKey(m_Bar , "UUT1BAR" , "BARMAC");
			m_IniReader.setKey(m_Mac , "UUT1MAC" , "BARMAC");
			m_IniReader.setKey("2" , "SupportUUT" , "DEBUG");
		}
		else if(str.Compare("2") == 0)
		{
			m_IniReader.setKey(m_Bar , "UUT2BAR" , "BARMAC");
			m_IniReader.setKey(m_Mac , "UUT2MAC" , "BARMAC");
			m_IniReader.setKey("3" , "SupportUUT" , "DEBUG");
		}
		else if(str.Compare("3") == 0)
		{
			m_IniReader.setKey(m_Bar , "UUT3BAR" , "BARMAC");
			m_IniReader.setKey(m_Mac , "UUT3MAC" , "BARMAC");
			m_IniReader.setKey("4" , "SupportUUT" , "DEBUG");
		}
		else if(str.Compare("4") == 0)
		{
			m_IniReader.setKey(m_Bar , "UUT4BAR" , "BARMAC");
			m_IniReader.setKey(m_Mac , "UUT4MAC" , "BARMAC");
			m_IniReader.setKey("5" , "SupportUUT" , "DEBUG");
		}
		else
		{
			//AfxMessageBox("WRONG!");
			m_IniReader.setKey("1" , "SupportUUT" , "DEBUG");
			m_IniReader.setKey("" , "UUT1BAR" , "BARMAC");
			m_IniReader.setKey("" , "UUT1MAC" , "BARMAC");
			m_IniReader.setKey("" , "UUT2BAR" , "BARMAC");
			m_IniReader.setKey("" , "UUT2MAC" , "BARMAC");
			m_IniReader.setKey("" , "UUT3BAR" , "BARMAC");
			m_IniReader.setKey("" , "UUT3MAC" , "BARMAC");
			m_IniReader.setKey("" , "UUT4BAR" , "BARMAC");
			m_IniReader.setKey("" , "UUT4MAC" , "BARMAC");
			return;
		}
	}
	*/

	UpdateData(TRUE);

	//AfxMessageBox(m_Bar);

	CDialog::OnOK();
}

void CBarcode::OnCancel() 
{
	// TODO: Add extra cleanup here

	//DeleteFile("d:\\usinote\\mac.bat");  
	//DeleteFile("d:\\usinote\\setbar.bat");

	CDialog::OnCancel();
}

void CBarcode::Test()
{
/*
	m_Bar = _T("11S46P0409ZJ1RJF32P17W");
	m_Mac = _T("23S112233445566");
	m_GUID = _T("0000000000000000");

	SetDlgItemText(IDC_EDIT1 , m_Bar);
	SetDlgItemText(IDC_EDIT2 , m_Mac);
	SetDlgItemText(IDC_EDIT3 , m_GUID);
*/
}

BOOL CBarcode::CheckBarcode()
{
/*
    MyDecryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");

    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	m_IniReader.sectionExists("BARCODE");

    str = m_IniReader.getKeyValue( "BARCODE" , "BARCODE");

	MyEncryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");

	CString strcomp;
	strcomp = m_Bar;
	strcomp = strcomp.Left(8);

	if((strcomp.Compare(str) == 0))
		   return TRUE;
	else 
*/
	return FALSE; 
}

BOOL CBarcode::CheckMac()
{
	str = "D:\\USINOTE\\TAURUS.INI";
    m_IniReader.setINIFileName(str);

    str = "MAC";

	BOOL bExists = m_IniReader.sectionExists(str);
	
	if(!bExists) 
	{
		AfxMessageBox("Section doesn't exist");
		return FALSE;
	}

	CStringList* myOtherList = m_IniReader.getSectionData(str);

	POSITION position;
	CString strcomp;
	strcomp = m_Mac;
	strcomp = strcomp.Left(10);

	for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
	{
		str = myOtherList->GetNext(position);
        str = str.Left(14);
		str = str.Right(10);
		if((strcomp.Compare(str) == 0))
		{
		    //AfxMessageBox(" Check MAC PASS! ");
			return TRUE;
		}
		//MessageBox(str , "Check MAC Range..");
	}

    return FALSE;
}


BOOL CBarcode::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPen myPen;
	int i ;
	CRect rect ;

	myPen.CreatePen(PS_SOLID, 1, RGB_MYCOLOR);
	
	CPen *oldPen = pDC->SelectObject(&myPen) ;
	GetClientRect(&rect);
	for(i = 0 ; i <= rect.bottom;)
	{
		pDC->MoveTo(0, i);
		pDC->LineTo(rect.right, i);
		i++;
		pDC->SelectObject (&myPen);
	}
	pDC->SelectObject(oldPen) ;
	return TRUE ; 
}