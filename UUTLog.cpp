// UUTLog.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "UUTLog.h"
#include "INI.h"
#include "Imports.h"
#include "SADirRead.h"

#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int UUTLog::ActiveUUTLog =  FALSE;
int UUTLog::ActiveUUTLog1 = FALSE;
int UUTLog::ActiveUUTLog2 = FALSE;
int UUTLog::ActiveUUTLog3 = FALSE;
int UUTLog::ActiveUUTLog4 = FALSE;
int UUTLog::ActiveUUTLog5 = FALSE;
int UUTLog::ActiveUUTLog6 = FALSE;

#define RGB_BLACK	RGB(0x00, 0x00, 0x00)
#define RGB_WHITE	RGB(0xFF, 0xFF, 0xFF)
#define RGB_RED	RGB(255,0,0)
#define RGB_GREEN	RGB(0,255,0)
#define RGB_YELLOW	RGB(255,255,0)
#define RGB_BLUE RGB(0,0,255)
#define RGB_GRAY RGB(128,128,128)
#define RGB_SKYBLUE RGB( 0 , 250 , 250)
#define RGB_AZURE RGB(240,255,240)
#define RGB_MEDIUMBLUE RGB(0,0,205)
#define RGB_SKYBLUE1 RGB(135,206,235)
#define RGB_BLUE2 RGB(175,238,238)

#define WM_TEST				WM_USER+1
#define WM_UUTFAIL			WM_USER+2
#define WM_UUTBOOT2			WM_USER+3
#define WM_UUTBOOT3			WM_USER+4
#define WM_KEYTEST			WM_USER+5
#define WM_ALLPASS			WM_USER+6
#define WM_PMTEST			WM_USER+7
#define WM_POWERON			WM_USER+8
#define WM_CMOSERROR		WM_USER+9
#define WM_UUTPASS			WM_USER+10	
#define WM_UUT1CLOSE		WM_USER+11	
#define WM_UUT2CLOSE		WM_USER+12
#define WM_UUT3CLOSE		WM_USER+13
#define WM_UUT4CLOSE		WM_USER+14
#define WM_UUT5CLOSE		WM_USER+15
#define WM_UUT6CLOSE		WM_USER+16

#define WM_ENDSFIS			WM_USER+20

#define BYTES_PER_READ 65536

CFont mTimeFont;

#define UUT1_TIMER	101
#define UUT2_TIMER	102
#define UUT3_TIMER	103
#define UUT4_TIMER	104
#define UUT5_TIMER	105
#define UUT6_TIMER	106

SOCKET  g_Socket  =   0;

/////////////////////////////////////////////////////////////////////////////
// UUTLog dialog
IMPLEMENT_DYNAMIC(UUTLog, CDialog)

UUTLog::UUTLog(ModelessDialogTracker& tracker)
	: ModelessDialogHelper(tracker, *this)
{
	//{{AFX_DATA_INIT(UUTLog)
	//}}AFX_DATA_INIT
}

void UUTLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UUTLog)
	DDX_Control(pDX, IDC_UUTTIME, m_UUTTime);
	DDX_Control(pDX, IDC_STRING, m_UUTString);
	DDX_Control(pDX, IDC_UUTEXIT, m_UUTExit);
	DDX_Control(pDX, IDC_UUTRETEST, m_UUTReTest);
	DDX_Control(pDX, IDC_UUTSTART, m_UUTStart);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_UUTNUM, m_UUTName);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_EDIT1, m_DispLog);
	//}}AFX_DATA_MAP
}

UUTLog::~UUTLog()
{

}

BEGIN_MESSAGE_MAP(UUTLog, CDialog)
	//{{AFX_MSG_MAP(UUTLog)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_UUTSTART, OnUutStart)
	ON_BN_CLICKED(IDC_UUTRETEST, OnUutReTest)
	ON_BN_CLICKED(IDC_UUTEXIT, OnUutExit)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDD_ENCRYPT, OnEncrypt)
	ON_COMMAND(IDD_DECRYPT, OnDecrypt)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDC_UUTTest, Show)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UUTLog message handlers

BOOL UUTLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_InitialState  = TRUE;

	STUUT1 = 0 ; 
	STUUT2 = 0 ; 
	STUUT3 = 0 ; 
	STUUT4 = 0 ; 
	STUUT5 = 0 ; 
	STUUT6 = 0 ; 

	SetTimer(1, 300, 0);

	//	UUT Timer Static Control
	
	mTimeFont.Detach();
	VERIFY(mTimeFont.CreateFont(
						   18,                        // nHeight
						   0,                         // nWidth
						   0,                         // nEscapement
						   0,                         // nOrientation
						   FW_NORMAL|FW_BOLD,         // nWeight
						   FALSE,                     // bItalic
						   FALSE,                     // bUnderline
						   0,                         // cStrikeOut
						   ANSI_CHARSET,              // nCharSet
						   OUT_DEFAULT_PRECIS,        // nOutPrecision
						   CLIP_DEFAULT_PRECIS,       // nClipPrecision
						   DEFAULT_QUALITY,           // nQuality
						   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						   "Courier New"));			  // lpszFacename

	m_UUTTime.SetFont(&mTimeFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void UUTLog::OnTest()
{
	UpdateData(FALSE);
}

UINT UUTLog::FileShareingFun(LPVOID pParam)
{
    _STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    UUTLog *ptr = (UUTLog*)(FromHandle(Thread_Info->m_hwnd));
    CEdit *m_Edt = (CEdit*)(ptr->GetDlgItem(IDC_EDIT1));
	CIniReader  m_IniReader;
	CString str , data , str1;
	CString FailStr = "";
	CString m_UUTNum;
	CString m_UUTBar,m_UUTMac,m_UUTGUID,m_TesterID;

	char buff[30720];					// Buffer size = 30k 
	memset(buff , NULL , 30720);

	CFileFind FindFile;
	CFile FileCtrl;			
	CFile FileCtrl2;
	CTime t; 

	ULONGLONG position1 , position2 = 0;

	HWND hWnd = ::FindWindow (NULL, NULL);
	char tempTitle [256];

	while (hWnd)
	{
		::GetWindowText (hWnd, tempTitle, 255);
		if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
		{
            str = tempTitle;

			if(str.Compare("USI FCT HostCtrl") == 0)
			{
				break;
			}
		}
		hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
	}

	if(Thread_Info->m_UUTNumber == 2)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT02.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT02.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT02.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT02.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT02.ERR open error!");
			return 1;
		}
	
		FileCtrl2.SeekToBegin();
	}
	else if(Thread_Info->m_UUTNumber == 3)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT03.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT03.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT03.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT03.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT03.ERR open error!");
			return 1;
		}
	
		FileCtrl2.SeekToBegin();
	}
	else if(Thread_Info->m_UUTNumber == 4)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT04.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT04.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT04.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT04.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT04.ERR open error!");
			return 1;
		}
	
		FileCtrl2.SeekToBegin();
	}
	else if(Thread_Info->m_UUTNumber == 1)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT01.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT01.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT01.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT01.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT01.ERR open error!");
			return 1;
		}

		FileCtrl2.SeekToBegin();
	}
	else if(Thread_Info->m_UUTNumber == 5)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT05.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT05.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT05.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT05.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT05.ERR open error!");
			return 1;
		}

		FileCtrl2.SeekToBegin();
	}
	else if(Thread_Info->m_UUTNumber == 6)
	{
		m_IniReader.setINIFileName("D:\\USINOTE\\UUT06.ERR");

		if(FileCtrl.Open("D:\\USINOTE\\UUT06.LOG", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT06.LOG open error!");
			return 1;
		}

		if(FileCtrl2.Open("D:\\USINOTE\\UUT06.ERR", CFile::modeRead | CFile::shareDenyNone ) == 0)
		{
			AfxMessageBox("UUT06.ERR open error!");
			return 1;
		}

		FileCtrl2.SeekToBegin();
	}
	else
	{
		//AfxMessageBox("Error occur!");
		ptr->SetDlgItemText(IDC_STRING , "Error occur!");
		return 0;
	}

	position1 = FileCtrl.GetPosition();

	while(ptr->ActiveUUTLog)
	{
		   FileCtrl.SeekToEnd();

           position2 = FileCtrl.GetPosition();
		   
	       str1 = m_IniReader.getKeyValue( "Check Point" , "UUT");

           if(position2 > position1)
		   {
			   FileCtrl.SeekToBegin();
			   FileCtrl.Seek( unsigned long(position1) , CFile::current );

               if((unsigned long(position2) - unsigned long(position1)) < 30720)
			   {
					FileCtrl.Read( buff , (unsigned long(position2)) - (unsigned long(position1)));
 				    //buff[position2] = NULL;

					m_Edt->ReplaceSel( buff );

					str = buff;

					memset( buff , NULL , unsigned long(position2) );

					position1 = position2; 
				}
			    else
				{
					AfxMessageBox("Buffer overflow!");
					//ActiveUUTLog1 = FALSE;
					ptr->SetDlgItemText(IDC_STRING , "UUT Restart!");
					m_IniReader.setKey("","Check Point","UUT");
					break;
				}
		   }

		   if(str1.Compare("") != 0)
		   {
			    if(str1.Compare("DIAG_END") == 0)		// USD Diagnostic Test End!
				{ 
					ptr->SetDlgItemText(IDC_STRING , "See you in the next boot!");
					m_IniReader.setKey("","Check Point","UUT");
					//break;
				}
				if(str1.Compare("RESTART") == 0)
				{
					ptr->SetDlgItemText(IDC_STRING , "UUT Restart!");
					m_IniReader.setKey("RESTART_OK","Check Point","UUT");
					break;
				}
				if(str1.Compare("RESTART1") == 0)
				{
					ptr->SetDlgItemText(IDC_STRING , "Restart1!");
					m_IniReader.setKey("","Check Point","UUT");
					//m_IniReader.setKey("RESTART_OK","Check Point","UUT");
					break;
				}
				if(str1.Compare("HOST_FAIL") == 0)
				{
					ptr->SetDlgItemText(IDC_STRING , "HOST FAIL!");
					m_IniReader.setKey("","Check Point","UUT");
					FailStr = "FFFE";
					break;
				}
				if(str1.Compare("UUT_DEAD") == 0)
				{
					ptr->SetDlgItemText(IDC_STRING , "UUT Dead!");
					m_IniReader.setKey("","Check Point","UUT");
					FailStr = "FFFF";
					break;
				}
				if(str1.Compare("UUT_HANG") == 0)
				{
					ptr->SetDlgItemText(IDC_STRING , "UUT Hang-up!");
					m_IniReader.setKey("","Check Point","UUT");
					FailStr = "FFFF";
					m_IniReader.setKey(FailStr,"Error Code","UUT");
					break;
				}
				if(str1.Compare("TOTAL_END") == 0)		// USD Diagnostic Test End!
				{ 
					str1 = m_IniReader.getKeyValue( "Error Code" , "UUT");
					if(str1.Compare("") != 0)
					{
						//Fail
						FailStr +=str1;
						m_IniReader.setKey("","Check Point","UUT");
						break;
					}
					else 
						break;
				}
			    ptr->SetDlgItemText(IDC_STRING , "");
				ptr->SetDlgItemText(IDC_STRING , str1);
				m_IniReader.setKey("","Check Point","UUT");
		   }

		   //   Read Barcode/Maccode/GUID from UUT.ERR

		   ptr->GetDlgItemText(IDC_STRING , str1);

		   if(str1.Compare("Close") != 0)
		   {
			   	ptr->GetDlgItemText(IDC_STATIC1 , str1);
				if(str1.Compare("BAR: ") == 0)
				{
					str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");
					ptr->SetDlgItemText(IDC_STATIC1 , str1+str);
					m_UUTBar = str;
				}
				ptr->GetDlgItemText(IDC_STATIC2 , str1);
				if(str1.Compare("MAC: ") == 0)
				{
					str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");
					ptr->SetDlgItemText(IDC_STATIC2 , str1+str);
					m_UUTMac = str;
				}
				ptr->GetDlgItemText(IDC_STATIC3 , str1);
				if(str1.Compare("GUID: ") == 0)
				{
					str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");
					ptr->SetDlgItemText(IDC_STATIC3 , str1+str);
					m_UUTGUID = str;
				}
           }

		   Sleep(1);
	}

	m_TesterID = m_IniReader.getKeyValue( "TesterID" , "UUT");

	ptr->GetDlgItemText(IDC_STRING , str);

	if(str.Compare("Restart1!") == 0 )
	{
		//ptr->SetDlgItemText(IDC_STRING , "Restart");

		FileCtrl.Close();
		FileCtrl2.Close();

		ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

		str = m_UUTNum.Right(1);

		m_UUTNum = str;

		DeleteFile("D:\\USINOTE\\UUT0" + m_UUTNum + ".LOG");
		DeleteFile("D:\\USINOTE\\UUT0" + m_UUTNum + ".ERR");

		ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.ini");

		m_IniReader.setKey("Restart",m_UUTNum,"UUTSTATE");
		
		return 0;
	}

	ptr->GetDlgItemText(IDC_STRING , str);

	if(str.Compare("UUT Restart!") != 0 )
	{
		    CString str10 = "";
		    ptr->GetDlgItemText(IDC_STRING , str10);
			
			if(FailStr.Compare("") == 0)						
			{
				ptr->SetDlgItemText(IDC_STRING , "Test PASS!");
				Thread_Info->_this->ShowPass();
				str1 = "Test PASS!";
			}
			else
			{
				str1 = "Fail! " + FailStr;
				ptr->SetDlgItemText(IDC_STRING , str1);
				Thread_Info->_this->ShowFail();
			}

			FileCtrl.Close();
			FileCtrl2.Close();

			//*********************************************************************//
			//		             Add test info. to UUTX.LOG file		           //
			//*********************************************************************//

			ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

			str = m_UUTNum.Right(1);
		 
			m_UUTNum = str;

			FileCtrl.Open("D:\\USINOTE\\UUT0" + m_UUTNum + ".LOG" , CFile::modeReadWrite);	 
	
			FileCtrl.SeekToEnd();

			str = "\r\n";
			FileCtrl.Write(str , str.GetLength()+1);

			str = "****************************************************************\r\n";
			FileCtrl.Write(str , str.GetLength()+1);

			str = "Test Station : UUT" + m_UUTNum + "\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
		
			t = CTime::GetCurrentTime();

			str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
			FileCtrl.Write(str , str.GetLength()+1);

			str = "\r\n";
			FileCtrl.Write(str , str.GetLength()+1);

			//******************************************//
			//		Record Test Fail information		// 
			if(str10.Compare("UUT Hang-up!") == 0)
			{
				str1 = "UUT Hang-up! FFFF";
			}
			
			if(str10.Compare("UUT Dead!") == 0)
			{
				str1 = "UUT HEAD! FFFF";
			}

			if(str10.Compare("HOST FAIL!") == 0)
			{
				str1 = "HOST Fail! FFFE";
			}
			//******************************************//

			str = "Test Result : " + str1;
			FileCtrl.Write(str , str.GetLength()+1);

			str = "\r\n";
			FileCtrl.Write(str , str.GetLength()+1);

			CStringList* myOtherList = m_IniReader.getSectionData("UUT");

			POSITION position;

			for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
			{
				str = myOtherList->GetNext(position);
				str+= "\r\n";
				FileCtrl.Write(str , str.GetLength());
			}

			str = "*************************   END of LOG   ***********************\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
		
			str = "\r\n";
			FileCtrl.Write(str , str.GetLength()+1);

			FileCtrl.Close();

			if(FailStr.Compare("FFFE") == 0)
			{
				ptr->SetDlgItemText(IDC_STRING, "FFFE Return!");

				return 0;
			}

			if(FailStr.Compare("FFFF") == 0)
			{
				if(str10.Compare("UUT Hang-up!") != 0)
				{
					ptr->SetDlgItemText(IDC_STRING, "FFFF Return!");

					return 0;
				}
			}

			//*********************************************************************//
			//		                         End of UUTX.log		               //
			//*********************************************************************//

			ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_IniReader.setKey(m_UUTBar,m_UUTNum + "BAR","BARMAC");
			m_IniReader.setKey(m_UUTMac,m_UUTNum + "MAC","BARMAC");
			m_IniReader.setKey(m_UUTGUID,m_UUTNum + "GUID","BARMAC");
			m_IniReader.setKey(FailStr,m_UUTNum + "RESULT","BARMAC");
			m_IniReader.setKey(m_TesterID,m_UUTNum + "TESTID","BARMAC");

			m_IniReader.setKey("TEST END",m_UUTNum,"UUTSTATE");

			return 0;
	}
	else 	
	{
		//**********************************************************************//
		//						When HOST receive RESTART						//
		//**********************************************************************//

		int count = 0;
		while(1)
		{
			CString str1 = m_IniReader.getKeyValue( "Check Point" , "UUT");

			if(str1.Compare("RESTART_ACK") == 0)
			{
				break;
			}
			else
			{
				Sleep(1000);
				count++;
			}

			if(count >= 30)
			{
				ptr->SetDlgItemText(IDC_STRING, "No Restart_Ack");
				break;
			}
		}
		
		FileCtrl.Close();
		FileCtrl2.Close();

		ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

		str = m_UUTNum.Right(1);

		m_UUTNum = str;

		DeleteFile("D:\\USINOTE\\UUT0" + m_UUTNum + ".LOG");
		DeleteFile("D:\\USINOTE\\UUT0" + m_UUTNum + ".ERR");

		ptr->GetDlgItemText(IDC_UUTNUM , m_UUTNum);

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.ini");

		m_IniReader.setKey("Restart",m_UUTNum,"UUTSTATE");
		
		return 0;
	}

}

void UUTLog::StartUUT1()
{
	//AfxMessageBox("UUT1");

	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

    	m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 1;
		m_FileCtrl._this = this;

		m_Thread1 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");

		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);

		time = CTime::GetCurrentTime();
		STUUT1 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT1_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::OnOK()   
{

}

void UUTLog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    if(nIDEvent == 1)
	{
		if(m_InitialState)
		{
			m_InitialState = FALSE;
			IniDlgGUI();

			KillTimer(1);
		}
	}
	else if(nIDEvent == 2)
	{
		
	}

	CDialog::OnTimer(nIDEvent);
}

void UUTLog::StartUUT2()
{
	//AfxMessageBox("UUT2");
	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

		m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 2;
		m_FileCtrl._this = this;

		m_Thread2 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");
		
		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);

		time = CTime::GetCurrentTime();
		STUUT2 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT2_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::StartUUT3()
{
	//AfxMessageBox("UUT3");
	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

		m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 3;
		m_FileCtrl._this = this;

		m_Thread3 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");
		
		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);
		
		time = CTime::GetCurrentTime();
		STUUT3 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT3_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::StartUUT4()
{
	//AfxMessageBox("UUT4");
	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

		m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 4;
		m_FileCtrl._this = this;
	
		m_Thread4 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");
		
		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);
		
		time = CTime::GetCurrentTime();
		STUUT4 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT4_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::IniDlgGUI()
{
	rgbBkgnd = RGB_WHITE;
	rgbText = RGB_BLUE;
			
	rgbBkgnd = RGB_AZURE;
	m_Static1.ChangeFontHeight(18, TRUE);
	m_Static1.SetTextColor(rgbText);
	m_Static1.SetBkColor(rgbBkgnd);
			
	m_Static2.ChangeFontHeight(20, TRUE);
	m_Static2.SetTextColor(rgbText);
	m_Static2.SetBkColor(rgbBkgnd);

	m_Static3.ChangeFontHeight(20, TRUE);
	m_Static3.SetTextColor(rgbText);
	m_Static3.SetBkColor(rgbBkgnd);

	rgbText = RGB_BLACK;
	m_UUTName.ChangeFontHeight(20, TRUE);
	m_UUTName.SetTextColor(rgbText);
	m_UUTName.SetBkColor(rgbBkgnd);

	//    Buuton
	m_UUTStart.ChangeFontHeight(15, TRUE);
	m_UUTReTest.ChangeFontHeight(15, TRUE);
	m_UUTExit.ChangeFontHeight(15, TRUE);

	rgbText = RGB_GRAY;
	rgbBkgnd = RGB_GREEN;
	m_UUTString.SetTextColor(rgbText);
	m_UUTString.ChangeFontHeight(50, TRUE);
	m_UUTString.SetBkColor(rgbBkgnd);
}

void UUTLog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	/*
	GetDlgItemText(IDC_UUTNUM , m_Str);

	if(m_Str.Compare("UUT1") == 0)
	{
		//AfxMessageBox("UUT1");
		SetWindowPos(0,0,0,1024/3,384, SWP_SHOWWINDOW);
	}
	if(m_Str.Compare("UUT2") == 0)
	{
		//AfxMessageBox("UUT2");
		SetWindowPos(0,342,0,341,384, SWP_SHOWWINDOW);
	}
	if(m_Str.Compare("UUT3") == 0)
	{
		//AfxMessageBox("UUT3");
		SetWindowPos(0,683,0,int(1024/3),768/2, SWP_SHOWWINDOW);
	}
	if(m_Str.Compare("UUT4") == 0)
	{
		//AfxMessageBox("UUT4");
		SetWindowPos(0,0,385,int (1024/3),768/2, SWP_SHOWWINDOW);
	}
	if(m_Str.Compare("UUT5") == 0)
	{
		//AfxMessageBox("UUT5");
		SetWindowPos(0,342,385,int (1024/3),768/2, SWP_SHOWWINDOW);
	}
	if(m_Str.Compare("UUT6") == 0)
	{
		//AfxMessageBox("UUT6");
		SetWindowPos(0,683,385,1024/3,768/2, SWP_SHOWWINDOW);
	}
	*/
}

void UUTLog::Show()
{
	//rgbText = RGB_BLUE;
	//m_UUTString.SetTextColor(rgbText);
	//m_UUTString.ChangeFontHeight(20, TRUE);
	//SetDlgItemText(IDC_STRING, " SFIS Test! ");

	//m_UUTString.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );


	//_this->TrackSFIS1.CloseDialog();
}

void UUTLog::Setenv()
{
	GetDlgItemText(IDC_UUTNUM , m_Str);

	if(m_Str.Compare("UUT1") == 0)
	{
		//AfxMessageBox("UUT1");

		DeleteFile("d:\\usinote\\UUT01.LOG");
		DeleteFile("d:\\usinote\\UUT01.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT01.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT01.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
	if(m_Str.Compare("UUT2") == 0)
	{
		//AfxMessageBox("UUT2");

		DeleteFile("d:\\usinote\\UUT02.LOG");
		DeleteFile("d:\\usinote\\UUT02.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT02.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT02.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
	if(m_Str.Compare("UUT3") == 0)
	{
		//AfxMessageBox("UUT3");

		DeleteFile("d:\\usinote\\UUT03.LOG");
		DeleteFile("d:\\usinote\\UUT03.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT03.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT03.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
	if(m_Str.Compare("UUT4") == 0)
	{
		//AfxMessageBox("UUT4");

		DeleteFile("d:\\usinote\\UUT04.LOG");
		DeleteFile("d:\\usinote\\UUT04.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT04.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT04.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
	if(m_Str.Compare("UUT5") == 0)
	{
		//AfxMessageBox("UUT5");

		DeleteFile("d:\\usinote\\UUT05.LOG");
		DeleteFile("d:\\usinote\\UUT05.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT05.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT05.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
	if(m_Str.Compare("UUT6") == 0)
	{
		//AfxMessageBox("UUT2");

		DeleteFile("d:\\usinote\\UUT06.LOG");
		DeleteFile("d:\\usinote\\UUT06.ERR");

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT06.LOG") , CFile::modeCreate);

		m_NewFile.Close();

		Sleep(100);
		m_NewFile.Open(("D:\\USINOTE\\UUT06.ERR") , CFile::modeCreate | CFile::modeReadWrite);

		m_Str = "[UUT]\r\n";
		m_NewFile.Write(m_Str , m_Str.GetLength());

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			m_NewFile.Write(m_Str , m_Str.GetLength());
		}

		m_NewFile.Close();
	}
}

void UUTLog::OnUutStart() 
{
	// TODO: Add your control notification handler code here

	//******************************************************************//
	//					       HOST / UUT FAIL					        //
	//******************************************************************//

	SetDlgItemText(IDC_STRING , "Connect to SFIS");

	CFileFind findfile;
	
	HWND hWnd = ::FindWindow (NULL, NULL);
	char tempTitle [256];

	GetDlgItemText(IDC_UUTNUM , m_Str);

	::WinExec("D:\\USINOTE\\HOSTFail.exe" , 0);

	Sleep(500);

	if(m_Str.Compare("UUT1") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT01.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT1 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
	
	if(m_Str.Compare("UUT2") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT02.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT2 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
	
	if(m_Str.Compare("UUT3") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT03.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT3 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}

	}
	
	if(m_Str.Compare("UUT4") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT04.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT4 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}

	}
	
	if(m_Str.Compare("UUT5") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT05.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT5 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}

	if(m_Str.Compare("UUT6") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT06.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT6 Dialog			    //

				m_IniReader.setKey("HOST_FAIL","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
}

void UUTLog::OnUutReTest() 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("ReTest");

	GetDlgItemText(IDC_UUTNUM , str);

	if(str.Compare("UUT1") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
	if(str.Compare("UUT2") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
	if(str.Compare("UUT3") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
	if(str.Compare("UUT4") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
	if(str.Compare("UUT5") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
	if(str.Compare("UUT6") == 0)
	{
		m_FileCtrl.m_Run = FALSE;
	}
 
}

void UUTLog::OnUutExit() 
{
	// TODO: Add your control notification handler code here

	//******************************************************************//
	//					           UUT Dead	    				        //
	//******************************************************************//

	SetDlgItemText(IDC_STRING , "Connect to SFIS");

	CFileFind findfile;
	
	HWND hWnd = ::FindWindow (NULL, NULL);
	char tempTitle [256];

	GetDlgItemText(IDC_UUTNUM , m_Str);

	::WinExec("D:\\USINOTE\\UUTDead.exe" , 0);

	Sleep(500);

	if(m_Str.Compare("UUT1") == 0)
	{
		//				If HOST is running test				//
		if(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT01.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT1 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
	
	if(m_Str.Compare("UUT2") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT02.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT2 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
	
	if(m_Str.Compare("UUT3") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT03.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT3 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}

	}
	
	if(m_Str.Compare("UUT4") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0))	
			{	
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT04.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT4 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}

	}
	
	if(m_Str.Compare("UUT5") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT05.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT5 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}

	if(m_Str.Compare("UUT6") == 0)
	{
		//				If HOST is running test				//

		if(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
				m_Btn->EnableWindow(FALSE);

				while (hWnd)
				{
					::GetWindowText (hWnd, tempTitle, 255);
					if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
					{
						m_Str = tempTitle;

						if(m_Str.Compare("Taurus PClient") == 0)
						{
							break;
						}
					}
	
					hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
				}

				char *buf = NULL;

				GetDlgItemText(IDC_UUTNUM , m_Str);

				buf	= new char [m_Str.GetLength()+1];

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				//				Find 1st window ID				//
				HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find 2nd window ID				//
				HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

				m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT06.ERR");

				//				Find 3rd window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Bar Code ID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "Lan1 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_Child = ::GetNextWindow(m_NextChild , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "IEEE1394 Mac" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

				//				Find next window ID				//
				m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

				m_Str = m_IniReader.getKeyValue( "TesterID" , "UUT");

				memcpy(buf , m_Str , m_Str.GetLength()+1);

				::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);

				Sleep(100);

				//				Close UUT6 Dialog			    //

				m_IniReader.setKey("UUT_DEAD","Check Point","UUT");
			}
		}
		else
		{
			SetDlgItemText(IDC_STRING , "No Log File..");

			while (hWnd)
			{
				::GetWindowText (hWnd, tempTitle, 255);
				if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
				{
					m_Str = tempTitle;

					if(m_Str.Compare("Taurus PClient") == 0)
					{
						break;
					}
				}
	
				hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
			}

			char *buf = NULL;

			GetDlgItemText(IDC_UUTNUM , m_Str);

			buf	= new char [m_Str.GetLength()+1];

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			//				Find 1st window ID				//
			HWND m_Child = ::GetWindow(hWnd , GW_CHILD);

			::SendMessage(m_Child , WM_SETTEXT , 0 , (LPARAM)buf);

			//				Find 2nd window ID				//
			HWND m_NextChild = ::GetNextWindow(m_Child , GW_HWNDNEXT);

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			m_Str = m_IniReader.getKeyValue( "Location" , "PROJECT");

			memcpy(buf , m_Str , m_Str.GetLength()+1);

			::SendMessage(m_NextChild , WM_SETTEXT , 0 , (LPARAM)buf);
		}
	}
}

void UUTLog::StartUUT5()
{
	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

		m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 5;
		m_FileCtrl._this = this;
	
		m_Thread5 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");
		
		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);
		
		time = CTime::GetCurrentTime();
		STUUT5 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT5_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::StartUUT6()
{
	GetDlgItemText(IDC_UUTSTATUS , m_Str);

	if(m_Str.Compare("Connect") != 0)
	{
		if(!(ChkLog()))
		{
			SetDlgItemText(IDC_STRING, "Log-File Path Fail!");
			ShowFail();
			return;
		}

		//if(!(PingTest1()))
		//{
		//	SetDlgItemText(IDC_STRING, "Ping SFIS Fail!");
		//	ShowFail();
		//	return;
		//}

		m_FileCtrl.m_hwnd = this->m_hWnd;
		m_FileCtrl.m_UUTNumber = 6;
		m_FileCtrl._this = this;
	
		m_Thread6 = AfxBeginThread(FileShareingFun , (LPVOID)&m_FileCtrl);

		SetDlgItemText(IDC_STRING, "Open");
		SetDlgItemText(IDC_UUTSTATUS, "Connect");
		
		CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
		m_Btn->EnableWindow(FALSE);
		m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
		m_Btn->EnableWindow(TRUE);
		m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
		m_Btn->EnableWindow(TRUE);

		rgbText = RGB_BLUE;
		m_UUTString.SetTextColor(rgbText);
		
		//time = CTime::GetCurrentTime();
		//STUUT6 = ( time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond() );

		//SetTimer(UUT6_TIMER, 800, NULL);
		m_Str.Format("Start Time : %d:%d:%d", time.GetHour() , time.GetMinute() , time.GetSecond());
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT1()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT1");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT1 = 0;
		m_Str.Format("UUT1 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT2()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT2");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT2 = 0;
		m_Str.Format("UUT2 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT3()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT3");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT3 = 0;
		m_Str.Format("UUT3 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT4()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT4");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT4 = 0;
		m_Str.Format("UUT4 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT5()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT5");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT5 = 0;
		m_Str.Format("UUT5 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::InitialUUT6()
{
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	SetDlgItemText(IDC_UUTNUM , "UUT6");

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(FALSE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTEXIT);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	m_Btn->EnableWindow(FALSE);
	
    if(!(FindFile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0)))	
	{
		rgbText = RGB_RED;
		m_UUTString.SetTextColor(rgbText);
		SetDlgItemText(IDC_STRING, "Close");

		STUUT6 = 0;
		m_Str.Format("UUT6 Test Time : 0%d sec",0);
		m_UUTTime.SetWindowText(m_Str);
	}
}

void UUTLog::TestLog()
{
	CFile FileCtrl;	
	CTime t; 

	GetDlgItemText(IDC_UUTNUM , m_Str);

	if(m_Str.Compare("UUT1") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT1.LOG", CFile::modeReadWrite);	 
	
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT1\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT1.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}
	if(m_Str.Compare("UUT2") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT2.LOG", CFile::modeReadWrite);	 
			
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT2\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT2.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}	
	if(m_Str.Compare("UUT3") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT3.LOG", CFile::modeReadWrite);	 
			
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT3\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT3.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}
	if(m_Str.Compare("UUT4") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT4.LOG", CFile::modeReadWrite);	 
			
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT4\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT4.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}
	if(m_Str.Compare("UUT5") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT5.LOG", CFile::modeReadWrite);	 
			
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT5\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT5.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}
	if(m_Str.Compare("UUT6") == 0)
	{
		FileCtrl.Open("D:\\USINOTE\\UUT6.LOG", CFile::modeReadWrite);	 
			
		FileCtrl.SeekToEnd();

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		str = "Test Station : UUT6\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		
		t = CTime::GetCurrentTime();

		str.Format("Time:  %d/%d/%d    %d:%d:%d" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		GetDlgItemText(IDC_STRING , m_Str);
		str = "Test Result : " + m_Str;
		FileCtrl.Write(str , str.GetLength()+1);

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		m_IniReader.setINIFileName("D:\\USINOTE\\UUT6.ERR");
		CStringList* myOtherList = m_IniReader.getSectionData("UUT");

		POSITION position;

		for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
		{
			m_Str = myOtherList->GetNext(position);
			m_Str+= "\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());
		}

		str = "****************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);

		FileCtrl.Close();
	}
}

void UUTLog::ShowPass()
{
	rgbText = RGB_BLUE;
	rgbBkgnd = RGB_GRAY;
	m_UUTString.SetTextBlinkColors(rgbText , rgbBkgnd);
    m_UUTString.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
}

void UUTLog::ShowFail()
{
	rgbText = RGB_RED;
	rgbBkgnd = RGB_GRAY;
	m_UUTString.SetTextBlinkColors(rgbText , rgbBkgnd);
    m_UUTString.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
}

HBRUSH UUTLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	if(pWnd->GetDlgCtrlID() == IDC_UUTTIME)
	{
		hbr = CreateSolidBrush(RGB(0,0,255));
		pDC->SetBkColor(RGB(0,0,255));
		pDC->SetTextColor(RGB(0,255,0));
		pDC->SetBkMode(OPAQUE);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL UUTLog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPen myPen;
	int i ;
	CRect rect ;

	myPen.CreatePen(PS_SOLID, 1, RGB_SKYBLUE);
	
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

void UUTLog::InitialSFIS1()
{
	SetDlgItemText(IDC_UUTNUM , "UUT1");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);

	//m_UUTString.ChangeFontHeight(20, TRUE);
}

BOOL UUTLog::InitWinSock1()
{
	int             nRet    =   NULL;
	WSADATA         wsaData;
	sockaddr_in     local;

	nRet    =   WSAStartup(MAKEWORD(1,1),&wsaData);
	if(nRet!=0)
    {
		//AfxMessageBox("WSAStartup Error!!");
		SetDlgItemText(IDC_STRING , "WSAStartup Error!");
		return FALSE;
    }

	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short)1024);

	g_Socket =   socket(AF_INET,SOCK_STREAM,0);
    if(g_Socket == INVALID_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL UUTLog::ConnectSFIS()
{
	sockaddr_in     server;
    hostent        *hp      =   NULL;
    char           *pchIP   =   NULL;
	
    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_szIP = m_IniReader.getKeyValue( "IP" , "SFIS");
	m_szPort = m_IniReader.getKeyValue( "PORT" , "SFIS");

    if(!(InitWinSock1()))
	{
		SetDlgItemText(IDC_STRING , "Initial Fail!");

		return FALSE;
	}

	//if(!(PingTest()))
	//{
		//AfxMessageBox("Ping Test Fail!");
		//SetDlgItemText(IDC_STRING , "Ping Test Fail!");
		//return FALSE;
	//}

    //UpdateData(TRUE);
    unsigned int addr;
    pchIP   =   m_szIP.GetBuffer(m_szIP.GetLength());
    addr    =   inet_addr(m_szIP);

    server.sin_addr.s_addr=addr;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(m_szPort));

	if(connect(g_Socket,(struct sockaddr*)&server,sizeof(server)))
    {
		//AfxMessageBox("Can't connect to SFIS Server !!");
		SetDlgItemText(IDC_STRING , "Connect Fail!");
		return FALSE;
    }

    m_FileCtrl.m_hwnd = this->m_hWnd;

	m_FileCtrl._this = this;

	m_FileCtrl.m_Run = TRUE;

	AfxBeginThread(SFISSocket , (LPVOID)&m_FileCtrl);

	return TRUE;
}

UINT UUTLog::SFISSocket(LPVOID pParam)
{
    _STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    UUTLog *ptr = (UUTLog*)(FromHandle(Thread_Info->m_hwnd));
	CEdit *m_Edt = (CEdit*)(ptr->GetDlgItem(IDC_EDIT1));

	CIniReader  m_IniReader;
 	char mess[24] ;
	CString str,FailStr; 
	int count = 0;
	CFileFind FindFile;
	CFile FileCtrl;
	CTime t; 

	memset(mess , 0 , 24);

	CString m_FixtureID,m_Bar,m_Employee,m_LineNO,m_Mac,m_Result,m_UUTNum,m_GUID,m_Path;
	
	//*****************************************************************************//
	//				  Get Barcode/Maccode/GUID/Result/Employee                     //
	//*****************************************************************************//

    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_FixtureID = m_IniReader.getKeyValue( "Fixture-ID" , "SFIS");
	m_LineNO = m_IniReader.getKeyValue( "Line-NO" , "SFIS");
	m_Path = m_IniReader.getKeyValue( "Location" , "PROJECT");

	ptr->GetDlgItemText(IDC_UUTNUM , str);
	m_UUTNum = str;
	m_Employee = m_IniReader.getKeyValue( str + "TESTID" , "BARMAC");

	m_Result = m_IniReader.getKeyValue( str + "RESULT" , "BARMAC");

	if(m_Result.Compare("") == 0)
	{
		m_Result = "OK,,,,";
	}
	else
	{
		m_Result = "FAIL,," + m_Result + ",,";
	}

	ptr->GetDlgItemText(IDC_STATIC1 , m_Bar);
	ptr->GetDlgItemText(IDC_STATIC2 , m_Mac);
	ptr->GetDlgItemText(IDC_STATIC3 , m_GUID);

	m_Mac = m_Mac.Right(12);

	goto Start;

Loop:

    while(Thread_Info->m_Run)
	{
 		str.Format("%d", count);
		m_Edt->ReplaceSel("Retry  " + str + "\r\n");
		m_Edt->ReplaceSel("\r\n");
		Sleep(6000);
	    count++;
Start:
		
	//*****************************************************************************//
	//	           Step1 : Send Barcode/Maccode for duplication check              //
	//*****************************************************************************//

	ptr->SetDlgItemText(IDC_STRING , "Connect to SFIS");
	Thread_Info->_this->ShowPass();

	m_Edt->ReplaceSel("Step1 : Send Barcode/Maccode for duplication check.\r\n");

 	str = m_FixtureID + "," + m_Bar + ",1," + m_Employee + "," + m_LineNO + "," + m_Mac;

	if((send(g_Socket , str , str.GetLength() , 0)) == SOCKET_ERROR)
	{
		ptr->SetDlgItemText(IDC_STRING , "SOCKET ERROR!");
		Thread_Info->_this->ShowFail();

		goto Loop;
	}
	
	str = "OK1";

	while(1)
	{
		if(!(Thread_Info->m_Run))
			goto Loop;

		if((recv(g_Socket , mess , 24 , 0)) != SOCKET_ERROR)
		{
			if(!(Thread_Info->m_Run))
				goto Loop;

			if((memcmp(str , mess , 3)) == 0)
			{
				m_Edt->ReplaceSel("PASS!...OK1\r\n");
				break;
			}
			else
			{
				if(!(Thread_Info->m_Run))
					goto Loop;

				Sleep(10);

				str = mess;

				m_Edt->ReplaceSel("FAIL! " + str + "\r\n");
				ptr->SetDlgItemText(IDC_STRING , "OK1 FAIL!\r\n");
				
				m_Edt->ReplaceSel("Please check your SFIS routing first..\r\n");

				Thread_Info->_this->ShowFail();

				goto Loop;

			}
		}
	}

	//*****************************************************************************//
	//	                                  Step1 End                                //
	//*****************************************************************************//

	Sleep(1500);
	
	//*****************************************************************************//
	//	               Step2 : Send Test result to SFIS for record                 //
	//*****************************************************************************//

	m_Edt->ReplaceSel("Step2 : Send Test Result for confirmation\r\n");

	str = m_FixtureID + "," + m_Bar + ",2," + m_Employee + "," + m_LineNO + "," + m_Mac  + "," + m_Result ;

	if((send(g_Socket , str , str.GetLength() , 0)) == SOCKET_ERROR)
	{
		ptr->SetDlgItemText(IDC_STRING , "SOCKET ERROR!");
		Thread_Info->_this->ShowFail();

		goto Loop;
	}

	str = "OK2";

	while(1)
	{
		if(!(Thread_Info->m_Run))
			goto Loop;

		if((recv(g_Socket , mess , 20 , 0)) != SOCKET_ERROR)
		{
			if(!(Thread_Info->m_Run))
				goto Loop;

			if((memcmp(str , mess , 3)) == 0)
			{
				m_Edt->ReplaceSel("SFIS communication PASS!\r\n");
				ptr->SetDlgItemText(IDC_STRING , "SFIS PASS!");

				Thread_Info->_this->ShowPass();

				//**************************************//
				//		Create TestLog.txt file		    //
				//**************************************//

				if(FindFile.FindFile("d:\\usinote\\log\\" + m_Path + "\\" + "testlog.txt" , 0)) 
				{
					if(!(FileCtrl.Open("d:\\usinote\\log\\" + m_Path + "\\" + "testlog.txt", CFile::modeReadWrite | CFile::shareDenyNone)))
					{
						ptr->SetDlgItemText(IDC_STRING , "TestLog.txt Fail!");
						//AfxMessageBox("Testlog File Open Error!");
						return 0;
					}
				}
				else
				{
					if(!(FileCtrl.Open("d:\\usinote\\log\\" + m_Path + "\\" + "testlog.txt", CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone)))
					{
						ptr->SetDlgItemText(IDC_STRING , "TestLog.txt Fail!");
						//AfxMessageBox("Testlog File Open Error!");
						return 0;
					}

					str = "HOSTName  UUT  TesterID   Path       Date      Time            Barcode            Maccode          GUID       Result          \r\n";
					FileCtrl.Write(str , str.GetLength()+1);
					str = "*****************************************************************************************************************\r\n";
					FileCtrl.Write(str , str.GetLength()+1);
				}

				FileCtrl.SeekToEnd();

				t = CTime::GetCurrentTime();

				ptr->GetDlgItemText(IDC_UUTNUM , str);

				m_Result = m_IniReader.getKeyValue( str + "RESULT" , "BARMAC");

				if(m_Result.Compare("") == 0)
				{
					m_Result = "PASS";
				}
				else
				{
					m_Result = "FAIL  " + m_Result;
				}
		
				char name[10];
				gethostname(name , 10);
				CString str1 = name;

				str.Format("%-8s %-5s %-7s %-10s %-2d/%-2d/%-2d %-2d:%-2d:%-2d %-22s %-15s %-16s  %-10s" , str1 , m_UUTNum , m_Employee , m_Path ,  t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond() , m_Bar , m_Mac , m_GUID , m_Result );
				FileCtrl.Write(str , str.GetLength()+1);

				str = "\r\n";
				FileCtrl.Write(str , str.GetLength()+1);

				FileCtrl.Close();

				//*********************************************************************//
				//		                     End of Testlog.txt		                   //
				//*********************************************************************//

				if(FindFile.FindFile("D:\\USINOTE\\LOG\\" + m_Path + "\\" + m_Bar + ".LOG" , 0)) 
				{
					//******************************//
					//		Combine two Files		//
					//******************************//

					//AfxMessageBox("Process1");

					CFile *pOutFile,*pInFile;

					pOutFile = new CStdioFile();
					pInFile  = new CStdioFile();

					CStringArray arr;
					char	pBuf[BYTES_PER_READ];

					ptr->GetDlgItemText(IDC_UUTNUM , str);
					str = str.Right(1);

					arr.Add("D:\\USINOTE\\LOG\\" + m_Path + "\\" + m_Bar + ".LOG");	// Frist File
					arr.Add("D:\\USINOTE\\UUT0" + str + ".LOG");	// Second File

					if(!pOutFile->Open("D:\\USINOTE\\LOG\\TEMP.LOG", CFile::modeCreate  | CFile::typeBinary | CFile::modeWrite))
					{
						ptr->SetDlgItemText(IDC_STRING , "Log Fail!");
						//AfxMessageBox("File Open Error!");
						return 0;
					}

					for(int i=0;i<2;++i)
					{
						if(!pInFile->Open(arr[i],CFile::modeRead | CFile::typeBinary))
						{
							ptr->SetDlgItemText(IDC_STRING , "Log Fail!");
							//AfxMessageBox("File Open Error!");
							return 0;
						}
						int nBytesRead = 0;
						do
						{
							nBytesRead = pInFile->Read(pBuf,BYTES_PER_READ);
							Sleep(1);
							pOutFile->Write(pBuf,nBytesRead);
							Sleep(1);
						}
						while(nBytesRead==BYTES_PER_READ);
						pInFile->Close();
						Sleep(50);
					}

					pOutFile->Close();
					Sleep(100);

					delete pOutFile;
					delete pInFile;

					DeleteFile("D:\\USINOTE\\LOG\\" + m_Path + "\\" + m_Bar + ".LOG");

					Sleep(10);
				
					rename("D:\\USINOTE\\LOG\\TEMP.LOG" , "D:\\USINOTE\\LOG\\" + m_Path + "\\" + m_Bar + ".LOG");
			
				    DeleteFile("D:\\USINOTE\\UUT0" + str + ".LOG");
				}
				else
				{
					//AfxMessageBox("Process2");

					ptr->GetDlgItemText(IDC_UUTNUM , str);

					str = str.Right(1);

					rename("D:\\USINOTE\\UUT0" + str + ".LOG" , ("D:\\USINOTE\\LOG\\" + m_Path + "\\" + m_Bar + ".LOG"));
				}

				m_IniReader.setINIFileName("D:\\USINOTE\\UUT0" + str + ".ERR");

				m_IniReader.setKey("SFIS_OK","Check Point","UUT");

				Sleep(500);

				int count = 0;

				while(1)
				{
					CString str1 = m_IniReader.getKeyValue( "Check Point" , "UUT");

					if(str1.Compare("SFIS_ACK") == 0)
					{
						//AfxMessageBox("Ack!");
						DeleteFile("D:\\USINOTE\\UUT0" + str + ".ERR");
						break;
					}
					else
					{
						Sleep(500);
						count++;
					}

					if(count >= 20)
					{
						ptr->SetDlgItemText(IDC_STRING , "No SFIS_Ack");
						DeleteFile("D:\\USINOTE\\UUT0" + str + ".ERR");
						//AfxMessageBox("break");
						break;
					}
				}

				ptr->GetDlgItemText(IDC_UUTNUM , str);

				m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
								
				m_IniReader.setKey("",str,"UUTSTATE");

				m_IniReader.setKey("END","RESULT","SFIS");

				return 0;
			}
			else
			{
				if(!(Thread_Info->m_Run))
					goto Loop;

				Sleep(10);
									
				str = mess;

				m_Edt->ReplaceSel("FAIL! " + str + "\r\n");
				ptr->SetDlgItemText(IDC_STRING , "OK2 FAIL!\r\n");
								
				m_Edt->ReplaceSel("Please check your SFIS routing first..\r\n");

				Thread_Info->_this->ShowFail();

				goto Loop;
			}
		}
	}

	//*****************************************************************************//
	//	                                  Step2 End                                //
	//*****************************************************************************//
	
	}

	//*****************************************************************************//
	//	                     ABORT !!!  (Connect SFIS FAIL!)                       //
	//*****************************************************************************//

	ptr->GetDlgItemText(IDC_UUTNUM , str);

	ptr->SetDlgItemText(IDC_STRING , "SFIS ABORT!");

	ptr->GetDlgItem(IDC_UUTEXIT)->EnableWindow(FALSE);
	ptr->GetDlgItem(IDC_UUTSTART)->EnableWindow(FALSE);
	ptr->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	Thread_Info->_this->ShowFail();

	str = str.Right(1);

	m_IniReader.setINIFileName("D:\\USINOTE\\UUT0" + str + ".ERR");

	m_IniReader.setKey("SFIS_FAIL","Check Point","UUT");

	count = 0;

	while(1)
	{
		CString str1 = m_IniReader.getKeyValue( "Check Point" , "UUT");

		if(str1.Compare("SFIS_ACK") == 0)
		{
			//AfxMessageBox("Ack!");
			DeleteFile("D:\\USINOTE\\UUT0" + str + ".ERR");
			DeleteFile("D:\\USINOTE\\UUT0" + str + ".LOG");

			break;
		}
		else
		{
			Sleep(500);
			count++;
		}
		
		if(count >= 20)
		{
			ptr->SetDlgItemText(IDC_STRING , "No SFIS_Ack");
			DeleteFile("D:\\USINOTE\\UUT0" + str + ".ERR");
			DeleteFile("D:\\USINOTE\\UUT0" + str + ".LOG");
			break;
		}
	}
	
	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	ptr->GetDlgItemText(IDC_UUTNUM , str);

	m_IniReader.setKey("",str,"UUTSTATE");

	Sleep(500);

	m_IniReader.setKey("END","RESULT","SFIS");

   	return 1;
}

void UUTLog::InitialSFIS2()
{
	SetDlgItemText(IDC_UUTNUM , "UUT2");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);
}

void UUTLog::InitialSFIS3()
{
	SetDlgItemText(IDC_UUTNUM , "UUT3");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);
}

void UUTLog::InitialSFIS4()
{
	SetDlgItemText(IDC_UUTNUM , "UUT4");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);
}

void UUTLog::InitialSFIS5()
{
	SetDlgItemText(IDC_UUTNUM , "UUT5");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);
}

void UUTLog::InitialSFIS6()
{
	SetDlgItemText(IDC_UUTNUM , "UUT6");
	SetDlgItemText(IDC_UUTSTATUS , "SFIS");
	SetDlgItemText(IDC_STATIC1 , m_Bar2);
	SetDlgItemText(IDC_STATIC2 , m_Mac2);
	SetDlgItemText(IDC_STATIC3 , m_GUID);

	CButton *m_Btn = (CButton*)GetDlgItem(IDC_UUTSTART);
	m_Btn->EnableWindow(TRUE);
	m_Btn = (CButton*)GetDlgItem(IDC_UUTRETEST);
	m_Btn->EnableWindow(TRUE);
}

BOOL UUTLog::PingTest1()
{
	/*
    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_Str = m_IniReader.getKeyValue( "IP" , "SFIS");

	CString csExecute;
	csExecute = "d:\\usinote\\cping.exe " + m_Str;
	
	STARTUPINFO startUpInfo;
	PROCESS_INFORMATION procInfo;
	unsigned long dwExitCode;

	GetStartupInfo(&startUpInfo);

	char command[128]; 
	strcpy(command, csExecute.GetBuffer(csExecute.GetLength()));

	if(!(CreateProcess(0,command,0,0,FALSE,CREATE_NEW_CONSOLE,0,0,&startUpInfo,&procInfo)))
	{
		//AfxMessageBox("Create Process Fail!");
		return FALSE;
	}

	WaitForSingleObject(procInfo.hProcess , INFINITE);

	GetExitCodeProcess(procInfo.hProcess, &dwExitCode );

	if(dwExitCode)
	{
		//AfxMessageBox("Return 1");
		return FALSE;
	}
	else
	{
		//AfxMessageBox("Return 0");
		return TRUE;
	}
	*/

	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_Str = m_IniReader.getKeyValue( "IP" , "SFIS");

	//if(PingTest(m_Str))
	//	return TRUE;
	//else
		return FALSE;
}

void UUTLog::OnEncrypt() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox("Encrypt Test");

	//MyEncryptFile("d:\\usinote\\im121.ini" , "SomePassKey");

	//MyEncryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");
}

void UUTLog::OnDecrypt() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox("Decrypt Test");

	//MyDecryptFile("d:\\usinote\\im121.ini" , "SomePassKey");

	//MyDecryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");
}

void UUTLog::OnButton1() 
{
	// TODO: Add your control notification handler code here

	CString m_Str;
	CFileFind findfile;

	GetDlgItemText(IDC_UUTNUM , m_Str);

	if(m_Str.Compare("UUT1") == 0)
	{
		//AfxMessageBox("UUT1 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT1.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT01.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}
	
	if(m_Str.Compare("UUT2") == 0)
	{
		//AfxMessageBox("UUT2 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT2.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT02.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}

	if(m_Str.Compare("UUT3") == 0)
	{
		//AfxMessageBox("UUT3 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT3.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT03.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}

	if(m_Str.Compare("UUT4") == 0)
	{
		//AfxMessageBox("UUT4 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT4.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT04.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}

	if(m_Str.Compare("UUT5") == 0)
	{
		//AfxMessageBox("UUT5 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT5.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT05.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}

	if(m_Str.Compare("UUT6") == 0)
	{
		//AfxMessageBox("UUT6 Restart!");
		if(!(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0)))	
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0)))	
			{
				CButton *m_Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
				m_Btn->EnableWindow(FALSE);
			    _this->TrackUUT6.CloseDialog();
			}
		}
		else
		{
			if((findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0)))	
			{
				if((findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0)))	
				{
					m_IniReader.setINIFileName("D:\\USINOTE\\UUT06.ERR");
					m_IniReader.setKey("RESTART1","Check Point","UUT");
				}
			}
		}
	}

}

BOOL UUTLog::ChkLog()
{
    CString csTemp;

	CSADirRead dr;

	CTime t; 
	CString Date = "";
	CString y,m,d;
	int yyy = 0;
	int mmm = 0;
	int ddd = 0;
	BOOL m_FindFolder = TRUE;

	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
		
	t = CTime::GetCurrentTime();

	int yy = t.GetYear();
	int mm = t.GetMonth();
	int dd = t.GetDay();

	if(dd == 14 || dd == 28)
	{
		//**********************************//
		//		Scan Folders and Files		//
		//**********************************//

		dr.GetDirs("D:\\USINOTE\\LOG", true);

		CSADirRead::SADirVector &dirs = dr.Dirs();
	
		for (CSADirRead::SADirVector::const_iterator dit = dirs.begin(); dit!=dirs.end(); dit++)
		{
			csTemp.Format("%s\r\n", (*dit).m_sName);
			Date = csTemp.Right(12);

			y = Date.Left(4);
			sscanf(y,"%d", &yyy);
			d = Date.Right(4);
			sscanf(d,"%d", &ddd);
			y = Date.Left(7);
			m = y.Right(2);
			sscanf(m,"%d", &mmm);

			if(yy != yyy || mm != mmm || dd != ddd)
			{
				m_FindFolder = FALSE;
			}
			else
			{
				//AfxMessageBox("Error! Folder is already exist.");
				m_FindFolder = TRUE;
				break;
			}
		}

		if(!m_FindFolder)
		{
			y.Format("%d",yy);
			m.Format("%d",mm);
			
			if(dd == 14)
				Date = y + '-' + m + '-' + "03";
			if(dd == 28)
				Date = y + '-' + m + '-' + "28";

			if((CreateDirectory(("D:\\USINOTE\\LOG\\" + Date) , NULL)) == 0)
			{
				//AfxMessageBox("Create Directory FAIL!");
				//int i = GetLastError();
				return FALSE;
			}

			m_IniReader.setKey(Date,"Location","PROJECT");
				
			return TRUE;
		}
		else
		{
			//AfxMessageBox("Error! Folder is already exist.");
			return FALSE;
		}
	}
	else
	{
		Date = m_IniReader.getKeyValue( "Location" , "PROJECT");

		y = Date.Left(4);
		sscanf(y,"%d", &yy);
		d = Date.Right(2);
		sscanf(d,"%d", &dd);
		y = Date.Left(7);
		m = y.Right(2);
		sscanf(m,"%d", &mm);

		//**********************************//
		//		Scan Folders and Files		//
		//**********************************//

		dr.GetDirs("D:\\USINOTE\\LOG", true);

		CSADirRead::SADirVector &dirs = dr.Dirs();
	
		for (CSADirRead::SADirVector::const_iterator dit = dirs.begin(); dit!=dirs.end(); dit++)
		{
			csTemp.Format("%s\r\n", (*dit).m_sName);
			Date = csTemp.Right(12);

			y = Date.Left(4);
			sscanf(y,"%d", &yyy);
			d = Date.Right(4);
			sscanf(d,"%d", &ddd);
			y = Date.Left(7);
			m = y.Right(2);
			sscanf(m,"%d", &mmm);

			if(yy != yyy || mm != mmm || dd != ddd)
			{
				m_FindFolder = FALSE;
			}
			else
			{
				//AfxMessageBox("Error! Folder is already exist.");
				m_FindFolder = TRUE;
				break;
			}
		}

		if(m_FindFolder)
			return TRUE;
		else
		{
			//******************************************************//
			//				Error! Folder is not exist!				//
			//******************************************************//
			
			//AfxMessageBox("Error! LOG File Folder is not exist.");
			return FALSE;
		}
	}

}
