// DIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "DIO.h"
#include "DIODlg.h"
#include "ni488.h"	//NI GPIB Card
//#include "nidaqex.h"	//NI DIO96 Card
#include "decl-32.h"
#include "conio.h"
#include "DebugDlg.h"
#include "KeyTest.h"
#include "DIO96.h"
#include "INI.h"
#include "UUTLog.h" 
#include "SFIS.h"
#include "SetUUT.h"
#include "Imports.h"
#include <objbase.h>

#include "serialThread.h"
#include <atlconv.h>

//CSR Bluesuite
#include "testflash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//define ini file neame
#define INIFile	"bhc302.ini"

//***********************************************
//#define UTF001
//***********************************************

//#define Debug_RS232_Output_Message
//#define Debug_RS232_Input_Message

#define MPT_Handshake
#define MT8852_Handshake

#ifdef MPT_Handshake
	#define MPT_Handshake_Message
#endif 

static bool  MT8852_Handshake_flag = false;
static bool  MPT_Handshake_flag = false;

//*****************************************************************************
//NFC Tag content verfication

//#define WithoutPegoda
//#define NFC_Tag_B31_
#define NFC_Tag_B42_
//*****************************************************************************

#define M8852INStimer		1
#define SingleICRunTimer	2
#define M8852EchoInstTimer	3
#define RunStep3Timer		4
#define Run8852delay		5
#define AutorunTimer		10
#define DetectPIOTimer		20
#define USBConnectTimer		25

//#define M8852EchoInstTime	5000
#define M8852EchoInstTime	500

//BHC302_B2.3
//Vol_Up		: PIO[13]
//Vol_Down		: PIO[14]
//One_Pairing	: PIO[5]

#define PIOMask		0x9fdf		//PIO14,PIO13,PIO5
#define PIOValue	0x0000
#define One_Pairing 0x0020
#define Vol_Up		0x2000
#define Vol_Down	0x4000
static int Buttonstate = 0xf8; 
static unsigned char MPTErrorCode[2];
static unsigned int MPTErrorLen;

//MPT & PC communication command 
#define MPT_Start			0x40
#define MPT_ConnectReq		MPT_Start+1				//0x41
#define MPT_BlueEnd			MPT_ConnectReq+1		//0x42
#define MPT_DataStreamStart MPT_BlueEnd+1			//0x43
#define MPT_DataStreamEnd	MPT_DataStreamStart+1	//0x44
#define MPT_SentRealBAComd	MPT_DataStreamEnd+1		//0x45
#define MPT_AudioPathReq	MPT_SentRealBAComd+1	//0x46
#define T2_MOI_1			MPT_AudioPathReq+1		//0x47
#define T2_MOI_2			T2_MOI_1+1				//0x48
#define T2_MOI_3			T2_MOI_2+1				//0x49
#define T2_MOI_4			T2_MOI_3+1				//0x4A
#define T2_MOI_5			T2_MOI_4+1				//0x4B
#define T2_MOI_6			T2_MOI_5+1				//0x4C
#define PC_RFEnd			0x50
#define PC_PIOReq			PC_RFEnd+1
#define PC_PIOEnd			PC_PIOReq+1
#define PC_ProcessEnd		PC_PIOEnd+1
#define PCMPT_ACK			0x60
#define PCMPT_Abort			PCMPT_ACK+1
#define PCMPT_Debug			PCMPT_About+1
	
#define WM_START			WM_USER+20
#define WM_MPTAbout			WM_START+1
#define WM_MPTDebug			WM_MPTAbout+1
#define WM_PC_ProcessEnd	WM_MPTDebug+1

//PC Process control
#define PCAbortTimer		0x70
#define PCAbortTime			1000

#define DebugSPITimer		0xC2
#define DebugSPITime		1000

//T2.0
#define WM_SentRealBA		WM_PC_ProcessEnd+1
#define WM_PLAYAUDIO		WM_SentRealBA+1	
#define WM_AudioPathReq		WM_PLAYAUDIO+1
#define WM_DataTransfer		WM_AudioPathReq+1
#define WM_DataStreamEnd	WM_DataTransfer+1
#define WM_BTError			WM_DataStreamEnd+1

#define RunBTA2DPTimer		50
#define RunBTA2DPTime		10000

#define ProcessEndTimer		52
#define ProcessEndTime		50

#define SplashScreenShow	54
#define SplashScreenShowt	5		

#define SplashScreenHide	56
#define SplashScreenHidet	5

#define DummyBTimer			58
#define DummyBTime			3000

#define RunBTA2DP			0x90
#define BTA2DPFail			0x92
#define Audio_Linein		0x94

#define T2DebugMode

#define T2_Combine_Vol_UpDn_Test

static int CountCWPower = 0;

//***********************************************
//T2.1
//MT8852 CW Measurement mode
#define T21_CW_Measurement	0xD6
int TestLoop = 0;
float PowerMax = 0;
float PowerMin = 0;
int T21_Fail = 0;
int Freq = 0;
int GateWidth = 0;
//***********************************************

//#define Support_Bluesoleil_SDK

static bool ShowPic	= false;	
static bool ToggleBitmap = false;

//T1.3
#define WM_MPTReady			WM_BTError+1
#define WM_ACK				WM_MPTReady+1

#define T13Timer1			30
#define T13Time1			15000	//Wait_MPT_SentRealBT_Time

#define T13RecRealBTData		32
#define T13RecRealBTDataTime	3000

#define T13WriteRealBT		T13Timer1+1
#define T13WriteBTime		500

#define AckTimer			33
#define AckTime				30

#define PC_WriteRealBT		0x70
#define MPT_SentRealBAData  0x80

static int RecRealBTLen = 0;
unsigned char RealBT[6];

#define NFCVerifyProcess	0x88

#define NFCVerifyTimer		0xB0
#define NFCVerifyTime		500

static int NFCVerifyCount = 0;

//T1.0
#define T1_SPI_Debug

#define T1_Fail_Retry

#define Chk8852Timer		80
#define Chk8852Time			2500

static char Initial8852;

//T3.0
#define SetWndPos			0x96
#define T3Record			0x98

#define WaitSoundcheckTimer 90
#define WaitSoundcheckTime  500

//#define MPT_Process_debug

#define WM_ConnectReq		WM_ACK+1
#define WM_BlueEnd			WM_ConnectReq+1		

#define ConnectReqAck		0xA0
#define T1RFTest			ConnectReqAck+1
#define PCRFTestEnd			T1RFTest+1

#define ConnectReqAckTimer	60
#define ConnectReqAckTime	50

#define WriteDummyBDTimer	ConnectReqAckTimer+1
#define WriteDummyBDTime	30

#define PIOEndTimer			WriteDummyBDTimer+1
#define PIOEndTime			30

static int Count_Volumn_up;
static int Count_Volumn_down;
static int Count_One_pair;

//S4.0

#define S4WithMPT

//#define S4_Debug					//Debug mode w/o Pegoda

#define S4RecordTimer		40
#define S4RecordTime		5

// GPIB Interface

#define BDINDEX               0     // Board Index
#define PRIMARY_ADDR_OF_DMM   1     // Primary address of device
#define NO_SECONDARY_ADDR     0     // Secondary address of device
#define TIMEOUT               T10s  // Timeout value = 10 seconds
#define EOTMODE               1     // Enable the END message
#define EOSMODE               0     // Disable the EOS mode
#define ARRAYSIZE 100               // Size of read buffer

// Key Test
#define Enter		0x78
#define Esc			0x60
#define Space		0x88
#define num2		0x21

#define RGB_MYCOLOR		RGB(217 , 255, 217)
#define RGB_BLACK		RGB(0x00, 0x00, 0x00)
#define RGB_WHITE		RGB(0xFF, 0xFF, 0xFF)
#define RGB_RED			RGB(255,0,0)
#define RGB_GREEN		RGB(0,255,0)
#define RGB_YELLOW		RGB(255,255,0)
#define RGB_BLUE		RGB(0,0,255)
#define RGB_GRAY1		RGB(201,201,201)
#define RGB_SKYBLUE		RGB( 0 , 250 , 250)
#define RGB_AZURE		RGB(240,255,240)
#define RGB_MEDIUMBLUE	RGB(0,0,205)
#define RGB_SKYBLUE1	RGB(135,206,235)
#define RGB_BLUE2		RGB(175,238,238)
#define RGB_GRAY		RGB(105,105,105)

i16 iStatus = 0;
i16 iRetVal = 0;
i16 iDevice = 1;
i16 iPort = 0;
i16 iMode = 0;
i16 iDir = 0;
i32 iPattern = 0;
i16 iIgnoreWarning = 0;

//	Global variable	
static BOOL m_Continue = TRUE;
int CDIODlg::ActiveProcess = 0;
int CDIODlg::ActiveSerial = 0;
int CDIODlg::ActiveTest = 0;

ULONGLONG position1 , position2 = 0;

SOCKET  g_SocketClient  =   0;

SOCKET  g_Socket1  =   0;
/*
SOCKET  g_Socket2  =   0;
SOCKET  g_Socket3  =   0;
SOCKET  g_Socket4  =   0;
SOCKET  g_Socket5  =   0;
SOCKET  g_Socket6  =   0;
*/

//  Prototypes of DLL function  //
//short _stdcall Inp32(short PortAddress);
//void  _stdcall Out32(short PortAddress , short data);

//BHC302_MPT_HOST//
static bool ConnectToMPT;
static int DebugState = 0x01;
static bool exoprtLogFileEnable = false;
static bool BTA2DPConnect = false;

static bool CommandHeader = false;
static int  CommandLength = 0;
static bool CommandReady = false;

static int TestErrorCode = 0;
static bool TestResultFlag = true;
static bool RunScript3Test = false;

unsigned char MPTDataBuff[64];
unsigned char MPTDataIndex;

struct  _STRUCT_THREAD
{
	HWND m_statichwnd;
	HWND m_hwnd;
	int phase;
	CDIODlg* _this;
}m_LptthrParam,m_ThreadParam;

UINT CDIODlg::DIO96Threadfun(LPVOID pParam)
{
    int i = 0;
	int j = 0;
	CString str , data , str1;
	CString FailStr = "";
	CFileFind findfile;
	CFile FileCtrl;			
	CFile FileCtrl2;
	CIniReader  m_IniReader;
	position1 = position2 = 0;
	BOOL  CRT = TRUE;
	
	CMapStringToString m_MapString;

	char buff[30720];							// Buffer size = 30k 
	memset(buff , NULL , 30720);

    _STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    CDIODlg *ptr = (CDIODlg*)(FromHandle(Thread_Info->m_hwnd));
    CEdit *m_Edt = (CEdit*)(ptr->GetDlgItem(IDC_DSPMESS));

	m_MapString.RemoveAll( );

    //m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	//str = m_IniReader.getKeyValue( "ITEM" , "PROJECT");

	//BOOL bExists = m_IniReader.sectionExists("TEST");
	
	//CStringList* myOtherList = m_IniReader.getSectionData("TEST");

	//POSITION position;
    
	//for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
	//{
		//myOtherList->GetNext(position);
		//str.Format("%x", i );
		//str1 = m_IniReader.getKeyValue( str , "TEST");
		//data = str1.Left(1);
		//if(data.Compare("#") != 0)
		//{			 			
            //str.Format("%x", j );
			//data = str1.Right( (str1.GetLength() - 13));
			//m_MapString[data] = str ;
			//j++;
		//}
		//i++;
	//}

	/*
    m_IniReader.setINIFileName("D:\\USINOTE\\usiown.ini");

    FileCtrl.Open("d:\\usinote\\uut.log", CFile::modeRead | CFile::shareDenyNone );	 

    FileCtrl2.Open("d:\\usinote\\usiown.ini", CFile::modeRead | CFile::shareDenyNone );
	FileCtrl2.SeekToBegin();

	position1 = FileCtrl.GetPosition();

	m_IniReader.setKey("","Check Point","UUT");
	ptr->SetDlgItemText(IDC_PASSFAIL , "");

	i = 0;
	*/

	/*
	while(ActiveProcess)
	{
		if(Thread_Info->phase == 2)
		{
		   FileCtrl.SeekToEnd();
           position2 = FileCtrl.GetPosition();
		   
	       str1 = m_IniReader.getKeyValue( "Check Point" , "UUT");
		   //str1 = str1.Left(str1.GetLength() - 4));

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
					ActiveProcess = FALSE;
				}
		   }

           if(str1.Compare("") != 0)
		   {
				if(str1.Find(",") >= 0)
				{
					if(CRT)
					{
						//DIG_OUT_Line(1, 4, 7 , 1);			// Change CRT to HOST Mode  
						CRT = FALSE;
						ptr->m_SwitchCRT = TRUE;
					}

					if(str1.Compare("Keyboard Matrix Test,EF0") == 0)					// Total Test End!
					{
						ptr->SetDlgItemText(IDC_PASSFAIL , "");
						ptr->SetDlgItemText(IDC_PASSFAIL , "Keyboard Matrix Test,EF0");
						//ptr->DisplayTest(i);
						Thread_Info->_this->DisplayTest(i);
						m_IniReader.setKey("","Check Point","UUT");
						i++;
						Sleep(2000);
						::SendMessage(Thread_Info->m_hwnd , WM_KEYTEST , 0 , 0);
					}
					else
					{
						ptr->SetDlgItemText(IDC_PASSFAIL , "");
						ptr->SetDlgItemText(IDC_PASSFAIL , str1);
						//ptr->DisplayTest(i);
						Thread_Info->_this->DisplayTest(i);
						m_IniReader.setKey("","Check Point","UUT");
						i++;

						if(i % 16 == 0)
							ptr->GotoNextPage();
					}

					if(i >1 )
					{
						str1 = m_IniReader.getKeyValue( "Error Code" , "UUT");
						if(str1.Compare("") != 0)
						{
							//Fail
							Thread_Info->_this->DisplayResult( i-2 , 0 );
							FailStr +=str1;
							m_IniReader.setKey("","Error Code","UUT");
						}
						else
						{
							//Pass
							Thread_Info->_this->DisplayResult( i-2 , 1 );
						}
					}
				}
				else
				{
					if(str1.Compare("DIAG_END") == 0)						// USD Diagnostic Test End!
					{
						str1 = m_IniReader.getKeyValue( "Error Code" , "UUT");
						if(str1.Compare("") != 0)
						{
							//Fail
							Thread_Info->_this->DisplayResult( i-1 , 0 );
							FailStr +=str1;
							m_IniReader.setKey("","Error Code","UUT");
						}
						else
						{
							//Pass
							Thread_Info->_this->DisplayResult( i-1 , 1 );
						}

						m_IniReader.setKey("","Check Point","UUT");
					}
					
					if(str1.Compare("TOTAL_END") == 0)						// Total Test End!
					{
						ptr->SetDlgItemText(IDC_PASSFAIL , "Total Test End!");
						break;
					}

					ptr->SetDlgItemText(IDC_PASSFAIL , "");
					ptr->SetDlgItemText(IDC_PASSFAIL , str1);
					m_IniReader.setKey("","Check Point","UUT");
				}
		   }

		   Sleep(5);
		}
	}
	
	//AfxMessageBox("ThreadEnd");											// Debug

	if(FailStr.Compare("") == 0)						
	{
		Thread_Info->_this->OnAllPASS();
	}
	else
	{
		FailStr = "FUCK";
		str1 = "Fail! " + FailStr;
		ptr->ShowTEST(str1);
		Thread_Info->_this->OnUUTFail();
	}
	*/

	/*
    for(int h = 0 ; h < 100 ; h++)
	{
		str.Format("%X", h);
		ptr->SetDlgItemText(IDC_PASSFAIL , str);
		Thread_Info->_this->DisplayResult( h , 0 );
	}
	*/
	
	//Sleep(1000);
	//FileCtrl.Close();
	//FileCtrl2.Close();

	//rename("d:\\usinote\\uut.log" , ("d:\\usinote\\log\\" + Thread_Info->_this->m_Barcode + ".txt"));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIODlg dialog

CDIODlg::CDIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDIODlg::IDD, pParent)
	, TrackUUT1(*this), TrackUUT2(*this), TrackUUT3(*this), TrackUUT4(*this), TrackUUT5(*this), TrackUUT6(*this), TrackSFIS1(*this)  
{
	//{{AFX_DATA_INIT(CDIODlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDIODlg)
	DDX_Control(pDX, IDC_RETEST, m_ReTest);
	DDX_Control(pDX, IDC_NAME, m_ComputerName);
	DDX_Control(pDX, ID_BTN_EXIT, m_End);
	DDX_Control(pDX, IDC_BUTTON1, m_Start);
	DDX_Control(pDX, IDC_STATIC2, m_Mac);
	DDX_Control(pDX, IDC_STATIC1, m_Bar);
	DDX_Control(pDX, IDC_POST, m_Boot);
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_CLOCKFRAME , m_clock);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_DSPMESS, m_Edit);
	DDX_Control(pDX, IDC_PASSFAIL, m_PASS);
}

BEGIN_MESSAGE_MAP(CDIODlg, CDialog)
	//{{AFX_MSG_MAP(CDIODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnStart)
	ON_BN_CLICKED(ID_BTN_EXIT, OnBtnExit)
	ON_WM_TIMER()
	ON_COMMAND(IDC_POWER, OnDebug)
	ON_COMMAND(IDC_KEYTEST, OnKeytest)
	ON_COMMAND(IDD_CLOSEFILE, OnClosefile)
	ON_COMMAND(IDD_SERIAL, OnSerialCtrl)
	ON_COMMAND(IDD_SWITCH, CRTSwitch)
	ON_COMMAND(IDD_CLOSESERIAL, CloseSerial)
	ON_COMMAND(IDD_RS232SEND, OnRs232send)
	ON_COMMAND(IDD_LPTTEST, OnLpttest)
	ON_COMMAND(IDD_UUT, OnUUT)
	ON_COMMAND(IDD_NOTIFYUUT, OnNotifyUUT)
	ON_COMMAND(IDD_BATTTEST, OnBattTest)
	ON_COMMAND(IDD_NEXTPAGE, OnNextpage)
	ON_BN_CLICKED(IDC_RETEST, OnReTest)
	ON_COMMAND(IDC_FCTEXIT, OnFCTExit)
	ON_COMMAND(IDD_UUT1, CloseUUT1)
	ON_COMMAND(IDD_UUT2, CloseUUT2)
	ON_COMMAND(IDD_UUT3, CloseUUT3)
	ON_COMMAND(IDD_UUT4, CloseUUT4)
	ON_COMMAND(IDD_UUT5, CloseUUT5)
	ON_COMMAND(IDD_UUT6, CloseUUT6)
	ON_COMMAND(IDD_USER, ScanBarcode)
	ON_COMMAND(IDD_UUTDEBUG, OnUUT)
	ON_COMMAND(IDD_SFIS1, OpenSFIS1)
	ON_COMMAND(IDD_SETUUT, SetUUT1)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_TEST , TestLog)
	//ON_MESSAGE(WM_PMTEST , PMTest)
	//ON_MESSAGE(WM_UUTFAIL , OnUUTFail)
    //ON_MESSAGE(WM_KEYTEST ,  Sendkeymatrix)
	//ON_MESSAGE(WM_POWERON ,  Power_on)
	//ON_MESSAGE(WM_CMOSERROR , CMOSError)
	//ON_MESSAGE(WM_UUTPASS , OnAllPASS)
	ON_WM_ERASEBKGND()
    ON_COMMAND(IDD_PowerOn, Power_on)
    ON_COMMAND(IDD_POWEROFF, Power_off)
    ON_COMMAND(IDD_KEYMATRIX, Sendkeymatrix)
    ON_COMMAND(IDD_PMTEST, PMTest)
	ON_COMMAND(IDD_ACON, AC_On)
	ON_COMMAND(IDD_ACOFF, AC_Off)
    ON_COMMAND(IDD_BATTON, EnableBattery)
	ON_COMMAND(IDD_BATTOFF, DisableBattery)
	ON_COMMAND(IDD_SMBUSON, EnableSMBUS)
	ON_COMMAND(IDD_SMBUSOFF, DisableSMBUS)
	ON_COMMAND(IDD_LIDON, LidSwitchOn)
    ON_COMMAND(IDD_LIDOFF, LidSwitchOff)
    ON_COMMAND(IDD_CRT, SWtoUUT)
	ON_COMMAND(IDD_LCD, SWtoHOST)
    ON_COMMAND(IDD_THERMAL, ThermalShutdown)
    ON_COMMAND(IDD_USBON, USBOn)
    ON_COMMAND(IDD_USBOFF, USBOff)
    ON_COMMAND(IDD_WOL, WakeOnLan)
	ON_COMMAND(IDD_FIXTURE, FixturePower)
	ON_COMMAND(IDD_SET9V, Set9V)
	ON_COMMAND(IDD_SET10V, Set10V)
	ON_COMMAND(IDD_SET12V, Set12V)
	ON_COMMAND(IDD_WOLBOOT, SetWOL)
	ON_COMMAND(IDD_HDDON, TurnOnHDD)
	ON_COMMAND(IDD_HDDOFF, TurnOffHDD)
    ON_COMMAND(IDD_RINGON, RINGON)
	ON_COMMAND(IDD_RINGOFF, RINGOFF)
	ON_COMMAND(IDD_TIPON, TIPON)
	ON_COMMAND(IDD_TIPOFF, TIPOFF)
	ON_COMMAND(IDD_ENBRJ11, EnableRJ11)
	ON_COMMAND(IDD_WAKEUP, WakeUpTest)
	ON_COMMAND(IDD_SET0, Set0V)
	ON_COMMAND(IDD_ENABLEPOST, EnablePost)
    ON_COMMAND(IDD_RW, DIO96Ctrl)
	ON_COMMAND(IDD_LOOPTEST, LoopTest)
	ON_COMMAND(IDD_STARTTIME, Start)
    ON_COMMAND(IDD_STOPTIME, Stop)
	ON_COMMAND(IDD_RESETTIME, Reset)
	ON_COMMAND(IDD_SHOWITEM, ShowItem )
    ON_COMMAND(IDD_DISABLEPOST, DisablePost)
	ON_COMMAND(IDD_TESTTHREAD, ThreadTest)
	ON_COMMAND(IDD_WINWAKE, Winwake)
	ON_COMMAND(IDD_SUS, SuspendThread)
	ON_COMMAND(IDD_RESUME, ResumeThread)
	ON_COMMAND(IDD_ENDTHREAD, EndTHread)
	ON_COMMAND(IDD_INITEST, INIFileTest)
	ON_COMMAND(IDD_SENDDATA, SendData)
	ON_COMMAND(IDD_INITIAL, InitWinSock1)
	ON_COMMAND(IDD_CONNECTSERVER, ConnectToServer)
	ON_COMMAND(IDD_TIMER2, TestTimer)
	ON_COMMAND(IDD_FUNTEST, SendPASS)
	ON_COMMAND(IDD_STARTTEST, OnStart)
	ON_COMMAND(IDD_TT , OnFileSharing)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_MESSAGE(WM_MY_MESSAGE_M8852_GET_RESULT, OnMyMessageM8852_GetResult)
	ON_MESSAGE(WM_PLAYAUDIO, PlayAudio)
	ON_MESSAGE(WM_BTError, BTA2DPConnectFail)
	ON_MESSAGE(WM_PC_ProcessEnd , PCProcessEnd)
	//ON_MESSAGE(WM_COMM_RXCHAR, &CDIODlg::OnCommunication)
	ON_COMMAND(IDC_MYDEBUG, BHC302_DEBUG_CMD)
	ON_COMMAND(IDC_USBTX, BHC302_USB_TX)
	ON_COMMAND(IDC_USBRX, BHC302_USB_RX)
	ON_COMMAND(IDC_DATABASE, ConnectDB)
	ON_COMMAND(IDC_RDNFC , NFCTest)
	ON_COMMAND(IDC_T13VERIFY , NFCTest)
	ON_COMMAND(IDC_S4REVERSE , BTReverse)
	ON_COMMAND(IDC_S4EXPORT , S4BTExport)
	ON_COMMAND(IDC_CLEARS4 , S4Clear)
	ON_COMMAND(IDC_CSRSPION , OpenTestEngine)
	ON_COMMAND(IDC_CSRSPIOFF , CloseTestEngine)
	ON_COMMAND(IDC_CSRSPITEST , CSRSPILoopTest)
	ON_COMMAND(IDC_CSRSPISTOP , CSRSPITestStop)
	ON_COMMAND(IDC_DNWUSBDOWN , DNW_USBDownload)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIODlg message handlers

BOOL CDIODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	double TempVar;
	int k;
	CString tmp1,tmp2;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	HWND hWnd = ::FindWindow (NULL, NULL);
	char tempTitle [256];

	while (hWnd)
	{
		::GetWindowText (hWnd, tempTitle, 255);
		if (tempTitle [0] != 0 && ::IsWindowVisible (hWnd))
		{
            m_Str = tempTitle;
			if(m_Str.Compare("BHC302_MPT_HOST") == 0)
			{
				AfxMessageBox("Program is already running!");
				CDialog::OnCancel();
				return FALSE;
			}
		}
		hWnd = ::GetNextWindow (hWnd, GW_HWNDNEXT);
	}

    m_clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);

	m_Continue = TRUE;
	m_CheckBar = TRUE;
	m_CheckMac = TRUE;
    m_Initial  = TRUE;

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT);

	InitListCtrl(m_List);

	InitialGUI();

	ReadBHC302INIData();

	IsConnected = FALSE;

	//devHandle = openTestEngine(2, "\\\\.\\csr0", 0 , 5000 , 5000);

	//ReadDUT_BDaddress();

#ifdef MPT_Handshake
	#ifdef S4WithMPT
	if(m_AppAction.Compare("debug") != 0 && m_TestStation.Compare("T0") != 0 && m_TestStation.Compare("T2.1") != 0)
	#else
	if(m_AppAction.Compare("debug") != 0 && m_TestStation.Compare("T0") != 0 && m_TestStation.Compare("S4") != 0 && m_TestStation.Compare("T2.1") != 0)
	//if(m_AppAction.Compare("debug") != 0 && m_TestStation.Compare("T2") != 0 && m_TestStation.Compare("S4") != 0)
	#endif
	{
		if(MPTCOM.InitPort(this , i_MPTComPort , i_MPTBaudrate))
		{
			MPT_Recv_string = "";
			MPTCOM.StartMonitoring();
		}
		else
		{
			AfxMessageBox("MPT Serial port is occupied!\n");
			CDialog::OnCancel();
		}
	}
#endif

	ConnectToMPT = false;

	devHandle = 0;

	//PSkeyCrystalTrim = 43;

	m_TestProcess = 0;

	CrystalTrimLowerlimit = 10;
	
	CrystalTrimUpperlimit = 63;

	DefPSkeyCrystalTrim = PSkeyCrystalTrim;

	run_step = 0;

	crystaltrim_diff = 1;

	ListCtrlIndex = 0;

	pSplash = NULL;

///*
#ifdef MT8852_Handshake
	if(m_AppAction.Compare("debug") != 0)//Run in Normal mode
	{
		if(m_TestStation.Compare("T2") != 0 && m_TestStation.Compare("T1.3") != 0 && m_TestStation.Compare("T0") != 0 && m_TestStation.Compare("S4") != 0 && m_TestStation.Compare("T3") != 0 && m_TestStation.Compare("T0.1") != 0)
		{
			if (MT8852COM.InitPort(this, i_MT8852ComPort, i_MT8852Baudrate))
			{
				MT8852COM.StartMonitoring();

				TestErrorCode = 0x9000;
			
				//m_M8852IsHandShakingFlag = true;
				//M8852EchoCmd_ptr ="SYSCFG? EUTRS232\r\n";
				//MT8852_Op_str.Format(_T("SYSCFG EUTRS232,57600"));

				//SetTimer(M8852INStimer,1500,NULL); 
			}
			else
			{
				AfxMessageBox("MT8852 com port is occupied");
			}
		}
	}
#endif
//*/

	m_ResultStr = "";
	m_ErrorCodeStr = "";
	m_TestItemStr = "";

	ShowItem();

	/*
	if(m_Autorun.Compare("true") == 0)
	{
		//if(MT8852_Handshake_flag == true)
		SetTimer(AutorunTimer , 1000 , NULL);
	}
	else
		BHC302_GO_TEST();
	*/

	///*
#ifndef UTF001
	if(m_TestStation.Compare("T0") == 0)
	{
		//Get CSR Firmware version
		m_BHC302_Fw_Version = GetINIData("T0" , "Firmware_version");

		m_Str = "BHC302_T0_";
		m_Str += m_BHC302_Fw_Version;

		//Initial 
		if(m_PromptMessage)
		{
			pSplash=new CSplashScreenEx();
			pSplash->Create(this,m_Str,0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);

			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);//設定文字背景圖片(Blue)
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);//設定文字背景圖片(Yellow)
			}
			pSplash->SetTextFont("Impact",200,CSS_TEXT_NORMAL);
			pSplash->SetTextRect(CRect(0,0,1207,197));
			pSplash->SetTextColor(RGB(0,0,255));
			pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			pSplash->Show();
		}

		//m_clock.StartTimer(); 
		Start();
	}
	else if(m_TestStation.Compare("T1.3") == 0)
	{
		if(m_PromptMessage)
		{
			pSplash=new CSplashScreenEx();
			pSplash->Create(this,"BHC302_T1.3_V1.0.071",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);

			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			pSplash->SetTextFont("Impact",200,CSS_TEXT_NORMAL);
			pSplash->SetTextRect(CRect(0,0,1207,197));
			pSplash->SetTextColor(RGB(0,0,255));
			pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			pSplash->Show();

			SplashScreenString = "將主板置於治具上,並且將NFC感應器置於NFC Tag上方,之後按下測試鈕";

			SetTimer(SplashScreenShow , SplashScreenShowt , NULL);
		}
	}
	else if(m_TestStation.Compare("T2") == 0)
	{
		//ShowWindow(SW_MINIMIZE);

		m_IniReader.setKey("Run","Status","BTApp");//Terminate btapp.exe
		m_IniReader.setKey("","Message","BTApp");

		//Initial 
		if(m_PromptMessage)
		{
			pSplash=new CSplashScreenEx();
			pSplash->Create(this,"BHC302",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
			//pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			pSplash->SetTextFont("Impact",200,CSS_TEXT_NORMAL);
			//pSplash->SetTextRect(CRect(125,60,291,104));
			//pSplash->SetTextRect(CRect(0,0,484,197));
			pSplash->SetTextRect(CRect(0,0,1207,197));
			pSplash->SetTextColor(RGB(0,0,255));
			//pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			//pSplash->SetTextFormat(DT_CENTER | DT_VCENTER);
			pSplash->SetTextFormat(DT_VCENTER);//顯示多行
			pSplash->Show();
			//pSplash->Hide();

			SplashScreenString = "更新待測物,插入USB接頭,安裝電池,按壓綠色紐";	
			//SplashScreenString = "測試音量---鍵和+++鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";
			//SplashScreenString = "更新待測物,插入USB接頭,安裝電池,按壓綠色紐,更新待測物,插入USB接頭,安裝電池,按壓綠色紐,更新待測物\r\n,插入USB接頭,安裝電池,按壓綠色紐";	

			SetTimer(SplashScreenShow , SplashScreenShowt , NULL);
		}
	}
	else if(m_TestStation.Compare("T3") == 0)
	{
		//m_IniReader.setKey("Run","Status","BTApp");//Terminate btapp.exe
		//m_IniReader.setKey("","Message","BTApp");
	}
	else if(m_TestStation.Compare("T2.1") == 0)
	{
		m_T21TestLoop = GetINIData("T2.1" , "TestLoop");
		TestLoop = atoi(m_T21TestLoop);

		m_T21PowerMax = GetINIData("T2.1" , "PowerMax");
		PowerMax = atof(m_T21PowerMax);

		m_T21PowerMin = GetINIData("T2.1" , "PowerMin");
		PowerMin = atof(m_T21PowerMin);

		m_T21Freq = GetINIData("T2.1" , "MeasureFreq");
		Freq = atoi(m_T21Freq);

		m_T21GateWidth = GetINIData("T2.1" , "GateWidth");
		GateWidth = atoi(m_T21GateWidth); 
	}
	else if(m_TestStation.Compare("S4") == 0)
	{
		tmp1 = GetINIData("S4_NFC" , "export");
		tmp2 = GetINIData("S4_NFC" , "ReverseBT");

		if(m_Autorun.Compare("true") == 0 && tmp1.Compare("false") == 0 && tmp2.Compare("false") == 0)
		{
			///*
			S4LoadData();

			//**********************************MySQL
			///*
			m_Str = GetINIData("S4_NFC" , "SupportDB");
			if(m_Str.Compare("true") == 0)
			{
				ConnectToMySQLServer();
				GetDatabases();
				ShowDBTables();
				//DBRunQuery();
				DBInsertData();
				DBRunQuery();
			}
			//*/
			//***************************************

			//Initial 
			if(m_PromptMessage)
			{
				pSplash=new CSplashScreenEx();
				pSplash->Create(this,"BHC302",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
				if(ToggleBitmap)
				{
					ToggleBitmap = false;
					pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
				}
				else
				{
					ToggleBitmap = true;
					pSplash->SetBitmap(IDB_SPLASH,255,0,255);
				}
				pSplash->SetTextFont("Impact",200,CSS_TEXT_NORMAL);
				pSplash->SetTextRect(CRect(0,0,1207,197));
				pSplash->SetTextColor(RGB(0,0,255));
				pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				pSplash->Show();
				//pSplash->Hide();

#ifdef S4WithMPT
				SplashScreenString = "將NFC感應器置於待測物上方";	
#else
				SplashScreenString = "請將待測物置於Pegoda感應器上";
#endif

				SetTimer(SplashScreenShow , SplashScreenShowt , NULL);
			}
			//*/
		}
		else
			IsConnected = FALSE;
	}
#endif

	m_Str = GetINIData("ConnectSetting" , "USB");
	if(m_Str.Compare("true") == 0)
	{
		SetTimer(USBConnectTimer , 1500 , NULL);
	}

	m_Str = GetINIData("Modle Name" , "Menu");

	if(m_Str.Compare("true") == 0)
	{
		m_menu.LoadMenu(IDR_MENU4);
		SetMenu(&m_menu);
	}

#ifndef UTF001
	if(m_Autorun.Compare("true") == 0)
	{
		//if(MT8852_Handshake_flag == true)
		SetTimer(AutorunTimer , 500 , NULL);
	}
	else
		BHC302_GO_TEST();
#endif
	//*/

	//SetTimer(DummyBTimer , DummyBTime , NULL);

	///*
	//ShowWindow(SW_MINIMIZE);
	//pSplash=new CSplashScreenEx();
	//pSplash->Create(this,NULL,0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	//pSplash->SetBitmap("lineout.bmp",255,0,255);
	//pSplash->Show();
	//ShowPic = true;

	//pSplash->Create(this,"CSplashScreenEx dynamic text:",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	//pSplash->SetBitmap(IDB_SPLASH,255,0,255);
	//pSplash->SetTextFont("Impact",100,CSS_TEXT_NORMAL);
	//pSplash->SetTextRect(CRect(125,60,291,104));
	//pSplash->SetTextColor(RGB(0,0,255));
	//pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//pSplash->Show();

	//Sleep(3000);

	//pSplash->Hide();

	//ShowWindow(SW_RESTORE);
	//*/
	//SetTimer(SplashScreenHide , SplashScreenHidet , NULL);

	// Demo 2
	/*
	CSplashScreenEx *pSplash=new CSplashScreenEx();
	pSplash->Create(this,"BHC302",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	pSplash->SetBitmap(IDB_SPLASH,255,0,255);
	pSplash->SetTextFont("Impact",200,CSS_TEXT_NORMAL);
	//pSplash->SetTextRect(CRect(125,60,291,104));
	pSplash->SetTextRect(CRect(0,0,484,197));
	pSplash->SetTextColor(RGB(0,0,255));
	pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pSplash->Show();
	pSplash->Hide();

	Sleep(1000);
	pSplash->SetText("You can display infos");

	Sleep(1000);
	pSplash->SetText("While your app is loading");

	Sleep(1000);
	pSplash->SetText("Just call Hide() when loading");
	
	Sleep(1000);
	pSplash->SetText("is finished");
	Sleep(1500);

	pSplash->Hide();
	*/

	//SetWindowPos(0 , 600 , 570 , 500 , 330 , SWP_SHOWWINDOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDIODlg::ReadBHC302INIData()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	double TempVar;
	//CString mm;
	//CString dd;
	//CString yy;
	//CString hour;
	//CString minute;
	//CString second;

	m_Edt->ReplaceSel("************************************Get INI Data\r\n");

	m_Str = GetINIData("Modle Name" , "GUID");
	if(m_Str.Compare("") == 0)
	{
		CreateGUID();
		//m_Edt->ReplaceSel("HostPC : " + m_Str);
		m_IniReader.setKey(m_Str,"GUID","Modle Name");
	}
	else
	{
		m_Edt->ReplaceSel("GUID : " + m_Str);
		m_Edt->ReplaceSel("\r\n");
	}

	CTime t = CTime::GetCurrentTime();
	m_Str = "Current Time :";
	data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	m_Edt->ReplaceSel(data);
	
/*
	if(t.GetSecond() < 10)
		second.Format("0%d",t.GetSecond());
	else
		second.Format("0%d",t.GetSecond());

	if(t.GetMinute() < 10)
		minute.Format("0%d",t.GetMinute());
	else
		minute.Format("0%d",t.GetMinute());

	if(t.GetHour() < 10)
		hour.Format("0%d",t.GetHour());
	else
		hour.Format("0%d",t.GetHour());

	m_Str.Format("%d%d%d%d%d%d",t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	DummyBD = m_Str.Right(12);
*/

	GenerateDummyBT();

	m_Edt->ReplaceSel("SN : " + m_Barcode);
	m_Edt->ReplaceSel("\r\n");

	m_Str = GetINIData("DefaultSetting" , "Final BD Address");
	m_finalBDAddress = m_Str;
	m_Edt->ReplaceSel("BD : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	m_Edt->ReplaceSel("Dummy BD : " + DummyBD);
	m_Edt->ReplaceSel("\r\n");

	rgbText = RGB_BLUE;
	m_Bar.SetTextColor(rgbText);
	m_Mac.SetTextColor(rgbText);
	SetDlgItemText(IDC_STATIC1 , "SN : ");//S/N
	//SetDlgItemText(IDC_STATIC2 , "BD : " + m_Str);//BD
	SetDlgItemText(IDC_STATIC2 , "BD : ");//BD

	model_name = GetINIData("Modle Name" , "ModleName");
	m_Edt->ReplaceSel("Modle Name : " + model_name);
	m_Edt->ReplaceSel("\r\n");

	m_Edt->ReplaceSel("Test Station : ");
	m_Edt->ReplaceSel(m_TestStation + "\r\n");

	m_Str = GetINIData("ConnectSetting" , "MPT (COM Port)");
	m_Edt->ReplaceSel("MPT (COM Port) : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	i_MPTComPort = atoi(m_Str);

	m_Str = GetINIData("ConnectSetting" , "MT8852 (COM Port)");
	m_Edt->ReplaceSel("MT8852 (COM Port) : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	i_MT8852ComPort = atoi(m_Str);

	if(i_MT8852ComPort == i_MPTComPort)
	{
		AfxMessageBox("INI/COMPORT setting error!!");
		CDialog::OnCancel();
	}

	m_Str = GetINIData("ConnectSetting" , "MPT (Baudrate)");
	m_Edt->ReplaceSel("MPT (Baudrate) : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	i_MPTBaudrate = atoi(m_Str);

	m_Str = GetINIData("ConnectSetting" , "MT8852 (Baudrate)");
	m_Edt->ReplaceSel("MT8852 (Baudrate) : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	i_MT8852Baudrate = atoi(m_Str);

	m_Str = GetINIData("Mt8852-Setting" , "ScriptFile");
	m_Edt->ReplaceSel("MT8852 Script : " + m_Str);
	m_Edt->ReplaceSel("\r\n");
	i_MT8852Script = atoi(m_Str);

	m_MT8852Action = GetINIData("Mt8852-Setting" , "Action");
	m_Edt->ReplaceSel("MT8852 Action : ");
	m_Edt->ReplaceSel(m_MT8852Action + "\r\n");

	m_Str = GetINIData("Modle Name" , "PromptMessage");
	if(m_Str.Compare("true") == 0)
	{
		m_Edt->ReplaceSel("USB enable\r\n");
		m_PromptMessage = true;
	}
	else
	{
		m_Edt->ReplaceSel("USB disable\r\n");
		m_PromptMessage = false;
	}

	m_AppAction = GetINIData("Modle Name" , "AppAction");
	m_Edt->ReplaceSel("App Action : ");
	m_Edt->ReplaceSel(m_AppAction + "\r\n");

	m_Str = GetINIData("Mt8852-Setting" , "Fixed offset");
	Fixedoffset = atof(m_Str);

	m_Str = GetINIData("Mt8852-Setting" , "OutputPowerAverageMax");
	OPAverageMax = atof(m_Str);

	m_Str = GetINIData("Mt8852-Setting" , "OutputPowerAverageMin");
	OPAverageMin = atof(m_Str);

	m_Str = GetINIData("Mt8852-Setting" , "SingleSensitivityTXPower");
	SSTxPower = atof(m_Str);

	m_Str = GetINIData("DefaultSetting" , "Frequency Crystal Trim lower limit");
	CrystalTrimLowerlimit = atoi(m_Str);
	
	if (CrystalTrimLowerlimit < 0)
	{
		CrystalTrimLowerlimit = 0;
		//AfxMessageBox("The value of Frequency Crystal Trim lower limit in ConnectSetting.ini is not valid value.We set 0 instead.");
		m_Edt->ReplaceSel("The value of Frequency Crystal Trim lower limit in ConnectSetting.ini is not valid value.We set 0 instead.\r\n");
	}

	m_Str = GetINIData("DefaultSetting" , "Frequency Crystal Trim upper limit");
	CrystalTrimUpperlimit = atoi(m_Str);

	if (CrystalTrimUpperlimit > 63)
	{
		CrystalTrimUpperlimit = 63;
		//AfxMessageBox("The value of Frequency Crystal Trim upper limit in ConnectSetting.ini is not valid value.We set 63 instead.");
		m_Edt->ReplaceSel("The value of Frequency Crystal Trim upper limit in ConnectSetting.ini is not valid value.We set 63 instead.\r\n");
	}

	m_Str = GetINIData("DefaultSetting" , "Frequency Crystal Trim");
	PSkeyCrystalTrim = atoi(m_Str);

	if ((CrystalTrimUpperlimit >= 63) || (CrystalTrimUpperlimit <= 0)) 
		CrystalTrimUpperlimit = 63;
	if ((CrystalTrimLowerlimit >= 63) || (CrystalTrimLowerlimit <= 0)) 
		CrystalTrimLowerlimit = 0;
	
	if (CrystalTrimLowerlimit >= CrystalTrimUpperlimit)
	{
		TempVar = CrystalTrimLowerlimit;
		CrystalTrimLowerlimit = CrystalTrimUpperlimit;
		CrystalTrimUpperlimit = TempVar;
	}

	m_Str = GetINIData("DefaultSetting" , "max active current");
	m_MaxActiveCurrent = atof(m_Str);

	m_Str = GetINIData("DefaultSetting" , "min active current");
	m_MinActiveCurrent = atof(m_Str);

	m_Str = GetINIData("DefaultSetting" , "active current time delay");
	m_activeCurrenttimeDelay = atoi(m_Str);

	m_Autorun = GetINIData("Modle Name" , "AutoRun");

	m_Edt->ReplaceSel("AutoRun : ");
	m_Edt->ReplaceSel(m_Autorun + "\r\n");

	m_Str = GetINIData("log file seting" , "export log file");

	if(m_Str.Compare("true") == 0)
	{
		exoprtLogFileEnable = true;
		m_Edt->ReplaceSel("Export log file : yes\r\n");
	}
	else
		m_Edt->ReplaceSel("Export log file : no\r\n");

	if(m_AppAction.Compare("debug") != 0)//Run in Normal mode
		m_Edt->ReplaceSel("App is run in normal mode!\r\n");
	else
		m_Edt->ReplaceSel("App is run in debug mode!\r\n");

	m_AudioFile = GetINIData("BTApp" , "WaveFile");

	m_IniReader.setKey("","Message","BTApp");

	//S4.0
	m_Str = GetINIData("S4_NFC" , "NumberofBT");
	NumOfBT = atoi(m_Str);

	m_Edt->ReplaceSel("************************************************\r\n");
}

void CDIODlg::GenerateDummyBT()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString mm;
	CString dd;
	CString yy;
	CString hour;
	CString minute;
	CString second;
	CTime t = CTime::GetCurrentTime();
	DummyBD = "";

	if(t.GetSecond() < 10)
		second.Format("0%d",t.GetSecond());
	else
		second.Format("%d",t.GetSecond());

	if(t.GetMinute() < 10)
		minute.Format("0%d",t.GetMinute());
	else
		minute.Format("%d",t.GetMinute());

	if(t.GetHour() < 10)
		hour.Format("0%d",t.GetHour());
	else
		hour.Format("%d",t.GetHour());

	if(t.GetDay() < 10)
		dd.Format("0%d",t.GetDay());
	else
		dd.Format("%d",t.GetDay());

	if(t.GetMonth() < 10)
		mm.Format("0%d",t.GetMonth());
	else
		mm.Format("%d",t.GetMonth());

	yy.Format("%d",t.GetYear());
	yy = yy.Right(2);

	DummyBD += yy;
	DummyBD += mm;
	DummyBD += dd;
	DummyBD += hour;
	DummyBD += minute;
	DummyBD += second;

	m_Edt->ReplaceSel("Dummy BT : " + DummyBD);
	m_Edt->ReplaceSel("\r\n");

	//m_Str.Format("%d%d%d%d%d%d",t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	//DummyBD = m_Str.Right(12);
}

void CDIODlg::CreateGUID()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	static char buf[64] = {0};
	GUID guid;
	CString str = "";

    DWORD Data1;
    WORD   Data2;
    WORD   Data3;
    BYTE  Data4[8];

	//--COM
	CoInitialize(NULL);

	::CoCreateGuid(&guid);

	//m_Str.Format("%X\r\n",guid);
	//m_Edt->ReplaceSel(m_Str);
	
	m_Str.Format("%X",guid.Data1);
	str += m_Str;
	//m_Edt->ReplaceSel(m_Str);
	m_Str.Format("%X",guid.Data2);
	str += m_Str;
	//m_Edt->ReplaceSel(m_Str);
	m_Str.Format("%X",guid.Data3);
	str += m_Str;
	//m_Edt->ReplaceSel(m_Str);
	m_Str.Format("%X",guid.Data4);
	str += m_Str;
	//m_Edt->ReplaceSel(m_Str);
	m_Edt->ReplaceSel("GUID : " + str);
	m_Edt->ReplaceSel("\r\n");

	m_Str = str;

	CoUninitialize();
}

void CDIODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDD_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


BOOL CDIODlg::OnEraseBkgnd(CDC* pDC) 
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


void CDIODlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CDIODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDIODlg::OnStart() 
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16] = {0};
	CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	double current;

	CButton *pBtn;

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

///*
	GetDlgItemText(IDC_BUTTON1 , m_Str);

	if(m_Str.Compare("T2.1") == 0)//MT8852 CW Measurement
	{
		GetDlgItemText(IDC_PASSFAIL , m_Str);

		if(m_Str.Compare("") != 0)
		{
			m_PASS.StopBlink(CC_BLINK_BOTH);
			SetDlgItemText(IDC_PASSFAIL , "");
			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			Sleep(100);
			ReadBHC302INIData();
		}

		pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
		pBtn->EnableWindow(FALSE);

		m_Edt->ReplaceSel(m_T21TestLoop + " " + m_T21Freq + " " +  m_T21GateWidth + " " + m_T21PowerMax + " " + m_T21PowerMin + "\r\n");
		//m_Edt->ReplaceSel(m_T21Freq + "\r\n");
		//m_Edt->ReplaceSel(m_T21GateWidth + "\r\n");
		//m_Edt->ReplaceSel(m_T21PowerMax + "\r\n");
		//m_Edt->ReplaceSel(m_T21PowerMin + "\r\n");	
		
		T21_Fail = 0;

		SetEUT_CW_OPMD_To8852();

		//m_TestProcess = T21_CW_Measurement;
	
		///*
		//TCHAR SetMT8852[] = "CWMEAS FREQ,2440e6,3e-3\r\n";
		//TCHAR SetMT8852[] = "*IDN?\r\n";
		//M8852EchoCmd_ptr ="SIGGEN?\r\n";
		//MT8852COM.WriteToPort(SetMT8852);
		//m_Edt->ReplaceSel("Set MT8852 to CW Measurement mode!\r\n");
		//*/
		
		///*
		//m_TestItemStr = "CW Measurement";
		//TestErrorCode++;
		//m_Str.Format("%X",TestErrorCode);
		//m_ErrorCodeStr = m_Str;			
		//AddListCtrlItem(m_List , ListCtrlIndex);
		//ListCtrlIndex++;
		//*/

		//SetTimer(M8852INStimer,100,NULL); 
		
		//Sleep(1000);

		//MT8852COM.WriteToPort(M8852EchoCmd_ptr);

		//m_TestProcess = T21_CW_Measurement;

		//SetTimer(T21_CW_Measurement , 2000 , NULL);
		//SetTimer(T21_CW_Measurement , 1000 , NULL);
	}
	else if(m_Str.Compare("T3") == 0)
	{
		m_Edt->ReplaceSel("T3 Test!\r\n");
		
		/*
		current = RdGPIBCurrent();
		m_Edt->ReplaceSel("GPIB/Read Current complete!\r\n");
		*/
	}
	else if(m_Str.Compare("BTA2DP") == 0)
	{
		m_Str = GetINIData("BTApp" , "Status");

		if(m_Str.Compare("Run") == 0)
		{		
			m_Str = GetINIData("BTApp" , "Message");

			if(m_Str.Compare("") == 0)
			{	
				m_Edt->ReplaceSel("BTA2DP Test!\r\n");

				//ShellExecute(this->m_hWnd,"open","btapp.exe","","", SW_SHOW );

				m_ThreadParam.m_hwnd = this->m_hWnd;
				m_ThreadParam.phase = 0;
				m_pLptThrd = AfxBeginThread(BTThreadfun , (LPVOID)&m_ThreadParam);//Create thread to monitor debug message 
				
				//m_Edt->ReplaceSel("Connect..\r\n");
			}
		}
		else
		{
			m_Edt->ReplaceSel("Read ini,BTApp status : ");
			m_Edt->ReplaceSel( m_Str + "\r\n");
		}
	}
	else if(m_Str.Compare("S4_Start") == 0)
	{
#ifndef S4_Debug
		NFCTest();
#endif
		if(TestResultFlag)
		{
			S4RecordSN();
		}
		else
		{
			m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxx\r\n");
			m_Edt->ReplaceSel("NFC fail!\r\n");
			m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxx\r\n");

			SetDlgItemText(IDC_PASSFAIL , "FAIL");

			rgbText = RGB_RED;
			rgbBkgnd = RGB_GRAY1;
			m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

			if(m_PromptMessage)
			{
				if(ToggleBitmap)
				{
					ToggleBitmap = false;
					pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
				}
				else
				{
					ToggleBitmap = true;
					pSplash->SetBitmap(IDB_SPLASH,255,0,255);
				}
				SplashScreenString = "NFC Tag資料錯誤";
				pSplash->SetText(SplashScreenString);
			}

			m_TestProcess = 0;
			TestErrorCode = 0xF200;

			MPT_RealBT = m_Barcode = "";
		}
	}
	else if(m_Str.Compare("Tag_Verify") == 0)
	{
		NFCTest();
	}
	else if(m_Str.Compare("Play") == 0)
	{
		if (!m_Wave1.IsPlaying())
		{
			m_Wave1.Play();

			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("Play Wav file\r\n");
		}
		else
		{
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("Already playing..\r\n");
		}
	}
	else if(m_Str.Compare("CrystalTrim") == 0)
	{
		//m_TestProcess = MPT_Start;	
		//m_Edt->ReplaceSel("Wait MPT Start command!\r\n");

		if(m_AppAction.Compare("debug") == 0)
		{
			m_Edt->ReplaceSel("Crystal Trim testing\r\n");
			CrystalTrimTest();
		}
		else
		{
			m_Edt->ReplaceSel("Crystal Trim testing\r\n");
			CrystalTrimTest();

			//m_TestProcess = MPT_Start;	
			//m_Edt->ReplaceSel("Wait MPT Start command!\r\n");
		}
	}
	else if(m_Str.Compare("QuickTest") == 0)
	{
		m_Edt->ReplaceSel("QuickTest\r\n");
		QuickTest();
	}
	else if(m_Str.Compare("Reset8852") == 0)
	{
		m_Edt->ReplaceSel("Initial MT8852 Script3\r\n");
		ResetMT8852();
	}
	else if(m_Str.Compare("Script3Test") == 0)
	{
		//m_Edt->ReplaceSel("RF Test\r\n");
		MT8852Script3Test();
	}
	else if(m_Str.Compare("BlueFlash") == 0)
	{
		m_Edt->ReplaceSel("Download code!\r\n");
		if(CSRBlueFlash() != 0)
		{
			TestResultFlag = false;
			m_ResultStr = "FAIL";
		}
		else
		{
			TestResultFlag = true;
			m_ResultStr = "PASS";
			SetDlgItemText(IDC_RETEST , "Re-Test");
		}

		m_TestItemStr = "Download code";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
			
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("****************************************************\r\n");
		CTime t = CTime::GetCurrentTime();
		m_Str = "Current Time :";
		data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		m_Edt->ReplaceSel(data);

		if(TestResultFlag)
		{
			m_Edt->ReplaceSel("	Download code PASS!				                   \r\n");	
			rgbText = RGB_BLUE;
			SetDlgItemText(IDC_PASSFAIL , "PASS");
		}
		else
		{
			m_Edt->ReplaceSel("	Download code FAIL!				                   \r\n");
			rgbText = RGB_RED;
			SetDlgItemText(IDC_PASSFAIL , "FAIL");
		}
		m_Edt->ReplaceSel("****************************************************\r\n");
				
		rgbBkgnd = RGB_GRAY1;
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
	}
	else if(m_Str.Compare("Burn BD") == 0)
	{		
		GetDlgItemText(IDC_PASSFAIL , m_Str);

		if(m_Str.Compare("") != 0)
		{
			m_PASS.StopBlink(CC_BLINK_BOTH);
			SetDlgItemText(IDC_PASSFAIL , "");
			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			Sleep(100);
			ReadBHC302INIData();
		}

		m_Edt->ReplaceSel("Wait MPT Start command!\r\n");

		m_TestProcess = MPT_Start;	
	}
	else if(m_Str.Compare("Record SN") == 0)
	{
		m_Edt->ReplaceSel("Record SN\r\n");
		//S4RecordSN();
	}
	else if(m_Str.Compare("BTExport") == 0)
	{
		S4BTExport();
	}
	if(m_Str.Compare("Next") == 0)
	{
		m_clock.ClearDate();
		pProgess->SetPos(0);

		//pSplash->Hide();
		if(m_PromptMessage)
		{
			SplashScreenString = "程式燒錄中...";
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			pSplash->SetText(SplashScreenString);
		}

		GetDlgItemText(IDC_PASSFAIL , m_Str);

		if(m_Str.Compare("") != 0)
		{
			m_PASS.StopBlink(CC_BLINK_BOTH);
			SetDlgItemText(IDC_PASSFAIL , "");
			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			ReadBHC302INIData();
		}

		SetTimer(AutorunTimer , 100 , NULL);
	}
	else if(m_Str.Compare("Database") == 0)
	{
		m_Edt->ReplaceSel("Connect to Database..\r\n");
		CMySQL dlg;
		dlg.DoModal();
	}
	else
	{
		// Debug(RS232 write)
		/*
		buff[0] = 0xf0;
		buff[1] = 0x04;
		buff[2] = 0x31;
		buff[3] = 0x01;
		buff[4] = 0x36;
		//m_Edt->ReplaceSel("ACK!\r\n");
		MPT_EchoCmd_ptr = (TCHAR *)&buff[0];
#ifdef Debug_RS232_Output_Message
		m_Edt->ReplaceSel("MPT Start!\r\n");
#endif
		Sleep(1000);
		MPTCOM.WriteToPort(MPT_EchoCmd_ptr);
		*/

		/*
		//Detect CSR PIO
		if(OpenTestEngine())
			return;

		if(SetDUTMode())
		{
			return;
		}

		m_Edt->ReplaceSel("TEST!\r\n");

		Sleep(500);
		
		bccmdSetPio(devHandle , PIOMask , PIOValue);

		Sleep(500);

		Buttonstate = 0xf8; 

		SetTimer(DetectPIOTimer , 500 , 0);
		*/
	}
//*/

//=============================================================================
/*
	//RS232 debug 
	TCHAR ICconfigCmd[] = "RUN?";
	buff[0] = 0xf0;
	buff[1] = 0x04;
	buff[2] = 0x31;
	buff[3] = 0x00;
	buff[4] = 0x35;
	//m_Edt->ReplaceSel("ACK!\r\n");
	MPT_EchoCmd_ptr = (TCHAR *)&buff[0];
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel("MPT Start!\r\n");
#endif
	Sleep(500);
	//MPTCOM.WriteToPort(ICconfigCmd);
	MPTCOM.WriteBufferToPort(buff , 5);
	return;
*/

	//GetDlgItemText(IDC_RETEST , m_Str);
	//SetDlgItemText(IDC_RETEST , "Stop");
	//GetDlgItemText(IDC_RETEST , m_Str);

	//uint32 devHandle;
	//CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	//uint16 i , j; 

	//OpenTestEngine();

	//i = 0xfffc;

	//bccmdGetPio(devHandle,&i,&j);

	//m_Str.Format("PIO = %x\r\n",j);

	//m_Edt->ReplaceSel(m_Str);

	//devHandle = openTestEngineSpi(1, 0, SPI_LPT);

	//ReadDUT_BDaddress();

	//CrystalTrimTest();

	//ResetMT8852();

	//unsigned int pos; 

	//pos = 0;

	//CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	//m_Edt->ReplaceSel("TEST\r\n");
	//pos = m_Edt->GetSel();

	//m_Edt->ReplaceSel("TEST\r\n");
	//pos = m_Edt->GetSel();

	//OpenTestEngine();

	//SetDUTMode();

	//SetTimer(DetectPIOTimer , 1000 , NULL);

	/*
	m_Str = GetINIData("BTApp" , "Status");
	
	if(m_Str.Compare("Run") == 0)
	{			
		CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
		m_Edt->ReplaceSel("BT A2DP Test!\r\n");
		
		//ExecuteFile();
		//ShellExecute(0 , "open" , "btapp.exe" , NULL , NULL , SW_SHOWMAXIMIZED );//Debug mode
		
		ShellExecute(0 , "open" , "btapp.exe" , NULL , NULL , SW_HIDE );

		m_ThreadParam.m_hwnd = this->m_hWnd;
		m_ThreadParam.phase = 0;
		m_pLptThrd = AfxBeginThread(BTThreadfun , (LPVOID)&m_ThreadParam);
		
		//m_pLptThrd = AfxBeginThread(BTThreadfun,(LPVOID)&m_ThreadParam,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		//m_pLptThrd->ResumeThread();
		//WaitForSingleObject(m_pLptThrd->m_hThread,INFINITE);

		//while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
		//{
		//	Sleep(500);
		//}

		//m_Wave1.Load("d:\\ho005167\\sound.wav");
		//m_Wave1.Play();
	}
	else
	{

	}
	*/

	//m_Wave1.Load("d:\\ho005167\\sound.wav");
	//m_Wave1.Play();

	//if (!m_Wave1.IsPaused())
	//{
	//	m_Wave1.Pause();
	//}

	//if (!m_Wave1.IsStopped())
	//{
	//	m_Wave1.Stop();
	//}

	//**********************************************************************
	/*
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if(m_Autorun.Compare("true") != 0)
	{
		if(i_MT8852Script == 3 && m_MT8852Action.Compare("Reset") == 0)
		{
			m_Edt->ReplaceSel("Reset MT8852 & Select Script 3\r\n");
			ResetMT8852();
		}
		else if(i_MT8852Script == 0 && m_MT8852Action.Compare("Reset") == 0)
		{
			m_Edt->ReplaceSel("Reset MT8852\r\n");
			ResetMT8852();
		}
		else if(m_MT8852Action.Compare("Run") == 0)
		{
			if(i_MT8852Script == 1)
			{
				//Quick Test
				QuickTest();
				m_Edt->ReplaceSel("MT8852 Quick Test\r\n");					
			}
			else if(i_MT8852Script == 3)
			{
				//Script 3 Test
			}
		}
		else if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 3)
		{
			m_Edt->ReplaceSel("Crystal Trim testing\r\n");
			CrystalTrimTest();
		}
	}
	*/
	//**********************************************************************
}   
   	//***********************************************************
    //***************** USI Own Test procedure ******************
	//***********************************************************
	/*
	//ConnectToServer();

	//Sleep(1000);

	//if(WaitForSingleObject( ConnectSFIS->m_hThread , 0 ) == WAIT_TIMEOUT )
	//{
	//	AfxMessageBox("SFIS no response!");
	//	return;
	//}

	//AfxMessageBox("PASS!");

	CButton *pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
	
	pBtn->EnableWindow(FALSE);

	Sleep(1000);

	ThreadTest();				// Post Code Enable

	AC_On();

	Sleep(1500);

	Power_on();

	Sleep(500);

	if(ChkPowerOn())
	{
		SetDlgItemText(IDC_PASSFAIL , "");

		rgbText = RGB_RED;
		rgbBkgnd = RGB_GREEN;

		SetDlgItemText(IDC_PASSFAIL , "VCC5M is not Active!");

		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

		ActiveTest = FALSE;				// Stop ThreadTest();

		return;
	}
	
	Start();	// Start Timer

	OnFileSharing();

	ShowItem();

	CRTSwitch();

	m_Initial = TRUE;

	//SetTimer(2, 2000, 0);
	*/
/*
//****************************************************
//*******		  TAURUS Test Procedure        ********
//****************************************************

  if(m_Password != 12419)

    FillListCtrl(m_List);

  	m_clock.StartTimer();  // Start Timer
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE); 

	KillTimer(1);	 
	CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgess->SetPos(0);

	//rgbText = RGB_BLUE;
	//m_PASS.SetTextColor(rgbText);
    //m_PASS.ChangeFontHeight(90, TRUE); 
	
	//if(!(ReadFixtureID()))
	//{
	//	m_Str = "Fixture ID Fail!(Check Fixture Power)";
    //    DispString(m_Str);
    //    m_clock.StopTimer();
	//	return;
	//}
	
    EnablePost();

    AC_On();

	Sleep(1500);

    Power_on();

    Sleep(10000);

    //DIG_IN_Prt(1, 9, &iPattern);

    if(iPattern == 0x85)	
	{
        Sendkey(24);		// F2 (Load Default BIOS)     
		Sendkey(24);
		Sleep(3000);

        AC_Off();
	}

	AC_Off();

	EnableRJ11();

	Sleep(500);

	if(!(RJ11LoopTest()))
   	{
		rgbText = RGB_RED;
		m_PASS.SetTextColor(rgbText);
		m_clock.StopTimer();
		DispXList(0 , 0);
		m_Str = "RJ11 Test FAIL!";
		SetDlgItemText(IDC_DSPMESS , m_Str);
		pProgess->SetPos(1);
		return ; 
	}
	else
	{
		m_Str = GetINIData("DEBUG" , "RJ11");

	    if(m_Str.Compare("N") == 0 )
		{
			CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

            m_Str =  "Skip";
		    m_list->SetItemText(0 , 2 , m_Str, RGB_BLUE , RGB_AZURE );
			m_list->SetItemText(0 , 3 , m_Str, RGB_BLUE , RGB_AZURE );
			pProgess->SetPos(1);
		}
        else
		{
			DispXList(0 , 1);
			pProgess->SetPos(1);
		}
	}

    m_LptthrParam.m_hwnd = this->m_hWnd ;
    m_LptthrParam.phase = BootSequence;

    ActiveProcess = 1;  // Start DIO96 Thread
	
    EnablePost();

    m_pLptThrd = AfxBeginThread(DIO96Threadfun , (LPVOID)&m_LptthrParam);

    if(m_pLptThrd == NULL)
	{
	    m_Str = "DIOThreadError !";
        DispString(m_Str);
		m_clock.StopTimer();
		return;
	}

	SetGPIB(10 , 4.5);
	EnableSMBUS();
	EnableBattery();
    
	AC_On();

	Sleep(500);

	SetTimer(2, 300, 0);

	FileCtrl.Open("d:\\usinote\\wakeup.bat" ,CFile::modeCreate | CFile::modeReadWrite );

    FileCtrl.Write("winwake /d 5 " + m_Maccode , 25 );

    FileCtrl.Close();

*/

BOOL CDIODlg::SetGPIB(double V, double I)
{
    CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

 	char VSET[15];
	char ISET[15];
	double value = 0 ;
	//char *endptr;
	int Dev;

	//char ReadBuffer[ARRAYSIZE + 1];  // Read data buffer
	
	int (_stdcall *Pibdev)(int ud, int pad, int sad, int tmo,int eot, int eos);
	int (_stdcall *Pibonl)(int ud, int v);
	int (_stdcall *Pibwrt)(int ud, PVOID buf, LONG cnt);
	int (_stdcall *Pibrd)(int ud, PVOID buf, LONG cnt);

	int *Pibsta;
	long *Pibcntl;

  	HINSTANCE Gpib32Lib = NULL;
	Gpib32Lib=LoadLibrary("GPIB-32.DLL");
	if (Gpib32Lib == NULL) {
		return FALSE;
	}

   	Pibdev = (int (__stdcall *)(int, int, int, int, int, int))GetProcAddress(Gpib32Lib, (LPCSTR)"ibdev");
	Pibonl = (int (__stdcall *)(int, int))GetProcAddress(Gpib32Lib, (LPCSTR)"ibonl");
	Pibwrt  = (int (_stdcall *)(int, PVOID, LONG))GetProcAddress(Gpib32Lib, (LPCSTR)"ibwrt");
	Pibrd   = (int (_stdcall *)(int, PVOID, LONG))GetProcAddress(Gpib32Lib, (LPCSTR)"ibrd");
	Pibcntl = (long *)GetProcAddress(Gpib32Lib, (LPCSTR)"user_ibcnt");
	Pibsta = (int *)GetProcAddress(Gpib32Lib , (LPCSTR)"user_ibsta");

	if ((Pibdev == NULL) ||
        (Pibwrt == NULL) ||
        (Pibrd  == NULL) )
	{
        FreeLibrary(Gpib32Lib);
        Gpib32Lib = NULL;
        return FALSE;
    }

	Dev = (*Pibdev) (BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
                     13, EOTMODE, EOSMODE);
	
	if((*Pibsta) & ERR)
	{
        FreeLibrary(Gpib32Lib);
        Gpib32Lib = NULL;
        m_Str = "Unable to open Power Supply(6632B)!";
		DispString(m_Str);
        return FALSE;
	}

	sprintf(VSET,"VSET %f",V);
	(*Pibwrt) (Dev, VSET , 12L);  
    Sleep(500);

	if((*Pibsta) & ERR)
	{
        FreeLibrary(Gpib32Lib);
        Gpib32Lib = NULL;
        m_Str = "Power Supply is not ready!(6632B)";
		DispString(m_Str);
        return FALSE;
	}

	sprintf(ISET,"ISET %f",I);
	(*Pibwrt) (Dev, ISET , 12L);
     Sleep(300);

	(*Pibwrt) (Dev, "IOUT?" , 5L);
    Sleep(500);

	FreeLibrary (Gpib32Lib);
	Gpib32Lib = NULL;
		return TRUE;
}

void CDIODlg::Power_on()   //Power Button On 
{
    m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
    
	if((m_Str.Compare("TAURUS") == 0 ))
	{	
		//DIG_OUT_Line(1, 11, 0 , 1 );  // Port 11 , Bit 0 : Power On/Off (0/1)
		Sleep(500);

		//DIG_OUT_Line(1, 11, 0 , 0 );  // Port 11 , Bit 0 : Power On/Off (0/1)
		Sleep(700);

		//DIG_OUT_Line(1, 11, 0 , 1 );  // Port 11 , Bit 0 : Power On/Off (0/1)
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{
		// For IBM Project
	    /*
		//DIG_OUT_Prt(1, 10, 255);

		Sleep(400);
		//DIG_OUT_Prt(1, 10, 254);

		Sleep(400);
	    //DIG_OUT_Prt(1, 10, 255);  

	    Sleep(400);   // Add by Ken
		*/
	}
}

void CDIODlg::SetBIOS()
{
    //F9  Load Default
    Sendkey(37);
	Sleep(200);

    Sendkey(81);   // Enter Key
	Sleep(500);

	Sendkey(128);  // Down Key
    Sleep(200);

	Sendkey(84);   // "0"
	Sleep(200);

	Sendkey(56);   // "1"
    Sleep(200);

    Sendkey(81);   // Enter Key
	Sleep(200);

	Sendkey(84);   // "0"
	Sleep(200);

	Sendkey(56);   // "1"
	Sleep(200);

    Sendkey(81);   // Enter Key
	Sleep(200);

	
    Sendkey(72);	// "2" Key
	Sleep(200);
    Sendkey(84);	// "0" Key
	Sleep(200);
	Sendkey(84);	// "0" Key
	Sleep(200);
	Sendkey(84);	// "0" Key
	Sleep(200);

    Sendkey(81);	// Enter Key
	Sleep(200);

	Sendkey(112);   //  Key
	Sleep(200);

	Sendkey(112);   //  Key
	Sleep(200);

	Sendkey(112);   //  Key
	Sleep(200);

	Sendkey(112);   //  Key
	Sleep(200);

	Sendkey(112);   //  Key
	Sleep(200);

	Sendkey(128);   //  Key
	Sleep(200);

	Sendkey(128);   //  Key
	Sleep(200);

	Sendkey(128);   //  Key
	Sleep(200);

    Sendkey(128);   //  Key
	Sleep(200);

	Sendkey(81);	// Enter Key
	Sleep(200);

	Sendkey(128);	// Down Key
    Sleep(200);

	Sendkey(81);	// Enter Key
	Sleep(200);

	Sendkey(36);	// F10 Key
	Sleep(200);

    Sendkey(81);	// Enter Key
	Sleep(200);

}
    //************  F1 IBM BIOS setup ***************
   	/*
	//F9 load default
	Sleep(2000);

	Sendkey(24);	Sleep(100);
	//Yes
	Sendkey(68);	Sleep(200);
	//Enter
	Sendkey(120);	Sleep(200);
	//.....//
	Sendkey(120);	Sleep(200);
	Sendkey(138);	Sleep(100);
	Sendkey(120);	Sleep(200);
	Sendkey(120);	Sleep(200);
	Sendkey(108);	Sleep(100);
	Sendkey(120);	Sleep(200);
	
	Sendkey(96);	Sleep(200);
	Sendkey(138);	Sleep(100);
	Sendkey(120);	Sleep(200);
	Sendkey(120);	Sleep(200);
	Sendkey(108);	Sleep(100);
	
	Sendkey(120);	Sleep(200);
	Sendkey(96);	Sleep(100);
	Sendkey(96);	Sleep(100);
	Sendkey(138);	Sleep(100);
	Sendkey(120);	Sleep(200);

	Sendkey(39);	Sleep(100);
	Sendkey(32);	Sleep(100);
	Sendkey(64);	Sleep(100);
	Sendkey(39);	Sleep(100);
	Sendkey(32);	Sleep(100);
	Sendkey(64);	Sleep(100);
	
	Sendkey(33);	Sleep(100);
	Sendkey(39);	Sleep(100);
	Sendkey(39);	Sleep(100);
	Sendkey(39);	Sleep(100);
	Sendkey(96);	Sleep(200);
	//F10
	Sendkey(40);	Sleep(500);
	//Enter
	Sendkey(120);	Sleep(500);
    */

void CDIODlg::Power_off()						//Power Button Off 
{
    m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
    
	if((m_Str.Compare("TAURUS") == 0 ))
	{	
		////DIG_OUT_Line(1, 11, 0 , 1 );  // Port 11 , Bit 0 : Power On/Off (0/1)
		//Sleep(1000); 

		////DIG_OUT_Line(1, 11, 0 , 0 );  // Port 11 , Bit 0 : Power On/Off (0/1)
		//Sleep(1500);

		////DIG_OUT_Line(1, 11, 0 , 1 );  // Port 11 , Bit 0 : Power On/Off (0/1)
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{
		//     IBM Project
		/*
		//DIG_OUT_Prt(1, 10, 255);

		Sleep(1500);
		//DIG_OUT_Prt(1, 10, 254);

		Sleep(1500);
		//DIG_OUT_Prt(1, 10, 255);
		*/
	}
}

BOOL CDIODlg::ReadFixtureID()
{
    //CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	/*
	iStatus = 0;
    //DIG_OUT_Prt(1,2,1);
    Sleep(100);
    //DIG_IN_Prt(iDevice, 9, &iPattern); 
	if(iPattern != 0x76){
		return FALSE;
	}
    else{
		//m_Edt->ReplaceSel("Fixture ID Check....PASS!\r\n");
		return TRUE;
	}
	*/
	return TRUE;
}

void CDIODlg::Sendkeymatrix()
{
	//AfxMessageBox("TEST");
	
	Sendkey(124);
	Sendkey(17); Sendkey(18); Sendkey(66); Sendkey(98); Sendkey(104);Sendkey(101);Sendkey(70); Sendkey(22); Sendkey(24); Sendkey(40); Sendkey(42); Sendkey(41);
	Sendkey(16); Sendkey(32); Sendkey(33); Sendkey(34); Sendkey(35); Sendkey(19); Sendkey(20); Sendkey(36); Sendkey(37); Sendkey(38); Sendkey(39); Sendkey(23); Sendkey(21); Sendkey(72);
	Sendkey(64); Sendkey(48); Sendkey(49); Sendkey(50); Sendkey(51); Sendkey(67); Sendkey(68); Sendkey(52); Sendkey(53); Sendkey(54); Sendkey(55); Sendkey(71); Sendkey(69); Sendkey(88);
	Sendkey(65); Sendkey(80); Sendkey(81); Sendkey(82); Sendkey(83); Sendkey(99); Sendkey(100);Sendkey(84); Sendkey(85); Sendkey(86); Sendkey(87); Sendkey(103);Sendkey(120);
	Sendkey(78); Sendkey(112);Sendkey(113);Sendkey(114);Sendkey(115);Sendkey(131);Sendkey(132);Sendkey(116);Sendkey(117);Sendkey(118);Sendkey(135);Sendkey(126);
	Sendkey(32); Sendkey(31); Sendkey(109);Sendkey(136);Sendkey(141);Sendkey(127);
	Sendkey(123); Sendkey(139); 
	Sendkey(108);Sendkey(140);Sendkey(138);Sendkey(137);

	Sendkey(45); Sleep(30);
	Sendkey(61); Sleep(30);
	Sendkey(124);Sleep(30);
	Sendkey(25); Sleep(30);
	Sendkey(28); Sleep(30);
	Sendkey(27); Sleep(30);
	Sendkey(26); Sleep(30);
	Sendkey(44); Sleep(30);
	Sendkey(43); Sleep(30);

	Sendkey(124);Sleep(100);
	Sendkey(116);Sendkey(117);Sendkey(118);Sendkey(135);Sendkey(126);Sendkey(31);  Sendkey(109);Sendkey(136);
	Sendkey(141);Sendkey(127);Sendkey(108);Sendkey(140);Sendkey(138);Sendkey(137);

	Sendkey(96);

	Sleep(1000);
	DisableKbd();
	
}

/*
//   NB-TAURUS Keyboard Matrix

	Sendkey(26);  Sendkey(42);  Sendkey(24);  Sendkey(40);  Sendkey(23);  Sendkey(39);  Sendkey(25);
	Sendkey(22);  Sendkey(21);  Sendkey(37);  Sendkey(36);  Sendkey(64);  Sendkey(80);  Sendkey(16); 
    Sendkey(114); Sendkey(58);  Sendkey(56);  Sendkey(72);  Sendkey(71);  Sendkey(55);  Sendkey(41);
	Sendkey(38);  Sendkey(54);  Sendkey(53);  Sendkey(69);  Sendkey(84);  Sendkey(67);  Sendkey(51);
	Sendkey(33);  Sendkey(74);  Sendkey(88);  Sendkey(104); Sendkey(87);  Sendkey(57);  Sendkey(73);
	Sendkey(70);  Sendkey(85);  Sendkey(101); Sendkey(52);  Sendkey(68);  Sendkey(83);  Sendkey(66);
	Sendkey(65);  Sendkey(120); Sendkey(103); Sendkey(119); Sendkey(89);  Sendkey(105); Sendkey(86);
	Sendkey(102); Sendkey(117); Sendkey(100); Sendkey(116); Sendkey(99);  Sendkey(81);  Sendkey(139);
	Sendkey(106); Sendkey(135); Sendkey(121); Sendkey(137); Sendkey(118); Sendkey(134); Sendkey(133);
	Sendkey(132); Sendkey(115); Sendkey(98);  Sendkey(113); Sendkey(123); Sendkey(143); Sendkey(142);
	Sendkey(141); Sendkey(140); Sendkey(138); Sendkey(124);	Sendkey(130); Sendkey(126); Sendkey(129);
	Sendkey(128); Sendkey(112); Sendkey(125); Sendkey(17);  Sendkey(90);  Sendkey(35);  Sendkey(90);
	Sendkey(35);  Sendkey(58);

//  Esc(26)		F1(42)		F2(24)		F3(40)		F4(23)		F5(39)		F6(25)
//	F7(22)		F8(21)		F9(37)		F10(36)		PgUp(64)	PgDn(80)	Pause(16)
//	Num(35)		Insert(114)	Delete(17)	~(58)		1(56)		2(72)		3(71)
//	4(55)		5(41)		6(38)		7(54)		8(53)		9(69)		0(84)
//	-(67)		+(51)		Back(33)	Tab(74)		Q(88)		W(104)		E(87)	
//	R(57)		T(73)		Y(70)		U(85)		I(101)		O(52)		P(68)
//	{(83)		}(66)		|(65)		Caps(90)	A(120)		S(103)		D(119)
//  F(89)		G(105)		H(86)		J(102)		K(117)		L(100)		:(116)
//	"(99)		Enter(81)	Shift(139)	Z(106)		X(135)		C(121)		V(137)
//	B(118)		N(134)		M(133) 		<(132)		>(115)		?(98)		Up(113)
//  Shift(123)	Fn(143)		Ctrl(142)	Win(141)	Alt(140)	Space(138)	Alt(124)
//	Ptr(130)	Ctrl(126)	Left(129)	Down(128)	Right(112)	Spk(125)
*/

//   IBM  Keyboard Matrix

//Sendkey(96//Esc); Sendkey(25//Insert); Sendkey(28//Home); Sendkey(27//PgUp); Sendkey(17//F1); Sendkey(18//F2);  Sendkey(66//F3); Sendkey(98//F4);


//Sendkey(104//F5);Sendkey(101//F6);Sendkey(70//F7); Sendkey(22//F8); Sendkey(24//F9); Sendkey(40//F10);  Sendkey(26/Delete); Sendkey(44//End);


//Sendkey(43//PgUp); Sendkey(16//`); Sendkey(32//1); Sendkey(33//2); Sendkey(34//3); Sendkey(35//4);  Sendkey(19//5); Sendkey(20//6);


//Sendkey(36//7); Sendkey(37//8); Sendkey(38//9); Sendkey(39//0); Sendkey(23); Sendkey(21);  Sendkey(72); Sendkey(64);


//Sendkey(48); Sendkey(49); Sendkey(50); Sendkey(51); Sendkey(67); Sendkey(68);  Sendkey(52); Sendkey(53);


//Sendkey(54); Sendkey(55); Sendkey(71); Sendkey(69); Sendkey(88); Sendkey(80);  Sendkey(81); Sendkey(82);


//Sendkey(83); Sendkey(99); Sendkey(100);Sendkey(84); Sendkey(85); Sendkey(86);  Sendkey(87); Sendkey(103);


//Sendkey(120);Sendkey(78); Sendkey(112);Sendkey(113);Sendkey(114);Sendkey(115); Sendkey(131);Sendkey(132);


//Sendkey(116);Sendkey(117);Sendkey(118);Sendkey(135);Sendkey(126);Sendkey(31);  Sendkey(109);Sendkey(136);


//Sendkey(141);Sendkey(127);Sendkey(108);Sendkey(140);Sendkey(138);Sendkey(137);

/*         //  Define Keyboard Matrix Test for IBM NB Project

// Common Key
Esc				110	60 = 96
PrtSc			124	2D = 45
ScrLk			125	3D = 61
Pause			126	7C = 124
Insert			75	19 = 25
Home			80	1C = 28
PgUp			85	1B = 27
F1				112	11 = 17
F2				113	12 = 18
F3				114	42 = 66
F4				115	62 = 98
F5				116	68 = 104
F6				117	65 = 101
F7				118	46 = 70
F8				119	16 = 22
F9				120	18 = 24
F10				121	28 = 40
F11				122	2A = 42
F12				123	29 = 41
Delete			76	1A = 26
End				81	2C = 44
PgDn			86	2B = 43
1				2	20 = 32
2				3	21 = 33
3				4	22 = 34
4				5	23 = 35
5				6	13 = 19
6				7	14 = 20
7				8	24 = 36
8				9	25 = 37
9				10	26 = 38
0				11	27 = 39
-				12	17 = 23
Tab				16	40 = 64
Q				17	30 = 48
W				18	31 = 49
E				19	32 = 50
R				20	33 = 51
T				21	43 = 67
Y				22	44 = 68
U				23	34 = 52
I				24	35 = 53
O				25	36 = 54
P				26	37 = 55
Caps			30	41 = 65
A				31	50 = 80
S				32	51 = 81
D				33	52 = 82
F				34	53 = 83 
G				35	63 = 99
H				36	64 = 100
J				37	54 = 84
K				38	55 = 85
L				39	56 = 86
;				40	57 = 87
Enter			43	78 = 120
Shift_Left		44	4E = 78
Z				46	70 = 112
X				47	71 = 113
C				48	72 = 114
V				49	73 = 115
B				50	83 = 131
N				51	84 = 132
M				52	74 = 116
,				53	75 = 117
.				54	76 = 118
/				55	87 = 135
Shit_Right		57	7E = 126
Fn				Fn	20 = 32
Ctrl_Left		58	1F = 31
Alt_Left		60	6D = 109
Space			61	88 = 136
Alt_Right		62	8D = 141
Ctrl_Right		64	7F = 127
Up				83	6C = 108
Left			79	8C = 140
Down			84	8A = 138
Right			89	89 = 137

// US Key
`				1	10 = 16
=				13	15 = 21
Backspace		15	48 = 72
BracketLeftUS	27	47 = 71
BracketRightUS	28	45 = 69
BackslashUS		29	58 = 88
'				41	67 = 103

// Japanese Key

Kanji			1	10
^				13	15
Yen				14	20
BackspaceJp		15	20
@				27	47
BracketLeftJp	28	45
ColonJp			41	67
BracketRightJp	42	20
BackslashJp		56	20
Muhenkan		131	20
Henkan			132	20
Kana			133	20

// ThinkPad Key

ThinkPad		EZ1	6A
VolumeDown		EZ2	4A
VolumeUp		EZ3	3A
Muete			EZ4	5A

// e-Xperience Key

Mail			IN1	49
Home			IN2	39
Search			IN3	59
Bookmark		IN4	4C
Reload			IN5	3C
Stop			IN6	5C
Back			INB	7B
Next			INF	8B
*/

void CDIODlg::Sendkey(int keyvalue)
{
	/*
	// *********************************** //
	// IBM Keyboard
	
    //DIG_OUT_Prt(1, 7, keyvalue);
    Sleep(40); 	
	//DIG_OUT_Prt(1, 11, 251);
	Sleep(40);
	//DIG_OUT_Prt(1, 7, 0);
    Sleep(40); 
    //DIG_OUT_Prt(1, 11, 243);
	//Sleep(40);							   // Add on 2005/05/11

	// *********************************** //
	*/
}

	// *********************************** //
	//TAURUS Keyboard

    //iStatus = //DIG_OUT_Prt(1, 7, keyvalue);
    //Sleep(40); 	
	//iStatus = //DIG_OUT_Prt(1, 1, 254);
	//Sleep(40);
	//iStatus = //DIG_OUT_Prt(1, 7, 0);
    //Sleep(40); 
    //iStatus = //DIG_OUT_Prt(1, 1, 255);

	// *********************************** // 

void CDIODlg::OnUUTFail()
{
	//AfxMessageBox("UUT FAIL!");

	m_clock.StopTimer();

	CString	str;
	rgbText = RGB_RED;
	rgbBkgnd = RGB_GREEN;

	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

	rename("d:\\usinote\\uut.log" , ("d:\\usinote\\log\\" + m_Barcode + ".txt"));
	
	if(findfile.FindFile("d:\\usinote\\testlog.txt" , 0)) 
	{
		if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeReadWrite)))
		{
			MessageBox("Open Testlog File Error!"  , "" , MB_OK);
			return;
		}
    }
	else
	{
		if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeCreate | CFile::modeReadWrite)))
		{
			MessageBox("Testlog File Open Error!"  , "" , MB_OK);
			return;
		}

		str = "ComputerName\tDate\tTime\tBarcode\tMaccode\tResult\tErrorCode\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		str = "********************************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
	}

	FileCtrl.SeekToEnd();
	GetLocalName();

	CTime t = CTime::GetCurrentTime();

	str.Format("%s    %d/%d/%d    %d:%d:%d" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	FileCtrl.Write(str , str.GetLength()+1);

	GetDlgItemText(IDC_PASSFAIL , m_Str);

	str = ( "    " + m_Barcode + "    " + m_Maccode + "    " + m_Str ) ;
	FileCtrl.Write(str , str.GetLength()+1);
	
	str = "\r\n";
	FileCtrl.Write(str , str.GetLength()+1);

	FileCtrl.Close();
}

void CDIODlg::OnOK()
{

}

void CDIODlg::OnCancel()
{

}

void CDIODlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
/*
	if(m_pLptThrd != NULL)
	{
		ActiveProcess = FALSE;
	
		while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
		//AfxMessageBox("TEST1");
	}

	if(serialProcess != NULL)
	{
		ActiveSerial = FALSE;

		while(WaitForSingleObject( serialProcess , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
		//AfxMessageBox("TEST2");
	}

	if(m_LPTThread != NULL)		
	{
		ActiveTest = 0;

		while(WaitForSingleObject( m_LPTThread , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
	}

	SetDlgItemText(IDC_PASSFAIL , "");
	
	if(ChkPowerOff())
	{
		SetDlgItemText(IDC_PASSFAIL , "");

		rgbText = RGB_RED;
		rgbBkgnd = RGB_AZURE;

		//AfxMessageBox("VCC5M is still Active!");

		//SetDlgItemText(IDC_PASSFAIL , "VCC5M is still Active!");

		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

		Power_off();

		Sleep(2000);

		AC_Off();
	}

	OnNotifyUUT(); 
*/
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	GetDlgItemText(ID_BTN_EXIT , m_Str);

	if(m_Str.Compare("A2DPEnd") == 0)
	{
		m_IniReader.setKey("Stop","Status","BTApp");//Terminate btapp.exe

		m_IniReader.setKey("","Message","BTApp");

		Sleep(1000);

		m_IniReader.setKey("Run","Status","BTApp");
		m_IniReader.setKey("","Message","BTApp");

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("BTA2DP disconnect!\r\n");
		SetDlgItemText(ID_BTN_EXIT , "Exit");

		BTA2DPConnect = false;
	}
	else if(m_Str.Compare("Reset8852") == 0)
	{
		ResetMT8852();
	}
	else if(m_Str.Compare("Exit") == 0)
	{
		if(m_Autorun.Compare("true") == 0)
		{
			if(pSplash)
				delete pSplash;
		}

		if(IsConnected)
		{
			IsConnected = FALSE;
			mysql_close(myData);
		}

		CDialog::OnCancel();
	}
}

int timeCount = 0; //detect if 8852 connect timeout or not
void CDIODlg::OnTimer(UINT nIDEvent) 
{
  // TODO: Add your message handler code here and/or call default

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	uint16 i,j;
	unsigned char buff[16];
	CButton *pBtn;
	int errorcode;
	DWORD status = 0;
	TCHAR SetMT8852CW[] = "CWRESULT POWER\r\n";
	//TCHAR SetMT8852CW[] = "*INS?\r\n";

	switch(nIDEvent)
	{
		case T21_CW_Measurement:
			//SetMT8852CW[] = "CWRESULT POWER\r\n";
			//M8852EchoCmd_ptr ="CWRESULT POWER\r\n";
			CountCWPower++;
			
			if(CountCWPower <= TestLoop)
			//if(CountCWPower <= 5)
			{
				MT8852COM.WriteToPort(SetMT8852CW);
				m_Edt->ReplaceSel("\r\n");
				m_Edt->ReplaceSel("Query...\r\n");
			}
			else
			{
				KillTimer(T21_CW_Measurement);

				m_TestProcess = 0;
				CountCWPower = 0;

				pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
				pBtn->EnableWindow(TRUE);

				///*
				if(T21_Fail >= 2)
				{
					SetDlgItemText(IDC_PASSFAIL , "FAIL");
					rgbText = RGB_RED;
					rgbBkgnd = RGB_GRAY1;
					m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
					m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
				}
				else
				{
					SetDlgItemText(IDC_PASSFAIL , "PASS");
					rgbText = RGB_BLUE;
					rgbBkgnd = RGB_GRAY1;
					m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
					m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
				}
				//*/
			}
			break;
		case DebugSPITimer:
			//OpenTestEngine();
			//ReadDUT_BDaddress();
			T13WriteBT();
			break;
		case USBConnectTimer:
			//KillTimer(USBConnectTimer);

			/*
			status = m_USBDev.GetDeviceFromRegistry();
			if(status > 0)
			{
				SetWindowText("BHC302_MPT_HOST[F340 USB:OK]");
			}
			else
			{
				SetWindowText("BHC302_MPT_HOST[F340 USB:x]");
			}
			*/

			///*
			if(m_FriendlyARMUSBDev.SearchFriendlyARMUSB())
				SetWindowText("BHC302_MPT_HOST[ARM USB:OK]");
			else
				SetWindowText("BHC302_MPT_HOST[ARM USB:x]");
			//*/
			break;
		case NFCVerifyTimer:
			NFCVerifyCount++;
			if(NFCVerifyCount > 3)
			{
				KillTimer(NFCVerifyTimer);
				NFCVerifyCount = 0;
				m_PASS.StopBlink(CC_BLINK_BOTH);
				SetDlgItemText(IDC_PASSFAIL , "Next");
			}
			break;
		case Chk8852Timer:
			KillTimer(Chk8852Timer);
			if(MT8852_Handshake_flag == false)
			{
				if(Initial8852 < 2)
				{
					TCHAR SetMT8852[] = "SYSCFG? EUTRS232\r\n";
					M8852EchoCmd_ptr ="SYSCFG? EUTRS232\r\n";
					MT8852_Op_str.Format(_T("SYSCFG EUTRS232,57600"));
					MT8852COM.WriteToPort(SetMT8852);
					SetTimer(Chk8852Timer , Chk8852Time , NULL);
					Initial8852++;
				}
				else
				{
					Initial8852 = 0;
					m_Edt->ReplaceSel("Fail to initial 8852!\r\n");

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(FALSE);
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);

					m_TestItemStr = "Handshake MT8852";
					TestErrorCode++;//9001
					m_Str.Format("%X",TestErrorCode);
					m_ErrorCodeStr = m_Str;
					m_ResultStr = "FAIL";
					AddListCtrlItem(m_List , ListCtrlIndex);
					ListCtrlIndex++;

					SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

					rgbText = RGB_RED;
					rgbBkgnd = RGB_GRAY1;
					m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
					m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
				}
			}
			break;
		case DetectPIOTimer:
			i = PIOMask;
			j =0;//Value
			bccmdGetPio(devHandle,&i,&j);
			//m_Str.Format("PIO = %x\r\n",j);
			//m_Edt->ReplaceSel(m_Str);
			timeCount++;

			//BHC302_B2.3
			//Vol_Up		: PIO[13]
			//Vol_Down		: PIO[14]
			//One_Pairing	: PIO[5]
			//#define PIOMask		0x9fdf		//PIO14,PIO13,PIO5
			//#define PIOValue	0x0000
			//#define One_Pairing 0x0020
			//#define Vol_Up		0x2000
			//#define Vol_Down	0x4000
			//if((j & One_Pairing) == One_Pairing && Count_One_pair < 3)//PIO[5]
			if((j & One_Pairing) == One_Pairing && Count_One_pair < 3 && (j & 0x6000) == 0x0000)//PIO[5]
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("One_Pairing button!\r\n");
				Buttonstate |= 0x01;
				timeCount = 0;
				Count_One_pair++;
			}
			//else if((j & Vol_Up) == Vol_Up && Count_Volumn_up < 3)//PIO[13]
			else if((j & Vol_Up) == Vol_Up && Count_Volumn_up < 3 && (j & 0x4020) == 0x0000)//PIO[13]
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("Vol_Up button!\r\n");
				Buttonstate |= 0x02;
				timeCount = 0;
				Count_Volumn_up++;
			}
			//else if((j & Vol_Down) == Vol_Down && Count_Volumn_down < 3)//PIO[14]
			else if((j & Vol_Down) == Vol_Down && Count_Volumn_down < 3 && (j & 0x2020) == 0x0000)//PIO[14]
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("Vol_Down button!\r\n");
				Buttonstate |= 0x04;
				timeCount = 0;
				Count_Volumn_down++;
			}
			else
			{
				//m_Str.Format("PIO = %x\r\n",j);
				//m_Edt->ReplaceSel(m_Str);
			}

			//Time interval = 10ms , Timeout value = 10ms * 10 = sec
			//if(timeCount == 10)
			if(timeCount >= 300)
			{
				//if(Buttonstate != 0xff)
				//{
				//	m_Edt->GetWindowText(m_Str);
				//	m_Edt->SetFocus();
				//	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				//	m_Str.Format("PIO test fail! : %x\r\n",Buttonstate);
				//	m_Edt->ReplaceSel(m_Str);
				//}

				KillTimer(DetectPIOTimer);

				CloseTestEngine();

				TestResultFlag = false;

				m_TestItemStr = "CSR PIO Test";
				TestErrorCode++;
				m_Str.Format("%X",TestErrorCode);
				m_ErrorCodeStr = m_Str;
				m_ResultStr = "FAIL";
				AddListCtrlItem(m_List , ListCtrlIndex);
				ListCtrlIndex++;

				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("****************************************************\r\n");
				m_Str.Format("Read PIO value : %x\r\n",Buttonstate);
				m_Edt->ReplaceSel(m_Str);
				m_Edt->ReplaceSel("PIO test fail!\r\n");
				m_Edt->ReplaceSel("****************************************************\r\n");

				if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
					m_RFTestResult = m_ErrorCodeStr;

				SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_RFTestResult);

				rgbText = RGB_RED;
				rgbBkgnd = RGB_GRAY1;
				m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
				m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

				SetTimer(PCAbortTimer , PCAbortTime , NULL);
			}
			else
			{
				if(Buttonstate == 0xff && Count_Volumn_down > 2 && Count_Volumn_up > 2 && Count_One_pair > 2)
				{
					KillTimer(DetectPIOTimer);

					CloseTestEngine();

					//m_Edt->GetWindowText(m_Str);
					//m_Edt->SetFocus();
					//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
					//m_Edt->ReplaceSel("PIO test pass!\r\n");

					m_TestItemStr = "CSR PIO Test";
					TestErrorCode++;
					m_Str.Format("%X",TestErrorCode);
					m_ErrorCodeStr = m_Str;
					m_ResultStr = "PASS";
					AddListCtrlItem(m_List , ListCtrlIndex);
					ListCtrlIndex++;

					SetTimer(PIOEndTimer , PIOEndTime , NULL);

					m_Edt->GetWindowText(m_Str);
					m_Edt->SetFocus();
					m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
					m_Edt->ReplaceSel("****************************************************\r\n");
					m_Edt->ReplaceSel("PIO test pass\r\n");
					m_Edt->ReplaceSel("****************************************************\r\n");
				}
			}
			break;
		case AutorunTimer:
			if(MT8852_Handshake_flag == true)
			{
				KillTimer(AutorunTimer);
				
				if(m_TestStation.Compare("T1") == 0)
				{
					if(m_TestProcess != MPT_Start && m_TestProcess != T1RFTest)
					{
						pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
						pBtn->EnableWindow(FALSE);
						pBtn = (CButton*)GetDlgItem(IDC_RETEST);
						pBtn->EnableWindow(FALSE);

						//m_clock.StartTimer();  // Start Timer

						TestResultFlag = true;

						m_TestProcess = MPT_Start;	

						m_Edt->ReplaceSel("Wait MPT Start command!\r\n");

						//CSRPIOTest();
					}
					else if(m_TestProcess == T1RFTest)
					{
						///*
						m_Edt->GetWindowText(m_Str);
						m_Edt->SetFocus();
						m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

						if(i_MT8852Script == 3 && m_MT8852Action.Compare("Reset") == 0)
						{
							m_Edt->ReplaceSel("Reset MT8852 & Select Script 3\r\n");
							ResetMT8852();
							TestErrorCode = 0xA000;
						}
						else if(i_MT8852Script == 0 && m_MT8852Action.Compare("Reset") == 0)
						{
							m_Edt->ReplaceSel("Reset MT8852\r\n");
							ResetMT8852();
							TestErrorCode = 0xB000;
						}
						else if(m_MT8852Action.Compare("Run") == 0)
						{
							if(i_MT8852Script == 1)
							{
								//Quick Test
								m_Edt->ReplaceSel("MT8852 Quick Test\r\n");
								TestErrorCode = 0xC000;
								Sleep(5000);
								QuickTest();
							}
							else if(i_MT8852Script == 3)
							{
								//Script 3 Test
								SetDlgItemText(IDC_BUTTON1 , "Script3Test");
								TestErrorCode = 0xD000;
								//MT8852Script3Test();

								if(m_Autorun.Compare("true") == 0)
								{
									MT8852Script3Test();
								}
								else
								{
									if(RunScript3Test)
										QuickTest();
								}
							}
						}
						else if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 3)
						{
							m_Edt->ReplaceSel("Crystal Trim testing\r\n");
							CrystalTrimTest();
							//SetDlgItemText(IDC_BUTTON1 , "CrystalTrim");
							//SetDlgItemText(IDC_RETEST , "Re-Test");
							//SetDlgItemText(ID_BTN_EXIT , "Reset8852");
							TestErrorCode = 0xE000;
						}
						//*/
					}
				}
				else if(m_TestStation.Compare("T2.1") == 0)
				{
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);
					SetDlgItemText(IDC_BUTTON1 , "T2.1");
				}
			}
			else
			{
				//KillTimer(AutorunTimer);
				if(m_TestStation.Compare("T0") == 0)
				{
					KillTimer(AutorunTimer);

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(FALSE);
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);

					m_clock.ClearDate();	
					m_clock.StartTimer();
					
					Sleep(50);

					//Start();

					m_Edt->ReplaceSel("Download code!\r\n");
///*				
					TestErrorCode = 0xA500;

					errorcode = CSRBlueFlash();
					if(errorcode != 0)
					{
						TestResultFlag = false;
						m_ResultStr = "FAIL";
						m_Str.Format("%X", errorcode);
						m_RFTestResult = m_Str;
					}
					else
					{
						TestResultFlag = true;
						m_ResultStr = "PASS";
						SetDlgItemText(IDC_RETEST , "Re-Test");
					}
//*/
					//Sleep(6000);

					m_TestItemStr = "Download code";
					if(TestResultFlag)
						TestErrorCode++;
					m_Str.Format("%X",TestErrorCode);
					m_ErrorCodeStr = m_Str;
						
					AddListCtrlItem(m_List , ListCtrlIndex);
					ListCtrlIndex++;

					m_Edt->GetWindowText(m_Str);
					m_Edt->SetFocus();
					m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
					m_Edt->ReplaceSel("****************************************************\r\n");
					CTime t = CTime::GetCurrentTime();
					m_Str = "Current Time :";
					data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
					m_Edt->ReplaceSel(data);

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(TRUE);

					if(TestResultFlag)
					{
						m_Edt->ReplaceSel("	Download code PASS!				                   \r\n");
						rgbText = RGB_BLUE;
						SetDlgItemText(IDC_PASSFAIL , "PASS");

						if(m_PromptMessage)
						{
						SplashScreenString = "燒錄成功,請更換下一塊主板,之後按壓Next鍵";
						if(ToggleBitmap)
						{
							ToggleBitmap = false;
							pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
						}
						else
						{
							ToggleBitmap = true;
							pSplash->SetBitmap(IDB_SPLASH,255,0,255);
						}
						pSplash->SetText(SplashScreenString);
						}
					}
					else
					{
						m_Edt->ReplaceSel("	Download code FAIL!				                   \r\n");
						rgbText = RGB_RED;
						//SetDlgItemText(IDC_PASSFAIL , "FAIL");
						SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_RFTestResult);
					
						if(m_PromptMessage)
						{
						SplashScreenString = "燒錄失敗,請更換下一塊主板,之後按壓Next鍵";
						if(ToggleBitmap)
						{
							ToggleBitmap = false;
							pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
						}
						else
						{
							ToggleBitmap = true;
							pSplash->SetBitmap(IDB_SPLASH,255,0,255);
						}
						pSplash->SetText(SplashScreenString);
						}
					}
					m_Edt->ReplaceSel("****************************************************\r\n");

					rgbBkgnd = RGB_GRAY1;
					m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
					m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

					m_clock.StopTimer();
					
					SetDlgItemText(IDC_BUTTON1 , "Next");
//*/
				}
				else if(m_TestStation.Compare("T2") == 0)
				{
					KillTimer(AutorunTimer);

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(FALSE);
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);

					GetDlgItemText(IDC_PASSFAIL , m_Str);

					//SetDlgItemText(IDC_BUTTON1 , "Burn BD");

					if(m_Str.Compare("") != 0)
					{
						m_PASS.StopBlink(CC_BLINK_BOTH);
						SetDlgItemText(IDC_PASSFAIL , "");
						ClearListData();
						ListCtrlIndex = 0;
						ClearAllEditMessage();
						Sleep(1000);
						ReadBHC302INIData();
					}

					TestErrorCode = 0xF000;

					m_TestProcess = MPT_Start;	

					TestResultFlag = true;

					m_Edt->GetWindowText(m_Str);
					m_Edt->SetFocus();
					m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
					m_Edt->ReplaceSel("Wait MPT Start command!\r\n");

					//ShowWindow(SW_MINIMIZE);

					/*
					m_Str = GetINIData("BTApp" , "Status");

					TestErrorCode = 0xF000;

					if(m_Str.Compare("Run") == 0)
					{			
						m_Str = GetINIData("BTApp" , "Message");

						if(m_Str.Compare("") == 0)
						{	
							m_Edt->ReplaceSel("BT A2DP Test!\r\n");

							ShellExecute(0 , "open" , "btapp.exe" , NULL , NULL , SW_HIDE );//Execute btapp.exe

							m_ThreadParam.m_hwnd = this->m_hWnd;
							m_ThreadParam.phase = 0;
							m_pLptThrd = AfxBeginThread(BTThreadfun , (LPVOID)&m_ThreadParam);//Create thread to monitor debug message 
						
							m_Edt->ReplaceSel("Connect..\r\n");
						}
					}
					*/
				}
				//else if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("T3") == 0)
				else if(m_TestStation.Compare("T1.3") == 0)
				{
					KillTimer(AutorunTimer);

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(FALSE);
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);

					m_Edt->GetWindowText(m_Str);
					m_Edt->SetFocus();
					m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

					if(m_TestStation.Compare("T1.3") == 0)
						TestErrorCode = 0xF100;
					//else
					//	TestErrorCode = 0xF300;
		
					GetDlgItemText(IDC_PASSFAIL , m_Str);

					if(m_TestStation.Compare("T1.3") == 0)
						SetDlgItemText(IDC_BUTTON1 , "Burn BD");
					//else
					//	SetDlgItemText(IDC_BUTTON1 , "T3");

					if(m_Str.Compare("") != 0)
					{
						m_PASS.StopBlink(CC_BLINK_BOTH);
						SetDlgItemText(IDC_PASSFAIL , "");
						ClearListData();
						ListCtrlIndex = 0;
						ClearAllEditMessage();
						Sleep(1000);
						ReadBHC302INIData();
					}

					TestResultFlag = true;

					m_TestProcess = MPT_Start;	

					m_Edt->ReplaceSel("Wait MPT Start command!\r\n");
				}
				//else if(m_TestStation.Compare("T3") == 0)//Auto BT Connect
				//{
				//	KillTimer(AutorunTimer);
				//}
				else if(m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T3") == 0)
				{
					KillTimer(AutorunTimer);

					//pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					//pBtn->EnableWindow(FALSE);
					//pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					//pBtn->EnableWindow(FALSE);

					if(m_TestStation.Compare("S4") == 0)
						TestErrorCode = 0xF200;
					else if(m_TestStation.Compare("T3") == 0)
						TestErrorCode = 0xF300;

					if(m_TestStation.Compare("S4") == 0)
					{
						//SetDlgItemText(IDC_BUTTON1 , "Record SN");
						m_Str = GetINIData("S4_NFC" , "export");
						if(m_Str.Compare("true") == 0)
						{
							SetDlgItemText(IDC_BUTTON1 , "BTExport");
						}	
						else
						{
							pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
							pBtn->EnableWindow(FALSE);
						}

						m_Str = GetINIData("S4_NFC" , "ReverseBT");
						if(m_Str.Compare("true") == 0)
						{
							SetDlgItemText(IDC_RETEST , "BTReverse");
						}	
						else
						{
							pBtn = (CButton*)GetDlgItem(IDC_RETEST);
							pBtn->EnableWindow(FALSE);
						}
					}
					else
						SetDlgItemText(IDC_BUTTON1 , "T3");

					GetDlgItemText(IDC_PASSFAIL , m_Str);					

					if(m_Str.Compare("") != 0)
					{
						m_PASS.StopBlink(CC_BLINK_BOTH);
						SetDlgItemText(IDC_PASSFAIL , "");
						ClearListData();
						ListCtrlIndex = 0;
						ClearAllEditMessage();
						Sleep(1000);
						ReadBHC302INIData();
					}

					//CString szFilter = "text|*.txt|*.*|*.*||";
					//CFileDialog fd(TRUE,"txt","*.txt",OFN_HIDEREADONLY,szFilter,this);

					/*
					if(fd.DoModal() == IDOK)
					{
						//如果使用者開啟了一個檔案
						CString szFileName = fd.GetPathName(); //取得開啟檔案的全名(包含路徑)
					}
					else
					{
						//如果使用者取消
					}
					*/

#ifdef S4WithMPT

					m_TestProcess = MPT_SentRealBAComd;

					m_Edt->ReplaceSel("Wait MPT SentRealBTComd!\r\n");
					/*
					if(m_MT8852Action.Compare("CrystalTrim") == 0)
						if(MessageBox("Retry?","CrystalTrim Error",MB_YESNO) == IDYES)
							AfxMessageBox("yes");
						else
							AfxMessageBox("no");
					else
						if(MessageBox("Retry?","RFTest Error",MB_YESNO) == IDYES)
							AfxMessageBox("yes");
						else
							AfxMessageBox("no");
					*/
#else
					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(TRUE);
					SetDlgItemText(IDC_BUTTON1 , "S4_Start");
					m_TestProcess = 0;
#endif
					//m_TestProcess = MPT_Start;	

					//m_Edt->ReplaceSel("Wait MPT Start command!\r\n");
				}
				else if(m_TestStation.Compare("T1") == 0 || m_TestStation.Compare("T2.1") == 0)
				{
					//m_Edt->ReplaceSel("Initial MT8852 fail!\r\n");
					KillTimer(AutorunTimer);
					
					if(MT8852_Handshake_flag == false)
					{
						TCHAR SetMT8852[] = "SYSCFG? EUTRS232\r\n";
						M8852EchoCmd_ptr ="SYSCFG? EUTRS232\r\n";
						MT8852_Op_str.Format(_T("SYSCFG EUTRS232,57600"));
						MT8852COM.WriteToPort(SetMT8852);
						m_Edt->ReplaceSel("Initial MT8852\r\n");
						Initial8852 = 0;
						SetTimer(Chk8852Timer , Chk8852Time , NULL);
					}
				}
				else if(m_TestStation.Compare("T0.1") == 0)
				{
					KillTimer(AutorunTimer);

					pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
					pBtn->EnableWindow(TRUE);
					pBtn = (CButton*)GetDlgItem(IDC_RETEST);
					pBtn->EnableWindow(FALSE);

					SetDlgItemText(IDC_BUTTON1 , "Tag_Verify");

				}
			}
			break;
		case M8852INStimer:	
			KillTimer(M8852INStimer);
			timeCount = 0;
			MT8852COM.WriteToPort(M8852EchoCmd_ptr);

#ifdef Debug_RS232_Output_Message
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel(M8852EchoCmd_ptr);//Debug
			m_Edt->ReplaceSel("..w\r\n");
#endif
			//if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 3)//CrystalTrim
			if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 1)
			{
				//m_Edt->ReplaceSel("Set CrystalTrim time-out timer\r\n");
				SetTimer(M8852EchoInstTimer,2000,NULL); 
				Query8852StatusCount = 8;
			}
			else if(m_MT8852Action.Compare("Run") == 0 && i_MT8852Script == 1)//Quick Test
			{
				SetTimer(M8852EchoInstTimer,3000,NULL);
				Query8852StatusCount = 15;
			}
			else if(m_MT8852Action.Compare("Run") == 0 && i_MT8852Script == 3)//Script Test
			{
				SetTimer(M8852EchoInstTimer,3000,NULL);
				Query8852StatusCount = 20;
			}
			else
			{
				SetTimer(M8852EchoInstTimer,M8852EchoInstTime,NULL); 
				Query8852StatusCount = 1;
			}
			break;
		case M8852EchoInstTimer:
			KillTimer(M8852EchoInstTimer);
			//if time out, show time out message and return		
			if ((timeCount > Query8852StatusCount) && m_M8852IsHandShakingFlag)
			{
				//AfxMessageBox("8852 handshaking timeout");
				//PostMessage(WM_CLOSE, IDD_TEST708CRYSTALTRIM_DIALOG,0); //close program
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("MT8852 handshaking timeout\r\n");
				//SetDlgItemText(ID_BTN_EXIT , "Exit");

#ifdef T1_Fail_Retry
				TCHAR SetMT8852[] = "DISCONNECT\r\n";
				MT8852COM.WriteToPort(SetMT8852);

				if(m_MT8852Action.Compare("CrystalTrim") == 0)
				{
					if(MessageBox("Retry?","CrystalTrim Error",MB_YESNO) == IDYES)
					{
						//AfxMessageBox("yes");
						
						m_Edt->ReplaceSel("*****CrystalTrim fail,retry*****\r\n");

						m_TestProcess = T1RFTest;

						m_MT8852Action = "CrystalTrim"; 
						i_MT8852Script = 1;
						CrystalTrimTest();
					}
					//else
					//	AfxMessageBox("no");
				}
				else
				{
					if(MessageBox("Retry?","RFTest Error",MB_YESNO) == IDYES)
					{
						//AfxMessageBox("yes");

						m_Edt->ReplaceSel("*****RF fail,retry*****\r\n");

						run_step = 1;

						m_MT8852Action = "Run";

						//QuickTest
						i_MT8852Script = 1;

						QuickTest();
					}
					//else
					//	AfxMessageBox("no");
				}
#else
				SetDlgItemText(ID_BTN_EXIT , "Exit");
#endif
				return;
			}
			else if (timeCount > 30)
			{		
				AfxMessageBox("8852 connect timeout");
				//enableAllButton(1);
				SetDlgItemText(ID_BTN_EXIT , "Exit");
				break;
			}
			++timeCount;

			MT8852COM.WriteToPort(M8852EchoCmd_ptr);
#ifdef Debug_RS232_Output_Message
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel(M8852EchoCmd_ptr);//Debug
			m_Str.Format("timeCount = %d\r\n",timeCount);
			m_Edt->ReplaceSel(m_Str + "..w\r\n");
#endif
			//SetTimer(M8852EchoInstTimer,M8852EchoInstTimer,NULL); 
			
			//if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 3)
			if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 1)
			{
				SetTimer(M8852EchoInstTimer,2000,NULL); 
				Query8852StatusCount = 8;
			}
			else if(m_MT8852Action.Compare("Run") == 0 && i_MT8852Script == 1)
			{
				SetTimer(M8852EchoInstTimer,3000,NULL);
				Query8852StatusCount = 15;
			}
			else if(m_MT8852Action.Compare("Run") == 0 && i_MT8852Script == 3)//Script Test
			{
				SetTimer(M8852EchoInstTimer,3000,NULL);
				Query8852StatusCount = 20;
			}
			else
			{
				SetTimer(M8852EchoInstTimer,M8852EchoInstTime,NULL); 
				Query8852StatusCount = 1;
			}
			break;
		case SingleICRunTimer:
			KillTimer(SingleICRunTimer);
			SetEUT_ICSingleTestTo8852();
			break;
		case Run8852delay:
			KillTimer(Run8852delay);
			SetEUT_RunTestTo8852();
			break;
		case RunStep3Timer:
			KillTimer(RunStep3Timer);
			//Stc_TestResult.SetBitmap(hOFF);
			//m_ListBox.ResetContent();
			//m_DUT_TestResult.ResetContent();

			run_step = 2;
			SetEUT_BDaddressTo8852();
			break;
		case DummyBTimer:
			//GenerateDummyBT();
			break;
		case PCAbortTimer://0xf0 0x03 0x61 0x64 
			KillTimer(PCAbortTimer);
			m_Edt->ReplaceSel("PC Abort!\r\n");
			buff[0] = 0xf0;
			buff[1] = 0x03;
			buff[2] = PCMPT_Abort;
			buff[3] = 0x03+PCMPT_Abort;									
			MPTCOM.WriteBufferToPort(buff , 4);
			if(m_TestStation.Compare("T3") != 0)
				CloseTestEngine();
			break;
		//*********************************************************************PC Send Ack to MPT
		case AckTimer:
			KillTimer(AckTimer);
			buff[0] = 0xf0;
			buff[1] = 0x03;
			buff[2] = PCMPT_ACK;
			buff[3] = 0x03+PCMPT_ACK;									
			MPTCOM.WriteBufferToPort(buff , 4);

			if(m_TestStation.Compare("T1") == 0)
			{
				if(m_TestProcess == MPT_Start)
				{
					m_TestProcess = ConnectReqAck;
					#ifdef MPT_Handshake_Message
						m_Edt->ReplaceSel("Rec Start,Send Ack to MPT\r\n");
					#endif
				}
				else if(m_TestProcess == ConnectReqAck)
				{
					m_TestProcess = T1RFTest;
					#ifdef MPT_Handshake_Message
						m_Edt->ReplaceSel("Rec ConnectReq,Send Ack to MPT\r\n");
						//m_Edt->ReplaceSel("Rf Test\r\n");
					#endif

#ifdef MPT_Process_debug
					#ifdef MPT_Handshake_Message
						m_Edt->ReplaceSel("Debug,skip RF test\r\n");
					#endif
					Sleep(1000);
					//F0 03 50 53 ==>PC_RFEnd
					buff[0] = 0xf0;
					buff[1] = 0x03;

					buff[2] = PC_RFEnd;
					buff[3] = 0x03+PC_RFEnd;

					MPTCOM.WriteBufferToPort(buff , 4);

					m_TestProcess = PCRFTestEnd;		
#else
					m_MT8852Action = "CrystalTrim"; 
					i_MT8852Script = 1;
					CrystalTrimTest();
#endif
					//MT8852Script3Test();
				}
				//SetTimer(T13Timer1 , T13Time1 , NULL);
			}
			else if(m_TestStation.Compare("T2") == 0 || m_TestStation.Compare("T3") == 0)
			{
				if(m_TestProcess == MPT_Start)
				{
					m_TestProcess = MPT_SentRealBAComd;
	
					#ifdef MPT_Handshake_Message
						m_Edt->ReplaceSel("Rec Start,Send Ack to MPT\r\n");
					#endif
				}
				else if(m_TestProcess == MPT_SentRealBAData || m_TestProcess == Audio_Linein)
				{
					#ifdef MPT_Handshake_Message
						m_Edt->ReplaceSel("Rec RealBTData,Send Ack to MPT\r\n");
					#endif

					if(m_PromptMessage)
					{
						if(ToggleBitmap)
						{
							ToggleBitmap = false;
							pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
						}
						else
						{
							ToggleBitmap = true;
							pSplash->SetBitmap(IDB_SPLASH,255,0,255);
						}

						//SplashScreenString = "將3.5mm接頭插入待測物,檢查是否聽到聲音,OK->綠色鍵;NG->紅色鍵";
						//SplashScreenString = "按壓BT鈕3秒,檢藍色LED是否有亮,之後將3.5mm接頭插入待測物,檢查是否聽到聲音,OK->綠色鍵;NG->紅色鍵";
						SplashScreenString = "按壓BT鈕3秒,並確認是否有藍色閃爍燈,之後將3.5mm接頭插入待測物,\r\n檢查是否聽到聲音,OK->綠色鍵;NG->紅色鍵";
						pSplash->SetText(SplashScreenString);
					}

					//if(m_TestStation.Compare("T2") == 0)
					//	SetTimer(RunBTA2DPTimer , 100 , NULL);

					//if(m_TestStation.Compare("T3") == 0)
					//	SetTimer(RunBTA2DPTimer , 100 , NULL);
					//else
					//	SetTimer(RunBTA2DPTimer , RunBTA2DPTime , NULL);//Run btapp.exe
				}
				else if(m_TestProcess == RunBTA2DP)
				{
					if(m_PromptMessage)
					{
					SplashScreenString = "(Audio Line-in test)檢查待測物是否有發出聲音,OK->綠色鍵;NG->紅色鍵";
					if(ToggleBitmap)
					{
						ToggleBitmap = false;
						pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
					}
					else
					{
						ToggleBitmap = true;
						pSplash->SetBitmap(IDB_SPLASH,255,0,255);
					}
					pSplash->SetText(SplashScreenString);
					}
				}
				else if(m_TestProcess == SetWndPos)
				{
					SetWindowPos(0 , 600 , 570 , 500 , 330 , SWP_SHOWWINDOW);
					
					SetDlgItemText(IDC_PASSFAIL , "PASS");

					//m_TestProcess = MPT_SentRealBAComd;

					//m_Edt->ReplaceSel("Wait MPT SentRealBTComd!\r\n");

					//Wait Soundcheck data
					m_TestProcess = T3Record;

					KillTimer(WaitSoundcheckTimer);

					//
					T3_file = GetINIData("T3" , "RenameFile");

					SetTimer(WaitSoundcheckTimer , WaitSoundcheckTime , NULL);
				}
			}
			else if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("T3") == 0)
			{
				m_TestProcess = MPT_SentRealBAComd;
				TestErrorCode++;
				SetTimer(T13Timer1 , T13Time1 , NULL);
			}
			break;
		//*********************************************************************T1.3
		case T13Timer1:
			KillTimer(T13Timer1);	
			if(m_TestProcess == MPT_SentRealBAComd)
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("MPT Send real BA Command Timeout!!\r\n");
				SetDlgItemText(IDC_RETEST , "Re-Test");

				m_TestItemStr = "MPT_SentRealBA Timeout";
				m_ResultStr = "FAIL";
				TestErrorCode++;
				m_Str.Format("%X",TestErrorCode);
				m_ErrorCodeStr = m_Str;
				AddListCtrlItem(m_List , ListCtrlIndex);
				ListCtrlIndex++;

				SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

				rgbText = RGB_RED;
				rgbBkgnd = RGB_GRAY1;
				m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
				m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

				SetTimer(PCAbortTimer , PCAbortTime , NULL);

				if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("T3") == 0)
					m_TestProcess = MPT_Start;
				else if(m_TestStation.Compare("S4") == 0)
					m_TestProcess = MPT_SentRealBAComd;
			}
			break;
		case T13WriteRealBT:
			KillTimer(T13WriteRealBT);
			T13WriteBT();
			break;
		case T13RecRealBTData:
			KillTimer(T13RecRealBTData);
			if(m_TestProcess == MPT_SentRealBAData)
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("MPT Send real BA Data Timeout!!\r\n");

				SetDlgItemText(IDC_RETEST , "Re-Test");
				SetDlgItemText(ID_BTN_EXIT , "Exit");

				m_TestItemStr = "Get Real BT timeout";
				m_ResultStr = "FAIL";
				TestErrorCode++;
				m_Str.Format("%X",TestErrorCode);
				m_ErrorCodeStr = m_Str;
				AddListCtrlItem(m_List , ListCtrlIndex);
				ListCtrlIndex++;

				SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

				rgbText = RGB_RED;
				rgbBkgnd = RGB_GRAY1;
				m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
				m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

				SetTimer(PCAbortTimer , PCAbortTime , NULL);

				if(m_TestStation.Compare("T1.3") == 0)
					m_TestProcess = MPT_Start;
			}
			break;
		//*********************************************************************S4
		case S4RecordTimer:
			KillTimer(S4RecordTimer);
			S4RecordSN();
			break;
		//*********************************************************************T2.0
		case RunBTA2DPTimer:
			KillTimer(RunBTA2DPTimer);
			m_Edt->ReplaceSel("Run BTA2DP\r\n");
			//AfxMessageBox("Press BT button at least 3 sec\r\n");
								
			m_Str = GetINIData("BTApp" , "Status");

			//TestErrorCode = 0xF000;

			m_TestProcess = 0;

			if(m_Str.Compare("Run") == 0)
			{
				if(m_TestStation.Compare("T2") == 0)
				{
					if(m_PromptMessage)
					{
					if(ToggleBitmap)
					{
						ToggleBitmap = false;
						pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
					}
					else
					{
						ToggleBitmap = true;
						pSplash->SetBitmap(IDB_SPLASH,255,0,255);
					}

					SplashScreenString = "等待藍芽連線(大約30秒)...,檢查待測物是否有發出聲音,OK->綠色鍵;NG->紅色鍵";
					pSplash->SetText(SplashScreenString);
					}
				}
#ifdef Support_Bluesoleil_SDK
				/*
				m_Str = GetINIData("BTApp" , "Message");

				if(m_Str.Compare("") == 0)
				{	
					m_Edt->ReplaceSel("BT A2DP Test!\r\n");

					m_ThreadParam.m_hwnd = this->m_hWnd;
					m_ThreadParam.phase = 0;
					m_pLptThrd = AfxBeginThread(BTThreadfun , (LPVOID)&m_ThreadParam);//Create thread to monitor debug message 
						
					//m_Edt->ReplaceSel("Connect..\r\n");
				}
				else
				{
					m_Edt->ReplaceSel("Read ini,BTApp status : ");
					m_Edt->ReplaceSel( m_Str + "\r\n");
				}
				*/
#endif
			}
			break;
		case SplashScreenShow:
			KillTimer(SplashScreenShow);
			if(m_PromptMessage)
			{
			if(ShowPic == false)
			{
				ShowPic = true;
				//ToggleBitmap = true;

				//ShowWindow(SW_MINIMIZE);
				//pSplash=new CSplashScreenEx();
				//pSplash->Create(this,NULL,0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
				//pSplash->SetBitmap("lineout.bmp",255,0,255);
				//pSplash->Show();

				//pSplash->SetTextFont("Impact",100,CSS_TEXT_NORMAL);
				//pSplash->SetTextRect(CRect(125,60,291,104));
				//pSplash->SetTextColor(RGB(255,255,255));
				//pSplash->SetTextColor(RGB(0,0,255));
				//pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				//m_Edt->ReplaceSel("&&\r\n");

				pSplash->SetText(SplashScreenString);			
				}
			}
			break;
		case SplashScreenHide:
			KillTimer(SplashScreenHide);
			if(m_PromptMessage)
			{
			if(ShowPic)
			{
				ShowPic = false;
				pSplash->Hide();
				}
			}
			break;
		//*********************************************************************T1.0
		case WriteDummyBDTimer:
			KillTimer(WriteDummyBDTimer);
			m_Edt->ReplaceSel("Write dummy BT\r\n");
			T1WriteDummyBT();
			break;
		case ProcessEndTimer:
			KillTimer(ProcessEndTimer);
			ProcessEnd();
			break;
		case PIOEndTimer:
			KillTimer(PIOEndTimer);
			buff[0] = 0xf0;
			buff[1] = 0x03;
			buff[2] = PC_PIOEnd;
			buff[3] = 0x03+PC_PIOEnd;									
			MPTCOM.WriteBufferToPort(buff , 4);
			break;
		//*********************************************************************T3
		case WaitSoundcheckTimer:
			if(findfile.FindFile(T3_file , 0))
			{
				//KillTimer(WaitSoundcheckTimer);
				//AfxMessageBox("Find soundcheck file\r\n");
				int len;
				len = T3_file.GetLength();
				m_Str = T3_file.Left(len-13);
				//m_Str = "BHC302_" + MPT_RealBT;
				m_Str += "BHC302_";
				m_Str += MPT_RealBT;
				m_Str += ".txt";
				//T3 debug
				m_Edt->ReplaceSel(m_Str + "\r\n");

				if(findfile.FindFile(m_Str , 0))
				{
					DeleteFile(m_Str);
					m_Edt->ReplaceSel("DeleteFile " + m_Str);
					m_Edt->ReplaceSel("\r\n");
				}

				rename(T3_file , m_Str);
				m_Edt->ReplaceSel("Old :" + T3_file);
				m_Edt->ReplaceSel("\r\n");
				m_Edt->ReplaceSel("New :" + m_Str);
				m_Edt->ReplaceSel("\r\n");

				m_TestProcess = MPT_SentRealBAComd;
			}
			break;
	}
  
  //if(nIDEvent == 1)
  //{
	//if(m_Initial)
	//{
		/*
		InitialGUI();
		m_Initial = FALSE;

		//MyEncryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");

		m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
		m_Str = m_IniReader.getKeyValue( "UUTMode" , "DEBUG");

		//MyDecryptFile("D:\\USINOTE\\TAURUS.INI" , "SomePassKey");

		if(m_Str.Compare("Disable") == 0)
		{
			CBarcode dlg(this);

			int k = dlg.DoModal();

			if(k == IDOK)
			{
				m_Barcode = dlg.m_Bar;
				m_Maccode = dlg.m_Mac;

				SetDlgItemText(IDC_STATIC1 , "BARCODE : " + m_Barcode);
				SetDlgItemText(IDC_STATIC2 , "MACCODE : " + m_Maccode);

    			::SetCursorPos(270,710);

				ShowWindow(SW_SHOWMAXIMIZED);
            
				m_Password = 0 ;

				KillTimer(1);

				SetEnv();						// Noted!! Update Barcode/Mac to INI file
			}
			else if(k == IDCANCEL)
			{
				KillTimer(1);

				CDialog::OnCancel();
			}
		}
		else
		{
			SetDlgItemText(IDC_STATIC1 , "BARCODE : ");
			SetDlgItemText(IDC_STATIC2 , "MACCODE : ");
			ShowWindow(SW_SHOWMAXIMIZED);
			           
			m_Password = 0 ;

			SetEnv();// Noted!! Update Barcode/Mac to INI file

			SetWindowPos(0 , 0 , 0 , 150 , 140 , SWP_SHOWWINDOW);

            OpenSixUUT();

			KillTimer(1);

			SetTimer(2, 3500, 0);

			CFile FileCtrl2;
			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
			FileCtrl2.Open("D:\\USINOTE\\TAURUS.INI", CFile::modeReadWrite | CFile::shareDenyNone );
		}
		*/
	//}
  //}
  //else if(nIDEvent == 2)  
  //{
	    //OpenSixUUT();
  //}
  
  CDialog::OnTimer(nIDEvent);
}

//*************//
//  For TAURUS  //
//*************//

  /*
  else if(nIDEvent == 2)  
  {
		//DIG_IN_Prt(1, 9, &iPattern);
        
		if(iPattern == 0x99)	// Press y key to stop net when UUT Lan Connect FAIL!
		{
			DisablePost();
            Sendkey(70);
            Sendkey(81);
			SWtoHOST();
   		}
		if(findfile.FindFile("d:\\usinote\\chk2.dat" , 0))	// Press y key to stop net when UUT Lan Connect FAIL!
		{
			SetTimer(4, 1000, 0);
			DeleteFile("d:\\usinote\\chk2.dat");
		}
	    if(findfile.FindFile("d:\\usinote\\key.dat" , 0))
		{
			DeleteFile("d:\\usinote\\key.dat");
			
			Sendkeymatrix();
		}
		if(findfile.FindFile("d:\\usinote\\testlog.dat" , 0))
		{
			DeleteFile("d:\\usinote\\testlog.dat");
			TestLog();
		}
		else if(findfile.FindFile("d:\\usinote\\allpass.dat" , 0))
		{
			DeleteFile("d:\\usinote\\allpass.dat");
			OnAllPASS();
		}
		else if(findfile.FindFile("d:\\usinote\\log.dat" , 0))
		{
        	m_Str = "d:\\usinote\\log.dat";

            if(findfile.FindFile(("d:\\usinote\\log\\" + m_Barcode + ".log")))
			{
				DeleteFile(("d:\\usinote\\log\\" + m_Barcode + ".log"));
			}
			else
			{
				rename(m_Str , ("d:\\usinote\\log\\" + m_Barcode + ".log")); 
				DeleteFile(m_Str);
			}
		}
		else if(findfile.FindFile("d:\\usinote\\ac.off" , 0))
		{
			DeleteFile("d:\\usinote\\ac.off");
            AC_Off();
		}
        else if(findfile.FindFile("d:\\usinote\\ac.on" , 0))
		{
			DeleteFile("d:\\usinote\\ac.on");
            AC_On();
		}
        else if(findfile.FindFile("d:\\usinote\\batt.on" , 0))
		{
			DeleteFile("d:\\usinote\\batt.on");
			SetGPIB(11 , 4.5);
            EnableBattery();
            EnableSMBUS();
		}
        else if(findfile.FindFile("d:\\usinote\\batt.off" , 0))
		{
			SetGPIB(0 , 0);
            DisableBattery();
            DisableSMBUS();
			DeleteFile("d:\\usinote\\batt.off");
		}
		else if(findfile.FindFile("d:\\usinote\\skip3020.dat" , 0))
		{
			DeleteFile("d:\\usinote\\skip3020.dat");
		}
		else if(findfile.FindFile("d:\\usinote\\led.on" , 0))
		{
			DeleteFile("d:\\usinote\\led.on");
			TurnOnHDD();
		}
		else if(findfile.FindFile("d:\\usinote\\led.off" , 0))
		{
			DeleteFile("d:\\usinote\\led.off");
			TurnOffHDD();
		}
		else if(findfile.FindFile("d:\\usinote\\upt2.on" , 0))
		{
			DeleteFile("d:\\usinote\\upt2.on");

			if(findfile.FindFile("d:\\usinote\\testctrl\\R3010.tst" , 0))
                USBOn();
		}
		else if(findfile.FindFile("d:\\usinote\\upt2.off" , 0))
		{
			DeleteFile("d:\\usinote\\upt2.off");
            USBOff();
		}
		else if(findfile.FindFile("d:\\usinote\\crt.on" , 0))		//Switch to UUT		  
		{
			DeleteFile("d:\\usinote\\crt.on");
            SWtoUUT();
		}
        else if(findfile.FindFile("d:\\usinote\\crt.off" , 0))		//Switch to HOST		  
		{
			DeleteFile("d:\\usinote\\crt.off");
            SWtoHOST();
		}
        else if(findfile.FindFile("d:\\usinote\\chk1.dat" , 0))		  
		{
            SetTimer(3, 4000, 0);
			m_Time1 = 0;
			DeleteFile("d:\\usinote\\chk1.dat");
		}
  }
  else if(nIDEvent == 3)
  {
	  if(findfile.FindFile("d:\\usinote\\testctrl\\R3003.tst" , 0))
	  {
	    m_Str = GetINIData("DEBUG" , "SuspendCurrHigh");
		high = strtod(m_Str , 0);
		m_Str = GetINIData("DEBUG" , "SuspendCurrLow");
		low = strtod(m_Str , 0);

		current = RdGPIBCurrent();

		if((current < high ) && (current > low))		// Check2
		{
			if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite)))
				MessageBox("File Open Error!"  , "" , MB_OK);

			FileCtrl.SeekToEnd();

			CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
			m_Str.Format("\r\nSuspend Charge Current Test PASS!  %0000fA\r\n" , current);
	  				
			FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			DispString(m_Str);
			DispXList(11 , 1);
			pProgess->SetPos(12);

			FileCtrl.Close();
			SetDlgItemText(IDC_DSPMESS , "");
			SetDlgItemText(IDC_DSPMESS , "Press Lid-Switch");
			KillTimer(3);
		}
		else
		{
			if(m_Time1 > 1)
			{
				if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite)))
					MessageBox("File Open Error!"  , "" , MB_OK);

				FileCtrl.SeekToEnd();

				CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
				m_Str.Format("\r\nSuspend Charge Current Test PASS!  %0000fA\r\n" , current);
	  				
				FileCtrl.Write(m_Str , m_Str.GetLength()+1);

				DispString(m_Str);
				DispXList(11 , 0);
				pProgess->SetPos(12);

				FileCtrl.Close();
				SetDlgItemText(IDC_DSPMESS , "");
				SetDlgItemText(IDC_DSPMESS , "Press Lid-Switch");
				KillTimer(3);
			}
			m_Time1++;
		}
	  }
	  else{
			KillTimer(3);
	  }
  }
  else if(nIDEvent == 4)
  {
		if(findfile.FindFile("d:\\usinote\\testctrl\\R2004.tst" , 0))
		{
			CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
            
			if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite)))
				MessageBox("File Open Error!"  , "" , MB_OK);

			FileCtrl.SeekToEnd();

			m_Str = GetINIData("DEBUG" , "SysOnQuickChargeHigh");
			high = strtod(m_Str , 0);
			m_Str = GetINIData("DEBUG" , "SysOnQuickChargeLow");
			low = strtod(m_Str , 0);

			if(ChkRange(high, low))
			{
				m_Str.Format("\r\nQuick Charge Current Test PASS(System ON)!  %0000fA\r\n" , current);
	  				
				FileCtrl.Write(m_Str , m_Str.GetLength()+1);

				DispString(m_Str);
				DispXList(5 , 1);
				pProgess->SetPos(6);
				FileCtrl.Close();
				KillTimer(4);
			}
			else
			{
				m_Str.Format("\r\nQuick Charge Current Test FAIL(System ON)!  %0000fA\r\n", current);
	  				
				FileCtrl.Write(m_Str , m_Str.GetLength()+1);

				DispString(m_Str);
				DispXList(5 , 0);
				pProgess->SetPos(6);
                FileCtrl.Close();
				KillTimer(4);
			}
		}
		else
				KillTimer(4);
  }
  */

BOOL CDIODlg::IniDIO96()
{
    // Define DIO96 port Input/Output  Configuration//

    //status = DIG_Prt_Config (deviceNumber, port, mode, dir)  dir: 1(Outport) 

    //status = //DIG_OUT_Prt (deviceNumber, port, pattern)

    m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
    
	if((m_Str.Compare("TAURUS") == 0 ))
	{	
/*
		DIG_Prt_Config(1 , 0 , 0 , 1);   //Function Board & Power Board Ctrl

		DIG_Prt_Config(1 , 1 , 0 , 1);   //CRT Switch & Audio Switch

		DIG_Prt_Config(1 , 2 , 0 , 1);   //Set Mode (PostCode)

		DIG_Prt_Config(1 , 11 , 0 , 1);  //Power-on & Lid-Switch Control

		DIG_Prt_Config(1 , 7 , 0 , 1);   //Keyboard Matrix

		DIG_Prt_Config(1 , 9 , 0 , 0);   //Read Fixture ID & PostCode (Read port)

		DIG_Prt_Config(1 , 8 , 0 , 1);   //RJ11

		DIG_Prt_Config(1 , 10 , 0 , 0);  

		//DIG_OUT_Prt(1, 0, 252);  

		//DIG_OUT_Prt(1, 1, 255);

		//DIG_OUT_Prt(1, 11, 255);

		//DIG_OUT_Prt(1, 2, 255);  

		//DIG_OUT_Prt(1, 8, 255);  
*/
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{
/*
		DIG_Prt_Config(1 , 5 , 0 , 1);   //DIO96 Port I/O enable(Act it first for )

		DIG_Prt_Config(1 , 0 , 0 , 1);   //TP-SMPL3 Power Ctrl

		DIG_Prt_Config(1 , 1 , 0 , 1);   //TP-SMPL3 Power Ctrl

		DIG_Prt_Config(1 , 3 , 0 , 1);   //Switch AC/DC to Fixture or UUT

		DIG_Prt_Config(1 , 9 , 0 , 0);   //Read Fixture ID / VCC5M Detection

		DIG_Prt_Config(1 , 10 , 0 , 1);  //UUT Pwr Switch  ON/OFF( to FukuRelay)

		DIG_Prt_Config(1 , 11 , 0 , 1);  //EXT IO /Keyboard scan/VCC 5M status enable

		DIG_Prt_Config(1 , 7 , 0 , 1);   //Keyboard Matrix

		DIG_Prt_Config(1 , 4 , 0 , 1);   //CRT Switch

		DIG_Prt_Config(1 , 2 , 0 , 1);   

		//DIG_OUT_Prt(1, 5, 253);						// Enable -DIO_EN  signal

		//DIG_OUT_Prt(1, 0, 0);						// Enable -DRV_K1  signal

		//DIG_OUT_Prt(1, 1, 15);						// Enable -DRV_K1  signal ( Provide TTL_5V for each IC )

		//DIG_OUT_Prt(1, 3, 255);						// Diable -DRV_K2,3  signal ( UUT AC On/Off control )

		//DIG_OUT_Prt(1, 11,255);  

		//DIG_OUT_Prt(1, 4, 255); 
		
		//DIG_OUT_Prt(1, 2, 112); 
*/
	}
	
	return TRUE;
}
//  IBM Project
//  port 2 : in    
//  port 2 , bit 0  :APA0 -> 0
//  port 1 , bit 1  :APB1 -> 0
//  port 5 , bit 0  :BPC0 -> 1
//  port 5 , bit 1  :BPC1 -> 0

BOOL CDIODlg::PMTest()
{
	double high = 0;
	double low = 0;

	SetDlgItemText(IDC_DSPMESS , "");

    CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
			 
	CProgressCtrl *pProgess = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);

	if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite )))
         MessageBox("File Open Error!"  , "" , MB_OK);

    FileCtrl.SeekToEnd();

	m_Str = "***********************************************************\r\n";
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	SetDlgItemText(IDC_DSPMESS , "Battery Charge Test");

    // ********  NB-TAURUS Battery Test ********

	//*******************************
    // Wake up Charge Current Test  *
	//*******************************

	if(!(findfile.FindFile("d:\\usinote\\testctrl\\R3021.tst" , 0)))
	{
		AC_On();
		return TRUE;
	}
	else
	{
		AC_On();

        Sleep(3000);

		SetGPIB(8.9 , 1);

        Sleep(3000);

		EnableSMBUS();

		Sleep(3000);

		EnableBattery();

		m_Str = GetINIData("DEBUG" , "WakeUpChargeHigh");
		high = strtod(m_Str , 0);
		m_Str = GetINIData("DEBUG" , "WakeUpChargeLow");
		low = strtod(m_Str , 0);

		if(ChkRange(high, low))
		{
			m_Str.Format("Wake Up Charge Current Test PASS!  %0000fA\r\n" , current);

		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	  		DispString(m_Str);
			DispXList(27 , 1);
			pProgess->SetPos(28);

		}
		else
		{
			m_Str.Format("Wake Up Charge Current Test FAIL!  %0000fA\r\n" , current);

		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	  		DispString(m_Str);	
			DispXList(27 , 0);
			pProgess->SetPos(28);

			//return FALSE;
		}
	}

	//**********************************************
    // Quick Charge Current Test (System-OFF)      *
	//**********************************************

	if(!(findfile.FindFile("d:\\usinote\\testctrl\\R3022.tst" , 0)))
	{
		AC_On();
	    return TRUE;
	}
	else
	{
		AC_Off();
		DisableSMBUS();
		DisableBattery();
		Sleep(2000);

		SetGPIB(10 , 4.5);
	
		EnableBattery();
    
		Sleep(500);
		AC_On();
		EnableSMBUS();

		m_Str = GetINIData("DEBUG" , "SysOffQuickChargeHigh");
		high = strtod(m_Str , 0);
		m_Str = GetINIData("DEBUG" , "SysOffQuickChargeLow");
		low = strtod(m_Str , 0);

		if(ChkRange(high, low))
		{
			m_Str.Format("Quick Charge Current Test PASS(System OFF)!  %0000fA\r\n" , current);
	  		
		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			DispString(m_Str);
			DispXList(28 , 1);
			pProgess->SetPos(29);
		}
		else
		{
			m_Str.Format("Quick Charge Current Test FAIL(System OFF)!  %0000fA\r\n" , current);
	  		
		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			DispString(m_Str);
			DispXList(28 , 0);
			pProgess->SetPos(29);

			//return FALSE;
		}
	}

	//**********************************************
    // Charge Stop Current Test (System-OFF)       *
	//**********************************************

	if(!(findfile.FindFile("d:\\usinote\\testctrl\\R3023.tst" , 0)))
	{
		AC_On();
        return TRUE;
	}
	else
	{
		AC_Off();
		DisableSMBUS();
		DisableBattery();
		Sleep(2000);

		SetGPIB(12.6 , 1.0);
	
		EnableBattery();
    
		Sleep(500);
		AC_On();
		EnableSMBUS();

  		m_Str = GetINIData("DEBUG" , "ChargeStopCurrent");
		high = strtod(m_Str , 0);

  		if(ChkRange(high, 0))
		{
			m_Str.Format("Charge Stop Current Test PASS!  %0000fA\r\n" , current);
	  			  		
		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			DispString(m_Str);
			DispXList(29 , 1);
			pProgess->SetPos(30);
		}
		else
		{
			m_Str.Format("Charge Stop Current Test FAIL!  %0000fA" , current);
	  			  		
		    FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			DispString(m_Str);
			DispXList(29 , 0);
			pProgess->SetPos(30);

			//return FALSE;
		}
		   
		m_Str = "***********************************************************\r\n";
		FileCtrl.Write(m_Str , m_Str.GetLength()+1);
	}
	
	SetGPIB(0, 0);					
	DisableSMBUS();
	DisableBattery();

    FileCtrl.Close();

    //SetDlgItemText(IDC_DSPMESS , "Running Testing!  ");

	Sleep(2000);

    return TRUE;
}
/*   // IBM Battery Test Procedure
 
    ////////////////////////Battery Power Management Test for IBM(Rome-1)////////////////

	//*******************************
	// Off Leak Current Test        *
    //*******************************

    //Disable AC power

    m_Edt->ReplaceSel("Off Leak Current Test.....");

    current = SetGPIB(12 , 3.5);

	if(current < 0 )
		current = current * (-1);

    if(current  < 0.005)  
        m_Edt->ReplaceSel("PASS!\r\n");
	else
	{
		m_Edt->ReplaceSel("FAIL!\r\n");
		return FALSE;
	}

	Sleep(1500);

	//***********************************
    // Trickle Charge Current Test  (5V)*
	//***********************************

    //Enable AC power

    // Disable Battery (SMBUS)

    m_Edt->ReplaceSel("Trickle Charge Current Test.....");

    current = SetGPIB(5 , 3.5);

    if(current < 0)
		current = current * (-1);
	else
        return FALSE; 

    if((current > (0.07)) && (current  < 0.2))  
        m_Edt->ReplaceSel("PASS!\r\n");
	else
	{
		m_Edt->ReplaceSel("FAIL!\r\n");
	    return FALSE;
	}
	Sleep(1500);

    //*******************************
    // Trickle Charge Current Test  (10V)*
	//*******************************

    // Disable Battery (SMBUS)

    m_Edt->ReplaceSel("Trickle Charge Current Test.....");

    current = SetGPIB(10 , 3.5);

    if(current < 0)
		current = current * (-1);
	else
        return FALSE; 

    if((current > (0.07)) && (current  < 0.2))  
        m_Edt->ReplaceSel("PASS!\r\n");
	else
	{
		m_Edt->ReplaceSel("FAIL!\r\n");
	    return FALSE;
	}

	Sleep(1500);
	
	//*******************************
    // Quick Charge Current Test    *
	//*******************************

    // Enbale Battery (SMBUS)

    m_Edt->ReplaceSel("Trickle Charge Current Test.....");

    current = SetGPIB(9 , 3.5);

    if(current < 0)
		current = current * (-1);
	else
        return FALSE; 

    if((current > (2.0)) && (current  < 2.9))  
        m_Edt->ReplaceSel("PASS!\r\n");
	else
	{
		m_Edt->ReplaceSel("FAIL!\r\n");
	    return FALSE;
	}

	Sleep(1500);
	
	//*******************************
    // Charge Stop Test             *
	//*******************************

    //Enable AC power

    // Disable Battery (SMBUS)

    m_Edt->ReplaceSel("Trickle Charge Current Test.....");

    current = SetGPIB(12.9 , 3.5);

    if(current < 0)
		if(current > -0.01)
            m_Edt->ReplaceSel("PASS!\r\n");
		else
		{
		    m_Edt->ReplaceSel("FAIL!\r\n");
	        return FALSE;
		}
	else
	{
        if(current < 0.04)
            m_Edt->ReplaceSel("PASS!\r\n");
		else
		{
		    m_Edt->ReplaceSel("FAIL!\r\n");
	        return FALSE;
		}
	}
 
	Sleep(1500);
	//////////////////////// End of PMTEST/////////////////////////////////
*/

void CDIODlg::AC_On()
{
	m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
    
	if((m_Str.Compare("TAURUS") == 0 ))
	{	
		// For TAURUS
		////DIG_OUT_Line(1, 0, 5 , 0 );   // AC-DC Power Control

		// Bit 5 : AC-DC On 
		// 0 : Enable 1: Disable
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{
		////DIG_OUT_Line(1, 3, 1 , 0 );	  
		//iStatus = //DIG_OUT_Line(1, 0, 5 , 0 );  
	}
}

void CDIODlg::AC_Off()
{
	m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
    
	if((m_Str.Compare("TAURUS") == 0 ))
	{	
		////DIG_OUT_Line(1, 0, 5 , 1 );   // AC-DC Power Control
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{
		////DIG_OUT_Line(1, 3, 1 , 1 );  
		//iStatus = //DIG_OUT_Line(1, 0, 5 , 1 ); 
	}
}

void CDIODlg::SWtoUUT()
{
	//iStatus = //DIG_OUT_Line(1, 4, 7 , 0 );

    //if(ChkDIOStatus())
	//	return;

	//			TAURUS
	//iStatus = //DIG_OUT_Line(1, 1, 1 , 0 );
    //if(ChkDIOStatus())
	//  return;

	//Port 1  , Bit 1 : CRT Switch Control
	// 0: Enable 1 : Disable
}

//  IBM TP-SMPL3 CRT Switch
//  Port4 bit7

void CDIODlg::SWtoHOST()
{
	//iStatus = //DIG_OUT_Line(1, 4, 7 , 1 );

    //if(ChkDIOStatus())
	//	return;

	//			TAURUS
	//iStatus = //DIG_OUT_Line(1, 1, 1 , 1 );
	//if(ChkDIOStatus())
	//  return;

	//Port 1  , Bit 1 : CRT Switch Control
	// 0: Enable 1 : Disable
}

void CDIODlg::SetEnv()
{
	/*
	m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file

	if((m_Str.Compare("TAURUS") == 0 ))
	{
        m_Str = m_IniReader.getKeyValue( "UUTMode" , "DEBUG");

		if(m_Str.Compare("Disable") == 0)
		{
			DeleteFile("d:\\usinote\\UUT.LOG");
			DeleteFile("d:\\usinote\\USIOWN.INI");

			Sleep(100);
			FileCtrl.Open(("d:\\usinote\\uut.log") , CFile::modeCreate);

			FileCtrl.Close();

			Sleep(100);
			FileCtrl.Open(("d:\\usinote\\USIOWN.INI") , CFile::modeCreate | CFile::modeReadWrite);

			m_Str = "[UUT]\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength());

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

			CStringList* myOtherList = m_IniReader.getSectionData("UUT");

			POSITION position;

			for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
			{
				m_Str = myOtherList->GetNext(position);
				m_Str+= "\r\n";
				FileCtrl.Write(m_Str , m_Str.GetLength());
			}

			FileCtrl.Close();
		}
		else
		{

			m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
			m_IniReader.setKey("","UUT1","UUTSTATE");
			m_IniReader.setKey("","UUT2","UUTSTATE");
			m_IniReader.setKey("","UUT3","UUTSTATE");
			m_IniReader.setKey("","UUT4","UUTSTATE");
			m_IniReader.setKey("","UUT5","UUTSTATE");
			m_IniReader.setKey("","UUT6","UUTSTATE");
		}
	}
	*/

	//****************************************//
	//	SFIF system and Host version control  //
	//****************************************//
	/*   
	m_Str = GetINIData("SFIS" , "IP"); 
	m_szIP = m_Str;
	m_Str = GetINIData("SFIS" , "PORT");
	m_szPort = m_Str;

	m_IniReader.setKey("" , "UUT1BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT1MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT1GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT1RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT1TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT1STATE" , "BARMAC");
	m_IniReader.setKey("" , "UUT2BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT2MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT2GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT2RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT2TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT2STATE" , "BARMAC");
	m_IniReader.setKey("" , "UUT3BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT3MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT3GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT3RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT3TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT3STATE" , "BARMAC");
	m_IniReader.setKey("" , "UUT4BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT4MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT4GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT4RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT4TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT4STATE" , "BARMAC");
	m_IniReader.setKey("" , "UUT5BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT5MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT5GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT5RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT5TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT5STATE" , "BARMAC");
	m_IniReader.setKey("" , "UUT6BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT6MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT6GUID" , "BARMAC");
	m_IniReader.setKey("" , "UUT6RESULT" , "BARMAC");
	m_IniReader.setKey("" , "UUT6TESTID" , "BARMAC");
	m_IniReader.setKey("" , "UUT6STATE" , "BARMAC");

	m_IniReader.setKey("","RESULT","SFIS");

	m_LptBaseIO = GetINIData("LPT" , "Baseio"); 
    unsigned long IO = 0 ; 
	IO = _tcstoul(m_LptBaseIO, 0, 10);
	LPTBaseIO = short(IO);

	//m_Str = GetINIData("PROJECT" , "Version"); 

	//m_Str = m_Str + "USI HostCtrl";
	//AfxGetApp()->m_pMainWnd->SetWindowText(m_Str);
	*/
}

void CDIODlg::KbdCtrl(int i)
{
	//iStatus = //DIG_OUT_Line(1, 1, 0 , i );  // Keyboard simulation
	 //if(ChkDIOStatus())
	  //return;

	    //iStatus = //DIG_OUT_Prt(1, 7, 255);
    //iStatus = //DIG_OUT_Prt(1, 1, 255);

    // Bit 7 : Keyboard simulation  
    // 0 : Enable 1: Disable
}
	//  IBM Keyboard
    //iStatus = //DIG_OUT_Prt(1, 7, 255);
    //iStatus = //DIG_OUT_Prt(1, 1, 255);

void CDIODlg::DispString(CString &str)
{
    CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	m_Edt->SetDlgItemText(IDC_DSPMESS , "");
 	Sleep(500);
	m_Edt->SetDlgItemText(IDC_DSPMESS , str);
}

void CDIODlg::DispPost(CString &post)
{
    SetDlgItemText(IDC_POST , post);
}

void CDIODlg::OnDebug() 
{
	// TODO: Add your command handler code here
	CPassword dlg;
	int i = dlg.DoModal();

	if(i == IDOK){
       m_Password = dlg.m_Password;
	   if(m_Password == 12345){

	        CButton *m_Btn = (CButton*)GetDlgItem(IDC_SEND);
	        m_Btn->ShowWindow(SW_SHOW);
	   }
	   else if(m_Password == 12419)
	   {
			GetDlgItem(ID_BTN_EXIT)->EnableWindow(TRUE);
			DebugDlg dlg(this);
			dlg.DoModal();

			m_Data = dlg.m_SerialSend;
	   } 
	}
}

void CDIODlg::InitialGUI()
{
	//rgbBkgnd = RGB_WHITE;
	//rgbText = RGB_BLUE;
	//SetDlgItemText(IDC_POST , "FF");

	//m_Boot.SetTextColor(rgbText);
    //m_Boot.SetBkColor(rgbBkgnd);
	//m_Boot.ChangeFontHeight(95, TRUE);


	m_Str = GetINIData("Modle Name" , "Test Station");
	m_TestStation = m_Str;	
	rgbBkgnd = RGB_WHITE;
	//rgbText = RGB_GREEN;
	rgbText = RGB_GRAY;
	//rgbText = RGB_GRAY1;

	SetDlgItemText(IDC_POST , m_Str);

	m_Boot.SetTextColor(rgbText);
    m_Boot.SetBkColor(rgbBkgnd);
	m_Boot.ChangeFontHeight(70, TRUE);

	rgbBkgnd = RGB_WHITE;
	rgbText = RGB_BLUE;

	m_ComputerName.SetTextColor(rgbText);
	m_ComputerName.SetBkColor(rgbBkgnd);
	m_ComputerName.ChangeFontHeight(75, TRUE);

	m_PASS.SetTextColor(rgbText);
	//m_PASS.SetBkColor(rgbBkgnd);
	m_PASS.ChangeFontHeight(100, TRUE);

	m_Status.SetTextColor(rgbText);
	m_Status.ChangeFontHeight(40, TRUE);  
    rgbBkgnd = RGB_SKYBLUE;

    m_Start.ChangeFontHeight(50, TRUE);
    m_Stop.ChangeFontHeight(50, TRUE);
	m_ReTest.ChangeFontHeight(50, TRUE);
    m_End.ChangeFontHeight(50, TRUE);

    rgbBkgnd = RGB_AZURE;
	m_Mac.SetBkColor(rgbBkgnd);
	m_Bar.SetBkColor(rgbBkgnd);

	m_Bar.ChangeFontHeight(30, TRUE);
	m_Mac.ChangeFontHeight(30, TRUE);

	//SetDlgItemText(IDC_STATIC1 , "TEST");//S/N
	//SetDlgItemText(IDC_STATIC2 , "TEST");//BD

	rgbText = RGB_RED;
	m_Bar.SetTextColor(rgbText);
	m_Mac.SetTextColor(rgbText);

	//rgbBkgnd = RGB_BLACK;
	//rgbText = RGB_WHITE;
	//m_Edit.ChangeFontHeight(20, TRUE);
	//m_Edit.SetTextColor(rgbText);
    //m_Edit.SetBkColor(rgbBkgnd);
		 
	CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);

	pProgess->SetRange(0,Item);
	pProgess->SetStep(1);

	GetLocalName();

#ifdef UTF001
	m_Str = "UTF001";
#else
	m_Str = GetINIData("Modle Name" , "ModleName");
#endif
	SetDlgItemText(IDC_NAME , m_Str);
}

void CDIODlg::InitListCtrl(CXListCtrl &list)
{
	CRect rect;
	list.GetWindowRect(&rect);

	int w = rect.Width() - 2;

	//int colwidths[5] = { 0,40,0,8,0 };	// sixty-fourths
	int colwidths[5] = { 0,35,0,8,0 };	// sixty-fourths
    
	TCHAR * lpszHeaders[] = { _T("Phase"),
							  _T("Test Item"),
							  _T("Status"),
							  _T("Pass"),
							  _T("Error Code"),
							  NULL };

	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		//lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * colwidths[i]) / 64;
		total_cx += lvcolumn.cx;
		list.InsertColumn(i, &lvcolumn);
	}

	// create the image list from bitmap resource
	VERIFY(list.m_cImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255, 0, 255)));
	list.m_HeaderCtrl.SetImageList(&list.m_cImageList);

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < list.m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		list.m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		//if (i == 0)
		//	hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		//else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;

		list.m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		//lvcolumn.fmt = (i == 4) ? LVCFMT_CENTER;
		if(i == 1) lvcolumn.fmt = LVCFMT_LEFT;
		else
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		list.SetColumn(i, &lvcolumn);
	}
}

void CDIODlg::FillListCtrl(CXListCtrl &list)
{
	int nItem, nSubItem ;

	// insert the items and subitems into the list
	for (nItem = 0; nItem < 200 ; nItem++) 
	{
#ifdef GetDataFromIniFile
		index.Format("%X" , nItem);
        m_MapCtrl.Lookup(index,data);
#endif
		
		for (nSubItem = 0; nSubItem < 5; nSubItem++)
		{
 			if (nSubItem == 0){
#ifdef GetDataFromIniFile
                m_Str = data;				// Phase
                m_Str = m_Str.Left(6);
#endif
				m_Str = ""; 
				list.InsertItem(nItem, m_Str);
			    list.SetItemText(nItem, nSubItem, m_Str, RGB(0,0,0), RGB(152,251,152));
			}
			else if (nSubItem == 1)
			{
#ifdef GetDataFromIniFile
			    m_Str = data;				// Test Item
#endif
				m_Str = m_TestItemStr;
                m_Str = m_Str.Right((data.GetLength() - 13));
				list.SetItemText(nItem, nSubItem, m_Str, RGB(0,0,0), RGB_AZURE);  
				//list.SetItemText(nItem, nSubItem, m_Str, RGB(0,0,0), RGB(152,251,152)); 
			}
			else if (nSubItem == 3)
			{
			    m_Str = "";					// PASS&FAIL
				list.SetItemText(nItem, nSubItem, m_Str , RGB_BLACK , RGB_AZURE);
			}
			else if (nSubItem == 4)
			{
#ifdef GetDataFromIniFile
			    m_Str = data.Left(11);
				m_Str = m_Str.Right(4);		// ErrorCode
#endif
				m_Str = m_ErrorCodeStr;
				list.SetItemText(nItem, nSubItem, m_Str , RGB(0,0,0) ,RGB_AZURE);
			}
			else							// nSubItem == 2
			{
                m_Str = "";					// Status
				list.SetItemText(nItem, nSubItem, m_Str , RGB_BLACK , RGB_AZURE);
			}
		}
	}
}

void CDIODlg::TestCtrl()
{
	/*
    if(!(findfile.FindFile("d:\\usinote\\TAURUS.ini" , 0)))	
	{
		KillTimer(1);	
		AfxMessageBox("Can't not find TAURUS INI file!!");
		CDialog::OnCancel();
	}

	m_MapCtrl.RemoveAll();

	m_Str = "D:\\USINOTE\\TAURUS.INI";

	m_IniReader.setINIFileName(m_Str);
	
	m_Str = "PROJECT";

	BOOL bExists = m_IniReader.sectionExists(m_Str);
	
	if(!bExists) 
	{
		AfxMessageBox("Section doesn't exist");
		return;
	}

	m_Str = "TEST";
	bExists = m_IniReader.sectionExists(m_Str);
	
	if(!bExists) 
	{
		AfxMessageBox("Section doesn't exist");
		return;
	}

	CStringList* myOtherList = m_IniReader.getSectionData("TEST");

	POSITION position;
	int i = 0;
	int j = 0 ;
	CString tmp;

	for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
	{
		myOtherList->GetNext(position);
		m_Str.Format("%X" , i);
		m_Str = m_IniReader.getKeyValue(m_Str,"TEST");
		m_Str = m_Str.Left(1);
		
		if(m_Str.Compare("#") != 0 )
		{
			m_Str.Format("%X" , i);
			tmp.Format("%X" , j);
			m_MapCtrl[tmp] =(m_IniReader.getKeyValue(m_Str,"TEST"));
			j++;
		}
			i++;
	}
	
	Item = j;
	m_Str.Format("%d" ,  Item); 

	UpdateINIValue(m_Str , "ITEM" , "PROJECT"); 
	*/
}

/*  // For IBM Rome Project
	m_TestCtrl.RemoveAll();
    m_TestCtrl.Add(_T("Boot 0 0001 Burn in default mac address"));  // Boot0
	m_TestCtrl.Add(_T("Boot 1 1001          Check code version"));  // Boot1
	m_TestCtrl.Add(_T("Boot 1 1002           Memory speed test"));
	m_TestCtrl.Add(_T("Boot 1 1003                 Memory test"));
	m_TestCtrl.Add(_T("Boot 1 1004       EEPROM initialization"));
	m_TestCtrl.Add(_T("Boot 1 1005   Mac eeprom initialization"));
	m_TestCtrl.Add(_T("Boot 1 1006               Write barcode"));
	m_TestCtrl.Add(_T("Boot 1 1007           Write mac address"));
	m_TestCtrl.Add(_T("Boot 1 1008            EEPROM signature"));
	m_TestCtrl.Add(_T("Boot 1 1009                 BIOS update"));
	m_TestCtrl.Add(_T("Boot 1 1010                   H8 update"));
    m_TestCtrl.Add(_T("Boot 2 2001               BIOS id check"));  // Boot2
	m_TestCtrl.Add(_T("Boot 2 2002            H8 version check"));
	m_TestCtrl.Add(_T("Boot 2 2003                Check memory"));
    m_TestCtrl.Add(_T("Boot 2 2004            Memory size test"));
    m_TestCtrl.Add(_T("Boot 2 2005                CDC id check"));
	m_TestCtrl.Add(_T("Boot 2 2006       Check ethernet device"));
	m_TestCtrl.Add(_T("Boot 2 2007           Power state check"));
	m_TestCtrl.Add(_T("Boot 2 2008               IR busy check"));
	m_TestCtrl.Add(_T("Boot 2 2009       IR communication test"));
	m_TestCtrl.Add(_T("Boot 2 2010                   POV3 test"));
	m_TestCtrl.Add(_T("Boot 2 2011               LPT wrap test"));
	m_TestCtrl.Add(_T("Boot 2 2012                Kbd id check"));
	m_TestCtrl.Add(_T("Boot 2 2013           System board test"));
	m_TestCtrl.Add(_T("Boot 2 2014                 LCD id test"));
    m_TestCtrl.Add(_T("Boot 2 2015                Ubay id test"));
	m_TestCtrl.Add(_T("Boot 2 2016               Crypt id test"));
	m_TestCtrl.Add(_T("Boot 2 2017                    IPD test"));
    m_TestCtrl.Add(_T("Boot 2 2018                Bay LED test"));
    m_TestCtrl.Add(_T("Boot 2 2019             Key matrix test"));
*/

void CDIODlg::OnAllPASS()		// Show ALL PASS/FDAIL Message if test End!
{
	//AfxMessageBox("ALL PASS!");
	/*
	int testtime = m_clock.StopTimer();
	m_Str.Format("%d" , testtime);

	CString	str;
	rgbText = RGB_BLUE;
	rgbBkgnd = RGB_GREEN;

	SetDlgItemText(IDC_PASSFAIL , "Test PASS!  " + m_Str);
 
	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

	rename("d:\\usinote\\uut.log" , ("d:\\usinote\\log\\" + m_Barcode + ".txt"));
	
	if(findfile.FindFile("d:\\usinote\\testlog.txt" , 0)) 
	{
		if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeReadWrite)))
		{
			MessageBox("Open Testlog File Error!"  , "" , MB_OK);
			return;
		}
    }
	else
	{
		if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeCreate | CFile::modeReadWrite)))
		{
			MessageBox("Testlog File Open Error!"  , "" , MB_OK);
			return;
		}

		str = "ComputerName\tDate\tTime\tBarcode\tMaccode\tResult\tErrorCode\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		str = "********************************************************************************\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
	}

	FileCtrl.SeekToEnd();
	GetLocalName();

	CTime t = CTime::GetCurrentTime();

	str.Format("%s    %d/%d/%d    %d:%d:%d" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	FileCtrl.Write(str , str.GetLength()+1);
			
	str = ( "    " + m_Barcode + "    " + m_Maccode + "    " + "PASS!    " ) ;
	FileCtrl.Write(str , str.GetLength()+1);

	str = "\r\n";
	FileCtrl.Write(str , str.GetLength()+1);
	FileCtrl.Close();
	*/
}

void CDIODlg::DispXList(int nItem , int passfail)
{
	CString m_Str;				// Important!!

	CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

	if(passfail)
	{
        m_Str =  "Completed!PASS";

		m_list->SetItemText(nItem , 2 , m_Str.Left(10) , RGB_MEDIUMBLUE , RGB_AZURE );
		m_list->SetItemText(nItem , 3 , m_Str.Right(4) , RGB_MEDIUMBLUE , RGB_AZURE );
		//m_list->SetItemText(nItem , 1 , (m_list->GetComboText(nItem,1)) , RGB_WHITE, RGB_BLUE );
	}
	else if(!passfail)
	{
		m_Str =  "Completed!FAIL";

		m_list->SetItemText(nItem , 2, m_Str.Left(10) , RGB_RED , RGB_AZURE );
		m_list->SetItemText(nItem , 3, m_Str.Right(4) , RGB_RED , RGB_AZURE );
		//m_list->SetItemText(nItem , 1 , (m_list->GetComboText(nItem,1)) , RGB_WHITE, RGB_BLUE );
	}
}

void CDIODlg::GotoNextPage()
{
    CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

	WPARAM m_wparam;
	LPARAM m_lparam;
	m_wparam  = MAKEWPARAM(SB_PAGEDOWN , 0);
	m_lparam  = MAKELPARAM(m_list->m_hWnd , 0);
	m_list->PostMessage(WM_VSCROLL , m_wparam , m_lparam);
}

void CDIODlg::EnableBattery()
{
	//iStatus = //DIG_OUT_Line(1, 0, 6 , 0 );		// Battery Power Control
	//if(ChkDIOStatus())
	//  return;
	//Device : 1 , Port 0 , bit 6  , 0 : Enable  1: Disable 
}

void CDIODlg::DisableBattery()
{  
    //iStatus = //DIG_OUT_Line(1, 0, 6 , 1 );		// Battery Power Control
    //if(ChkDIOStatus())
	//  return;
	//Device : 1 , Port 0 , bit 6  , 0 : Enable  1: Disable 
}

void CDIODlg::EnableSMBUS()
{
	//iStatus = //DIG_OUT_Line(1, 0, 7 , 0 );		// Battery SMBUS Control
	//if(ChkDIOStatus())
	 // return;
	//Device : 1 , Port 0 , bit 7  , 0 : Enable  1: Disable 
}

void CDIODlg::DisableSMBUS()
{
	//iStatus = //DIG_OUT_Line(1, 0, 7 , 1 );		// Battery SMBUS Control
	//if(ChkDIOStatus())
	//  return;
	//Device : 1 , Port 0 , bit 7  , 0 : Enable  1: Disable 
}

void CDIODlg::FixturePower()
{
	/*	
	m_Str = GetINIData("PROJECT" , "Name");		// Check TAURUS.ini file
        
	if((m_Str.Compare("TAURUS") == 0 ))
	{
												//   TAURUS Main Test card Power control   
		//iStatus = //DIG_OUT_Line(1, 0, 0 , 0 );	// Bit 0 : Function Board Power on
		if(ChkDIOStatus())
			return;
		//iStatus = //DIG_OUT_Line(1, 0, 1 , 0 );	// Bit 1 : DIO96 Enable
		if(ChkDIOStatus())
			return;
	}
	else if((m_Str.Compare("IBM") == 0 ))
	{

	}
	*/
}

BOOL CDIODlg::ChkDIOStatus()
{
/*
  CString str = "DIO96 Initial FAIL!!";
  if(iStatus != 0 )
  {
      DispString(str);
	  GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE); 
	  GetDlgItem(ID_BTN_EXIT)->EnableWindow(TRUE); 
	  return TRUE;
  }
  else
	  return FALSE;
*/
	return FALSE;
}

void CDIODlg::OnKeytest() 
{
	// TODO: Add your command handler code here
/*
	KeyTest dlg;
	int i = dlg.DoModal();

	if( i ==IDOK )
	{
       i = dlg.m_KeyNum;
	   Sendkey(i);
	}
*/
}

void CDIODlg::LidSwitchOn()
{
    //iStatus = //DIG_OUT_Line(1, 11, 1 , 0 );  // Port 11 , Bit 1 : Lid-Switch On/Off (0/1)
	
	//if(ChkDIOStatus())
	//  return;
}

double CDIODlg::RdGPIBCurrent()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	char ReadBuffer[ARRAYSIZE + 1];  // Read data buffer

	double value;
	int Dev;
	long *Pibcntl = NULL;
    char *endptr = NULL;

  	HINSTANCE Gpib32Lib = NULL;
	Gpib32Lib=LoadLibrary("GPIB-32.DLL");
	if (Gpib32Lib == NULL) {
		return FALSE;
	}

    int (_stdcall *Pibdev)(int ud, int pad, int sad, int tmo,int eot, int eos);
	int (_stdcall *Pibwrt)(int ud, PVOID buf, LONG cnt);
    int (_stdcall *Pibrd)(int ud, PVOID buf, LONG cnt);

	Pibdev = (int (__stdcall *)(int, int, int, int, int, int))GetProcAddress(Gpib32Lib, (LPCSTR)"ibdev");
    Pibwrt  = (int (_stdcall *)(int, PVOID, LONG))GetProcAddress(Gpib32Lib, (LPCSTR)"ibwrt");
	Pibrd   = (int (_stdcall *)(int, PVOID, LONG))GetProcAddress(Gpib32Lib, (LPCSTR)"ibrd");
	Pibcntl = (long *)GetProcAddress(Gpib32Lib, (LPCSTR)"user_ibcnt");

	if ( (Pibrd  == NULL) )
    {
        FreeLibrary(Gpib32Lib);
        Gpib32Lib = NULL;
        return FALSE;
    }

	Dev = (*Pibdev) (BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
                     13, EOTMODE, EOSMODE);
	
	(*Pibwrt) (Dev, "IOUT?" , 5L);

	Sleep(1000);
	(*Pibrd) (Dev, ReadBuffer, ARRAYSIZE);
		ReadBuffer[(*Pibcntl)] = '\0';

	value = strtod(ReadBuffer , &endptr);

	FreeLibrary (Gpib32Lib);
	Gpib32Lib = NULL;
		return value;
}

BOOL CDIODlg::ChkRange(double i, double j)
{
/*
   for(int k = 0 ; k < 15; k++)		
   {
        current = RdGPIBCurrent();
		if( current < 0)
		{
			current = current * (-1);
			if((current < i) && (current > j))
			{
				return TRUE;
			}
			else
			{
				Sleep(300);
			}
		}
		else
		{
			if((current < i) && (current > j))
			{
				return TRUE;
			}
			else
			{
				Sleep(300);
			}
		}
   }
*/                 
   return FALSE;
}

void CDIODlg::LidSwitchOff()
{
    //iStatus = //DIG_OUT_Line(1, 11, 1 , 1 );  // Port 11 , Bit 1 : Lid-Switch On/Off (0/1)
	
	//if(ChkDIOStatus())
	//  return;
}

void CDIODlg::EnablePost()
{
    //iStatus = //DIG_OUT_Line(1, 2, 0 , 1 );   
	//iStatus = //DIG_OUT_Line(1, 2, 1 , 1 );   
			
	//if(ChkDIOStatus())
	//  return;

	//iStatus = //DIG_OUT_Line(1, 2, 2 , 0 );   
	//iStatus = //DIG_OUT_Line(1, 2, 3 , 0 ); 
}

void CDIODlg::SetBIOS1()
{
/*
    //F9  Load Default
    Sendkey(37);
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(128);  // Down Key
    Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(56);   // "1"
    Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(56);   // "1"
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(72);   // "2" Key
	Sleep(300);
    Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(56);   // "1" Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(36);   // F10 Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);
*/
}

void CDIODlg::SetBIOS2()
{
/*
    //F9  Load Default
    Sendkey(37);
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(128);  // Down Key
    Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(72);   // "2"
    Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(72);   // "2"
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(72);   // "2" Key
	Sleep(300);
    Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(72);   // "2" Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(36);   // F10 Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);
*/
}

void CDIODlg::SetBIOS3()
{
/*
    //F9  Load Default
    Sendkey(37);
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(128);  // Down Key
    Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(71);   // "3"
    Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(84);   // "0"
	Sleep(300);

	Sendkey(71);   // "3"
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(72);   // "2" Key
	Sleep(300);
    Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(84);   // "0" Key
	Sleep(300);
	Sendkey(71);   // "3" Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(36);   // F10 Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);
*/
}

void CDIODlg::ThermalShutdown()
{
    //DIG_OUT_Line(1, 11, 2 , 0 );  // Port 11 , Bit 2 = 0 : Thermal Shutdown 
}

void CDIODlg::USBOn()
{
    //DIG_OUT_Line(1, 0 , 4 , 0 );  // Port 0 , Bit 4 = 0 : UPT2 Power On 
}

void CDIODlg::USBOff()
{
    //DIG_OUT_Line(1, 0 , 4 , 1 );  // Port 0 , Bit 4 = 0 : UPT2 Power Off 
}

BOOL CDIODlg::WakeOnLan()
{
	/*
	EnablePost();

	if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite)))
	   MessageBox("File Open Error!"  , "" , MB_OK);

	FileCtrl.SeekToEnd();
	CString str;
	int post;

    //DIG_IN_Prt(1, 9, &iPattern);

    if(iPattern != 0)
	{
	    MessageBox("AC Off FAil!" , NULL , MB_OK);
		return FALSE;
	}

    if((WinExec("wakeup.bat" , SW_HIDE)) < 31 )
	{
		MessageBox("Run wakeup.bat FAIL!" , NULL , MB_OK);
        return FALSE;
	}

	Sleep(500);

	for(int i = 0 ; i < 1000 ; i++)				// Wait Post Code (0x38)
	{
        //DIG_IN_Prt(1, 9, &iPattern);

		if(iPattern == 0x38)
		{
			str = "Wake On Lan .............PASS!\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
			FileCtrl.Close();
			SetDlgItemText(IDC_DSPMESS , "");
			return TRUE;
		}
		else
			Sleep(2);

		if(iPattern > 15)
		{
		   	post = iPattern;
			str.Format("%X" ,post); 
		   	DispPost(str);
		}
		else
		{
			post = iPattern;
			str.Format("0%X" , post);
            DispPost(str); 
		}
	}
	
	Sleep(1500);
	EnablePost();

	if((WinExec("wakeup.bat" , SW_HIDE)) < 31 )
	{
		MessageBox("Run wakeup.bat FAIL!" , NULL , MB_OK);
        return FALSE;
	}

	Sleep(500);

	for(i = 0 ; i < 1000 ; i++)				// Wait Post Code (0x38)
	{
        //DIG_IN_Prt(1, 9, &iPattern);

		if(iPattern == 0x38)
		{
			str = "Wake On Lan .............PASS!\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
			FileCtrl.Close();
			SetDlgItemText(IDC_DSPMESS , "");
			return TRUE;
		}
		else
			Sleep(2);

		if(iPattern > 15)
		{
		   	post = iPattern;
			str.Format("%X" ,post); 
		   	DispPost(str);
		}
		else
		{
			post = iPattern;
			str.Format("0%X" , post);
            DispPost(str); 
		}
	}

	Sleep(1500);
	EnablePost();

	if((WinExec("wakeup.bat" , SW_HIDE)) < 31 )
	{
		MessageBox("Run wakeup.bat FAIL!" , NULL , MB_OK);
        return FALSE;
	}

	Sleep(500);

	for(i = 0 ; i < 1000 ; i++)				// Wait Post Code (0x38)
	{
        //DIG_IN_Prt(1, 9, &iPattern);

		if(iPattern == 0x38)
		{
			str = "Wake On Lan .............PASS!\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
			FileCtrl.Close();
			SetDlgItemText(IDC_DSPMESS , "");
			return TRUE;
		}
		else
			Sleep(2);

		if(iPattern > 15)
		{
		   	post = iPattern;
			str.Format("%X" ,post); 
		   	DispPost(str);
		}
		else
		{
			post = iPattern;
			str.Format("0%X" , post);
            DispPost(str); 
		}
	}

	str = "Wake On Lan .............FAIL!\r\n";
	FileCtrl.Write(str , str.GetLength()+1);
	FileCtrl.Close();
	SetDlgItemText(IDC_DSPMESS , "");
	*/
	return FALSE;
	
}

BOOL CDIODlg::ChkPowerOn()
{
	////DIG_IN_Prt(1, 9 , &iPattern); 
/*
	iPattern = iPattern & 0x01;

	if(iPattern == 0)			
		return TRUE;			// No VCC5M power!
	else
*/
		return FALSE;

}

BOOL CDIODlg::ChkPowerOff()
{
	////DIG_IN_Prt(1, 9 , &iPattern); 
/*
	iPattern = iPattern & 0x01;

	if(iPattern == 0)
		return FALSE;
	else 
*/	
		return TRUE;			// VCC5M power is still active!
}

BOOL CDIODlg::ChkRJ11()
{
	/*
    //DIG_OUT_Line(1, 8 , 0 , 0 );   
    //DIG_OUT_Line(1, 8 , 1 , 0 ); 
	Sleep(300);
    //DIG_IN_Prt(iDevice, 10, &iPattern); 

	if((iPattern & 0x03) != 0)
	{
		m_Str.Format("Read is %x " , (iPattern & 0x03));
		//MessageBox(m_Str , "Write 0" , MB_OK); 
		return FALSE;
	}

    Sleep(100);

    //DIG_OUT_Line(1, 8 , 1 , 1 ); 
	Sleep(300);
    //DIG_IN_Prt(iDevice, 10, &iPattern); 
	if((iPattern & 0x03) != 2)
	{
		m_Str.Format("Read is %x " , (iPattern & 0x03));
		//MessageBox(m_Str , "Write 2" , MB_OK); 
		return FALSE;
	}

	Sleep(100);

    //DIG_OUT_Line(1, 8 , 0 , 1 );   
    //DIG_OUT_Line(1, 8 , 1 , 1 ); 
	Sleep(300);
    //DIG_IN_Prt(iDevice, 10, &iPattern); 
	if((iPattern & 0x03) != 3)
	{
		m_Str.Format("Read is %x " , (iPattern & 0x03));
		//MessageBox(m_Str , "Write 3" , MB_OK); 
		return FALSE;
	}

    Sleep(100);

	//DIG_OUT_Line(1, 8 , 0 , 1 );   
    //DIG_OUT_Line(1, 8 , 1 , 0 ); 
	Sleep(300);
    //DIG_IN_Prt(iDevice, 10, &iPattern); 
	if((iPattern & 0x03) != 1)
	{
		m_Str.Format("Read is %x " , (iPattern & 0x03));
		//MessageBox(m_Str , "Write 1" , MB_OK); 
		return FALSE;
	}

	if(ChkDIOStatus())
	  return FALSE;
	*/
    return TRUE;
}

void CDIODlg::Set9V()
{
    //SetGPIB(9 , 4.5);
}

void CDIODlg::Set10V()
{
	//SetGPIB(10 , 4.5);
}

void CDIODlg::Set12V()
{
	//SetGPIB(12.6 , 4.5);
}

void CDIODlg::SetWOL()
{
/*
    //F9  Load Default
    Sendkey(37);
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);

    Sendkey(112);   //  Key
	Sleep(300);

	Sendkey(112);   //  Key
	Sleep(300);

	Sendkey(112);   //  Key
	Sleep(300);

	Sendkey(112);   //  Key
	Sleep(300);

	Sendkey(112);   //  Key
	Sleep(300);

	Sendkey(128);   //  Key
	Sleep(300);

	Sendkey(128);   //  Key
	Sleep(300);

	Sendkey(128);   //  Key
	Sleep(300);

    Sendkey(128);   //  Key
	Sleep(300);

	Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(128);  // Down Key
    Sleep(300);

	Sendkey(81);   // Enter Key
	Sleep(300);

	Sendkey(36);   // F10 Key
	Sleep(300);

    Sendkey(81);   // Enter Key
	Sleep(300);
*/
}

void CDIODlg::TurnOnHDD()
{
    ////DIG_OUT_Line(1, 11, 1 , 0 );  // Port 11 , Bit 1 = 0 : TurnOn HDD LED
}

void CDIODlg::TurnOffHDD()
{
    ////DIG_OUT_Line(1, 11, 1 , 1 );  // Port 11 , Bit 1 = 0 : TurnOff HDD LED

}

void CDIODlg::RINGON()
{
    ////DIG_OUT_Line(1, 8 , 0 , 0 );   
}

void CDIODlg::RINGOFF()
{
    ////DIG_OUT_Line(1, 8 , 0 , 1 );   
}

void CDIODlg::TIPON()
{
    ////DIG_OUT_Line(1, 8 , 1 , 0 );   
}

void CDIODlg::TIPOFF()
{
	////DIG_OUT_Line(1, 8 , 1 , 1 );   
}

void CDIODlg::EnableRJ11()
{
/*
    //DIG_OUT_Line(1, 2, 0 , 1 );   
	//DIG_OUT_Line(1, 2, 1 , 1 );   
	//DIG_OUT_Line(1, 2, 2 , 1 );   
	//DIG_OUT_Line(1, 2, 3 , 0 );
*/	
}

BOOL CDIODlg::WakeUpTest()
{
	/*
		AC_On();

        Sleep(3000);

		SetGPIB(8.9 , 1);

        Sleep(3000);

		EnableSMBUS();

		Sleep(3000);

		EnableBattery();
	*/
		return TRUE;

}

BOOL CDIODlg::RJ11LoopTest()
{
/*
	if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeCreate | CFile::modeReadWrite)))
		MessageBox("File Open Error!"  , "" , MB_OK);
   
	m_Str = "***********************************************************\r\n";
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	m_Str = m_Barcode + "\r\n";							
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	CTime t = CTime::GetCurrentTime();

	m_Str.Format("Year %d, Month %d, Day %d, %d : %d : %d\r\n" , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);

    m_Str = GetINIData("DEBUG" , "RJ11");

    if(m_Str.Compare("Y") == 0 )
    {
		for(int k = 0 ; k < 2; k++)		
		{
			if(!(ChkRJ11()))
				Sleep(500);
			else
			{
				m_Str = "RJ11 Loop Back Test PASS!\r\n" ;
				FileCtrl.Write(m_Str , m_Str.GetLength()+1);
				FileCtrl.Close();
				return TRUE;
			}
		}

		m_Str = "RJ11 Loop Back Test FAIL!\r\n" ;
		FileCtrl.Write(m_Str , m_Str.GetLength()+1);
		FileCtrl.Close();

		return FALSE;
    }
    else
    {
		FileCtrl.Close();
		return TRUE;
    }
*/
	return TRUE;
}

void CDIODlg::Set0V()
{
	//SetGPIB(0 , 0);
}

void CDIODlg::DispPostCode()
{
/*
    EnablePost();
	
    m_LptthrParam.m_hwnd = this->m_hWnd ;
    m_LptthrParam.phase = BootSequence;

    ActiveProcess = TRUE; 

    m_pLptThrd = AfxBeginThread(DIO96Threadfun , (LPVOID)&m_LptthrParam);
*/
}

void CDIODlg::DIO96Ctrl()
{
	//DIO96 dlg;
	//dlg.DoModal();
}

void CDIODlg::LoopTest()
{
/*
	Loop = 3 ;

    for(int i = 0 ; i < 3 ; i++)
	{
         PMTest();
		 Sleep(2000);
	}
*/
}

BOOL CDIODlg::Find()
{
/*
   CWnd *pWnd;
   m_Str = "TAURUS HostCtrl";

   pWnd = FindWindow(NULL , m_Str);

   if(pWnd != NULL)
   {
      AfxMessageBox("Program is already running!!");

      return FALSE;
   }
*/
      return TRUE; 
}

void CDIODlg::Start()
{
	m_clock.StartTimer();  // Start Timer
}

void CDIODlg::Stop()
{
	m_clock.StopTimer();
}

void CDIODlg::Reset()
{
	m_clock.ClearDate();  // Timer Initialization
}

void CDIODlg::GetLocalName()
{
	WSADATA         wsaData;

	char name[10];
    int             nRet    =   NULL;

	nRet    =   WSAStartup(MAKEWORD(1,1),&wsaData);

	if(nRet!=0)
    {
        AfxMessageBox("WSAStartup Error!!");
    }

	gethostname(name , 10);

	m_Str = name;
}

void CDIODlg::ShowItem()
{
	FillListCtrl(m_List);
}

void CDIODlg::DisablePost()
{
    //iStatus = //DIG_OUT_Line(1, 2, 0 , 1 );   
	//iStatus = //DIG_OUT_Line(1, 2, 1 , 1 );   
			
	//if(ChkDIOStatus())
	//  return;

	//iStatus = //DIG_OUT_Line(1, 2, 2 , 1 );   
	//iStatus = //DIG_OUT_Line(1, 2, 3 , 1 ); 
}

void CDIODlg::ClearString()
{
	SetDlgItemText(IDC_DSPMESS , "");
}

BOOL CDIODlg::Chkpassfail()
{
/*
	for (int nItem = 0; nItem < Item; nItem++)  
	{
	    m_Str = m_List.GetComboText(nItem,3);

		if(!(m_Str.Compare("FAIL")))
		{
			return FALSE;
		}
	}
*/
	return TRUE;
}

BOOL CDIODlg::CheckPost()
{
/*
    int post;

    for(int i = 0 ; i < 1000 ; i++)				// Wait Post Code (0x38)
	{
        //DIG_IN_Prt(1, 9, &iPattern);

		if(iPattern == 0x38)
			goto next;
		else
			Sleep(3);

		if(iPattern > 15)
		{
		   	post = iPattern;
			m_Str.Format("%X" ,post); 
		   	DispPost(m_Str);
		}
		else
		{
			post = iPattern;
			m_Str.Format("0%X" , post);
            DispPost(m_Str); 
		}
	}

	m_Str.Format("POST %X ERROR!" , post);
    DispString(m_Str);

	return FALSE;

next:

    for(i = 0 ; i < 2300 ; i++)				// Wait Post Code (0x38)
	{
        //DIG_IN_Prt(1, 9, &iPattern);

		if(iPattern == 0)
			goto pass;
		else
			Sleep(3);

		if(iPattern == 0x85)	
		{
				Sendkey(24);		// F2 (Load Default BIOS)     
				Sendkey(24);
				Sleep(3000);

				AC_Off();
				return TRUE;
		}

		if(iPattern > 15)
		{
		   	post = iPattern;
			m_Str.Format("%X" ,post); 
		   	DispPost(m_Str);
		}
		else
		{
			post = iPattern;
			m_Str.Format("0%X" , post);
            DispPost(m_Str); 

		}

	}
	
	m_Str.Format("POST %X ERROR!" , post);
    DispString(m_Str);
	return FALSE;

pass:
*/
	return TRUE;
}

BOOL CDIODlg::ChkLanCopy()
{
/*
	if(!(FileCtrl.Open("d:\\usinote\\pm.txt", CFile::modeReadWrite)))
	   MessageBox("File Open Error!"  , "" , MB_OK);

    for(int i = 0 ; i < 14 ; i++)
	{
		if(findfile.FindFile("d:\\usinote\\start.dat" , 0))
		{
			DispXList(2 , 1);	
			DeleteFile("d:\\usinote\\start.dat");
			m_Str = "LAN Copy Test ...........PASS!\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength()+1);
					   
			m_Str = "***********************************************************\r\n";
			FileCtrl.Write(m_Str , m_Str.GetLength()+1);

			FileCtrl.Close();
			return TRUE;
		}
		else
			Sleep(500);
	}

    DispXList(2 , 0);	
	m_Str = "LAN Copy Test ...........FAIL!\r\n";
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);
			   
	m_Str = "***********************************************************\r\n";
	FileCtrl.Write(m_Str , m_Str.GetLength()+1);

	FileCtrl.Close();
*/
	return FALSE;
}

void CDIODlg::CMOSError()
{
/*
	AfxMessageBox("DEBUG!");

    KillTimer(2);

	m_clock.StopTimer();

	rgbText = RGB_RED;
	rgbBkgnd = RGB_AZURE;

	m_PASS.SetBkColor(rgbBkgnd);
	m_PASS.SetTextColor(rgbText);
	m_PASS.ChangeFontHeight(110, TRUE); 

	SetDlgItemText(IDC_DSPMESS , "");
	SetDlgItemText(IDC_DSPMESS , "CMOS FAIL!  " + m_Str);
    
	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
    m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

	//Power_off();
	//Sleep(1500);
	AC_Off();
	
	while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
	{
		Sleep(500);
	}

	AfxMessageBox("ThreadEnd!!");				// Debug

	CDialog::OnCancel();
*/
}

void CDIODlg::ThreadTest()
{
/*
    m_LptthrParam.m_hwnd = this->m_hWnd ;
	m_LptthrParam.phase = LPTBaseIO;

	ActiveTest = 1;

	m_LPTThread = AfxBeginThread(TestThread , (LPVOID)&m_LptthrParam);
*/
}

UINT CDIODlg::TestThread(LPVOID pParam)
{
/*
	_STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    CDIODlg *ptr = (CDIODlg*)(FromHandle(Thread_Info->m_hwnd));

	short BaseIO = Thread_Info->phase;

    int	data1 = 255;
	int	data2 = 0;

	CString str;

	//	_outp(0x37a , (_inp(0x37a) | 0x20));		// Read Post Code 
	//	Set HOST PC LPT to EPP Mode

	//data2 = Inp32(BaseIO + 2);
    //Out32((BaseIO + 2 ) , (data2 | 0x20));
	
	while(ptr->ActiveTest)
	{
		//data2 = Inp32(BaseIO);
		if(data2 < 16)
			str.Format("%0X" , data2);
		else
			str.Format("%X" , data2);

		if(data1 != data2)
		{
			ptr->SetDlgItemText(IDC_POST , str);
			data1 = data2;
		}

		Sleep(3);
	}

	//AfxMessageBox("END!");
*/
 	return 0;
}

void CDIODlg::Winwake()
{
    //if((WinExec("wakeup.bat" , SW_HIDE)) < 31 )
	//{
	//	MessageBox("Run wakeup.bat FAIL!" , NULL , MB_OK);
	//}
}

void CDIODlg::SuspendThread()
{
	//m_pLptThrd->SuspendThread();
}

void CDIODlg::ResumeThread()
{
    //m_LptthrParam.phase = 1;

	//m_pLptThrd->ResumeThread();
}

void CDIODlg::TestLog()
{
	//AfxMessageBox("test");

	/*
    CString str;

    if(!(Chkpassfail()))	// TEST FAIL!
    {
		rgbText = RGB_RED;
		rgbBkgnd = RGB_AZURE;
		m_PASS.SetTextColor(rgbText);
		SetDlgItemText(IDC_DSPMESS , "");

		if(findfile.FindFile("d:\\usinote\\testlog.txt" , 0)) 
		{
			if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeReadWrite)))
				MessageBox("Testlog File Open Error!"  , "" , MB_OK);
        }
		else
		{
			if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeCreate | CFile::modeReadWrite)))
				MessageBox("Testlog File Open Error!"  , "" , MB_OK);
			str = "ComputerName\tDate\tTime\tBarcode\tMaccode\tResult\tErrorCode\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
			str = "********************************************************************************\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
		}

		FileCtrl.SeekToEnd();
		GetLocalName();

		CTime t = CTime::GetCurrentTime();

		str.Format("%s    %d/%d/%d    %d:%d:%d" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);
			
		str = ( "    " + m_Barcode + "    " + m_Maccode + "    " + "FAIL!    " ) ;
		FileCtrl.Write(str , str.GetLength()+1);

		for (int nItem = 0; nItem < Item; nItem++)  
		{
			str = m_List.GetComboText(nItem,3);

			if(!(str.Compare("FAIL")))
			{
				str = m_List.GetComboText(nItem,4);	
				str = str + "\t";
				FileCtrl.Write(str , str.GetLength()+1);
			}
		}

		str = "\r\n";
		FileCtrl.Write(str , str.GetLength()+1);
		FileCtrl.Close();

		int testtime = m_clock.StopTimer();
		m_Str.Format("%d" , testtime);
		SetDlgItemText(IDC_DSPMESS , "Test FAIL!  " + m_Str);
	    m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	    m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
	}
	else																// TEST ALL PASS!
	{
		m_PASS.StopBlink(CC_BLINK_BOTH);
		rgbText = RGB_BLUE;
		rgbBkgnd = RGB_AZURE;
		m_PASS.SetTextColor(rgbText);
		SetDlgItemText(IDC_DSPMESS , "");

		if(findfile.FindFile("d:\\usinote\\testlog.txt" , 0)) 
		{
			if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeReadWrite)))
				MessageBox("Testlog File Open Error!"  , "" , MB_OK);
        }
		else
		{
			if(!(FileCtrl.Open("d:\\usinote\\testlog.txt", CFile::modeCreate | CFile::modeReadWrite)))
				MessageBox("Testlog File Open Error!"  , "" , MB_OK);
			str = "ComputerName\tDate\tTime\tBarcode\tMaccode\tResult\tErrorCode\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
			str = "********************************************************************************\r\n";
			FileCtrl.Write(str , str.GetLength()+1);
		}

		FileCtrl.SeekToEnd();
		GetLocalName();

		CTime t = CTime::GetCurrentTime();

		str.Format("%s    %d/%d/%d    %d:%d:%d" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		FileCtrl.Write(str , str.GetLength()+1);
			
		str = ( "    " + m_Barcode + "    " + m_Maccode + "    " + "PASS!\r\n" ) ;
		FileCtrl.Write(str , str.GetLength()+1);
		
		FileCtrl.Close();

		int testtime = m_clock.StopTimer();
		m_Str.Format("%d" , testtime);
		SetDlgItemText(IDC_DSPMESS , "ALL PASS!  " + m_Str);
		
	}

	DisablePost();
	Sleep(500);
    //DIG_IN_Prt(1, 9, &iPattern);
    m_Time2 = iPattern;
	m_Str.Format("%X",m_Time2);
	SetDlgItemText(IDC_POST , m_Str);

	AC_Off();

	if(findfile.FindFile(("d:\\usinote\\log\\" + (m_Barcode.Right(6)) + ".log")))
	{
		DeleteFile(("d:\\usinote\\log\\" + (m_Barcode.Right(6)) + ".log"));
	}

	rename("d:\\usinote\\pm.txt" , ("d:\\usinote\\log\\" + (m_Barcode.Right(6)) + ".log"));
    */
}

void CDIODlg::DispProgress(int n)
{
    //CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);

    //pProgess->SetPos(n);
}

void CDIODlg::EndTHread()
{
/*
    ActiveProcess = FALSE;
	
	while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
	{
		Sleep(500);
	}

	AfxMessageBox("TAURUS End!");
*/
}

void CDIODlg::INIFileTest()
{
/*
	m_Str = "e:\\ini\\test.ini";
    m_IniReader.setINIFileName(m_Str);

    m_Str = "TEST";
	BOOL bExists = m_IniReader.sectionExists(m_Str);
	
	if(!bExists) 
	{
		AfxMessageBox("Section doesn't exist");
		return;
	}
	
	CStringList* myOtherList = m_IniReader.getSectionData(m_Str);

	m_Str = m_IniReader.getKeyValue("1","TEST");

	AfxMessageBox(m_Str);
*/
}

BOOL CDIODlg::InitWinSock1()
{
	int             nRet    =   NULL;
	WSADATA         wsaData;
	sockaddr_in     local;

	nRet    =   WSAStartup(MAKEWORD(1,1),&wsaData);
	if(nRet!=0)
    {
		AfxMessageBox("WSAStartup Error!!");
		return FALSE;
    }

	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short)1024);

	g_Socket1 =   socket(AF_INET,SOCK_STREAM,0);
    if(g_Socket1 == INVALID_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL CDIODlg::ConnectToServer()
{
	sockaddr_in     server;
    hostent        *hp      =   NULL;
    char           *pchIP   =   NULL;
	
    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_szIP = m_IniReader.getKeyValue( "IP" , "SFIS");
	m_szPort = m_IniReader.getKeyValue( "PORT" , "SFIS");

    InitWinSock1();
    //UpdateData(TRUE);
    unsigned int addr;
    pchIP   =   m_szIP.GetBuffer(m_szIP.GetLength());
    addr    =   inet_addr(m_szIP);

    server.sin_addr.s_addr=addr;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(m_szPort));

	if(connect(g_Socket1,(struct sockaddr*)&server,sizeof(server)))
    {
		AfxMessageBox("Can't connect to SFIS Server !!");
		return FALSE;
    }

	ConnectSFIS = AfxBeginThread(SFISSocketThread , NULL);

	return TRUE;
}

void CDIODlg::SendData()
{
	int nLen = 0 ;
	m_Str = "Hello World!\r\n";
    nLen = m_Str.GetLength() + 1;

	send(g_SocketClient,m_Str,nLen,0);
}

void CDIODlg::SendPASS()
{

}

void CDIODlg::SendFAIL()
{

}

void CDIODlg::TestTimer()
{
	
}

CString CDIODlg::GetINIData(CString section, CString key)
{
	CString Str = "";

	TCHAR cCurrentDirectory[300]={0};
	
	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, INIFile);

	//m_IniReader.setINIFileName("D:\\ho005167\\TEST\\MPT_Host\\TAU1104\\USIOWN\\Debug\\bhc302.ini");

	m_IniReader.setINIFileName(cCurrentDirectory);

	m_IniReader.sectionExists(section);

	Str = m_IniReader.getKeyValue( key , section);

    return Str;
}

void CDIODlg::UpdateINIValue(CString m_keyvalue, CString m_key, CString m_section)
{
    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
 
 	m_IniReader.sectionExists(m_section);

	m_IniReader.setKey(m_keyvalue,m_key,m_section);

}

void CDIODlg::OnFileSharing()
{
	//ShellExecute(0 , "open" , "d:\\usinote\\hello.js" , NULL , NULL , SW_SHOWMAXIMIZED );
/*
    m_Str = GetINIData("DEBUG" , "FileSharing");		// Check TAURUS.ini file
    
	if((m_Str.Compare("Y") != 0 ))
	{
		return;
	}

    m_LptthrParam.m_hwnd = this->m_hWnd ;
    m_LptthrParam.phase = 2;
	m_LptthrParam._this = this;

    ActiveProcess = 1;									// Start DIO96 Thread

	SetDlgItemText(IDC_DSPMESS , "");

    m_pLptThrd = AfxBeginThread(DIO96Threadfun , (LPVOID)&m_LptthrParam);
*/	
}

void CDIODlg::OnClosefile() 
{
	// TODO: Add your command handler code here
/*
	if(m_pLptThrd != NULL)
	{
		ActiveProcess = FALSE;
	
		while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}

		AfxMessageBox("ThreadEnd!");
	}
*/
}

void CDIODlg::OnSerialCtrl() 
{
	// TODO: Add your command handler code here
/*
	m_Str = GetINIData("SerialConfig" , "SerialSupport");		// Check TAURUS.ini file
    
	if((m_Str.Compare("Y") != 0 ))
	{
		return;
	}

	SetDlgItemText(IDC_DSPMESS , "");

 	if(ActiveSerial)
	{
	   AfxMessageBox("Serial port already open!");
	   return;
	}

	if(!(Openport()))
	{
		AfxMessageBox("Get configuration port has problem.");
		return;
	}

	ActiveSerial = TRUE;
	//ActiveSerial = FALSE;

    m_LptthrParam.m_hwnd = this->m_hWnd ;

	serialProcess = (SerialThread*)AfxBeginThread(RUNTIME_CLASS(SerialThread), THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
    
    if(serialProcess == NULL)
	{
		AfxMessageBox("Create Serial thread fail!");
		return;
	}

	serialProcess->setOwner(this);

	serialProcess->ResumeThread();
*/
}

BOOL CDIODlg::Openport()
{
/*
	m_Str = GetINIData("SerialConfig" , "Port");		// Check TAURUS.ini file

    handlePort_ = CreateFile("COM1",					// Specify port device: default "COM1"
    GENERIC_READ | GENERIC_WRITE,						// Specify mode that open device.
    0,													// the devide isn't shared.
    NULL,												// the object gets a default security.
    OPEN_EXISTING,										// Specify which action to take on file. 
    0,													// default.
    NULL);												// default.

    if(handlePort_ == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateFile error!");
		return FALSE;
	}

	// Get current configuration of serial communication port.
    if (GetCommState(handlePort_,&configSerial_) == 0)
    {
		AfxMessageBox("Get configuration port has problem.");
		return FALSE;
    }
  
    configSerial_.BaudRate = CBR_9600;					// Specify buad rate of communicaiton.
    
    configSerial_.StopBits = ONESTOPBIT;				// Specify stopbit of communication.
    configSerial_.Parity = NOPARITY;					// Specify parity of communication.
    configSerial_.ByteSize = 8;							// Specify  byte of size of communication.

    // Set current configuration of serial communication port.
    if (SetCommState(handlePort_,&configSerial_) == 0)
    {
       AfxMessageBox("Set configuration port has problem.");
       return FALSE;
    }
 */
    /*

    // instance an object of COMMTIMEOUTS.
    COMMTIMEOUTS comTimeOut;                   
    // Specify time-out between charactor for receiving.
    comTimeOut.ReadIntervalTimeout = 3;
    // Specify value that is multiplied 
    // by the requested number of bytes to be read. 
    comTimeOut.ReadTotalTimeoutMultiplier = 3;
    // Specify value is added to the product of the 
    // ReadTotalTimeoutMultiplier member
    comTimeOut.ReadTotalTimeoutConstant = 2;
    // Specify value that is multiplied 
    // by the requested number of bytes to be sent. 
    comTimeOut.WriteTotalTimeoutMultiplier = 3;
    // Specify value is added to the product of the 
    // WriteTotalTimeoutMultiplier member
    comTimeOut.WriteTotalTimeoutConstant = 2;
    // set the time-out parameter into device control.
    SetCommTimeouts(handlePort_,&comTimeOut);
    
	*/

	return TRUE;
}

void CDIODlg::CRTSwitch()
{
/*
	//AfxMessageBox("test");
	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	m_Str = m_IniReader.getKeyValue( "UUTMode" , "DEBUG");

    if(m_Str.Compare("Disable") == 0)
	{
		if(m_SwitchCRT)
		{
			//DIG_OUT_Line(1, 4, 7 , 0);  
			m_SwitchCRT = FALSE;
		}
		else
		{
			//DIG_OUT_Line(1, 4, 7 , 1);  
			m_SwitchCRT = TRUE;
		}
	}
*/
}

CString CDIODlg::GetFileData(CString Filename, CString section, CString key)
{
	CString Str = "";
	
    m_IniReader.setINIFileName(Filename);

	m_IniReader.sectionExists(section);

	Str = m_IniReader.getKeyValue( key , section);

    return Str;
}

void CDIODlg::ShowTEST(CString &Str)
{
	SetDlgItemText(IDC_PASSFAIL , Str);
}

void CDIODlg::CloseSerial() 
{
	// TODO: Add your command handler code here
/*
    if(serialProcess == NULL)
		return;

	serialProcess->SuspendThread();
*/
}

void CDIODlg::DisplayTest(int nItem)
{
	//CString m_Str;				// Important!!

	GetDlgItemText(IDC_PASSFAIL , m_Str);

	CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

	if(m_Str.Find(",") >= 0)
	{
		m_list->SetItemText(nItem , 4 , m_Str.Right(3), RGB_BLACK, RGB_YELLOW );

		m_list->SetItemText(nItem , 1 , m_Str.Left((m_Str.GetLength() - 4)), RGB_BLACK, RGB_BLUE2 );
	}
	else
		m_list->SetItemText(nItem , 1 , m_Str, RGB_BLACK, RGB_BLUE2 );
}

UINT CDIODlg::IrDAThread(LPVOID pParam)
{
	/*
    _STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    CDIODlg *ptr = (CDIODlg*)(FromHandle(Thread_Info->m_hwnd));
    CEdit *m_Edt = (CEdit*)(ptr->GetDlgItem(IDC_DSPMESS));

	COMSTAT     cs;
	DWORD       dwError;
	DWORD       nBytesRead;
	unsigned char	mess[1024];
	int			StateFlag = 0;
	int         IrSendFlag= 0;
	int			RxByteFlag = 0;
	unsigned char Data = 0;
	unsigned char   *PtrData = &Data;
	
	memset( mess , NULL , 1024);

	//AfxMessageBox("TEST");

	while(ptr->ActiveIrDA)
	{
		ClearCommError(ptr->handlePort_ , &dwError , &cs);
		
		//if(cs.cbInQue > sizeof(mess))
		//{
		//	PurgeComm(ptrDlg->handlePort_ , PURGE_RXCLEAR);
		//	return FALSE;
		//}
		
		ReadFile(ptr->handlePort_ , PtrData, cs.cbInQue , &nBytesRead , NULL);

		if((Data != 0) && (Data != 0xff))
			RxByteFlag = 1;
		else
			RxByteFlag = 0;

        if(RxByteFlag)
		{
			switch(StateFlag)
			{
				case 0:
					if(Data == 0xc2)
					{
						if(IrSendFlag)
						{
							//Send 0xc3
							m_Edt->ReplaceSel("Send C3");
							unsigned long len;
							Data = 0xc3;
							//		Sleep(100);
							WriteFile(ptr->handlePort_ , PtrData , 1 , &len , NULL);
						}
						else
						   IrSendFlag = 1;
					}
					break;
			}
			Data = 0;
		}
        
		//if(StateFlag == 1)
		//{
		//	if((mess[0] == 0xc2) && (mess[1] == 0xc2))
		//	{
		//		m_Edt->ReplaceSel("Send C3");
		//		unsigned long len;
		//		Data = 0xc3;
		//		Sleep(100);
		//		WriteFile(ptrDlg->handlePort_ , PtrData , 1 , &len , NULL);
		//		memset( mess , NULL , nBytesRead );
		//	}
		//}
	}
	*/
	return 0;
}

UINT CDIODlg::SFISSocketThread(LPVOID pParam)
{
	/*
	CIniReader  m_IniReader;
 	char mess[24] ;
	CString str; 
	int count = 0;
		
	memset(mess , 0 , 24);
	int bytesSent ;

	CString m_FixtureID,m_Bar,m_Employee,m_LineNO,m_Mac,m_Result;
	
    m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_FixtureID = m_IniReader.getKeyValue( "Fixture-ID" , "SFIS");
	m_LineNO = m_IniReader.getKeyValue( "Line-NO" , "SFIS");
	m_Employee = m_IniReader.getKeyValue( "USER-ID" , "SFIS");

	m_Bar = "11S1111111111111111111";
	m_Mac = "222222222222";

	m_Result = "PASS";

 	str = m_FixtureID + "," + m_Bar + ",1," + m_Employee + "," + m_LineNO + "," + m_Mac;
	bytesSent = send(g_Socket1 , str , str.GetLength() , 0);
	
	str = "OK1";

	while(1)
	{
		if(recv(g_Socket1 , mess , 24 , 0))
		{
			if((memcmp(str , mess , 3)) == 0)
			{
				AfxMessageBox("UUT1 Step1 : Send Barcode/Maccode for duplication check.PASS!...OK1");
				//ptr->SetDlgItemText(IDC_SHOW , "Step1 : Send Barcode/Maccode for duplication check.PASS!...OK1");
				break;
			}
			else
			{
				Sleep(10);

				str = mess;
				//ptr->SetDlgItemText(IDC_SHOW , "FAIL! " + str);
				AfxMessageBox("UUT1 FAIL! " + str);
				return 1;
			}
		}
	}

	Sleep(1500);

	//ptr->SetDlgItemText(IDC_SHOW , "Step1 : Send Barcode/Maccode for duplication check.PASS!...OK1     Step2 : Send Test Result.");

	str = m_FixtureID + "," + m_Bar + ",2," + m_Employee + "," + m_LineNO + "," + m_Mac  + "," + m_Result ;

	bytesSent = send(g_Socket1 , str , str.GetLength() , 0);
	
	str = "OK2";

	while(1)
	{
		if(recv(g_Socket1 , mess , 20 , 0))
		{
			if((memcmp(str , mess , 3)) == 0)
			{
				//AfxMessageBox("Step1 : Send Barcode/Maccode for duplication check.PASS!...OK1     Step2 : Send Test result. PASS!...OK2                   (ALL PASS!)");
				//ptr->SetDlgItemText(IDC_SHOW , "Step1 : Send Barcode/Maccode for duplication check.PASS!...OK1     Step2 : Send Test result. PASS!...OK2                   (ALL PASS!)");
				AfxMessageBox("UUT1 ALL PASS");
				return 0;
			}
			else
			{
				Sleep(10);
									
				str = mess;
				//ptr->SetDlgItemText(IDC_SHOW , "FAIL! " + str);
				AfxMessageBox("UUT1 FAIL! " + str);
				return 1;
			}
		}
	}
	*/
	return 1;
}

void CDIODlg::OnRs232send() 
{
	// TODO: Add your command handler code here
	//unsigned long len;

	//WriteFile(handlePort_, m_Str , m_Str.GetLength() , &len , NULL);
}

void CDIODlg::OnLpttest() 
{
	// TODO: Add your command handler code here
	/*
	Out32(LPTBaseIO , 0x55);
	Sleep(3000);
	Out32(LPTBaseIO , 0xaa);
	Sleep(3000);
	Out32(LPTBaseIO , 0xff);
	*/
}

void CDIODlg::OnUUT() 
{
	// TODO: Add your command handler code here
	OnNotifyUUT();
	OnBtnExit();
}

void CDIODlg::OnUUTClosed()
{
    //AfxMessageBox("UUT Closed");
}

void CDIODlg::OnNotifyUUT() 
{
	// TODO: Add your command handler code here
/*
	KillTimer(2);

	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");

	if (TrackUUT1.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT1");
		TrackUUT1.CloseDialog();
		m_IniReader.setKey("" , "UUT1BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT1MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT1GUID" , "BARMAC");
	}
	if (TrackUUT2.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT2");
		TrackUUT2.CloseDialog();
		m_IniReader.setKey("" , "UUT2BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT2MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT2GUID" , "BARMAC");
	}
	if (TrackUUT3.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT3");
		TrackUUT3.CloseDialog();
		m_IniReader.setKey("" , "UUT3BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT3MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT3GUID" , "BARMAC");
	}
	if (TrackUUT4.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT4");
		TrackUUT4.CloseDialog();
		m_IniReader.setKey("" , "UUT4BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT4MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT4GUID" , "BARMAC");
	}
	if (TrackUUT5.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT5");
		TrackUUT5.CloseDialog();
		m_IniReader.setKey("" , "UUT5BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT5MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT5GUID" , "BARMAC");
	}
	if (TrackUUT6.IsAlreadyPopped())
	{
		//AfxMessageBox("Detect UUT4");
		TrackUUT6.CloseDialog();
		m_IniReader.setKey("" , "UUT6BAR" , "BARMAC");
		m_IniReader.setKey("" , "UUT6MAC" , "BARMAC");
		m_IniReader.setKey("" , "UUT6GUID" , "BARMAC");
	}
*/
}

void CDIODlg::ScanBarcode()
{
	/*
	m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	m_Str = m_IniReader.getKeyValue( "UUTMode" , "DEBUG");

    if(m_Str.Compare("Enable") == 0)
	{
		CBarcode BARdlg(this);
		
		int k = BARdlg.DoModal();

		if(k == IDOK)
		{
			if (!(TrackUUT1.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT1");
				UUTLog* pnewdlg = new UUTLog(TrackUUT1);	// note the passing of the tracker.
				pnewdlg->Create(pnewdlg->IDD,NULL);	
				pnewdlg->SetWindowPos(NULL,0,0,1024/3,768/2,0);
				UUTLog& dlg = (UUTLog&)*TrackUUT1.GetDlg();

				dlg.ActiveUUTLog = TRUE;
				dlg.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg._this = this;

				m_IniReader.setKey(BARdlg.m_Bar , "UUT1BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT1MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT1GUID" , "BARMAC");

				dlg.StartUUT1();

				return;

			}
			if (!(TrackUUT2.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT2");
				UUTLog* pnewdlg1 = new UUTLog(TrackUUT2);	// note the passing of the tracker.
				pnewdlg1->Create(pnewdlg1->IDD,NULL);	
				pnewdlg1->SetWindowPos(NULL,342,0,1024/3,768/2,0);
				UUTLog& dlg1 = (UUTLog&)*TrackUUT2.GetDlg();

				dlg1.ActiveUUTLog = TRUE;
				dlg1.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg1.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg1.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg1._this = this;

				m_IniReader.setKey(BARdlg.m_Bar , "UUT2BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT2MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT2GUID" , "BARMAC");

				dlg1.StartUUT2();

				return;
			}
			if (!(TrackUUT3.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT3");
				UUTLog* pnewdlg1 = new UUTLog(TrackUUT3);	// note the passing of the tracker.
				pnewdlg1->Create(pnewdlg1->IDD,NULL);	
				pnewdlg1->SetWindowPos(NULL,683,0,1024/3,768/2,0);
				UUTLog& dlg1 = (UUTLog&)*TrackUUT3.GetDlg();

				dlg1.ActiveUUTLog = TRUE;
				dlg1.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg1.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg1.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg1._this = this;

				m_IniReader.setKey(BARdlg.m_Bar , "UUT3BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT3MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT3GUID" , "BARMAC");
				dlg._this = this;

				dlg1.StartUUT3();

				return;
			}
			if (!(TrackUUT4.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT4");
				UUTLog* pnewdlg1 = new UUTLog(TrackUUT4);	// note the passing of the tracker.
				pnewdlg1->Create(pnewdlg1->IDD,NULL);	
				pnewdlg1->SetWindowPos(NULL,0,385,1024/3,768/2,0);
				UUTLog& dlg1 = (UUTLog&)*TrackUUT4.GetDlg();

				dlg1.ActiveUUTLog = TRUE;
				dlg1.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg1.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg1.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg1._this = this;
				
				m_IniReader.setKey(BARdlg.m_Bar , "UUT4BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT4MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT4GUID" , "BARMAC");

				dlg1.StartUUT4();

				return;
			}
			if (!(TrackUUT5.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT5");
				UUTLog* pnewdlg1 = new UUTLog(TrackUUT5);	// note the passing of the tracker.
				pnewdlg1->Create(pnewdlg1->IDD,NULL);	
				pnewdlg1->SetWindowPos(NULL,342,385,1024/3,768/2,0);
				UUTLog& dlg1 = (UUTLog&)*TrackUUT5.GetDlg();

				dlg1.ActiveUUTLog = TRUE;
				dlg1.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg1.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg1.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg1._this = this;
				
				m_IniReader.setKey(BARdlg.m_Bar , "UUT5BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT5MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT5GUID" , "BARMAC");

				dlg1.StartUUT5();

				return;
			}
			if (!(TrackUUT6.IsAlreadyPopped()))
			{
				//AfxMessageBox("Detect UUT6");
				UUTLog* pnewdlg1 = new UUTLog(TrackUUT6);	// note the passing of the tracker.
				pnewdlg1->Create(pnewdlg1->IDD,NULL);	
				pnewdlg1->SetWindowPos(NULL,683,385,1024/3,768/2,0);
				UUTLog& dlg1 = (UUTLog&)*TrackUUT6.GetDlg();

				dlg1.ActiveUUTLog = TRUE;
				dlg1.m_Bar2 = "BAR: " + BARdlg.m_Bar;
				dlg1.m_Mac2 = "MAC: " + BARdlg.m_Mac;
				dlg1.m_GUID = "GUID: " + BARdlg.m_GUID;
				dlg1._this = this;
				
				m_IniReader.setKey(BARdlg.m_Bar , "UUT6BAR" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_Mac , "UUT6MAC" , "BARMAC");
				m_IniReader.setKey(BARdlg.m_GUID , "UUT6GUID" , "BARMAC");

				dlg1.StartUUT6();

				return;
			}
		}
	}
	*/
}

void CDIODlg::OnBattTest() 
{
	// TODO: Add your command handler code here

	//SetDlgItemText(IDC_PASSFAIL , "Battery Test");

	//ShellExecute(NULL , "open" , "d:\\usinote\\pm.js" , NULL , NULL , SW_SHOWNORMAL);
}

void CDIODlg::DisableKbd()
{
	////DIG_OUT_Prt(1, 7, 0);
	////DIG_OUT_Prt(1, 11, 243);
}

void CDIODlg::OnNextpage() 
{
	// TODO: Add your command handler code here
	GotoNextPage();
}

void CDIODlg::DisplayResult(int nItem , int result)
{
	CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

	if(result)
	{
		m_list->SetItemText(nItem , 3 , "PASS", RGB_BLUE, RGB_AZURE );
	}
	else
		m_list->SetItemText(nItem , 3 , "FAIL", RGB_RED, RGB_AZURE );
}

void CDIODlg::OnReTest() 
{
	// TODO: Add your control notification handler code here
	/*
	if(m_pLptThrd != NULL)
	{
		ActiveProcess = FALSE;
	
		while(WaitForSingleObject( m_pLptThrd , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
		//AfxMessageBox("TEST1");
	}

	if(serialProcess != NULL)
	{
		ActiveSerial = FALSE;

		while(WaitForSingleObject( serialProcess , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
		//AfxMessageBox("TEST2");
	}

	if(m_LPTThread != NULL)		
	{
		ActiveTest = 0;

		while(WaitForSingleObject( m_LPTThread , 0 ) == WAIT_TIMEOUT )
		{
			Sleep(500);
		}
	}

	Stop();

	SetDlgItemText(IDC_PASSFAIL , "");

	SetEnv();				// Initial INI file

	ClearListData();

	OnFileSharing();

	Start();
	*/

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	GetDlgItemText(IDC_RETEST , m_Str);

	if(m_Str.Compare("Stop") == 0) 
	{
		if (!m_Wave1.IsStopped())
		{
			m_Wave1.Stop();

			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("Stop Wav file\r\n");
		}
	}
	else if(m_Str.Compare("BTReverse") == 0)
	{
		//m_Edt->ReplaceSel("Reverse BT!\r\n");
		BTReverse();
	}
	else if(m_Str.Compare("Re-Test") == 0) 
	{
		//SetDlgItemText(IDC_BUTTON1 , "CrystalTrim");
		//SetDlgItemText(IDC_RETEST , "Re-Test");

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

		SetDlgItemText(IDC_PASSFAIL , "");
		m_PASS.StopBlink(CC_BLINK_BOTH);

		CTime t = CTime::GetCurrentTime();
		m_Str = "Current Time :";
		data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
		m_Edt->ReplaceSel(data);

		GetDlgItemText(IDC_BUTTON1 , m_Str);

		if(m_Str.Compare("CrystalTrim") == 0) 
		{
			m_Edt->ReplaceSel("CrystalTrim\r\n");

			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			Sleep(1000);
			
			CrystalTrimTest();
		}
		else if(m_Str.Compare("QuickTest") == 0)
		{
			m_Edt->ReplaceSel("QuickTest\r\n");

			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			Sleep(1000);
			
			QuickTest();
		}
		else if(m_Str.Compare("Script3Test") == 0)
		{
			if(RunScript3Test)
			{
				ClearListData();
				ListCtrlIndex = 0;
				ClearAllEditMessage();
				Sleep(1000);

				m_Edt->ReplaceSel("RF Test\r\n");
				m_MT8852Action = "Run";
				i_MT8852Script = 3;
				SetTimer(AutorunTimer , 500 , NULL);
			}
		}
		else if(m_Str.Compare("BlueFlash") == 0)
		{
			m_Edt->ReplaceSel("Download code!\r\n");
			SetDlgItemText(IDC_BUTTON1 , "BlueFlash");

			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
			Sleep(1000);

			if(CSRBlueFlash() != 1)
			{
				TestResultFlag = false;
				m_ResultStr = "FAIL";
			}
			else
			{
				TestResultFlag = true;
				m_ResultStr = "PASS";
				SetDlgItemText(IDC_RETEST , "Re-Test");
			}

			m_TestItemStr = "Download code";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
				
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("****************************************************\r\n");
			CTime t = CTime::GetCurrentTime();
			m_Str = "Current Time :";
			data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
			m_Edt->ReplaceSel(data);

			if(TestResultFlag)
				m_Edt->ReplaceSel("	Download code PASS!				                   \r\n");	
			else
				m_Edt->ReplaceSel("	Download code FAIL!				                   \r\n");
			m_Edt->ReplaceSel("****************************************************\r\n");
		}
		else if(m_Str.Compare("Burn BD") == 0)
		{
			GetDlgItemText(IDC_PASSFAIL , m_Str);

			if(m_Str.Compare("") != 0)
			{
				m_PASS.StopBlink(CC_BLINK_BOTH);
				SetDlgItemText(IDC_PASSFAIL , "");
				ClearListData();
				ListCtrlIndex = 0;
				ClearAllEditMessage();
				Sleep(1000);
				ReadBHC302INIData();
			}

			m_Edt->ReplaceSel("Wait MPT Start command!\r\n");

			m_TestProcess = MPT_Start;	
		}
	}
}

void CDIODlg::ClearListData()
{
	int nItem;
	CXListCtrl *m_list = (CXListCtrl*)GetDlgItem(IDC_LIST1);

	for (nItem = 0; nItem < 200 ; nItem++)  
	{
		m_list->SetItemText(nItem , 3 , "", RGB_BLACK , RGB_AZURE );
		m_list->SetItemText(nItem , 1 , "", RGB(0,0,0), RGB_WHITE);
		m_list->SetItemText(nItem, 4 , "" , RGB(0,0,0) ,RGB_AZURE);
	}
}

void CDIODlg::OnFCTExit() 
{
	// TODO: Add your command handler code here
	//OnNotifyUUT();
	
	//OnBtnExit();
}

void CDIODlg::OpenSixUUT()
{
/*
	if (!(TrackUUT1.IsAlreadyPopped()))				// If not create Window
	{
		m_Str = m_IniReader.getKeyValue( "UUT1" , "UUTSTATE");
		
		//if(m_Str.Compare("") == 0 && !(TrackSFIS1.IsAlreadyPopped()))
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT1");
			UUTLog* pnewdlg = new UUTLog(TrackUUT1);	// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			//pnewdlg->SetWindowPos(&this->wndTopMost,0,0,1024/3,768/2,0);//Keep always in the top window
			pnewdlg->SetWindowPos(0,0,0,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackUUT1.GetDlg();
		
			dlg.ActiveUUTLog = TRUE;
			dlg.ActiveUUTLog1 = FALSE;
			dlg.m_Bar2 = "BAR: ";
			dlg.m_Mac2 = "MAC: ";
			dlg.m_GUID = "GUID: ";
			dlg._this = this;

			dlg.InitialUUT1();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0))
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0))
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT1.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT1();
			}
		}
	}

	if (!(TrackUUT2.IsAlreadyPopped()))
	{
		m_Str = m_IniReader.getKeyValue( "UUT2" , "UUTSTATE");
		
		//if(m_Str.Compare("") == 0 && !(TrackSFIS1.IsAlreadyPopped()))
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT2");
			UUTLog* pnewdlg1 = new UUTLog(TrackUUT2);	// note the passing of the tracker.
			pnewdlg1->Create(pnewdlg1->IDD,NULL);				//pnewdlg1->SetWindowPos(&this->wndTopMost,342,0,1024/3,768/2,0);
			pnewdlg1->SetWindowPos(0,342,0,1024/3,768/2,0);
			UUTLog& dlg1 = (UUTLog&)*TrackUUT2.GetDlg();

			dlg1.ActiveUUTLog = TRUE;
			dlg1.ActiveUUTLog2 = FALSE;
			dlg1.m_Bar2 = "BAR: ";
			dlg1.m_Mac2 = "MAC: ";
			dlg1.m_GUID = "GUID: ";
			dlg1._this = this;

			dlg1.InitialUUT2();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0))	
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0))	
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT2.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT2();
			}
		}
	}

	if (!(TrackUUT3.IsAlreadyPopped()))
	{
		m_Str = m_IniReader.getKeyValue( "UUT3" , "UUTSTATE");
		
		//if(m_Str.Compare("") == 0 && !(TrackSFIS1.IsAlreadyPopped()))
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT3");
			UUTLog* pnewdlg1 = new UUTLog(TrackUUT3);	// note the passing of the tracker.
			pnewdlg1->Create(pnewdlg1->IDD,NULL);	
			//pnewdlg1->SetWindowPos(&this->wndTopMost,683,0,1024/3,768/2,0);
			pnewdlg1->SetWindowPos(0,683,0,1024/3,768/2,0);
			UUTLog& dlg1 = (UUTLog&)*TrackUUT3.GetDlg();

			dlg1.ActiveUUTLog = TRUE;
			dlg1.ActiveUUTLog3 = FALSE;
			dlg1.m_Bar2 = "BAR: ";
			dlg1.m_Mac2 = "MAC: ";
			dlg1.m_GUID = "GUID: ";
			dlg1._this = this;

			dlg1.InitialUUT3();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0))
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0))
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT3.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT3();
			}
		}
	}

	if (!(TrackUUT4.IsAlreadyPopped()))
	{
		m_Str = m_IniReader.getKeyValue( "UUT4" , "UUTSTATE");
		
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT4");
			UUTLog* pnewdlg1 = new UUTLog(TrackUUT4);	// note the passing of the tracker.
			pnewdlg1->Create(pnewdlg1->IDD,NULL);	
			//pnewdlg1->SetWindowPos(&this->wndTopMost,0,385,1024/3,768/2,0);
			pnewdlg1->SetWindowPos(0,0,385,1024/3,768/2,0);
			UUTLog& dlg1 = (UUTLog&)*TrackUUT4.GetDlg();

			dlg1.ActiveUUTLog = TRUE;
			dlg1.ActiveUUTLog4 = FALSE;
			dlg1.m_Bar2 = "BAR: ";
			dlg1.m_Mac2 = "MAC: ";
			dlg1.m_GUID = "GUID: ";
			dlg1._this = this;

			dlg1.InitialUUT4();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0))
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0))
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT4.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT4();
			}
		}
	}

	if (!(TrackUUT5.IsAlreadyPopped()))
	{
		m_Str = m_IniReader.getKeyValue( "UUT5" , "UUTSTATE");
		
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT5");
			UUTLog* pnewdlg1 = new UUTLog(TrackUUT5);	// note the passing of the tracker.
			pnewdlg1->Create(pnewdlg1->IDD,NULL);	
			pnewdlg1->SetWindowPos(0,342,385,1024/3,768/2,0);
			UUTLog& dlg1 = (UUTLog&)*TrackUUT5.GetDlg();

			dlg1.ActiveUUTLog = TRUE;
			dlg1.ActiveUUTLog5 = FALSE;
			dlg1.m_Bar2 = "BAR: ";
			dlg1.m_Mac2 = "MAC: ";
			dlg1.m_GUID = "GUID: ";
			dlg1._this = this;

			dlg1.InitialUUT5();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0))
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0))
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT5.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT5();
			}
		}
	}

	if (!(TrackUUT6.IsAlreadyPopped()))
	{
		m_Str = m_IniReader.getKeyValue( "UUT6" , "UUTSTATE");
		
		//if(m_Str.Compare("") == 0 && !(TrackSFIS1.IsAlreadyPopped()))
		if(m_Str.Compare("") == 0)
		{
			//AfxMessageBox("Detect UUT6");
			UUTLog* pnewdlg1 = new UUTLog(TrackUUT6);	// note the passing of the tracker.
			pnewdlg1->Create(pnewdlg1->IDD,NULL);	
			//pnewdlg1->SetWindowPos(&this->wndTopMost,683,385,1024/3,768/2,0);
			pnewdlg1->SetWindowPos(0,683,385,1024/3,768/2,0);
			UUTLog& dlg1 = (UUTLog&)*TrackUUT6.GetDlg();

			dlg1.ActiveUUTLog = TRUE;
			dlg1.ActiveUUTLog6 = FALSE;
			dlg1.m_Bar2 = "BAR: ";
			dlg1.m_Mac2 = "MAC: ";
			dlg1.m_GUID = "GUID: ";
			dlg1._this = this;

			dlg1.InitialUUT6();
		}
	}
	else
	{
		if(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0))
		{
			if(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0))
			{
				UUTLog& dlg = (UUTLog&)*TrackUUT6.GetDlg();
				//dlg.ActiveUUTLog = TRUE;
				dlg._this = this;
				dlg.StartUUT6();
			}
		}
	}

	//m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.ini");
	
	m_Str = m_IniReader.getKeyValue( "UUT1" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT1","UUTSTATE");
		TrackUUT1.CloseDialog();
		//AfxMessageBox("UUT1 Closed!");
	}
    else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT1.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT01.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT01.LOG" , 0)))
				{
					TrackUUT1.CloseDialog();
				}
			}
		}
	}
    else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT1","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT1","UUTSTATE");

			TrackUUT1.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);					// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,0,0,1024/3,768/2,0);//Keep always in the top window
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT1BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT1MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT1GUID" , "BARMAC");
			dlg.InitialSFIS1();
			dlg.ConnectSFIS();
		}
	}
 
	m_Str = m_IniReader.getKeyValue( "UUT2" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT2","UUTSTATE");
		TrackUUT2.CloseDialog();
		//AfxMessageBox("UUT2 Closed!");
	}
    else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT2.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT02.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT02.LOG" , 0)))
				{
					TrackUUT2.CloseDialog();
				}
			}
		}
	}
	else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT2","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT2","UUTSTATE");

			TrackUUT2.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);	// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,342,0,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT2BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT2MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT2GUID" , "BARMAC");
			dlg.InitialSFIS2();
			dlg.ConnectSFIS();
		}
	}

	m_Str = m_IniReader.getKeyValue( "UUT3" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT3","UUTSTATE");
		TrackUUT3.CloseDialog();
		//AfxMessageBox("UUT3 Closed!");
	}
	else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT3.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT03.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT03.LOG" , 0)))
				{
					TrackUUT3.CloseDialog();
				}
			}
		}
	}
	else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT3","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT3","UUTSTATE");

			TrackUUT3.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);					// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,683,0,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT3BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT3MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT3GUID" , "BARMAC");
			dlg.InitialSFIS3();
			dlg.ConnectSFIS();
		}
	}

	m_Str = m_IniReader.getKeyValue( "UUT4" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT4","UUTSTATE");
		TrackUUT4.CloseDialog();
		//AfxMessageBox("UUT4 Closed!");
	}
	else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT4.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT04.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT04.LOG" , 0)))
				{
					TrackUUT4.CloseDialog();
				}
			}
		}
	}
	else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT4","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT4","UUTSTATE");

			TrackUUT4.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);	// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,0,385,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT4BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT4MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT4GUID" , "BARMAC");
			dlg.InitialSFIS4();
			dlg.ConnectSFIS();
		}
	}

	m_Str = m_IniReader.getKeyValue( "UUT5" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT5","UUTSTATE");
		TrackUUT5.CloseDialog();
		//AfxMessageBox("UUT5 Closed!");
	}
    else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT5.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT05.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT05.LOG" , 0)))
				{
					TrackUUT5.CloseDialog();
				}
			}
		}
	}
	else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT5","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT5","UUTSTATE");

			TrackUUT5.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);	// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,342,385,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT5BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT5MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT5GUID" , "BARMAC");
			dlg.InitialSFIS5();
			dlg.ConnectSFIS();
		}
	}

	m_Str = m_IniReader.getKeyValue( "UUT6" , "UUTSTATE");

	if(m_Str.Compare("Restart") == 0)
	{
		m_IniReader.setKey("","UUT6","UUTSTATE");
		TrackUUT6.CloseDialog();
		//AfxMessageBox("UUT6 Closed!");
	}
	else if(m_Str.Compare("CLOSE") == 0)
	{
		if ((TrackUUT6.IsAlreadyPopped()))
		{
			if(!(findfile.FindFile("D:\\USINOTE\\UUT06.ERR" , 0)))
			{
				if(!(findfile.FindFile("D:\\USINOTE\\UUT06.LOG" , 0)))
				{
					TrackUUT6.CloseDialog();
				}
			}
		}
	}
	else if(m_Str.Compare("TEST END") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))	
		{
			m_IniReader.setKey("","UUT6","UUTSTATE");
			//AfxMessageBox("UUT1 Test End!");
			m_IniReader.setKey("SFIS","UUT6","UUTSTATE");

			TrackUUT6.CloseDialog();
		}
	}

	if(m_Str.Compare("SFIS") == 0)
	{
		if (!(TrackSFIS1.IsAlreadyPopped()))
		{
			UUTLog* pnewdlg = new UUTLog(TrackSFIS1);	// note the passing of the tracker.
			pnewdlg->Create(pnewdlg->IDD,NULL);	
			pnewdlg->SetWindowPos(&this->wndTopMost,683,385,1024/3,768/2,0);
			UUTLog& dlg = (UUTLog&)*TrackSFIS1.GetDlg();
			dlg._this = this;
			dlg.m_Bar2 = m_IniReader.getKeyValue( "UUT6BAR" , "BARMAC");
			dlg.m_Mac2 = m_IniReader.getKeyValue( "UUT6MAC" , "BARMAC");
			dlg.m_GUID = m_IniReader.getKeyValue( "UUT6GUID" , "BARMAC");
			dlg.InitialSFIS6();
			dlg.ConnectSFIS();
		}
	}

	m_Str = m_IniReader.getKeyValue( "RESULT" , "SFIS");

	if(m_Str.Compare("END") == 0)
	{
		TrackSFIS1.CloseDialog();
		m_IniReader.setKey("","RESULT","SFIS");
	}
*/
}

void CDIODlg::RunTest()
{

}

void CDIODlg::CloseUUT1() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT1.CloseDialog();
	m_IniReader.setKey("" , "UUT1BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT1MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT1GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT01.LOG");
	DeleteFile("D:\\USINOTE\\UUT01.ERR");
*/
}

void CDIODlg::CloseUUT2() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT2.CloseDialog();
	m_IniReader.setKey("" , "UUT2BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT2MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT2GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT02.LOG");
	DeleteFile("D:\\USINOTE\\UUT02.ERR");
*/
}

void CDIODlg::CloseUUT3() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT3.CloseDialog();
	m_IniReader.setKey("" , "UUT3BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT3MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT3GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT03.LOG");
	DeleteFile("D:\\USINOTE\\UUT03.ERR");
*/
}

void CDIODlg::CloseUUT4() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT4.CloseDialog();
	m_IniReader.setKey("" , "UUT4BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT4MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT4GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT04.LOG");
	DeleteFile("D:\\USINOTE\\UUT04.ERR");
*/
}

void CDIODlg::CloseUUT5() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT5.CloseDialog();
	m_IniReader.setKey("" , "UUT5BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT5MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT5GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT05.LOG");
	DeleteFile("D:\\USINOTE\\UUT05.ERR");
*/
}

void CDIODlg::CloseUUT6() 
{
/*
	// TODO: Add your command handler code here
	TrackUUT6.CloseDialog();
	m_IniReader.setKey("" , "UUT6BAR" , "BARMAC");
	m_IniReader.setKey("" , "UUT6MAC" , "BARMAC");
	m_IniReader.setKey("" , "UUT6GUID" , "BARMAC");
	DeleteFile("D:\\USINOTE\\UUT06.LOG");
	DeleteFile("D:\\USINOTE\\UUT06.ERR");
*/
}

void CDIODlg::OpenSFIS1()
{
	//TrackSFIS1.CloseDialog();
}

BOOL CDIODlg::CheckLAN()
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
		AfxMessageBox("Create Process Fail!");
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

	//m_IniReader.setINIFileName("D:\\USINOTE\\TAURUS.INI");
	//m_Str = m_IniReader.getKeyValue( "IP" , "SFIS");

	//if(PingTest(m_Str))
	//	return TRUE;
	//else
		return FALSE;
}

void CDIODlg::SetUUT1()
{
/*
	SetUUT dlg;

	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		if (TrackUUT1.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 0 , 0 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}

		if (TrackUUT2.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 342 , 0 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}

		if (TrackUUT3.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 683 , 0 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}

		if (TrackUUT4.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 0 , 385 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}

		if (TrackUUT5.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 342 , 385 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}

		if (TrackUUT6.IsAlreadyPopped())	
		{
			SetWindowPos(0 , 683 , 385 , 150 , 140 , SWP_SHOWWINDOW);
			return;
		}
	}
*/
}

LONG CDIODlg::OnCommunication(WPARAM ch , LPARAM port)
{
	int i,len;
	CString tmp;
	CString tmp1;
	unsigned char *str;
	unsigned char buff[16];
	unsigned char BA;
	int Chksum = 0;
	bool ChkComdHeader = false;
	bool InvalidCommand = true;
	float pwr = 0;

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if (port == i_MT8852ComPort)	////receive MT8852 return value
	{
		if (ch == 10)
		{
			if (m_M8852StrReceived.Find(MT8852_Op_str) != -1)
			{
				KillTimer(M8852EchoInstTimer);
				Msg = WM_MY_MESSAGE_M8852_GET_RESULT;
				
				//m_Log.ReplaceSel("aa");

				PostMessage(Msg,0,0);
			}
			else
			{
				m_M8852StrReceived.Empty();
			}
		}
		else 
		{
			m_M8852StrReceived += (TCHAR) ch;

			if(m_TestProcess == T21_CW_Measurement)
			{
				//R-7.94e+001
				if(m_M8852StrReceived.GetLength() == 11)
				{
					m_Str = m_M8852StrReceived.Right(10);
					//m_Edt->ReplaceSel(m_M8852StrReceived);
					tmp = m_Str.Left(6);
					tmp1 = m_Str.Right(3);
					//m_Edt->ReplaceSel(m_Str);
					//m_Edt->ReplaceSel(tmp);
					//m_Edt->ReplaceSel(tmp1);
					m_Str = tmp + tmp1;
					m_Edt->ReplaceSel(m_Str);
					pwr = atof(m_Str);
					if((pwr > PowerMax) || (pwr < PowerMin))
					{
						T21_Fail++;
						m_Edt->ReplaceSel(" fail!");	
					}
					else
						m_Edt->ReplaceSel(" pass!");	
				}
			}
		}
	}
	else if(port == i_MPTComPort)
	{
			MPT_Recv_string += (TCHAR) ch;
		
#ifdef UTF001
			//RS232 = COM1/57600
			m_Edt->ReplaceSel(MPT_Recv_string);
			MPT_Recv_string = "";
#else
			if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T2") == 0 || m_TestStation.Compare("T1") == 0 || m_TestStation.Compare("T3") == 0)
			{
				if(m_TestProcess == MPT_SentRealBAData)
				{
					//i = MPT_Recv_string.GetLength();

					if(RecRealBTLen == 0)
						KillTimer(T13RecRealBTData);

					//m_Str.Format("%d\r\n",RecRealBTLen);
					//m_Edt->ReplaceSel(m_Str);

					RealBT[RecRealBTLen] = (unsigned char)ch;
					RecRealBTLen++;				

					if(RecRealBTLen == 6)
					{
						m_TestProcess = 0;
						//m_Edt->ReplaceSel("Bye!!\r\n");

						//str = (unsigned char*)MPT_Recv_string.GetBuffer(len);

						//for(i = 0; i < 6 ; i++)
						//	buff[i] = *(str+i);

						tmp = "";
						for(i = 0 ; i < 6 ; i++)
						{
							if(RealBT[i] < 16)
							{
								m_Str.Format("0%X",RealBT[i]);
								tmp += m_Str;
							}
							else
							{
								m_Str.Format("%X",RealBT[i]);
								tmp += m_Str;
							}
						}
						m_Edt->ReplaceSel("Read real BT from MPT : " + tmp);
						m_Edt->ReplaceSel("\r\n");
						MPT_RealBT = tmp;

						SetDlgItemText(IDC_STATIC2 , "BD : " + MPT_RealBT);//BD

						m_TestItemStr = "Get Real BT";
						m_ResultStr = "PASS";
						TestErrorCode++;
						m_Str.Format("%X",TestErrorCode);
						m_ErrorCodeStr = m_Str;
						AddListCtrlItem(m_List , ListCtrlIndex);
						ListCtrlIndex++;
										
						if(m_TestStation.Compare("T1.3") == 0)
						{
							//buff[0] = 0xf0;
							//buff[1] = 0x03;
							//buff[2] = PCMPT_ACK;
							//buff[3] = 0x03+PCMPT_ACK;
							//Sleep(250);

							//MPTCOM.WriteBufferToPort(buff , 4);
										
							SetTimer(T13WriteRealBT , T13WriteBTime , NULL);
						}
						else if(m_TestStation.Compare("S4") == 0)
						{
							//buff[0] = 0xf0;
							//buff[1] = 0x03;
							//buff[2] = PC_ProcessEnd;
							//buff[3] = 0x03+PC_ProcessEnd;
		
							//MPTCOM.WriteBufferToPort(buff , 4);

							//pSplash->Hide();

							//Record SN
							SetTimer(S4RecordTimer , S4RecordTime , NULL);
						}
						else if(m_TestStation.Compare("T2") == 0 || m_TestStation.Compare("T3") == 0)
						{
							///*
							m_Edt->ReplaceSel("Get real BT from NFC Tag and set to DUT\r\n");

							m_Str.Format("%d",RealBT[0]);
							m_IniReader.setKey(m_Str,"BD_5","BTApp");
							m_Str.Format("%d",RealBT[1]);
							m_IniReader.setKey(m_Str,"BD_4","BTApp");
							m_Str.Format("%d",RealBT[2]);
							m_IniReader.setKey(m_Str,"BD_3","BTApp");
							m_Str.Format("%d",RealBT[3]);
							m_IniReader.setKey(m_Str,"BD_2","BTApp");
							m_Str.Format("%d",RealBT[4]);
							m_IniReader.setKey(m_Str,"BD_1","BTApp");
							m_Str.Format("%d",RealBT[5]);
							m_IniReader.setKey(m_Str,"BD_0","BTApp");
							//*/

							//m_TestProcess = MPT_SentRealBAData;

							if(m_TestStation.Compare("T2") == 0)
								m_TestProcess = Audio_Linein;
							else
							{
								m_TestProcess = SetWndPos;
							}
							
							SetTimer(AckTimer , 100 , NULL);
						}

						MPT_Recv_string = "";

						return 1;
					}
					else
						return 1;
				}
				else if(m_TestProcess == PCMPT_Abort)
				{
					MPTErrorCode[MPTErrorLen] = (unsigned char)ch;
					MPTErrorLen++;

					if(MPTErrorLen == 2)
					{
						m_TestProcess = 0;

						tmp = "";
						for(i = 0 ; i < 2 ; i++)
						{
							if(MPTErrorCode[i] < 16)
							{
								m_Str.Format("0%X",MPTErrorCode[i]);
								tmp += m_Str;
							}
							else
							{
								m_Str.Format("%X",MPTErrorCode[i]);
								tmp += m_Str;
							}
						}

						m_Edt->ReplaceSel("MPT ErrorCode : " + tmp);
						m_Edt->ReplaceSel("\r\n");
						MPT_ErrorCode = tmp;
						
						m_TestItemStr = "MPT Abort";
						m_ResultStr = "FAIL";
						//TestErrorCode++;
						//m_Str.Format("%x",TestErrorCode);
						m_ErrorCodeStr = MPT_ErrorCode;
						AddListCtrlItem(m_List , ListCtrlIndex);
						ListCtrlIndex++;

						//GetDlgItemText(IDC_PASSFAIL , m_Str);
						SetDlgItemText(IDC_PASSFAIL , "FAIL : " + tmp);

						rgbText = RGB_RED;
						rgbBkgnd = RGB_GRAY1;
		
						m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
						m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

						if(m_TestStation.Compare("T2") == 0)
						{
							if(ShowPic)
							{
								//m_Edt->ReplaceSel("SplashScreen Hide\r\n");
								//pSplash->Hide();
								//SetTimer(SplashScreenHide , SplashScreenHidet , NULL);
							}
						}

						m_TestProcess = MPT_Start;

						if(MPT_ErrorCode.Compare("FFA0") == 0 && m_TestStation.Compare("T1.3") == 0)
						{
							if(m_PromptMessage)
							{
							if(ToggleBitmap)
							{
								ToggleBitmap = false;
								pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
							}
							else
							{
								ToggleBitmap = true;
								pSplash->SetBitmap(IDB_SPLASH,255,0,255);
							}
							SplashScreenString = "NFC Tag感應失敗";
							pSplash->SetText(SplashScreenString);
							}
						}
					}
				}
			}

			if(m_TestProcess == MPT_DataStreamStart)
			{
				//if(m_TestProcess != MPT_DataStreamEnd)
				//	m_TestProcess = MPT_DataStreamEnd;

				if(ch < 16)
				{
					//m_Str.Format("0%X  %d\r\n", (unsigned char)ch,MPT_DataStreamLen);
					m_Str.Format("0%X ", (unsigned char)ch,MPT_DataStreamLen);
				}
				else
				{
					//m_Str.Format("%X  %d\r\n", (unsigned char)ch,MPT_DataStreamLen);
					m_Str.Format("%X ", (unsigned char)ch,MPT_DataStreamLen);
				}

				MPTDataBuff[MPTDataIndex] = (unsigned char)ch;

				//m_Edt->ReplaceSel(m_Str);
				MPT_DataStream += m_Str;
				MPT_DataStreamLen++;
				MPTDataIndex++;

				//if(MPT_DataStreamLen == 19)
				if(MPT_DataStreamLen == 64)
				{
					MPT_DataStreamLen = 0;
					m_TestProcess = MPT_DataStreamEnd;
					MPT_Recv_string = "";
#ifdef MPT_Handshake_Message
					m_Edt->GetWindowText(m_Str);
					m_Edt->SetFocus();
					m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
					m_Edt->ReplaceSel("MPT Data stream:\r\n");
					m_Edt->ReplaceSel(MPT_DataStream + "\r\n");
					//MPT_DataStream = "";
#endif
				}
				//return 1;
			}			

			if(CommandHeader)
			{
				CommandHeader = false;
				CommandLength = ch;
#ifdef MPT_Handshake_Message
				//m_Str.Format("Comd len = %d\r\n",CommandLength);
				//m_Edt->ReplaceSel(m_Str);
#endif
			}

			if(ch == 0xF0)
			{
				CommandHeader = true;
				CommandLength = 0;
#ifdef MPT_Handshake_Message
				//m_Edt->ReplaceSel("F0 ");
#endif
			}

			if(CommandLength)
			{
				CommandLength--;
			}

#ifdef MPT_Handshake_Message
			/*
			if(CommandHeader == false)
			{
				if((ch&0xf0) == 0x00)
				{
					m_Edt->ReplaceSel("0");
					m_Str.Format("%c",(ch & 0x0f) + 0x30);
					m_Edt->ReplaceSel(m_Str);
					m_Edt->ReplaceSel(" ");
				}
				else if((ch&0x0f) == 0x00)
				{
					m_Str.Format("%c",(ch >> 4) + 0x30);
					m_Edt->ReplaceSel(m_Str);
					m_Edt->ReplaceSel("0");
					m_Edt->ReplaceSel(" ");
				}
				else if((ch&0xf0) != 0x00 && (ch&0x0f) != 0x00)
				{
					m_Str.Format("%c",(ch >> 4) + 0x30);
					m_Edt->ReplaceSel(m_Str);
					m_Str.Format("%c",(ch & 0x0f) + 0x30);
					m_Edt->ReplaceSel(m_Str);
					m_Edt->ReplaceSel(" ");
				}
			}
			*/
#endif

			if(CommandLength == 0 && CommandHeader == false)
			{
				CommandReady = true;

#ifdef MPT_Handshake_Message
				//m_Edt->ReplaceSel("Parse command!\r\n");
#endif
				//Parse Command			
				len = MPT_Recv_string.GetLength();

				str = (unsigned char*)MPT_Recv_string.GetBuffer(len);

				for(i = 0; i < len ; i++)
					buff[i] = *(str+i);
				
				for(i = 0; i < len ; i++)
				{
					buff[i] = *(str+i);

					if(i == 0)
					{
						if(*(str+i) == 0xF0)
						{
							ChkComdHeader = true;
	#ifdef MPT_Handshake_Message
							//m_Edt->ReplaceSel("F0\r\n");
	#endif
						}
						else
						{
							InvalidCommand = true;
							break;
						}
					}

					if(ChkComdHeader)
					{
						if(i != 0 && i < (len-1))//
						{
							Chksum += *(str+i);
	#ifdef MPT_Handshake_Message
							//m_Edt->ReplaceSel(".");
	#endif
						}

						//if(i == (len-2))
						if(i == (len-1))
						{
	#ifdef MPT_Handshake_Message
							//tmp.Format("Chksum = %x\r\n",Chksum);
							//m_Edt->ReplaceSel(tmp);
							//tmp.Format("%x\r\n",buff[len-2]);
							//m_Edt->ReplaceSel(tmp);
	#endif

							Chksum &= 0xff;
							if(Chksum == buff[len-1])
							{
								InvalidCommand = false;
	#ifdef MPT_Handshake_Message
								//m_Edt->ReplaceSel(" Chksum pass!\r\n");
	#endif
							}
							else
							{
								InvalidCommand = true;
	#ifdef MPT_Handshake_Message
								m_Edt->ReplaceSel(" Chksum fail!\r\n");
	#endif
							}
						}
					}
				}

				MPT_Recv_string.ReleaseBuffer();

				if(InvalidCommand == false)
				{
					switch(buff[2])//Command
					{
						case MPT_Start:
							//if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T2") == 0)
							//{
								if(m_TestProcess == MPT_Start)
								{
									m_Edt->ReplaceSel("MPT start! 40\r\n");

									GetDlgItemText(IDC_PASSFAIL , m_Str);

									if(m_Str.Compare("") != 0)
									{
										//OnStart();
										m_PASS.StopBlink(CC_BLINK_BOTH);
										SetDlgItemText(IDC_PASSFAIL , "");
										ClearListData();
										ListCtrlIndex = 0;
										ClearAllEditMessage();
										//Sleep(80);
										ReadBHC302INIData();
										TestResultFlag = true;

										if(m_TestStation.Compare("T2") == 0 || m_TestStation.Compare("T3") == 0)
										{
											//Initial BTApp status
											//m_IniReader.setKey("Run","Status","BTApp");//Terminate btapp.exe
											//m_IniReader.setKey("","Message","BTApp");
											
										}
									}

									if(m_TestStation.Compare("T2") == 0)
									{
										if(ShowPic)
										{
											//ShowWindow(SW_MINIMIZE);
											if(m_PromptMessage)
											{
												if(ToggleBitmap)
												{
													ToggleBitmap = false;
													pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
												}
												else
												{
													ToggleBitmap = true;
													pSplash->SetBitmap(IDB_SPLASH,255,0,255);
												}
												//SplashScreenString = "按壓電源鍵三秒,檢查是否開機,LED由白燈轉藍閃爍,播放開機音效,OK->綠色鍵;NG->紅色鍵";
												//SplashScreenString = "按壓電源鍵三秒,檢查是否開機,LED變成白燈,播放開機音效,OK->綠色鍵;NG->紅色鍵";
												//SplashScreenString = "按壓電源鍵三秒,檢查是否開機,LED變成白燈,播放開機音效,並確認電池充電燈\r\n是否亮紅燈,OK->綠色鍵;NG->紅色鍵";
												SplashScreenString = "按壓電源鍵三秒,檢查是否開機,LED變成白燈,播放開機音效,OK->綠色鍵(並確認電池充電燈是否亮紅燈);\r\nNG->紅色鍵";
												pSplash->SetBitmap(IDB_SPLASH1,0,0,0);
												pSplash->SetText(SplashScreenString);
											}
										}

										m_clock.ClearDate();	
										m_clock.StartTimer(); 

										//SplashScreenString = "按壓電源鍵三秒,檢查是否開機,LED由白燈轉藍閃爍,播放開機音效";
										//SetTimer(SplashScreenShow , SplashScreenShowt , NULL);
									}
									else if(m_TestStation.Compare("T1") == 0)
									{										
										m_clock.ClearDate();	
										m_clock.StartTimer(); 
									}
									else if(m_TestStation.Compare("T1.3") == 0)
									{
										///*
										if(m_PromptMessage)
										{
											if(ToggleBitmap)
											{
												ToggleBitmap = false;
												pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
											}
											else
											{
												ToggleBitmap = true;
												pSplash->SetBitmap(IDB_SPLASH,255,0,255);
											}
											SplashScreenString = "NFC Tag與主板 BT address配對中";
											pSplash->SetText(SplashScreenString);
										}
										//*/
									}

									SetTimer(AckTimer , 50 , NULL);
								}
								else
								{
									m_Str.Format("XX,%x\r\n",m_TestProcess);
									m_Edt->ReplaceSel(m_Str);
								}
							//}
							break;
						case MPT_SentRealBAComd:
							if(m_TestStation.Compare("T1.3") == 0 || m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T2") == 0 || m_TestStation.Compare("T3") == 0)
							{
								if(m_TestProcess == MPT_SentRealBAComd)
								{
									KillTimer(T13Timer1);

									if(m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T3") == 0)//Clear old data
									{
										GetDlgItemText(IDC_PASSFAIL , m_Str);

										if(m_Str.Compare("") != 0)
										{
											m_PASS.StopBlink(CC_BLINK_BOTH);
											SetDlgItemText(IDC_PASSFAIL , "");
											ClearListData();
											ListCtrlIndex = 0;
											ClearAllEditMessage();
											ReadBHC302INIData();
										}
									}

									m_TestProcess = MPT_SentRealBAData;

									tmp = "";
									RecRealBTLen = 0;
									//MPT_Recv_string = "";

									m_Edt->ReplaceSel("MPT_SentRealBAComd 45\r\n");
									
									TestErrorCode++;

									SetTimer(T13RecRealBTData , T13RecRealBTDataTime , NULL);

									if(m_TestStation.Compare("S4") == 0 || m_TestStation.Compare("T3") == 0)
										Sleep(300);
								}
							}
							//else if(m_TestStation.Compare("S4.0") == 0)
							//{								
							//}
							break;
						case PCMPT_ACK:
							if(m_TestStation.Compare("T1.3") == 0)
							{
								//m_Edt->ReplaceSel("MPT return Ack! Restart.. 60\r\n");
								m_TestProcess = MPT_Start;

								//m_Edt->ReplaceSel("MPT return Ack1!\r\n");

								/*
								if(m_TestProcess == PC_ProcessEnd || m_TestProcess == PCMPT_Abort)
								{
									m_Edt->ReplaceSel("MPT return Ack! Restart..\r\n");

									//OnStart();

									m_TestProcess = MPT_Start;
								}
								*/
							}
							else if(m_TestStation.Compare("T1") == 0)
							{
								if(m_TestProcess == PCRFTestEnd)
								{
									m_Edt->ReplaceSel("RFEnd,Rec Ack from MPT\r\n");
									CSRPIOTest();
								}
							}
							break;
						case PCMPT_Abort:
							if(m_TestStation.Compare("T1.3") == 0)
							{								
								m_TestProcess = PCMPT_Abort; 
								MPTErrorLen = 0;
								m_Edt->ReplaceSel("MPT Abort! 61\r\n");
							}
							else
							{
								m_TestProcess = PCMPT_Abort; 
								MPTErrorLen = 0;
								m_Edt->ReplaceSel("MPT Abort! 61\r\n");

								TestResultFlag = false;
							}
							break;
						case MPT_AudioPathReq:
							if(m_TestStation.Compare("T2") == 0)
							{
								m_Edt->ReplaceSel("Rec MPT_AudioPathReq 46\r\n");

#ifdef Support_Bluesoleil_SDK
								m_IniReader.setKey("Stop","Status","BTApp");//Terminate btapp.exe

								m_IniReader.setKey("","Message","BTApp");

								//m_IniReader.setKey("Run","Status","BTApp");
								//m_IniReader.setKey("","Message","BTApp");

								m_Edt->ReplaceSel("BT disconnect(btapp.exe)\r\n");
#endif

								//m_Edt->GetWindowText(m_Str);
								//m_Edt->SetFocus();
								//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
								//m_Edt->ReplaceSel("BTA2DP disconnect!\r\n");
								//SetDlgItemText(ID_BTN_EXIT , "Exit");

								//BTA2DPConnect = false;
								
								//m_Edt->ReplaceSel("Rec MPT_AudioPathReq\r\n");

								//m_TestProcess = RunBTA2DP;

								//SetTimer(AckTimer , 100 , NULL);

								if(m_PromptMessage)
								{
								if(ToggleBitmap)
								{
									ToggleBitmap = false;
									pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
								}
								else
								{
									ToggleBitmap = true;
									pSplash->SetBitmap(IDB_SPLASH,255,0,255);
								}
								//SplashScreenString = "音量+++鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";
								SplashScreenString = "音量---鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";//0424
								pSplash->SetText(SplashScreenString);
								}
							}
							break;
						case MPT_DataStreamStart:
							if(m_TestStation.Compare("T1") == 0 || m_TestStation.Compare("T2") == 0)
							{
								m_Edt->ReplaceSel("Rec MPT_DataStreamStart 43\r\n");
								MPT_DataStream = "";
								MPT_DataStreamLen = 0;
								MPTDataIndex = 0;
								m_TestProcess = MPT_DataStreamStart;

								if(m_TestStation.Compare("T2") == 0)
								{
									//pSplash->Hide();
									//ShowWindow(SW_SHOWNORMAL);
								}
							}
							break;
						case MPT_DataStreamEnd:
							if(m_TestStation.Compare("T1") == 0 || m_TestStation.Compare("T2") == 0)
							{
								if(m_TestProcess == MPT_DataStreamEnd)
								{
									m_Edt->ReplaceSel("Rec MPT_DataStreamEnd 44\r\n");
									//MPT_Recv_string = Data
									SetTimer(ProcessEndTimer , ProcessEndTime , NULL);
									
									//ExportFile();

									//if(m_TestStation.Compare("T2") == 0)
									//{
									//	m_TestProcess = MPT_Start;
									//	m_Edt->ReplaceSel("T2 restart!\r\n");
									//}
								}
							}
							break;
						case MPT_ConnectReq:
							if(m_TestStation.Compare("T1") == 0)
							{
								if(m_TestProcess == ConnectReqAck)
								{
									m_Edt->ReplaceSel("Rec MPT_ConnectReq 41\r\n");

									//SetTimer(ConnectReqAckTimer , ConnectReqAckTime , NULL);
									SetTimer(AckTimer , 30 , NULL);
								}
							}
							break;
						case T2_MOI_1://0x47
#ifdef T2DebugMode
							/*
							m_Edt->ReplaceSel("***** T2 debug mode *****\r\n");
							
							GenerateDummyBT();

							if(OpenTestEngine())
							{
								m_Edt->ReplaceSel("OpenTestEngine fail\r\n");
							}

							Sleep(50);

							WriteDUT_BDaddress(DummyBD);

							m_Edt->ReplaceSel("Write dummy BT fail\r\n");

							CloseTestEngine();
							*/
#endif
							if(m_PromptMessage)
							{
							if(ToggleBitmap)
							{
								ToggleBitmap = false;
								pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
							}
							else
							{
								ToggleBitmap = true;
								pSplash->SetBitmap(IDB_SPLASH,255,0,255);
							}
							//SplashScreenString = "將NFC感應器置於待測物上方,LED由白燈轉藍閃爍,播放開機音效,OK->按壓綠色鍵;NG->按壓紅色鍵";
							//SplashScreenString = "將NFC置於待測物上方直到嗶聲,壓BT鍵,燈號由白轉藍閃爍,OK->綠色鍵;NG->紅色鍵";
							SplashScreenString = "將NFC感應器置於待測物上方直到嗶聲,OK->綠色鍵;NG->紅色鍵";
							pSplash->SetText(SplashScreenString);
							}
							break;
						case T2_MOI_2://0x48
							//SplashScreenString = "";
							//pSplash->SetText(SplashScreenString);
							if(m_PromptMessage)
							{
								if(ToggleBitmap)
								{
									ToggleBitmap = false;
									pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
								}
								else
								{
									ToggleBitmap = true;
									pSplash->SetBitmap(IDB_SPLASH,255,0,255);
								}
								//SplashScreenString = "音量+++鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";
								
#ifndef T2_Combine_Vol_UpDn_Test
								SplashScreenString = "音量---鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";
#else
								SplashScreenString = "測試音量---鍵/+++鍵功能是否正常,OK->綠色鍵;NG->紅色鍵";										
#endif
								pSplash->SetText(SplashScreenString);
							}
							break;
						case T2_MOI_3://0x49
#ifndef T2_Combine_Vol_UpDn_Test
							if(m_PromptMessage)
							{
								if(ToggleBitmap)
								{
									ToggleBitmap = false;
									pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
								}
								else
								{
									ToggleBitmap = true;
									pSplash->SetBitmap(IDB_SPLASH,255,0,255);
								}
								//SplashScreenString = "音量---鍵功能是否正常,OK->按壓綠色鍵;NG->按壓紅色鍵";
								SplashScreenString = "音量+++鍵功能是否正常,OK->按壓綠色鍵;NG->按壓紅色鍵";
								pSplash->SetText(SplashScreenString);
							}
#endif
							break;
						case T2_MOI_4://0x4a
							/*
							if(ToggleBitmap)
							{
								ToggleBitmap = false;
								pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
							}
							else
							{
								ToggleBitmap = true;
								pSplash->SetBitmap(IDB_SPLASH,255,0,255);
							}
							//SplashScreenString = "將3.5mm接頭插入待測物,LED由藍轉白,OK->綠色鍵;NG->紅色鍵";
							SplashScreenString = "將3.5mm接頭插入待測物,檢查是否聽到聲音,OK->綠色鍵;NG->紅色鍵";
							pSplash->SetText(SplashScreenString);
							*/
							if(m_PromptMessage)
							{
							if(ToggleBitmap)
							{
								ToggleBitmap = false;
								pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
							}
							else
							{
								ToggleBitmap = true;
								pSplash->SetBitmap(IDB_SPLASH,255,0,255);
							}
							SplashScreenString = "按壓電源鍵三秒,檢查是否關機,LED紅色閃爍,播放關機音效,OK->綠色鍵;NG->紅色鍵";
							pSplash->SetText(SplashScreenString);
							}
							break;
						case T2_MOI_5://0x4b
							if (m_Wave1.IsPlaying())
							{
								if (!m_Wave1.IsStopped())
								{
									m_Wave1.Stop();
									m_Edt->ReplaceSel("Stop playing wave file!\r\n");
								}
							}
							if(m_PromptMessage)
							{
							if(ToggleBitmap)
							{
								ToggleBitmap = false;
								pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
							}
							else
							{
								ToggleBitmap = true;
								pSplash->SetBitmap(IDB_SPLASH,255,0,255);
							}
							SplashScreenString = "按壓電源鍵三秒,檢查是否關機,LED紅色閃爍,播放關機音效,OK->綠色鍵;NG->紅色鍵";
							pSplash->SetText(SplashScreenString);
							}
							break;
						case T2_MOI_6:							
							/*
							m_Str.Format("Rec MOI_%X\r\n",buff[2]);
							m_Edt->ReplaceSel(m_Str);
							if(ShowPic == true)
							{
								SetTimer(SplashScreenHide , SplashScreenHidet , NULL);
							}
							else
							{
								SetTimer(SplashScreenShow , SplashScreenShowt , NULL);
							}
							*/
							break;
						default:
							m_Str.Format("Unknown : %x\r\n",buff[2]);
							m_Edt->ReplaceSel(m_Str);
							break;
					}
				}
				else
				{
	#ifdef MPT_Handshake_Message
					//m_Edt->ReplaceSel("Invalid command!!\r\n");
	#endif
				}

				MPT_Recv_string = "";
			}
#endif
	}

	return 0;
}

void CDIODlg::ReadDUT_BDaddress()
{
	CString str;
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	int i ;
	
	if (devHandle != 0)
	{
		i = psReadBdAddr(devHandle,&PSkeyBD_lap,&PSkeyBD_uap,&PSkeyBD_nap);
		if(i == 1)
		{
			//s_BDaddress.Format( _TEXT("%4x%2x%6x"), PSkeyBD_nap, PSkeyBD_uap, PSkeyBD_lap);
			s_BDaddress.Format( _TEXT("%4X%2X%6X"), PSkeyBD_nap, PSkeyBD_uap, PSkeyBD_lap);
			s_BDaddress.Replace(' ', '0');
			lstrcpy(TBDaddress, s_BDaddress);

#ifdef Debug_RS232_Output_Message
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			str = "DUT BD address: "; 
			str += s_BDaddress;
			str += "\r\n";
			m_Edt->ReplaceSel(str);
#endif
			m_TestItemStr = "Read DUT BD";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F001";
			m_ResultStr = "PASS";
			if(m_TestProcess != DebugSPITimer)
				AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			//debug
			m_Edt->ReplaceSel(s_BDaddress + "\r\n");
		}
		else
		{
			m_TestItemStr = "Read DUT BD";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F001";
			m_ResultStr = "FAIL";
			if(m_TestProcess != DebugSPITimer)
				AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;
		}
	}
}

LONG CDIODlg::OnMyMessageM8852_GetResult(WPARAM wParam, LPARAM lParam)
{
	int i;
	CString mTempStr1;
	CString mTempStr2;

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

#ifdef Debug_RS232_Input_Message
	m_Edt->ReplaceSel(m_M8852StrReceived);
	m_Edt->ReplaceSel("\r\n");
#endif

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	//if(m_TestProcess == T21_CW_Measurement)
	//{
	//	m_Edt->ReplaceSel(m_M8852StrReceived);
	//	m_Edt->ReplaceSel("\r\n");
	//}

///*
	//if(m_M8852StrReceived.Find("SIGGEN DATA") != -1)
	//{
	//	m_TestItemStr = "CW Measurement";
	//	TestErrorCode++;
	//	m_Str.Format("%X",TestErrorCode);
	//	m_ErrorCodeStr = m_Str;			
	//	AddListCtrlItem(m_List , ListCtrlIndex);
	//	ListCtrlIndex++;
	//}
	//else if (m_M8852StrReceived.Find("SYSCFG EUTRS232,57600") != -1)
	if (m_M8852StrReceived.Find("SYSCFG EUTRS232,57600") != -1)
	{
		//close the hand shake status

		KillTimer(Chk8852Timer);

		m_Edt->ReplaceSel("Handshake MT8852 Pass!\r\n");

		m_TestItemStr = "Handshake MT8852";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F001";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		MT8852_Handshake_flag = true;

		SetTimer(AutorunTimer , 1000 , NULL);
	}
	else if (m_M8852StrReceived.Find("MICFG 3,F1F2MAX") != -1)
	{
		//m_DUT_TestResult.AddString("Setting Script 3 completely");
		//enableAllButton(true);
		m_Edt->ReplaceSel("Setting Script 3 completely\r\n");

		m_TestItemStr = "Setting Script 3";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F002";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(RunScript3Test)
		{
			m_Edt->ReplaceSel("RF Test\r\n");
			m_TestItemStr = "RF Test";
			m_ErrorCodeStr = "";
			m_ResultStr = "";
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			m_MT8852Action = "Run";
			i_MT8852Script = 3;
			SetTimer(AutorunTimer , 500 , NULL);
		}
	}
	else if (m_M8852StrReceived.Find("MICFG 3,F2MAXLIM") != -1)
	{
		SetMIF1F2Max();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,F1AVGMAX") != -1)
	{
		SetMIF2MaxLim();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,F1AVGMIN") != -1)
	{
		SetMIAvgMax();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,HTXFREQ,FREQ") != -1)
	{
		SetMIAvgMin();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,HRXFREQ,FREQ") != -1)
	{
		SetMIHTxFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,HFREQSEL,ON") != -1)
	{
		SetMIHRxFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,MTXFREQ,FREQ") != -1)
	{
		SetMIHFreqSel();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,MRXFREQ,FREQ") != -1)
	{
		SetMImaxTxFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,MFREQSEL,ON") != -1)
	{
		SetMIRxmediumFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,LTXFREQ,FREQ") != -1)
	{
		SetMImedium();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,LRXFREQ,FREQ") != -1)
	{
		SetMILTxFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,LFREQSEL,ON") != -1)
	{
		SetMILRxFreq();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,TOGGLE,ONCE") != -1)
	{
		SetMILFreqSel();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,PKTTYPE,DH1") != -1)
	{
		//SetMItogglePad();
		SetMIAvgMin();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,TSTCTRL,LOOPBACK") != -1)
	{
		SetMIpkgType();
	}
	else if (m_M8852StrReceived.Find("MICFG 3,NUMPKTS,10") != -1)
	{
		SetMItestType();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,FERLIM,100") != -1)
	{
		//m_ListBox.AddString("Script 3 modulation index Setting");
		m_Edt->ReplaceSel("Script 3 modulation index Setting\r\n");
		m_TestItemStr = "Modulation index Setting";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F003";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		SetMIpkgNum();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,BERLIM") != -1)
	{
		SetSSFerLimit();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,HTXFREQ,FREQ") != -1)
	{
		SetSSBerLimit();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,HRXFREQ,FREQ") != -1)
	{
		SetSSTxHighFreq();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,HFREQSEL,ON") != -1)
	{
		SetSSRxHighFreq();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,MTXFREQ,FREQ") != -1)
	{
		SetSSHighFreq();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,MRXFREQ,FREQ") != -1)
	{
		SetSSEutTx();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,MFREQSEL,ON") != -1)
	{
		SetSSEutRx();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,LTXFREQ,FREQ") != -1)
	{
		SetSSMedium();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,DIRTYTAB,MODINDEX,0") != -1)
	{
		SetSSTxFreq();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,LRXFREQ,FREQ") != -1)
	{
		SetSSmodIndex();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,LFREQSEL,ON") != -1)
	{
		SetSSLRXFREQ();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,DIRTYTAB,SYMT,0") != -1)
	{
		SetSSLFreqSel();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,DIRTYTAB,OFFSET,0") != -1)
	{
		SetSSsymtTable();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,DIRTYTX,OFF") != -1)
	{
		SetSSoffsetTable();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,HOPPING,HOPON") != -1)
	{
		//SetSSDirty();
		SetSSBerLimit();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,PKTCOUNT,TX") != -1)
	{
		SetSSHopping();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,TXPWR") != -1)
	{
		SetSSTxPayload();
	}
	else if (m_M8852StrReceived.Find("SSCFG 3,NUMPKTS,500") != -1)
	{
		SetSSTxPower();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,TSTCTRL") != -1)
	{
		//m_ListBox.AddString("Script 3 single sensitivity Setting");
		m_Edt->ReplaceSel("Script 3 single sensitivity Setting\r\n");
		m_TestItemStr = "Single sensitivity Setting";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F004";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;		
		SetSSpkgNum();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,HOPMODE") != -1)
	{
		SetOPpkgNum();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,PEAKLIM") != -1)
	{
		SetOPHopMode();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,AVGMNLIM") != -1)
	{
		SetOPTestPeak();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,AVGMXLIM") != -1)
	{
		SetOPAverageMin();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,PKTTYPE") != -1)
	{
		SetOPAverageMax();
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,HOPPING") != -1)
	{
		SetOPPkgType();
	}
//----------------------------------------------------------------
	else if (m_M8852StrReceived.Find("ICCFG 3,MXPOSLIM") != -1)
	{
		//m_ListBox.AddString("Script 3 output power Setting");
		m_Edt->ReplaceSel("Script 3 output power Setting\r\n");
		m_TestItemStr = "Output power Setting";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F005";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		SetOPHopping();
	}
	else if (m_M8852StrReceived.Find("ICCFG 3,MXPOSLIM") != -1)
	{
		//SetICMinOffset();
	}
	else if (m_M8852StrReceived.Find("ICCFG 3,TSTCTRL") != -1)
	{
		SetICMaxOffset();
	}
	else if (m_M8852StrReceived.Find("ICCFG 3,HOPMODE") != -1)
	{
		SetICTestType();
	}
	else if (m_M8852StrReceived.Find("ICCFG 3,HOPPING") != -1)
	{
		SetICHopMode();
	}
	else if (m_M8852StrReceived.Find("ICCFG 3,NUMPKTS") != -1)
	{
		//m_ListBox.AddString("Script 3 initial Carrier Setting");
		m_Edt->ReplaceSel("Script 3 initial Carrier Setting\r\n");
		m_TestItemStr = "Initial Carrier Setting";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		//m_ErrorCodeStr = "F006";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		SetICHopping();
	}
	else if (m_M8852StrReceived.Find("OPMD CWMEAS") != -1)
	{
		m_TestItemStr = "CW Measurement,Power";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;			
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
///*
		m_Str = "CWMEAS FREQ,";
		m_Str += m_T21Freq;
		m_Str += "e6,";
        m_Str += m_T21GateWidth;
		m_Str += "e-3\r\n";
		char *Comdstr;
		Comdstr = m_Str.GetBuffer(0);
		MT8852COM.WriteToPort(Comdstr);

		m_Str = Comdstr;
		m_Edt->ReplaceSel(m_Str);
//*/
/*
		TCHAR SetMT8852[] = "CWMEAS FREQ,2441e6,3e-3\r\n";
		MT8852COM.WriteToPort(SetMT8852);
*/		
		//m_Edt->ReplaceSel("Set MT8852 to CW Measurement mode!\r\n");

		Sleep(1000);

		//MT8852COM.WriteToPort(M8852EchoCmd_ptr);

		m_TestProcess = T21_CW_Measurement;

		//SetTimer(T21_CW_Measurement , 2000 , NULL);
		SetTimer(T21_CW_Measurement , 1000 , NULL);
	}
	else if (m_M8852StrReceived.Find("OPMD SCRIPT") != -1)
	{
		m_TestItemStr = "Set Script Mode";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		SetEUT_SELECT_SCRIPT1To8852();//Script 1	

		//if (testActiveCurrentEnable)
		//{
		//	SetOPpkgNumTo10();
		//}
		//else
		//{
		//	SetTimer(Run8852delay,500,NULL); 			
		//}		
	}
	else if (m_M8852StrReceived.Find("OPMD STEST,IC") != -1)
	{
//		SetEUT_RunTestTo8852();

		SetTimer(Run8852delay,500,NULL); 
	}
//-----------------------------------------------------------------------
	else if (m_M8852StrReceived.Find(M8852_echo_BD) != -1)
	{
		if(m_TestProcess)
		{
			if(m_MT8852Action.Compare("CrystalTrim") == 0)
				SetEUT_ICSingleTestTo8852();
			else
			{
				if(i_MT8852Script == 3)
				{
					m_TestItemStr = "Set EUT BD";
					TestErrorCode++;
					m_Str.Format("%X",TestErrorCode);
					m_ErrorCodeStr = m_Str;
					//m_ErrorCodeStr = "F001";
					m_ResultStr = "PASS";
					AddListCtrlItem(m_List , ListCtrlIndex);
					ListCtrlIndex++;

					SetEUT_SELECT_SCRIPT3To8852();//Script 3 
				}
				else if(i_MT8852Script == 1)
				{
					SetEUT_SCRIPT_OPMD_To8852();
					/*
					m_TestItemStr = "Set EUT BD";
					TestErrorCode++;
					m_Str.Format("%X",TestErrorCode);
					m_ErrorCodeStr = m_Str;
					//m_ErrorCodeStr = "F001";
					m_ResultStr = "PASS";
					AddListCtrlItem(m_List , ListCtrlIndex);
					ListCtrlIndex++;

					SetEUT_SELECT_SCRIPT1To8852();//Script 1
					*/
				}
			}
		}
	}
	else if (m_M8852StrReceived.Find("RFIXEDOFF") != -1)
	{
		SetEUT_SCRIPT3PathOffsetTo8852();
	}
	else if (m_M8852StrReceived.Find("FIXED") != -1)	
	{
		SetEUT_ManualtTo8852();
	}
	else if (m_M8852StrReceived.Find("MANUAL") != -1)
	{
		SetEUT_SCRIPT3configTo8852();
	}
	else if (m_M8852StrReceived.Find("RON,OFF,ON,ON,OFF,ON,OFF,OFF") != -1)
	{
		 SetICpkgNum();
	}
	else if (m_M8852StrReceived.Find("RON") != -1)
	{	
		config_status ++;
		M8852EchoCmd_ptr = "SCPTCFG? 3\r\n";
		MT8852_Op_str.Format(_T("RON"));
		switch(config_status)
		{
		case 1:	
			MT8852COM.WriteToPort("SCPTCFG 3,PC,OFF\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 2:	
			MT8852COM.WriteToPort("SCPTCFG 3,MI,ON\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 3:	
			MT8852COM.WriteToPort("SCPTCFG 3,IC,ON\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 4:	
			MT8852COM.WriteToPort("SCPTCFG 3,CD,OFF\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 5:	
			MT8852COM.WriteToPort("SCPTCFG 3,SS,ON\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 6:	
			MT8852COM.WriteToPort("SCPTCFG 3,MS,OFF\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		case 7:	
			MT8852COM.WriteToPort("SCPTCFG 3,MP,OFF\r\n");
			SetTimer(M8852INStimer,100,NULL); 
			break;
		default: 
			break;
		};	
	}
	else if (m_M8852StrReceived.Find("TXPWR 3,-40") != -1)
	{
		SetEUT_SCRIPT3OffsetvalueTo8852();
	}
	else if (m_M8852StrReceived.Find("SCPTNM 3") != -1)
	{
		SetEUT_SCRIPT3TxPowerTo8852();
	}
	else if (m_M8852StrReceived.Find("SEL 1") != -1)
	{
		m_Edt->ReplaceSel("Select script 1\r\n");
		SetEUT_RunTestTo8852();
		//QuickTest();
	}
	else if (m_M8852StrReceived.Find("SEL 3") != -1)
	{
		if ((run_step == 1) || (run_step == 3))
		{
			SetEUT_ICSingleTestTo8852();//Crystal Trim testing
		}
		else if (run_step == 2)
		{
			/*
			if (testActiveCurrentEnable)	//frank active current test
			{
				SetEUT_OPSingleTestTo8852();
			}
			else
			{
				SetEUT_SCRIPT_OPMD_To8852();
			}
			*/
		}
		else if (run_step == 0)
		{
			//m_ListBox.AddString("Script Sel 3");
			if(MT8852_Handshake_flag == false)
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("MT8852 handshake fail!\r\n");
			}
			else
			{				
				if(m_MT8852Action.Compare("Reset") == 0)
				{
					m_Edt->ReplaceSel("Set Script3 Name\r\n");
					SetEUT_SCRIPT3NameTo8852();
				}
				else if(m_MT8852Action.Compare("Run") == 0)
				{
					SetEUT_RunTestTo8852();
				}
			}
		}	
	}
	else if (m_M8852StrReceived.Find("R14") != -1)
	{
		m_Edt->ReplaceSel("R14\r\n");

		if(i_MT8852Script != 0 && i_MT8852Script == 3)
		{
			SetEUT_SELECT_SCRIPT3To8852();
		}

		if(i_MT8852Script == 0)
		{
			KillTimer(M8852EchoInstTimer);
			m_Edt->ReplaceSel("PASS!\r\n");

			m_TestItemStr = "Reset MT8852";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F005";
			m_ResultStr = "PASS";
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;
		}
	}
	else if (m_M8852StrReceived.Find("R13") != -1)
	{		
		//if(m_TestStation.Compare("T1") == 0)
		//{
		//	m_Str.Format("run_step=%d\r\n",run_step);
		//	m_Edt->ReplaceSel(m_Str);
		//}

		if (run_step == 0)
		{
			if(i_MT8852Script == 1)
				m_TestItemStr = "MT8852 Quick test";
			else if(i_MT8852Script == 1)
				m_TestItemStr = "MT8852 Script3 test";

			m_ErrorCodeStr = "";
			m_ResultStr = "";
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			//Get Test report
			ReadEUT_OPTestTo8852();	
		}
		else if (run_step == 1)
		{
			if(i_MT8852Script == 3)
				ReadEUT_ICSingleTestTo8852();
			else if(i_MT8852Script == 1)
			{
				if(m_MT8852Action.Compare("CrystalTrim") == 0)
					ReadEUT_ICSingleTestTo8852();
				else
					ReadEUT_OPTestTo8852();
			}
			//	ReadEUT_OPTestTo8852();//Get Test report
		}
		else if (run_step == 3)
		{
			if(i_MT8852Script == 3)
				ReadEUT_ICSingleTestTo8852();
		}
		else
		{
			/*
			if (testActiveCurrentEnable) //frank test
			{
				if (m_isMeasureActiveCurrent == false)
				{
					ReadEUT_OPTestTo8852();	
				}
				else
				{
					m_isMeasureActiveCurrent = false;
				}			
			}
			else
			{
				ReadEUT_OPTestTo8852();	
			}
			*/
			
			//ReadEUT_OPTestTo8852();	
		}
	}
	else if (m_M8852StrReceived.Find("ROP0,TRUE") != -1)
	{
		/*
		if (m_M8852StrReceived.Find("PASS") != -1)
		{
			i = m_M8852StrReceived.Find("ROP0,TRUE,");
			
			m_outputPowerResult = m_M8852StrReceived; //for logfile export
			m_ListBox.AddString(m_M8852StrReceived);
			m_ListBox.SetSel(m_ListBox.GetCount()-1);
			ReadEUT_ICSingleTestTo8852();
		}
		else
		{
			m_outputPowerResult = m_M8852StrReceived; //for logfile export

			testOKFlag = false;   //first test item fail.

			DUT_8852TestReport = DUT_8852TestReport + " Output power FAIL(0x22)";
			ErrorCodeString += "0x22;";
			Stc_TestResult.SetBitmap(hFAIL);
			m_DUT_TestResult.AddString(DUT_8852TestReport);
			SetEUT_DisconnectTo8852();
			ReadEUT_ICSingleTestTo8852(); //next test item
		}
		*/

		if(m_MT8852Action.Compare("Run") == 0 && (i_MT8852Script == 1 || i_MT8852Script == 3))//QuickTest/Script3
		{
			m_RFTestResult = "";

			m_Edt->ReplaceSel(m_M8852StrReceived + "\r\n");
			if (m_M8852StrReceived.Find("PASS") != -1)
			{
				ReadEUT_ICSingleTestTo8852(); //next test item
				m_Edt->ReplaceSel("Output power pass!\r\n");
				m_ResultStr = "PASS";
				//TestResultFlag = true;

				//Check Average Output power
				CString tmp;
				double AvgOutputPower;
				tmp = m_M8852StrReceived.Left(14);
				m_Str = tmp.Right(4);
				AvgOutputPower  = atof(m_Str);
				if((AvgOutputPower > OPAverageMax) || (AvgOutputPower < OPAverageMin))
				{
					m_Edt->ReplaceSel("Average Output power fail " + m_Str);
					m_Edt->ReplaceSel("\r\n");
					m_ResultStr = "FAIL";
					TestResultFlag = false;
				}
				else
				{
					m_Edt->ReplaceSel("Average Output power = " + m_Str);
					m_Edt->ReplaceSel("\r\n");
				}
			}
			else
			{
				ReadEUT_ICSingleTestTo8852(); //next test item
				m_Edt->ReplaceSel("Output power fail!\r\n");
				m_ResultStr = "FAIL";
				TestResultFlag = false;
			}
							
			m_TestItemStr = "Output power";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F003";

			if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
				m_RFTestResult = m_ErrorCodeStr;
			
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;
		}
	}
	else if (m_M8852StrReceived.Find("RIC0,TRUE") != -1)
	{
		if ((run_step == 1) || (run_step == 3))
		{
			lstrcpy(TRICO, m_M8852StrReceived);
			GetValueFromString(TRICO,10,1);
			SET_SINGLE_InitCarrier_RUN_TEST();
		}
		else if (run_step == 2)
		{
			/*
			if (m_M8852StrReceived.Find("PASS") != -1)
			{
				m_initialCarrierTestResult = m_M8852StrReceived; //for logfile export
				m_ListBox.AddString(m_M8852StrReceived);
				m_ListBox.SetSel(m_ListBox.GetCount()-1);
				ReadEUT_MITestTo8852();
			}
			else
			{
				m_initialCarrierTestResult = m_M8852StrReceived; //for logfile export
				testOKFlag = false;   //second test item fail.

				DUT_8852TestReport = DUT_8852TestReport + " Init carrier FAIL(0x21)";
				ErrorCodeString += "0x21;";
				Stc_TestResult.SetBitmap(hFAIL);
				m_DUT_TestResult.AddString(DUT_8852TestReport);
//				SetEUT_DisconnectTo8852();
				ReadEUT_MITestTo8852();						//continue test next item 
//				CloseTestEngine();
			}
			*/
		}
		else
		{
			if(m_MT8852Action.Compare("Run") == 0 && (i_MT8852Script == 1 || i_MT8852Script == 3))//QuickTest/Script3
			{
				m_Edt->ReplaceSel(m_M8852StrReceived + "\r\n");
				if (m_M8852StrReceived.Find("PASS") != -1)
				{
					ReadEUT_MITestTo8852(); //continue next test item
					m_Edt->ReplaceSel("Initial carrier pass!\r\n");
					m_ResultStr = "PASS";
				}
				else
				{
					ReadEUT_MITestTo8852(); //continue next test item
					m_Edt->ReplaceSel("Initial carrier fail!\r\n");
					m_ResultStr = "FAIL";
					TestResultFlag = false;
				}

				m_TestItemStr = "Initial carrier";
				TestErrorCode++;
				m_Str.Format("%X",TestErrorCode);
				m_ErrorCodeStr = m_Str;
				//m_ErrorCodeStr = "F003";

				if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
					m_RFTestResult = m_ErrorCodeStr;
			
				AddListCtrlItem(m_List , ListCtrlIndex);
				ListCtrlIndex++;
			}
		}
	}
	else if (m_M8852StrReceived.Find("RMI0,TRUE") != -1)
	{
		/*
		if (m_M8852StrReceived.Find("PASS") != -1)
		{
			m_modulationResult = m_M8852StrReceived; //for logfile export
			m_ListBox.AddString(m_M8852StrReceived);
			m_ListBox.SetSel(m_ListBox.GetCount()-1);
			ReadEUT_SSTestTo8852();
		}
		else
		{
			m_modulationResult = m_M8852StrReceived; //for logfile export
			testOKFlag = false;   //third test item fail.

			DUT_8852TestReport = DUT_8852TestReport + " Modulation Index FAIL(0x23)";
			ErrorCodeString += "0x23;";
			Stc_TestResult.SetBitmap(hFAIL);
			m_DUT_TestResult.AddString(DUT_8852TestReport);
//			SetEUT_DisconnectTo8852();
//			CloseTestEngine();
			ReadEUT_SSTestTo8852();				//continue test next item 
		}
		*/
			
		if(m_MT8852Action.Compare("Run") == 0 && (i_MT8852Script == 1 || i_MT8852Script == 3))//QuickTest/Script3
		{
			m_Edt->ReplaceSel(m_M8852StrReceived + "\r\n");
			if (m_M8852StrReceived.Find("PASS") != -1)
			{
				ReadEUT_SSTestTo8852(); //continue next test item
				m_Edt->ReplaceSel("Modulation index pass!\r\n");
				m_ResultStr = "PASS";
			}
			else
			{
				ReadEUT_SSTestTo8852(); //continue next test item
				m_Edt->ReplaceSel("Modulation index fail!\r\n");
				m_ResultStr = "FAIL";
				TestResultFlag = false;
			}

			m_TestItemStr = "Modulation index";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F003";

			if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
				m_RFTestResult = m_ErrorCodeStr;
			
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;
		}
	}
	else if (m_M8852StrReceived.Find("RSS0,TRUE") != -1)
	{
		/*
		if (m_M8852StrReceived.Find("PASS") != -1)
		{
			m_singleSlotSensitivityResult = m_M8852StrReceived; //for logfile export
			m_ListBox.AddString(m_M8852StrReceived);
			m_ListBox.SetSel(m_ListBox.GetCount()-1);
			DUT_8852TestReport = DUT_8852TestReport + " PASS";
			Stc_TestResult.SetBitmap(hPASS);
			m_DUT_TestResult.AddString(DUT_8852TestReport);
//			SetEUT_DisconnectTo8852();
			
		}
		else
		{
			m_singleSlotSensitivityResult = m_M8852StrReceived; //for logfile export
			testOKFlag = false;   //fourth test item fail.
			DUT_8852TestReport = DUT_8852TestReport + " Single sensitivity FAIL(0x24)";
			ErrorCodeString += "0x24;";
			Stc_TestResult.SetBitmap(hFAIL);
			m_DUT_TestResult.AddString(DUT_8852TestReport);
//			SetEUT_DisconnectTo8852();
//			CloseTestEngine();
		}
		
		//don't write BDaddress and boot mode if test fail
		if (testOKFlag)	
		{
			SetTimer(WaitingNextDUTtimer,3000,NULL);
			if (writeBDAddressEnable)
			{
				WriteDUT_BDaddress(m_finalBDAddress);
			}
			
			if (m_isChangeModManuf2)
			{
				changePSKEY_MOD_MANUF2(devHandle,m_final_modManuf2);
			}
			
			
			if (changeBootMode)
			{
				psWrite(devHandle,0x3cd,0,1,&m_final_boot_mode); 
			}
				
			ReadDUT_BDaddress();						//check new bdaddress
			m_DUT_TestResult.InsertString(0, _T("BD address: ") + s_BDaddress);
			
			
		}
		else
		{
			if (writeBDAddressEnable)
			{
				s_BDaddress = m_failBDAddress;
			}
			
		}
			
		CloseTestEngine();

		//if one of the four test item fail,show fail bitmap	
		if (testOKFlag == false)
		{
			Stc_TestResult.SetBitmap(hFAIL);
		}

		//export log file after the four test item complete.
		if (exoprtLogFileEnable)
		{
			ExportFile(); 
		}
		
		enableAllButton(1);

		//multi times test
		
		//if (Count !=0)
		//{
		//	Count--;
		//	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_Trim_Test, BN_CLICKED), NULL);
		//}
		
		*/

		if(m_MT8852Action.Compare("Run") == 0 && (i_MT8852Script == 1 || i_MT8852Script == 3))//QuickTest/Script3
		{
			m_Edt->ReplaceSel(m_M8852StrReceived + "\r\n");
			if (m_M8852StrReceived.Find("PASS") != -1)
			{
				m_Edt->ReplaceSel("Single sensitivity pass!\r\n");
				m_ResultStr = "PASS";
			}
			else
			{
				m_Edt->ReplaceSel("Single sensitivity fail!\r\n");
				m_ResultStr = "FAIL";
				TestResultFlag = false;
			}

			m_TestItemStr = "Single sensitivity";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			//m_ErrorCodeStr = "F003";

			if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
				m_RFTestResult = m_ErrorCodeStr;
			
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			#ifdef T1_SPI_Debug
				CloseTestEngine();
			#endif

			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("****************************************************\r\n");
			//CTime t = CTime::GetCurrentTime();
			//m_Str = "Current Time :";
			//data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
			//m_Edt->ReplaceSel(data);
			if(TestResultFlag)
			{
				if(i_MT8852Script == 1)
					m_Edt->ReplaceSel("	RF Quick Test PASS!				                   \r\n");
				else if(i_MT8852Script == 3)
					m_Edt->ReplaceSel("	RF test pass				                   \r\n");

				//CloseTestEngine();

				SetTimer(WriteDummyBDTimer , WriteDummyBDTime , NULL);

				//rgbText = RGB_BLUE;
				//SetDlgItemText(IDC_PASSFAIL , "Test PASS!");
			}
			else
			{
				if(i_MT8852Script == 1)
					m_Edt->ReplaceSel("	RF Quick Test FAIL!				                   \r\n");
				else if(i_MT8852Script == 3)
					m_Edt->ReplaceSel("	RF test fail!				                   \r\n");
				SetDlgItemText(IDC_RETEST , "Re-Test");		

				SetTimer(PCAbortTimer , PCAbortTime , NULL);
				
				SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_RFTestResult);

				rgbText = RGB_RED;
				rgbBkgnd = RGB_GRAY1;
				m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
				m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

				//For debug
				//SetTimer(WriteDummyBDTimer , WriteDummyBDTime , NULL);
			}
			m_Edt->ReplaceSel("****************************************************\r\n");

			//rgbBkgnd = RGB_GRAY1;
			//m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			//m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

			//ExportFile();

			//SetTimer(WriteDummyBDTimer , WriteDummyBDTime , NULL);

			SetDlgItemText(ID_BTN_EXIT , "Exit");
		}
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,NUMPKTS,20") != -1)
	{
		//SetEUT_OPSingleTestTo8852();
		//ReadActiveCurrent();	
	}
	else if (m_M8852StrReceived.Find("OPCFG 3,NUMPKTS,10") != -1)
	{
		if (run_step == 0)
		{
			SetOPTestType();
		}
		else if (run_step == 2)
		{
			SetTimer(Run8852delay,500,NULL);
		}
	}
	else if (m_M8852StrReceived.Find("OPMD STEST,OP") != -1) //frank use to test active current
	{
		//ReadActiveCurrent();	
		//SetOPpkgNumTo20();
	}
	else
	{
		//Unknown command
		m_Edt->ReplaceSel(m_M8852StrReceived + "\r\n");
	}
//*/

	m_M8852StrReceived.Empty();
	
	return 0; 
}

void CDIODlg::CrystalTrimTest()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	uint16 len;
	uint16 CrystalFreq; 

	//if(m_MT8852Action.Compare("Run") == 0 && i_MT8852Script == 0)
	if(m_MT8852Action.Compare("CrystalTrim") == 0)
	{
		//m_Edt->SetSel(0,-1);
		//m_Edt->Clear();

		///*
		if(MT8852_Handshake_flag == false)
		{
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("MT8852 handshake fail!\r\n");
			return;
		}
		//*/

///*
		///*
		if(OpenTestEngine())
		{
			CloseTestEngine();
			Sleep(1000);

			m_Edt->ReplaceSel("Try again!\r\n");

			if(OpenTestEngine())
			{
				//SPI fail,check SPI cable or DUT power
				SetTimer(PCAbortTimer , PCAbortTime , NULL);
				return;
			}
		}
		//*/


		if(SetDUTMode())
		{
			return;
		}

		//Read Crystal frequency trim : PSKEY_ANA_FTRIM
		//Value range from 0 ~ 63
		psRead(devHandle , 0x1f6 , 0 , 1 , &CrystalTrimOldValue , &len);

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Str.Format("Original Crystal trim value = %d\r\n",CrystalTrimOldValue);
		m_Edt->ReplaceSel(m_Str);

		//Check Crystal frequence
		psRead(devHandle , 0x1fe , 0 , 1 , &CrystalFreq , &len);
		m_Str.Format("Crystal frequence = %d\r\n",CrystalFreq);
		m_Edt->ReplaceSel(m_Str);

		Sleep(1000);
//*/		
		run_step = 1;
		IC_test_states = 0;

		ReadDUT_BDaddress();

		DUT_8852TestReport = s_BDaddress;
		DUT_8852TestReport = DUT_8852TestReport + " -- ";

		SetDlgItemText(IDC_STATIC2 , "BD : " + s_BDaddress);//BD

		WriteDUT_CrystalFrequencyTrim();
		radiotestDUT_CrystalFrequencyTrim(PSkeyCrystalTrim);

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

		SetDlgItemText(IDC_BUTTON1 , "CrystalTrim");
		SetDlgItemText(IDC_RETEST , "Re-Test");
		SetDlgItemText(ID_BTN_EXIT , "Reset8852");
		
		SetEUT_BDaddressTo8852();
	}
}

int CDIODlg::OpenTestEngine()
{
	int success;
	uint16 temp;
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if (devHandle != 0)
	{
		closeTestEngine(devHandle);
		devHandle = 0;
		m_Edt->ReplaceSel("CloseTestEngine\r\n");
		Sleep(500);
	}

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	devHandle = openTestEngineSpi(1, 0, SPI_LPT);

	if(devHandle != 0)
	{
		success = bccmdGetChipVersion(devHandle, &temp);

		success = bccmdGetChipRevision(devHandle, &temp);

		if(temp != 0xE1)//ChipID
		{
			//m_Edt->ReplaceSel("OpenTestEngine,Get chip ID error!\r\n");
			m_Edt->ReplaceSel("Get chip ID error!\r\n");
			devHandle = 0;
			closeTestEngine(devHandle);
			return 1;
		}
		else
		{
			//m_Edt->ReplaceSel("OpenTestEngine,Get chip ID : BC5-MM\r\n");
			m_Edt->ReplaceSel("Open SPI,Get chip ID : BC5-MM\r\n");
			return 0;
		}
	}
	else// if(devHandle == 0)
	{
		m_Edt->ReplaceSel("OpenTestEngineSpi fail!\r\n");
		return 1;
	}
}

void CDIODlg::WriteDUT_CrystalFrequencyTrim()
{
	int i;

	//This function will write the crystal trim value to the persistent store

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	//This function will write the crystal trim value to the persistent store

	i = psWriteXtalFtrim(devHandle, PSkeyCrystalTrim);

	if(i == 1)
	{
		///*
		m_TestItemStr = "WrDUTCrystalFreqTrim";
		m_ErrorCodeStr = "F002";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		//*/

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Str.Format("Write PSkeyCrystalTrim = %d\r\n",PSkeyCrystalTrim);
		m_Edt->ReplaceSel(m_Str);
	}
	else
	{
		m_TestItemStr = "WrDUTCrystalFreqTrim";
		//m_ErrorCodeStr = "F002";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}
}

void CDIODlg::radiotestDUT_CrystalFrequencyTrim(uint16 XtalFtrim)
{
	int i;
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	//Timing for BlueCore is controlled by a crystal. 
	//This requires trimming for new hardware. 
	//This command can be used to set a new trim value either before a radiotest command is started or while a test is already in operation
	//the change takes effect immediately. 

	i = radiotestCfgXtalFtrim(devHandle,XtalFtrim);

	if(i == 1)
	{
		/*
		m_TestItemStr = "radiotestCfgXtalFtrim";
		m_ErrorCodeStr = "F002";
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		*/

		m_Str.Format("radiotestCfgXtalFtrim = %d\r\n",XtalFtrim);
		m_Edt->ReplaceSel(m_Str);
	}
	else
	{
		m_TestItemStr = "radiotestCfgXtalFtrim";
		//m_ErrorCodeStr = "F002";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}
}

void CDIODlg::SetEUT_BDaddressTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SendBDaddressCmd[36];
	CString SndBDaddress;
	SndBDaddress.Format(_T("SYSCFG EUTADDR, %s\r\n"), TBDaddress);
	strcpy(SendBDaddressCmd, (LPCTSTR)SndBDaddress);
	M8852EchoCmd_ptr = "SYSCFG? EUTADDR\r\n";
	MT8852_Op_str.Format(_T("R%s"), TBDaddress);
	MT8852_Op_str.MakeUpper();
	lstrcpy(M8852_echo_BD, MT8852_Op_str);

	MT8852COM.WriteToPort(SendBDaddressCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SendBDaddressCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,200,NULL);	
}

void CDIODlg::SetEUT_SELECT_SCRIPT3To8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SetSCRIPTSEL3Cmd[] = "SCPTSEL 3\r\n";
	M8852EchoCmd_ptr = "SCPTSEL?\r\n";
	MT8852_Op_str.Format(_T("SEL 3"));
	MT8852COM.WriteToPort(SetSCRIPTSEL3Cmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetSCRIPTSEL3Cmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,500,NULL); 
}

void CDIODlg::SetEUT_ICSingleTestTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SetICSingleTestCmd[] = "OPMD STEST,IC\r\n";
	M8852EchoCmd_ptr = "OPMD?\r\n";
	MT8852_Op_str.Format(_T("OPMD STEST,IC"));
	MT8852COM.WriteToPort(SetICSingleTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetICSingleTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,200,NULL); 
}

void CDIODlg::SetEUT_RunTestTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR RunTestCmd[] = "RUN\r\n";
	M8852EchoCmd_ptr = "*INS?\r\n";
	MT8852_Op_str.Format(_T("R13"));
	I8852InsTimerOut = 0;
	MT8852COM.WriteToPort(RunTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(RunTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	
	SetTimer(M8852INStimer,1500,NULL); 
	//SetTimer(M8852INStimer,3000,NULL); 
}

void CDIODlg::ReadEUT_ICSingleTestTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR ReadICSingleTestCmd[] = "ORESULT TEST,0,IC\r\n";
	M8852EchoCmd_ptr = "ORESULT TEST,0,IC\r\n";

	MT8852_Op_str.Format(_T("RIC0,TRUE"));
	MT8852COM.WriteToPort(ReadICSingleTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ReadICSingleTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
}

void CDIODlg::GetValueFromString(TCHAR *chararray, uint16 start_point, uint16 find_counter)
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR temp[30];
	uint8 i = 0;
	uint8 j = 0;
	while (find_counter != 0)
	{
		if (chararray[start_point+i] == ',')
		{
			find_counter --;
		}
		i++;
	}
	while (chararray[start_point+i] != ',') 
	{
		if (chararray[start_point+i] == 0) 
			break;
		*(temp+j)=chararray[start_point+i];
		j++;
		i++;
	}
	*(temp+j) = 0;
	//m_Str = temp;
	//m_Edt->ReplaceSel("Get IC_averageoffset from MT8852 : ");
	//m_Edt->ReplaceSel(m_Str);
	//m_Edt->ReplaceSel("\r\n");
	IC_averageoffset = atof(temp);
}

void CDIODlg::SET_SINGLE_InitCarrier_RUN_TEST()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if ((PSkeyCrystalTrim >= CrystalTrimUpperlimit) || (PSkeyCrystalTrim <= CrystalTrimLowerlimit))
	{
		//m_DUT_TestResult.AddString("Crystal Trim Fail");
		m_Edt->ReplaceSel("Crystal Trim Fail");
		//Stc_TestResult.SetBitmap(hFAIL);
		PSkeyCrystalTrim = DefPSkeyCrystalTrim;
		CloseTestEngine();

		//enableAllButton(1); //frank test
		//ErrorCodeString = ErrorCodeString + " 0x20;";
		//if (exoprtLogFileEnable)
		//{
		//	ExportFile();
		//}
		
		return;
	}
	if (IC_test_states == 0)
	{
		//m_Edt->ReplaceSel("IC_test_states 0\r\n");
		m_strCrystaltrimReport.Format(_T("CrystalTrim = %d Average offset = %4.2f kHz"),PSkeyCrystalTrim,IC_averageoffset/1000.0);
		//m_ListBox.AddString(m_strCrystaltrimReport);
		//m_ListBox.SetSel(m_ListBox.GetCount()-1);
		m_strCrystaltrimReport += "\r\n";
		m_Edt->ReplaceSel(m_strCrystaltrimReport);

		FirstCrystalTrim = PSkeyCrystalTrim;
		First_IC_averageoffset = IC_averageoffset;
		if (IC_averageoffset < 0)
		{
			PSkeyCrystalTrim = PSkeyCrystalTrim - crystaltrim_diff;
		}
		else
		{
			PSkeyCrystalTrim = PSkeyCrystalTrim + crystaltrim_diff;
		}
		radiotestDUT_CrystalFrequencyTrim(PSkeyCrystalTrim);
		
		IC_test_states ++;
		SetTimer(SingleICRunTimer,500,NULL); 
	}
	else if (IC_test_states == 1)
	{
		//m_Edt->ReplaceSel("IC_test_states 1\r\n");
		m_strCrystaltrimReport.Format(_T("CrystalTrim = %d Average offset = %4.2f kHz"),PSkeyCrystalTrim,IC_averageoffset/1000.0);
		//m_ListBox.InsertString(0,m_strCrystaltrimReport);
		m_strCrystaltrimReport += "\r\n";
		m_Edt->ReplaceSel(m_strCrystaltrimReport);

		SecondCrystalTrim = PSkeyCrystalTrim;
		Second_IC_averageoffset = IC_averageoffset;
		if ((Second_IC_averageoffset < 0 && First_IC_averageoffset < 0) || (Second_IC_averageoffset > 0 && First_IC_averageoffset > 0))
		{
			if (IC_averageoffset < 0)
			{
				PSkeyCrystalTrim = PSkeyCrystalTrim - crystaltrim_diff;
			}
			else
			{
				PSkeyCrystalTrim = PSkeyCrystalTrim + crystaltrim_diff;
			}
		}
		else
		{
			if (abs((FirstCrystalTrim - SecondCrystalTrim)) == 1)
			{
				if (abs(Second_IC_averageoffset) > abs(First_IC_averageoffset))
					PSkeyCrystalTrim = FirstCrystalTrim;
				else
					PSkeyCrystalTrim = SecondCrystalTrim;
			}
			else 
				PSkeyCrystalTrim = (FirstCrystalTrim + SecondCrystalTrim) / 2;

			if (Second_IC_averageoffset < 0)
				Second_IC_averageoffset = Second_IC_averageoffset * -1;
			IC_test_states ++;			 
		}
		FirstCrystalTrim = SecondCrystalTrim;
		First_IC_averageoffset = Second_IC_averageoffset;
		radiotestDUT_CrystalFrequencyTrim(PSkeyCrystalTrim);
		SetTimer(SingleICRunTimer,500,NULL); 
	}
	else if (IC_test_states == 2)
	{
		//m_Edt->ReplaceSel("IC_test_states 2\r\n");
		m_strCrystaltrimReport.Format(_T("CrystalTrim = %d Average offset = %4.2f kHz"),PSkeyCrystalTrim,IC_averageoffset/1000.0);
		//m_ListBox.AddString(m_strCrystaltrimReport);
		m_strCrystaltrimReport += "\r\n";
		m_Edt->ReplaceSel(m_strCrystaltrimReport);

		SecondCrystalTrim = PSkeyCrystalTrim;
		if (IC_averageoffset < 0) IC_averageoffset = IC_averageoffset * -1;
		Second_IC_averageoffset = IC_averageoffset;
		if (Second_IC_averageoffset <= First_IC_averageoffset)
		{
			PSkeyCrystalTrim = SecondCrystalTrim;
			IC_averageoffset = Second_IC_averageoffset;
		}
		else 
		{
			PSkeyCrystalTrim = FirstCrystalTrim;
			IC_averageoffset = First_IC_averageoffset;
		}
		crystaltrim_diff = 1;
		m_strCrystaltrimReport.Format(_T("CrystalTrim = %d ,  abs(Average offset) = %4.2f kHz"),PSkeyCrystalTrim,IC_averageoffset/1000.0);
		//m_DUT_TestResult.AddString(m_strCrystaltrimReport);
		m_strCrystaltrimReport += "\r\n";
		m_Edt->ReplaceSel(m_strCrystaltrimReport);

		//For debug pupporse only,Don't write new crystaltrim value to BC05 Persistance store 
		//m_Edt->ReplaceSel("Debug: Don't write new crystaltrim value to BC05\r\n");
		//PSkeyCrystalTrim = CrystalTrimOldValue;
		//WriteDUT_CrystalFrequencyTrim();
		//radiotestDUT_CrystalFrequencyTrim(PSkeyCrystalTrim);

		//Write new CrystalTrim value
		m_Str.Format("Write new CrystalTrim value : %d\r\n",PSkeyCrystalTrim);
		m_Edt->ReplaceSel(m_Str);
		WriteDUT_CrystalFrequencyTrim();
		radiotestDUT_CrystalFrequencyTrim(PSkeyCrystalTrim);
		
		/*
		rgbText = RGB_BLUE;
		rgbBkgnd = RGB_GRAY1;
		SetDlgItemText(IDC_PASSFAIL , "Test PASS!");
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
		*/

		m_TestItemStr = "Crystal Trim";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		m_Str.Format("PSkeyCrystalTrim = %d\r\n",PSkeyCrystalTrim);
		m_Edt->ReplaceSel(m_Str);

		m_Edt->ReplaceSel("****************************************************\r\n");
		m_Edt->ReplaceSel("	CrystalTrim pass.				                   \r\n");	
		m_Edt->ReplaceSel("****************************************************\r\n");

		SetDlgItemText(ID_BTN_EXIT , "Exit");

#ifdef T1_SPI_Debug
		CloseTestEngine();
#endif

		if (run_step == 1)
		{
			//CloseTestEngine();

			//m_Edt->ReplaceSel("CloseTestEngine\r\n");

			Sleep(200);

			m_MT8852Action = "Run";

			//Script 3 Test
			//MT8852Script3Test();

			//QuickTest
			i_MT8852Script = 1;
			QuickTest();
		}
		else if (run_step == 3)
		{
			SetTimer(RunStep3Timer,1000,NULL); 

			m_Edt->ReplaceSel("Run Script 3 test!\r\n");
		}

		//ExportFile();

		//SetDlgItemText(ID_BTN_EXIT , "Exit");
	}
}

void CDIODlg::CloseTestEngine()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if (devHandle != 0)
	{
		closeTestEngine(devHandle);
		devHandle = 0;
		m_Edt->ReplaceSel("CloseTestEngine\r\n");
	}

	//m_Edt->ReplaceSel("CloseTestEngine\r\n");
}

void CDIODlg::QuickTest()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR RunTestCmd[] = "RUN\r\n";
	M8852EchoCmd_ptr = "*INS?\r\n";

	if(MT8852_Handshake_flag == false)
	{
		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("MT8852 handshake fail!\r\n");
		return;
	}

	m_Edt->ReplaceSel("MT8852 QuickTest\r\n");

#ifdef T1_SPI_Debug
	if(OpenTestEngine())
		return;
#endif

	if(SetDUTMode())
	{
		return;
	}

	Sleep(1000);

	I8852InsTimerOut = 0;
	run_step = 0;

	ReadDUT_BDaddress();

	DUT_8852TestReport = s_BDaddress;
	DUT_8852TestReport = DUT_8852TestReport + " -- ";

	//m_Edt->GetWindowText(m_Str);
	//m_Edt->SetFocus();
	//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	//m_Edt->ReplaceSel("MT8852 handshake fail!\r\n");

	SetEUT_BDaddressTo8852();
/*
	MT8852_Op_str.Format(_T("R13"));
	I8852InsTimerOut = 0;
	run_step = 0;
	MT8852COM.WriteToPort(RunTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(RunTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	//SetTimer(M8852INStimer,1500,NULL);
	
	SetTimer(M8852INStimer,500,NULL);
*/
}

void CDIODlg::SetEUT_SELECT_SCRIPT1To8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SetSCRIPTSEL1Cmd[] = "SCPTSEL 1\r\n";
	M8852EchoCmd_ptr = "SCPTSEL?\r\n";
	MT8852_Op_str.Format(_T("SEL 1"));
	MT8852COM.WriteToPort(SetSCRIPTSEL1Cmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetSCRIPTSEL1Cmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,500,NULL); 
}

void CDIODlg::SetEUT_SCRIPT3NameTo8852()
{
	CString s_TestCmd;
	TCHAR ResetCmd[30];
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	s_TestCmd.Format( _TEXT("SCPTNM 3,%s\r\n"), model_name);
	strcpy(ResetCmd, (LPCTSTR)s_TestCmd);

	M8852EchoCmd_ptr = "SCPTNM? 3\r\n";
	MT8852_Op_str.Format(_T("SCPTNM 3"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetEUT_SCRIPT3TxPowerTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR SetSCRIPTSEL3TxPowerCmd[] = "TXPWR 3,-40\r\n";
	M8852EchoCmd_ptr = "TXPWR? 3\r\n";
	MT8852_Op_str.Format(_T("TXPWR 3,-40"));
	MT8852COM.WriteToPort(SetSCRIPTSEL3TxPowerCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetSCRIPTSEL3TxPowerCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::ResetMT8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if(m_MT8852Action.Compare("Reset") == 0)
	{
		//m_Edt->SetSel(0,-1);
		//m_Edt->Clear();

		if(MT8852_Handshake_flag == false)
		{
			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("MT8852 handshake fail!\r\n");
			return;
		}

		run_step = 0;
		IC_test_states = 0;
		TCHAR ResetCmd[] = "*RST\r\n";
		M8852EchoCmd_ptr = "*INS?\r\n";
		MT8852_Op_str.Format(_T("R14"));
		MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
		m_Edt->ReplaceSel(ResetCmd);//Debug
		m_Edt->ReplaceSel("..w\r\n");
#endif
		if(i_MT8852Script != 0)
			SetTimer(M8852INStimer,1000,NULL); 
	}
}

void CDIODlg::SetEUT_SCRIPT3OffsetvalueTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString s_offsetcmd;
	TCHAR  T_offsetcmd[36];

	s_offsetcmd.Format( _TEXT("FIXEDOFF 3,%4.2fDB\r\n"), Fixedoffset);
	lstrcpy(T_offsetcmd, s_offsetcmd);
	M8852EchoCmd_ptr = "FIXEDOFF? 3\r\n";
	MT8852_Op_str.Format(_T("RFIXEDOFF"));
	MT8852COM.WriteToPort(T_offsetcmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(T_offsetcmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetEUT_SCRIPT3PathOffsetTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR SetSCRIPTSEL3PathOffsetCmd[] = "PATHOFF 3,FIXED\r\n";
	M8852EchoCmd_ptr = "PATHOFF? 3\r\n";
	MT8852_Op_str.Format(_T("FIXED"));
	MT8852COM.WriteToPort(SetSCRIPTSEL3PathOffsetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetSCRIPTSEL3PathOffsetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetEUT_ManualtTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR SetManualCmd[] = "SYSCFG EUTSRCE,MANUAL\r\n";
	M8852EchoCmd_ptr = "SYSCFG? EUTSRCE\r\n";
	MT8852_Op_str.Format(_T("MANUAL"));
	MT8852COM.WriteToPort(SetManualCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetManualCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetEUT_SCRIPT3configTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR  T_configcmd[] = "SCPTCFG 3,OP,ON\r\n";
	M8852EchoCmd_ptr = "SCPTCFG? 3\r\n";
	MT8852_Op_str.Format(_T("RON"));
	config_status = 0;
	MT8852COM.WriteToPort(T_configcmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(T_configcmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetICpkgNum()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ICconfigCmd[] = "ICCFG 3,NUMPKTS,100\r\n";
	M8852EchoCmd_ptr = "ICCFG? 3,NUMPKTS\r\n";
	MT8852_Op_str.Format(_T("ICCFG 3,NUMPKTS"));
	MT8852COM.WriteToPort(ICconfigCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ICconfigCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetICHopping()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ICconfigCmd[] = "ICCFG 3,HOPPING,HOPON\r\n";
	M8852EchoCmd_ptr = "ICCFG? 3,HOPPING\r\n";
	MT8852_Op_str.Format(_T("ICCFG 3,HOPPING"));
	MT8852COM.WriteToPort(ICconfigCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ICconfigCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetICHopMode()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ICconfigCmd[] = "ICCFG 3,HOPMODE,ANY\r\n";
	M8852EchoCmd_ptr = "ICCFG? 3,HOPMODE\r\n";
	MT8852_Op_str.Format(_T("ICCFG 3,HOPMODE"));
	MT8852COM.WriteToPort(ICconfigCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ICconfigCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetICTestType()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ICconfigCmd[] = "ICCFG 3,TSTCTRL,LOOPBACK\r\n";
	M8852EchoCmd_ptr = "ICCFG? 3,TSTCTRL\r\n";
	MT8852_Op_str.Format(_T("ICCFG 3,TSTCTRL"));
	MT8852COM.WriteToPort(ICconfigCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ICconfigCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetICMaxOffset()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ICconfigCmd[] = "ICCFG 3,MXPOSLIM,75000\r\n";//Initial carrier Max positive offset = 75000
	M8852EchoCmd_ptr = "ICCFG? 3,MXPOSLIM\r\n";
	MT8852_Op_str.Format(_T("ICCFG 3,MXPOSLIM"));
	MT8852COM.WriteToPort(ICconfigCmd);

	m_Edt->ReplaceSel(ICconfigCmd);

#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ICconfigCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPHopping()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);	

	TCHAR ResetCmd[] = "OPCFG 3,HOPPING,HOPON\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,HOPPING\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,HOPPING"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPPkgType()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "OPCFG 3,PKTTYPE,DH1\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,PKTTYPE\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,PKTTYPE"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPAverageMax()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString s_TestCmd;
	TCHAR ResetCmd[30];
	s_TestCmd.Format( _T("OPCFG 3,AVGMXLIM,%4.2f\r\n"), OPAverageMax);
	strcpy(ResetCmd, (LPCTSTR)s_TestCmd);

	m_Edt->ReplaceSel(s_TestCmd);

	M8852EchoCmd_ptr = "OPCFG? 3,AVGMXLIM\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,AVGMXLIM"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPAverageMin()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString s_TestCmd;
	TCHAR ResetCmd[30];
	s_TestCmd.Format( _T("OPCFG 3,AVGMNLIM,%4.2f\r\n"), OPAverageMin);
	strcpy(ResetCmd, (LPCTSTR)s_TestCmd);

	m_Edt->ReplaceSel(s_TestCmd);

	M8852EchoCmd_ptr = "OPCFG? 3,AVGMNLIM\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,AVGMNLIM"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPTestPeak()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "OPCFG 3,PEAKLIM,23\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,PEAKLIM\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("OPCFG 3,PEAKLIM"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPHopMode()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	//TCHAR ResetCmd[] = "OPCFG 3,HOPMODE,DEFINED\r\n";
	TCHAR ResetCmd[] = "OPCFG 3,HOPMODE,ANY\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,HOPMODE\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,HOPMODE"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPpkgNum()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	//TCHAR ResetCmd[] = "OPCFG 3,NUMPKTS,10\r\n";
	TCHAR ResetCmd[] = "OPCFG 3,NUMPKTS,100\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,NUMPKTS\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,NUMPKTS"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetOPTestType()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "OPCFG 3,TSTCTRL,LOOPBACK\r\n";
	M8852EchoCmd_ptr = "OPCFG? 3,TSTCTRL\r\n";
	MT8852_Op_str.Format(_T("OPCFG 3,TSTCTRL"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSpkgNum()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,NUMPKTS,500\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,NUMPKTS\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,NUMPKTS,500"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL);
}

void CDIODlg::SetSSTxPower()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString s_TestCmd;
	TCHAR ResetCmd[30];
	s_TestCmd.Format( _TEXT("SSCFG 3,TXPWR,%4.2f\r\n"), SSTxPower);
	strcpy(ResetCmd, (LPCTSTR)s_TestCmd);

	M8852EchoCmd_ptr = "SSCFG? 3,TXPWR\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,TXPWR"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSTxPayload()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,PKTCOUNT,TX\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,PKTCOUNT\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,PKTCOUNT,TX"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSHopping()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,HOPPING,HOPON\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,HOPPING\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,HOPPING,HOPON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSDirty()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,DIRTYTX,OFF\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,DIRTYTX\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,DIRTYTX,OFF"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSoffsetTable()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,DIRTYTAB,OFFSET,0,75000,14000,-2000,1000,39000,0,-42000,74000,-19000,-75000\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,DIRTYTAB,OFFSET,0\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,DIRTYTAB,OFFSET,0"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSsymtTable()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,DIRTYTAB,SYMT,0,-20,-20,20,20,20,-20,-20,-20,-20,20\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,DIRTYTAB,SYMT,0\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,DIRTYTAB,SYMT,0"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSLFreqSel()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,LFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,LFREQSEL\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,LFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSLRXFREQ()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,LRXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,LRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,LRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSmodIndex()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,DIRTYTAB,MODINDEX,0,0.28,0.30,0.29,0.32,0.33,0.34,0.29,0.31,0.28,0.35\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,DIRTYTAB,MODINDEX,0\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,DIRTYTAB,MODINDEX,0"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSTxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,LTXFREQ,FREQ,2480 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,LTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,LTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSMedium()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,MFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,MFREQSEL\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,MFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSEutRx()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,MRXFREQ,FREQ,2441 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,MRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,MRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSEutTx()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,MTXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,MTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,MTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSHighFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,HFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,HFREQSEL\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,HFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSRxHighFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,HRXFREQ,FREQ,2480 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,HRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,HRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSTxHighFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,HTXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,HTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("SSCFG 3,HTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSBerLimit()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,BERLIM,0.1\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,BERLIM\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("SSCFG 3,BERLIM"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetSSFerLimit()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "SSCFG 3,FERLIM,100\r\n";
	M8852EchoCmd_ptr = "SSCFG? 3,FERLIM\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("SSCFG 3,FERLIM,100"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIpkgNum()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,NUMPKTS,10\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,NUMPKTS\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,NUMPKTS,10"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMItestType()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,TSTCTRL,LOOPBACK\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,TSTCTRL\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,TSTCTRL,LOOPBACK"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIpkgType()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,PKTTYPE,DH1\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,PKTTYPE\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,PKTTYPE,DH1"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMItogglePad()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,TOGGLE,ONCE\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,TOGGLE\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,TOGGLE,ONCE"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL);
}

void CDIODlg::SetMILFreqSel()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,LFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,LFREQSEL\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,LFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMILRxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,LRXFREQ,FREQ,2480 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,LRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,LRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMILTxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,LTXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,LTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,LTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMImedium()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,MFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,MFREQSEL\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,MFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIRxmediumFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,MRXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,MRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,MRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMImaxTxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,MTXFREQ,FREQ,2441 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,MTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,MTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIHFreqSel()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,HFREQSEL,ON\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,HFREQSEL\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,HFREQSEL,ON"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIHRxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,HRXFREQ,FREQ,2402 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,HRXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,HRXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIHTxFreq()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,HTXFREQ,FREQ,2480 MHz\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,HTXFREQ,FREQ\r\n";
	MT8852_Op_str.Format(_T("MICFG 3,HTXFREQ,FREQ"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIAvgMin()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,F1AVGMIN,140000\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,F1AVGMIN\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("MICFG 3,F1AVGMIN"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIAvgMax()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,F1AVGMAX,175000\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,F1AVGMAX\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("MICFG 3,F1AVGMAX"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIF2MaxLim()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,F2MAXLIM,115000\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,F2MAXLIM\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("MICFG 3,F2MAXLIM"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}

void CDIODlg::SetMIF1F2Max()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ResetCmd[] = "MICFG 3,F1F2MAX,0.8\r\n";
	M8852EchoCmd_ptr = "MICFG? 3,F1F2MAX\r\n";

	m_Edt->ReplaceSel(ResetCmd);

	MT8852_Op_str.Format(_T("MICFG 3,F1F2MAX"));
	MT8852COM.WriteToPort(ResetCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(ResetCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,100,NULL); 
}


void CDIODlg::AddListCtrlItem(CXListCtrl &list , int nItem)
{
	m_Str = ""; 
	list.InsertItem(nItem, m_Str);
	list.SetItemText(nItem, 0, m_Str, RGB(0,0,0), RGB(152,251,152));

	m_Str = m_TestItemStr;                
	list.SetItemText(nItem, 1, m_Str, RGB(0,0,0), RGB(152,251,152));  

    m_Str = "";					
	list.SetItemText(nItem, 2, m_Str , RGB_BLACK , RGB_AZURE);

	m_Str = m_ResultStr;					
	if(m_Str.Compare("PASS") == 0)
		list.SetItemText(nItem, 3, m_Str , RGB_MEDIUMBLUE , RGB_AZURE);
	else if(m_Str.Compare("FAIL") == 0)
		list.SetItemText(nItem, 3, m_Str , RGB_RED , RGB_AZURE);

	m_Str = m_ErrorCodeStr;
	list.SetItemText(nItem, 4, m_Str , RGB(0,0,0) ,RGB_AZURE);
	//list.SetItemText(nItem, 4, m_Str ,RGB_YELLOW , RGB_AZURE);

	list.SetCurSel(nItem);
}

void CDIODlg::ExportFile()
{
	int i;
	CFile file;
	CFile mptfile;
	CFileException e;
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CFileFind FindFile;
	CString tmp1,tmp2,tmp3;

	TCHAR cCurrentDirectory[300]={0};
	
	GetCurrentDirectory( 256, cCurrentDirectory );

	ExportFolder = GetINIData("log file seting" , "exportFolder");

	ExportFolder += "\\";

	if(m_TestStation.Compare("T1") == 0)
		ExportFolder += DummyBD;
	else if(m_TestStation.Compare("T2") == 0)
	{
		if(MPT_RealBT.Compare("") != 0)	
			ExportFolder += MPT_RealBT;
		else
			return;
	}

	ExportFolder += ".txt";

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, ExportFolder);
	//strcat(cCurrentDirectory, "log.txt");
	
	if(exoprtLogFileEnable)
	{
		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		//m_Edt->ReplaceSel("Export log file\r\n");

		if(FindFile.FindFile(cCurrentDirectory , 0))
		{
			DeleteFile(cCurrentDirectory);
			Sleep(1000);
		}

		m_Str = "";
		
		GetDlgItemText(IDC_DSPMESS , m_Str);

		if(!file.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create file\r\n");
			return;
		}

		file.SeekToBegin();

		file.Write(m_Str , m_Str.GetLength());

		m_Str = "=======================================================\r\n";
		file.Write(m_Str , m_Str.GetLength()+1);
		
		for(i = 0 ; i < ListCtrlIndex ; i++)
		{
			tmp1 = m_List.GetItemText(i , 1);//Get test item
			tmp2 = m_List.GetItemText(i , 3);//Get test result
			tmp3 = m_List.GetItemText(i , 4);//Get error code

			m_Str.Format("%-35s %-4s %-4s\r\n",tmp1,tmp2,tmp3);	
			file.Write(m_Str , m_Str.GetLength()+1);
		}
		m_Str = "=======================================================\r\n";
		file.Write(m_Str , m_Str.GetLength()+1);

		file.Close();
	//}

		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		
		ExportFolder = GetINIData("log file seting" , "exportFolder");

		if(m_TestStation.Compare("T1") == 0)
		{
			ExportFolder += "\\t1.txt";
		}
		else if(m_TestStation.Compare("T2") == 0)
		{
			if(MPT_RealBT.Compare("") != 0)	
				ExportFolder += "\\t2.txt";
			else
				return;
		}

		//ExportFolder += ".txt";
		
		strcat(cCurrentDirectory, ExportFolder);

		if(FindFile.FindFile(cCurrentDirectory , 0)) 
		{
			if(!(mptfile.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
			{
				m_Edt->ReplaceSel("Testlog File Open Error!");
				return;
			}
		}
		else
		{
			if(!mptfile.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
			{
				m_Edt->ReplaceSel("Can't create file\r\n");
				return;
			}
		}

		//Recode MPT data stream

		mptfile.SeekToEnd();
			
		MPT_DataStream += "\r\n";
		m_Str = MPT_DataStream;
		mptfile.Write(m_Str , m_Str.GetLength());

		mptfile.Close();
	}	
}

void CDIODlg::ExecuteFile()
{
  CString csExecute;
  CString csExeName = "btapp.exe";
  CString csArguments = "";
  CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

  csExecute=csExeName + " " + csArguments;
  
  SECURITY_ATTRIBUTES secattr; 
  ZeroMemory(&secattr,sizeof(secattr));
  secattr.nLength = sizeof(secattr);
  secattr.bInheritHandle = TRUE;

  HANDLE rPipe, wPipe;

  //Create pipes to write and read data
  CreatePipe(&rPipe,&wPipe,&secattr,0);
  //
  STARTUPINFO sInfo; 
  ZeroMemory(&sInfo,sizeof(sInfo));
  PROCESS_INFORMATION pInfo; 
  ZeroMemory(&pInfo,sizeof(pInfo));
  sInfo.cb=sizeof(sInfo);
  sInfo.dwFlags=STARTF_USESTDHANDLES;
  sInfo.hStdInput=NULL; 
  sInfo.hStdOutput=wPipe; 
  sInfo.hStdError=wPipe;
  char command[1024]; 
  strcpy(command,csExecute.GetBuffer(csExecute.GetLength()));

  //Create the process here.
  CreateProcess(0 ,command,0,0,TRUE,NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo);
  CloseHandle(wPipe);

  //now read the output pipe here.
  char buf[2048];
  DWORD reDword; 
  CString m_csOutput = "";
  CString csTemp = "";
  BOOL res;
  //do
  //{
	//res=::ReadFile(rPipe,buf,2048,&reDword,0);
    //csTemp=buf;
    //m_csOutput+=csTemp.Left(reDword);
  //}while(res);
  //return m_csOutput;
  
  m_Edt->ReplaceSel(csTemp);
}

UINT CDIODlg::BTThreadfun(LPVOID pParam)
{
    int i = 0;
	int j = 0;
	bool BTConnect = true;
	//CWave m_Wave;
	//CString csExecute = "btapp.exe";
	CString m_tmp;
	CString m_Temp = "";
	CString m_BTMessage = "";
	CIniReader  m_IniReader;
	TCHAR cCurrentDirectory[300]={0};
	GetCurrentDirectory( 256, cCurrentDirectory );
	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, INIFile);
	
	m_IniReader.setINIFileName(cCurrentDirectory);

    _STRUCT_THREAD *Thread_Info = (_STRUCT_THREAD*) pParam;

    CDIODlg *ptr = (CDIODlg*)(FromHandle(Thread_Info->m_hwnd));
    CEdit *m_Edt = (CEdit*)(ptr->GetDlgItem(IDC_DSPMESS));

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "btapp.exe";		
	ShExecInfo.lpParameters = "";	
	ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);
	//WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

/*
	//m_Edt->ReplaceSel("BT A2DP Test \r\n");

	//csExecute=csExeName + " " + csArguments;
  
	SECURITY_ATTRIBUTES secattr; 
	ZeroMemory(&secattr,sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data
	CreatePipe(&rPipe,&wPipe,&secattr,0);
	//
	STARTUPINFO sInfo; 
	ZeroMemory(&sInfo,sizeof(sInfo));
	PROCESS_INFORMATION pInfo; 
	ZeroMemory(&pInfo,sizeof(pInfo));
	sInfo.cb=sizeof(sInfo);
	sInfo.dwFlags=STARTF_USESTDHANDLES;
	sInfo.hStdInput=NULL; 
	sInfo.hStdOutput=wPipe; 
	sInfo.hStdError=wPipe;
	char command[1024]; 
	strcpy(command,csExecute.GetBuffer(csExecute.GetLength()));

	//now read the output pipe here.
    char buf[2048];
    DWORD reDword; 
    CString m_csOutput = "";
    CString csTemp = "";
    BOOL res;

    //Create the process here.
    CreateProcess(0 ,command,0,0,TRUE,NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo);
    CloseHandle(wPipe);
	  
	
	//do
	//{
	//	m_BTMessage = m_IniReader.getKeyValue( "Message" , "BTApp");

	//	if(m_BTMessage.Compare(m_Temp) != 0)
	//	{
	//		m_Edt->ReplaceSel("\r\n");
	//		m_Edt->ReplaceSel(m_BTMessage + "\r\n");
	//		m_Temp = m_BTMessage;
	//	}

	//	res=::ReadFile(rPipe,buf,2048,&reDword,0);
	//	csTemp=buf;
	//	m_Edt->ReplaceSel(csTemp);
	//	Sleep(1);
	//}while(res);
*/  
	while(m_BTMessage.Compare("Connect pass!") != 0)
	{
		m_BTMessage = m_IniReader.getKeyValue( "Message" , "BTApp");

		if(m_BTMessage.Compare(m_Temp) != 0)
		{
			m_Edt->GetWindowText(m_tmp);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_tmp.GetLength(),m_tmp.GetLength(),false);
			m_Edt->ReplaceSel("\r\n");
			m_Edt->ReplaceSel(m_BTMessage + "//A2DP//\r\n");
			m_Temp = m_BTMessage;
		}		
		else
			Sleep(3);

		//BT Error
		if(m_BTMessage.Find("Device not found!") != -1)		
		{
			BTConnect = false;
			break;
		}
		else if(m_BTMessage.Find("Connect fail!") != -1)
		{
			BTConnect = false;
			break;
		}
		else if(m_BTMessage.Find("Page fail!") != -1)
		{
			BTConnect = false;
			break;
		}
		else if(m_BTMessage.Find("Fail to initialize BlueSoleil.") != -1)
		{
			BTConnect = false;
			break;
		}
		else if(m_BTMessage.Find("Nokia 360 Wireless disconnect.") != -1)
		{
			BTConnect = false;
			break;
		}
		else if(m_BTMessage.Find("There isn't any Bluetooth hardware detected.") != -1)
		{
			BTConnect = false;
			break;
		}
	}
	
	if(BTConnect)
	{
		//m_Edt->ReplaceSel("A2DP connect pass\r\n");

		m_Edt->ReplaceSel("\r\nBD address : ");
		m_tmp = m_IniReader.getKeyValue( "BD_5" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x:",i);
		m_Edt->ReplaceSel(m_tmp);
		m_tmp = m_IniReader.getKeyValue( "BD_4" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x:",i);
		m_Edt->ReplaceSel(m_tmp);
		m_tmp = m_IniReader.getKeyValue( "BD_3" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x:",i);
		m_Edt->ReplaceSel(m_tmp);
		m_tmp = m_IniReader.getKeyValue( "BD_2" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x:",i);
		m_Edt->ReplaceSel(m_tmp);
		m_tmp = m_IniReader.getKeyValue( "BD_1" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x:",i);
		m_Edt->ReplaceSel(m_tmp);
		m_tmp = m_IniReader.getKeyValue( "BD_0" , "BTApp");
		i = atoi(m_tmp);
		m_tmp.Format("%x\r\n",i);
		m_Edt->ReplaceSel(m_tmp);

		Sleep(1500);

		ptr->PostMessage(WM_PLAYAUDIO,0,0);//Play Audio(Wave file)

		while(m_BTMessage.Compare("Nokia 360 Wireless disconnect.") != 0)
		{
			m_BTMessage = m_IniReader.getKeyValue( "Message" , "BTApp");

			if(m_BTMessage.Compare(m_Temp) != 0)
			{
				m_Edt->GetWindowText(m_tmp);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_tmp.GetLength(),m_tmp.GetLength(),false);
				m_Edt->ReplaceSel("\r\n");
				m_Edt->ReplaceSel(m_BTMessage + "//AVRCP//\r\n");
				m_Temp = m_BTMessage;
			}		
			else
				Sleep(3);
		}
	}
	else
	{
		ptr->PostMessage(WM_BTError,0,0);
	}

	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

	m_Edt->ReplaceSel("btapp process end!\r\n");

	return 1;
}

void CDIODlg::PlayAudio()
{
	char *p;

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	
	m_Edt->ReplaceSel("\r\nPlay wav file : ");
	m_Edt->ReplaceSel(m_AudioFile + "\r\n");

	m_TestItemStr = "BTA2DP Test";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	//m_ErrorCodeStr = "F003";
	m_ResultStr = "PASS";
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	p = m_AudioFile.GetBuffer(0);

	m_Wave1.Load(p);

	if (!m_Wave1.IsPlaying())
	{
		m_Wave1.Play();
	}

	m_AudioFile.ReleaseBuffer();

	SetDlgItemText(IDC_RETEST , "Stop");
	
	SetDlgItemText(IDC_BUTTON1 , "Play");
	
	SetDlgItemText(ID_BTN_EXIT , "A2DPEnd");

	BTA2DPConnect = true;

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	m_Edt->ReplaceSel("****************************************************\r\n");
	CTime t = CTime::GetCurrentTime();
	m_Str = "Current Time :";
	data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	m_Edt->ReplaceSel(data);
	m_Edt->ReplaceSel("	BT A2DP profile & Play Wav Music			       \r\n");	
	m_Edt->ReplaceSel("****************************************************\r\n");
}

void CDIODlg::BTA2DPConnectFail()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	m_Edt->ReplaceSel("PC abort!\r\n");

	m_TestProcess = BTA2DPFail;

	m_TestItemStr = "BT A2DP Test";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	m_ResultStr = "FAIL";
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	if(TestResultFlag == false && m_RFTestResult.Compare("") == 0)
		m_RFTestResult = m_ErrorCodeStr;

	TestResultFlag = false;

	SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

	rgbText = RGB_RED;
	rgbBkgnd = RGB_GRAY1;
	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

	SetTimer(PCAbortTimer , PCAbortTime , NULL);

	if(m_TestStation.Compare("T3") == 0)
	{
		//m_TestProcess = MPT_SentRealBAComd;
		//CDialog::OnCancel();
		//SetDlgItemText(IDC_BUTTON1 , "Reset8852");
	}
}

int CDIODlg::SetDUTMode()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	int32 success = 0;
	int loop = 0;

    if(devHandle != 0)
    {           
        do
        {
            // Make connectable
            //success = bccmdEnableDeviceConnect(handle);
            //if(success != TE_OK)
            //    break;
            
			// Enable DUT mode
            success = bccmdEnableDeviceUnderTestMode(devHandle);
            if(success != TE_OK)
			{
                //break;
				loop++;
				Sleep(100);

				if(loop > 3)
				{
					break;
				}
			}
    
			// Run tests on BT tester
        }while(0);

        if(success != TE_OK)
		{
            //cout << "TestEngine error" << endl;
			m_Edt->ReplaceSel("SetDUTMode Fail!\r\n");
			return 1;
		}
		else
		{
			m_Edt->ReplaceSel("SetDUTMode Pass!\r\n");
			return 0;
		}
    }
	else
	{
		m_Edt->ReplaceSel("Initial TestEngine Fail!\r\n");
		return 1;
	}
}

void CDIODlg::ClearAllEditMessage()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	m_Edt->SetReadOnly(FALSE);//Set Read-only : false
	m_Edt->SetSel(0 , -1);//Select all data
	m_Edt->Clear();
	m_Edt->SetReadOnly(TRUE);//Set Read-only : false
}

void CDIODlg::BHC302_GO_TEST()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if(m_TestStation.Compare("T1") == 0)
	{
		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

		if(i_MT8852Script == 3 && m_MT8852Action.Compare("Reset") == 0)
		{
			//m_Edt->ReplaceSel("Reset MT8852 & Select Script 3\r\n");
			//ResetMT8852();
			SetDlgItemText(IDC_BUTTON1 , "Reset8852");
			TestErrorCode = 0xA000;
		}
		else if(i_MT8852Script == 0 && m_MT8852Action.Compare("Reset") == 0)
		{
			//m_Edt->ReplaceSel("Reset MT8852\r\n");
			//ResetMT8852();
			SetDlgItemText(IDC_BUTTON1 , "Reset8852");
			TestErrorCode = 0xB000;
		}
		else if(m_MT8852Action.Compare("Run") == 0)
		{
			if(i_MT8852Script == 1)
			{
				//Quick Test
				//QuickTest();
				//m_Edt->ReplaceSel("MT8852 Quick Test\r\n");			
				SetDlgItemText(IDC_BUTTON1 , "QuickTest");
				TestErrorCode = 0xC000;
			}
			else if(i_MT8852Script == 3)
			{
				//Script 3 Test
				SetDlgItemText(IDC_BUTTON1 , "Script3Test");
				TestErrorCode = 0xD000;
			}
		}
		else if(m_MT8852Action.Compare("CrystalTrim") == 0 && i_MT8852Script == 3)
		{
			SetDlgItemText(IDC_BUTTON1 , "CrystalTrim");
			TestErrorCode = 0xE000;
		}
	}
	else if(m_TestStation.Compare("T2") == 0)
	{
		/*
		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

		m_Str = GetINIData("BTApp" , "Status");

		if(m_Str.Compare("Run") == 0)
		{			
			m_Edt->ReplaceSel("BT A2DP Test!\r\n");				
			ShellExecute(0 , "open" , "btapp.exe" , NULL , NULL , SW_HIDE );//Execute btapp.exe
			m_ThreadParam.m_hwnd = this->m_hWnd;
			m_ThreadParam.phase = 0;
			m_pLptThrd = AfxBeginThread(BTThreadfun , (LPVOID)&m_ThreadParam);//Create thread to monitor debug message 
		}
		*/
			
		if(BTA2DPConnect == true)
		{
			SetDlgItemText(IDC_BUTTON1 , "Play");
			SetDlgItemText(ID_BTN_EXIT , "A2DPEnd");
		}
		else if(BTA2DPConnect == false)
		{
			SetDlgItemText(IDC_BUTTON1 , "BTA2DP");
			SetDlgItemText(ID_BTN_EXIT , "Exit");
		}

		SetDlgItemText(IDC_RETEST , "Stop");

		TestErrorCode = 0xF000;
		
		//SetDlgItemText(ID_BTN_EXIT , "A2DPEnd");
	}
	else if(m_TestStation.Compare("T1.3") == 0)
	{
		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		SetDlgItemText(IDC_BUTTON1 , "Burn BD");

		TestErrorCode = 0xF100;
	}
	else if(m_TestStation.Compare("T0") == 0)
	{
		SetDlgItemText(IDC_BUTTON1 , "BlueFlash");

		TestErrorCode = 0xF200;
	}
	else if(m_TestStation.Compare("S4") == 0)
	{
		SetDlgItemText(IDC_BUTTON1 , "Database");
	}
}

void CDIODlg::ReadEUT_OPTestTo8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR ReadICSingleTestCmd[] = "ORESULT TEST,0,OP\r\n";
	//TCHAR ReadICSingleTestCmd[] = "ORESULT SCRIPT,1\r\n";
	M8852EchoCmd_ptr = "ORESULT TEST,0,OP\r\n";

	MT8852_Op_str.Format(_T("ROP0,TRUE"));
	MT8852COM.WriteToPort(ReadICSingleTestCmd);

	//m_Edt->ReplaceSel("*****************Read test result from MT8852\r\n");
	//SetTimer(M8852INStimer,200,NULL);
}

void CDIODlg::ReadEUT_MITestTo8852()
{
	TCHAR ReadICSingleTestCmd[] = "ORESULT TEST,0,MI\r\n";
	M8852EchoCmd_ptr = "ORESULT TEST,0,MI\r\n";

	MT8852_Op_str.Format(_T("RMI0,TRUE"));
	MT8852COM.WriteToPort(ReadICSingleTestCmd);
}

void CDIODlg::ReadEUT_SSTestTo8852()
{
	TCHAR ReadICSingleTestCmd[] = "ORESULT TEST,0,SS\r\n";
	M8852EchoCmd_ptr = "ORESULT TEST,0,SS\r\n";

	MT8852_Op_str.Format(_T("RSS0,TRUE"));
	MT8852COM.WriteToPort(ReadICSingleTestCmd);
}

void CDIODlg::MT8852Script3Test()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	i_MT8852Script = 3;
	m_MT8852Action = "Reset";
	m_Edt->ReplaceSel("Reset MT8852 & Select Script 3\r\n");
	ResetMT8852();
	TestErrorCode = 0xA000;

	RunScript3Test = true;
	m_Autorun = "false";
}

int CDIODlg::CSRBlueFlash()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	uint16 len;

	//uint16 PSKEY_LOCAL_DEVICE_NAME[] = {0x6F4E,0x696B,0x2061,0x3633,0x2030,0x6957,0x6572,0x656C,0x7373,0x4220,0x2E33,0x3530};//Nokia 360 Wireless B3.05
	//uint16 PSKEY_LOCAL_DEVICE_NAME[] = {0x6F4E,0x696B,0x2061,0x3633,0x2030,0x6957,0x6572,0x656C,0x7373,0x4220,0x2E33,0x2030};//Nokia 360 Wireless B3.0
	//uint16 PSKEY_LOCAL_DEVICE_NAME[] = {0x6F4E,0x696B,0x2061,0x444D,0x352D,0x5730,0x5320,0x6570,0x6B61,0x7265};//Nokia MD-50W Speaker
	uint16 PSKEY_LOCAL_DEVICE_NAME[] = {0x6f4e,0x696b,0x2061,0x6c50,0x7961,0x3320,0x3036,0xb0c2};//V0.22,Nokia play 360?
	//uint16 PSKEY_LOCAL_DEVICE_NAME[] = {0x6F4E,0x696B,0x2061,0x3633,0x2030,0x6957,0x6572,0x656C,0x7373};//Nokia 360 Wireless
	//uint16 PSKEY_USB_PRODUCT_STRING[] = {0x004e,0x006f,0x006b,0x0069,0x0061,0x0020,0x004d,0x00dd,0x002d,0x0035,0x0030,0x0057,0x0020,0x0053,0x0070,0x0065,0x0061,0x006b,0x0065,0x0072};    
	//uint16 PSKEY_USB_PRODUCT_STRING[] = {0x004e,0x006f,0x006b,0x0069,0x0061,0x0020,0x004d,0x0044,0x002d,0x0035,0x0030,0x0057,0x0020,0x0053,0x0070,0x0065,0x0061,0x006b,0x0065,0x0072};
	uint16 PSKEY_USB_PRODUCT_STRING[] = {0x004e,0x006f,0x006b,0x0069,0x0061,0x0020,0x0050,0x006c,0x0061,0x0079,0x0020,0x0033,0x0036,0x0030,0x00c2,0x00b0};//V0.22
	uint16 PSKEY_USER_38[] = {0x3fff,0x3fff,0x0000,0x007f,0xffff,0x0052,0x0052}; 
	uint16 PSKEY_MIC_BIAS_PIN_VOLTAGE = 0x000B;
	uint16 PSKEY_MIC_BIAS_PIN_CURRENT = 0x0007;
	uint16 PSKEY_ONCHIP_HCI_CLIENT = 0x0001;
	uint16 PSKEY_DIGITAL_AUDIO_CONFIG = 0x0006;
	uint16 PSKEY_DIGITAL_AUDIO_RATE[] = {0x0000,0x0000};
	uint16 PSKEY_DIGITAL_AUDIO_BITS_PER_SAMPLE = 0x0018;
	uint16 PSKEY_DEEP_SLEEP_STATE = 0x0001;
	uint16 PSKEY_PCM_MIN_CPU_CLOCK = 0x0000;
	//uint16 PSKEY_MAX_ACL = 0x0003;//V0.10
	uint16 PSKEY_MAX_ACL = 0x0004;
	//uint16 PSKEY_MAX_SCOS = 0x0001; //V0.14
	uint16 PSKEY_MAX_SCOS = 0x0000;
	//V0.15
	//uint16 PSKEY_LC_ENHANCED_POWER_TABLE[] = {0x2c00,0x0050,0x2600,0x0050,0xf000,0x2800,0x0040,0x2200,0x0040,0xf400,0x2a00,0x0030,0x2400,0x0030,0xf800,0x2d00,0x0020,0x2700,0x0020,0xfc00,0x3100,0x0010,0x2b00,0x0010,0x0000,0x3600,0x0000,0x3000,0x0000,0x0400};
	uint16 PSKEY_LC_ENHANCED_POWER_TABLE[] = {0x2c00,0x0050,0x3800,0x0050,0xf000,0x2800,0x0040,0x3200,0x0040,0xf400,0x2a00,0x0030,0x3400,0x0030,0xf800,0x2d00,0x0020,0x3700,0x0020,0xfc00,0x3100,0x0010,0x3b00,0x0010,0x0000,0x3600,0x0000,0x4000,0x0000,0x0400};
	uint16 PSKEY_FIXED_PIN[] = {0x0030,0x0030,0x0030,0x0030};
	uint16 PSKEY_INITIAL_BOOTMODE = 0x0001;

	//V0.20 : Deep sleep settings
	uint16 PSKEY_CHARGER_EN_DEEP_SLEEP_LEVEL = 0x0002;
	uint16 PSKEY_VREG_EN_DEEP_SLEEP_LEVEL = 0x0002;
	uint16 PSKEY_PIO_DEEP_SLEEP_EITHER_LEVEL[] = {0xffff,0x17c3};
	uint16 PSKEY_USB_ALLOW_DEEP_SLEEP = 0x0003;
	uint16 PSKEY_PIO_WAKEUP_STATE = 0x7ffb;

	uint16 PSKEY_USR10[] = {0x0003,0xa980};
	//uint16 PSKEY_CODEC_IN_GAIN = 0x0008;//V0.81
	//uint16 PSKEY_CODEC_IN_GAIN = 0x0000;//V0.9
	//uint16 PSKEY_CODEC_IN_GAIN = 0x0008;//V0.10
	uint16 PSKEY_CODEC_IN_GAIN = 0x0000;//V0.20
	//uint16 PSKEY_CODEC_IN_GAIN = 0x0000;//V0.14
	//uint16 PSKEY_USB_PIO_DETACH = 0x0002;
	uint16 PSKEY_USB_PIO_DETACH = 0x0000;//V0.20
	//uint16 PSKEY_USB_PIO_DETACH = 0x0000;//V0.15
	uint16 PSKEY_INITIAL_PIO_STATE[] = {0x1000,0x1000,0x1000};//V0.15
	uint16 PSKEY_USB_VENDOR_ID = 0x0a12;
	uint16 PSKEY_USB_PRODUCT_ID = 0xfffe;
	uint16 PSKEY_USB_DFU_PRODUCT_ID = 0xffff;
	// +--------------------------------------------------------------------------+
	// |  BootMode #0  USED FOR DFU                                               |
	// +--------------------------------------------------------------------------+
	// |  BOOTMODE_KEY_LIST_0: Overwrite : HOST_INTRFACE  (1F9)                   |
	// |                                   VM_DISABLE     (25D)                   |
	// +--------------------------------------------------------------------------+
	//uint16 BOOTMODE_KEY_LIST_0[] = {0x01F9,0x03C0,0x0139,0x03CC,0x0087,0x02bf,0x02be}; 
	uint16 BOOTMODE_KEY_LIST_0[] = {0x01F9,0x03C0,0x0139,0x03CC,0x0087,0x02bf,0x02be,0x02ce};//v0.20
	uint16 PSKEY_HOST_INTERFACE = 0x0002;
	uint16 PSKEY_USB_VM_CONTROL = 0x0000;
	uint16 PSKEY_DFU_ENABLE  = 0x0001;
	uint16 PSKEY_ONCHIP_HCI_CLIENT1 = 0x0000;
	uint16 PSKEY_USB_VM_NUM_DESCRIPTORS  = 0x0000;
	uint16 USB_PID = 0xffff;
	uint16 USB_VENDOR_ID = 0x0a12;
	uint16 PSKEY_USB_PIO_DETACH0 = 0x0002;//V0.20
	// +--------------------------------------------------------------------------+
	// |  BootMode #1  NORMAL APPLICATION USAGE (DEFAULT ONE)                     |
	// +--------------------------------------------------------------------------+
	// |  BOOTMODE_KEY_LIST_1: Overwrite : HOST_INTRFACE  (1F9)                   |
	// +--------------------------------------------------------------------------+
	//uint16 BOOTMODE_KEY_LIST_1[] = {0x01F9,0x025D,0x03C0,0x02c6,0x02c5};
	uint16 BOOTMODE_KEY_LIST_1[] = {0x01F9,0x025D,0x03C0,0x02c6,0x02c5,0x02ce};//V0.20
	uint16 HOST_INTERFACE = 0x0002;
	uint16 PSKEY_VM_DISABLE = 0x0000;
	uint16 PSKEY_USB_VM_CONTROL1 = 0x0001;
	uint16 USB_MAX_POWER  = 0x00fa;
	uint16 USB_ATTRIBUTES_POWER = 0x0080;
	uint16 PSKEY_USB_PIO_DETACH1 = 0x0000;

	CProgressCtrl *pProgess =   (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgess->SetRange(0,100);

///*
	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

    if(flInit(1, 26, 1, TFL_SPI_LPT) != TFL_OK)
    {
		m_Edt->ReplaceSel("Failed to initialise flash\r\n");		
		TestErrorCode++;//0xA501
        //return -1;
		return TestErrorCode;
    }
	else
		TestErrorCode++;//0xA501

	pProgess->SetPos(25);

	if(flStopProcessor() != 0)
	{
		m_Edt->ReplaceSel("Failed to stop processor\r\n");
        flClose();
		TestErrorCode++;//0xA502
        //return -1;
		flClose();
		return TestErrorCode;
	}
	else
	{
		TestErrorCode++;//0xA502
		m_Edt->ReplaceSel("Stop processor\r\n");
	}

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	m_TestItemStr = "Erase flash";
	TestErrorCode++;//0xA503
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;

	pProgess->SetPos(50);

	if(flErase() == -1)
	{
		m_Edt->ReplaceSel("Failed to erase flash\r\n");
        flClose();
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
        //return -1;
		flClose();
		return TestErrorCode;
	}
	else
	{
		m_Edt->ReplaceSel("Erase flash\r\n");
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}

	pProgess->SetPos(100);

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	m_TestItemStr = "Load firmware image";
	TestErrorCode++;//0xA504
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;

	//m_Str = "bhc302_t0_v081";
	//m_Str = "mezzo_v0.9.0_21-01-2011";//V0.9
	//m_Str = "mezzo_v0.10.0_31-01-2011";//V0.10
	//m_Str = "mezzo_v0.20_20-04-2011";//V0.20
	//m_Str = m_BHC302_Fw_Version;

	//m_Str = "mezzo_v0.13.0_18-02-2011";//V0.13
	//m_Str = "mezzo_v0.14.0_28-02-2011";//V0.14
	//m_Str = "mezzo_v0.15.0_07-03-2011";//V0.15
	//m_Str = "mezzo_v0.16.0_18-03-2011";

	m_Str = m_BHC302_Fw_Version;//BHC302.INI

    //if(flReadProgramFiles("bhc302_t0_v081") != TFL_OK)//V0.81
	//if(flReadProgramFiles(m_BHC302_Fw_Version) != TFL_OK)//V0.9
	if(flReadProgramFiles(m_Str) != TFL_OK)
    {
		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("Failed to read flash program files\r\n");
        flClose();
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
        //return -1;
		flClose();
		return TestErrorCode;
    }
	else
	{
		m_Edt->ReplaceSel("Load firmware image : " + m_Str);
		m_Edt->ReplaceSel("\r\n");
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	m_TestItemStr = "Write flash";
	TestErrorCode++;//0xA505
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;

    if(flProgramSpawn() != TFL_OK)
    {
        m_Edt->ReplaceSel("Failed to spawn flash program thread\r\n");
		flClose();
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
        //return -1;
		flClose();
		return TestErrorCode;
    }
	else
	{
		m_Edt->ReplaceSel("Write flash...\r\n");
		m_ResultStr = "PASS";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}

    int32 progress = flGetProgress();
    while(progress < 100)
    {
		pProgess->SetPos(progress);
		Sleep(1000);
        progress = flGetProgress();
    }

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	m_Edt->ReplaceSel("Completed\r\n");

    int32 error = flGetLastError();
    if(error != TFL_OK)
    {
		m_Str.Format("Programming failed with error: %d\r\n",error);
		//m_Edt->GetWindowText(m_Str);
		//m_Edt->SetFocus();
		//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
        m_Edt->ReplaceSel(m_Str);
		flClose();
        switch(error)
		{
			case 1:
				m_Str = "FLASH_ERROR_DOWNLOAD_FAILED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 2:
				m_Str = "FLASH_ERROR_VERIFY_FAILED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 3:
				m_Str = "FLASH_ERROR_TIMED_OUT\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 4:
				m_Str = "FLASH_ERROR_CRC_FAILED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 5:
				m_Str = "FLASH_ERROR_READBACK_FAILED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 6:
				m_Str = "FLASH_ERROR_COULD_NOT_DOWNLOAD_PROG\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 7:
				m_Str = "FLASH_ERROR_COULD_NOT_STOP_XAP\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 8:
				m_Str = "FLASH_ERROR_BOOT_PROG_HALTED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 9:
				m_Str = "FLASH_ERROR_ERASE_FAILED\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 10:
				m_Str = "FLASH_ERROR_XAP_ERROR\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 11:
				m_Str = "FLASH_ERROR_UNKNOWN_CHIP_TYPE\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 12:
				m_Str = "FLASH_ERROR_BROADCAST_MIXED_CHIP_TYPES\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 13:
				m_Str = "FLASH_ERROR_OPEN_FILE\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 14:
				m_Str = "FLASH_ERROR_NO_IMAGE\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 15:
				m_Str = "FLASH_ERROR_BUSY\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 16:
				m_Str = "FLASH_ERROR_NO_FLASH\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
			case 17:
				m_Str = "FLASH_ERROR_OOM (out of memory)\r\n";
				m_Edt->ReplaceSel(m_Str);
				break;
		}
		TestErrorCode++;//A506
		//return -1;
		//flClose();
		return TestErrorCode;
    }
	else
		TestErrorCode++;//A506

	//m_Edt->GetWindowText(m_Str);
	//m_Edt->SetFocus();
	//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
    //m_Edt->ReplaceSel("Successfully programmed device\r\n");
	flClose();

	pProgess->SetPos(100);
//*/

///*
	//m_Edt->ReplaceSel("Merge PSkey\r\n");

	if(OpenTestEngine())
	{
		TestErrorCode++;//A507
		//return -1;
		return TestErrorCode;
	}

	//Read BT address 
	//****************************************************************
	ReadDUT_BDaddress();

	SetDlgItemText(IDC_STATIC2 , "BD : " + s_BDaddress);
	//****************************************************************

	m_Edt->ReplaceSel("Write PSkey\r\n");
	
	//psRead(devHandle , 0x4B0 , 0 , 7 , BOOTMODE_KEY_LIST_0 , &len);

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	//if(psWrite(devHandle , 0x108, 0 , 9 ,PSKEY_LOCAL_DEVICE_NAME) != 1)
	//if(psWrite(devHandle , 0x108, 0 , 12 ,PSKEY_LOCAL_DEVICE_NAME) != 1)
	//if(psWrite(devHandle , 0x108, 0 , 10 ,PSKEY_LOCAL_DEVICE_NAME) != 1)
	if(psWrite(devHandle , 0x108, 0 , 8 ,PSKEY_LOCAL_DEVICE_NAME) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_LOCAL_DEVICE_NAME error!\r\n");
		TestErrorCode++;//A508
		//return -1;
		return TestErrorCode;
	}

	//if(psWrite(devHandle , 0x2c2, 0 , 18 ,PSKEY_USB_PRODUCT_STRING) != 1)
	//if(psWrite(devHandle , 0x2c2, 0 , 20 ,PSKEY_USB_PRODUCT_STRING) != 1)
	if(psWrite(devHandle , 0x2c2, 0 , 16 ,PSKEY_USB_PRODUCT_STRING) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_PRODUCT_STRING error!\r\n");
		TestErrorCode++;//A509
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x2b0, 0 , 7 ,PSKEY_USER_38) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USER_38 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1e8, 0 , 1 ,&PSKEY_MIC_BIAS_PIN_VOLTAGE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_MIC_BIAS_PIN_VOLTAGE error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1e9, 0 , 1 ,&PSKEY_MIC_BIAS_PIN_CURRENT) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_MIC_BIAS_PIN_CURRENT error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x3cc, 0 , 1 ,&PSKEY_ONCHIP_HCI_CLIENT) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_ONCHIP_HCI_CLIENT error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1d9, 0 , 1 ,&PSKEY_DIGITAL_AUDIO_CONFIG) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_CONFIG error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1da, 0 , 2 ,PSKEY_DIGITAL_AUDIO_RATE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_RATE error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1db, 0 , 1 ,&PSKEY_DIGITAL_AUDIO_BITS_PER_SAMPLE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_BITS_PER_SAMPLE error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x229, 0 , 1 ,&PSKEY_DEEP_SLEEP_STATE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_DEEP_SLEEP_STATE	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x24d, 0 , 1 ,&PSKEY_PCM_MIN_CPU_CLOCK) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_PCM_MIN_CPU_CLOCK error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}
	
	if(psWrite(devHandle , 0x000d, 0 , 1 ,&PSKEY_MAX_ACL) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_MAX_ACL error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x000e, 0 , 1 ,&PSKEY_MAX_SCOS) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_MAX_SCOS error!\r\n");
		TestErrorCode++;
		return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x0031, 0 , 30 ,PSKEY_LC_ENHANCED_POWER_TABLE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_LC_ENHANCED_POWER_TABLE error!\r\n");
		TestErrorCode++;
		return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x35b, 0 , 4 ,PSKEY_FIXED_PIN) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_FIXED_PIN error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x3cd, 0 , 1 ,&PSKEY_INITIAL_BOOTMODE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_INITIAL_BOOTMODE error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x21bb, 0 , 1 ,&PSKEY_CHARGER_EN_DEEP_SLEEP_LEVEL) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_CHARGER_EN_DEEP_SLEEP_LEVEL error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x21bc, 0 , 1 ,&PSKEY_VREG_EN_DEEP_SLEEP_LEVEL) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_VREG_EN_DEEP_SLEEP_LEVEL error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x21bd, 0 , 2 ,PSKEY_PIO_DEEP_SLEEP_EITHER_LEVEL) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_PIO_DEEP_SLEEP_EITHER_LEVEL error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x02fc, 0 , 1 ,&PSKEY_USB_ALLOW_DEEP_SLEEP) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_ALLOW_DEEP_SLEEP error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x039f, 0 , 1 ,&PSKEY_PIO_WAKEUP_STATE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_PIO_WAKEUP_STATE error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x294, 0 , 2 ,PSKEY_USR10) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USR10 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x1b8, 0 , 1 ,&PSKEY_CODEC_IN_GAIN) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_CODEC_IN_GAIN error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x2ce, 0 , 1 ,&PSKEY_USB_PIO_DETACH) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_PIO_DETACH	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x3b5, 0 , 3 ,PSKEY_INITIAL_PIO_STATE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_INITIAL_PIO_STATE	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x2be, 0 , 1 ,&PSKEY_USB_VENDOR_ID) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_VENDOR_ID	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x2bf, 0 , 1 ,&PSKEY_USB_PRODUCT_ID) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_PRODUCT_ID	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x2cb, 0 , 1 ,&PSKEY_USB_DFU_PRODUCT_ID) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_DFU_PRODUCT_ID	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	//if(psWrite(devHandle , 0x4b0, 0 , 7 ,BOOTMODE_KEY_LIST_0) != 1)
	if(psWrite(devHandle , 0x4b0, 0 , 8 ,BOOTMODE_KEY_LIST_0) != 1)//V0.20
	{
		m_Edt->ReplaceSel("Write BOOTMODE_KEY_LIST_0 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4b8, 0 , 1 ,&PSKEY_HOST_INTERFACE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_HOST_INTERFACE	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4b9, 0 , 1 ,&PSKEY_USB_VM_CONTROL) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_CONTROL	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4ba, 0 , 1 ,&PSKEY_DFU_ENABLE) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_DFU_ENABLE	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4bb, 0 , 1 ,&PSKEY_ONCHIP_HCI_CLIENT1) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_ONCHIP_HCI_CLIENT1	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}
	
	if(psWrite(devHandle , 0x4bc, 0 , 1 ,&PSKEY_USB_VM_NUM_DESCRIPTORS ) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_NUM_DESCRIPTORS 	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4bd, 0 , 1 ,&USB_PID) != 1)
	{
		m_Edt->ReplaceSel("Write USB_PID	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4be, 0 , 1 ,&USB_VENDOR_ID ) != 1)
	{
		m_Edt->ReplaceSel("Write USB_VENDOR_ID	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4bf, 0 , 1 ,&PSKEY_USB_PIO_DETACH0) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_PIO_DETACH0	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}
	//if(psWrite(devHandle , 0x4b0, 0 , 5 ,BOOTMODE_KEY_LIST_1) != 1)
	if(psWrite(devHandle , 0x4b1, 0 , 6 ,BOOTMODE_KEY_LIST_1) != 1)//V0.20
	{
		m_Edt->ReplaceSel("Write BOOTMODE_KEY_LIST_1 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4f8, 0 , 1 ,&HOST_INTERFACE  ) != 1)
	{
		m_Edt->ReplaceSel("Write HOST_INTERFACE 	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4f9, 0 , 1 ,&PSKEY_VM_DISABLE  ) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_VM_DISABLE 	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4fa, 0 , 1 ,&PSKEY_USB_VM_CONTROL  ) != 1)
	{
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_CONTROL 	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4fb, 0 , 1 ,&USB_MAX_POWER   ) != 1)
	{
		m_Edt->ReplaceSel("Write USB_MAX_POWER  	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4fc, 0 , 1 ,&USB_ATTRIBUTES_POWER ) != 1)
	{
		m_Edt->ReplaceSel("Write USB_ATTRIBUTES_POWER	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	if(psWrite(devHandle , 0x4fd, 0 , 1 ,&PSKEY_USB_PIO_DETACH1 ) != 1)
	{
		m_Edt->ReplaceSel("Write USB_ATTRIBUTES_POWER	 error!\r\n");
		TestErrorCode++;
		//return -1;
		return TestErrorCode;
	}

	m_TestItemStr = "Write PSKey";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	m_ResultStr = "PASS";
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	CloseTestEngine();

	/*
	if(psWriteVerify(devHandle , 0x108, 0 , 9 ,PSKEY_LOCAL_DEVICE_NAME) != 1)
		m_Edt->ReplaceSel("Write PSKEY_LOCAL_DEVICE_NAME error!\r\n");

	if(psWriteVerify(devHandle , 0x2c2, 0 , 18 ,PSKEY_USB_PRODUCT_STRING) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_PRODUCT_STRING error!\r\n");

	if(psWriteVerify(devHandle , 0x2b0, 0 , 7 ,PSKEY_USER_38) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USER_38 error!\r\n");

	if(psWriteVerify(devHandle , 0x1e8, 0 , 1 ,&PSKEY_MIC_BIAS_PIN_VOLTAGE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_MIC_BIAS_PIN_VOLTAGE error!\r\n");

	if(psWriteVerify(devHandle , 0x1e9, 0 , 1 ,&PSKEY_MIC_BIAS_PIN_CURRENT) != 1)
		m_Edt->ReplaceSel("Write PSKEY_MIC_BIAS_PIN_CURRENT error!\r\n");

	if(psWriteVerify(devHandle , 0x3cc, 0 , 1 ,&PSKEY_ONCHIP_HCI_CLIENT) != 1)
		m_Edt->ReplaceSel("Write PSKEY_ONCHIP_HCI_CLIENT error!\r\n");

	if(psWriteVerify(devHandle , 0x1d9, 0 , 1 ,&PSKEY_DIGITAL_AUDIO_CONFIG) != 1)
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_CONFIG error!\r\n");

	if(psWriteVerify(devHandle , 0x2c2, 0 , 2 ,PSKEY_DIGITAL_AUDIO_RATE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_RATE error!\r\n");

	if(psWriteVerify(devHandle , 0x1db, 0 , 1 ,&PSKEY_DIGITAL_AUDIO_BITS_PER_SAMPLE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_DIGITAL_AUDIO_BITS_PER_SAMPLE error!\r\n");

	if(psWriteVerify(devHandle , 0x229, 0 , 1 ,&PSKEY_DEEP_SLEEP_STATE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_DEEP_SLEEP_STATE	 error!\r\n");

	if(psWriteVerify(devHandle , 0x24d, 0 , 1 ,&PSKEY_PCM_MIN_CPU_CLOCK) != 1)
		m_Edt->ReplaceSel("Write PSKEY_PCM_MIN_CPU_CLOCK error!\r\n");
	
	if(psWriteVerify(devHandle , 0x000d, 0 , 1 ,&PSKEY_MAX_ACL) != 1)
		m_Edt->ReplaceSel("Write PSKEY_MAX_ACL error!\r\n");

	if(psWriteVerify(devHandle , 0x35b, 0 , 4 ,PSKEY_FIXED_PIN) != 1)
		m_Edt->ReplaceSel("Write PSKEY_FIXED_PIN error!\r\n");

	if(psWriteVerify(devHandle , 0x3cd, 0 , 1 ,&PSKEY_INITIAL_BOOTMODE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_INITIAL_BOOTMODE error!\r\n");

	if(psWriteVerify(devHandle , 0x294, 0 , 2 ,PSKEY_USR10) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USR10 error!\r\n");

	if(psWriteVerify(devHandle , 0x1b8, 0 , 1 ,&PSKEY_CODEC_IN_GAIN) != 1)
		m_Edt->ReplaceSel("Write PSKEY_CODEC_IN_GAIN error!\r\n");

	if(psWriteVerify(devHandle , 0x2ce, 0 , 1 ,&PSKEY_USB_PIO_DETACH) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_PIO_DETACH	 error!\r\n");

	if(psWriteVerify(devHandle , 0x2be, 0 , 1 ,&PSKEY_USB_VENDOR_ID) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_VENDOR_ID	 error!\r\n");

	if(psWriteVerify(devHandle , 0x2bf, 0 , 1 ,&PSKEY_USB_PRODUCT_ID) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_PRODUCT_ID	 error!\r\n");

	if(psWriteVerify(devHandle , 0x2cb, 0 , 1 ,&PSKEY_USB_DFU_PRODUCT_ID) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_DFU_PRODUCT_ID	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4b0, 0 , 7 ,BOOTMODE_KEY_LIST_0) != 1)
		m_Edt->ReplaceSel("Write BOOTMODE_KEY_LIST_0 error!\r\n");

	if(psWriteVerify(devHandle , 0x4b8, 0 , 1 ,&PSKEY_HOST_INTERFACE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_HOST_INTERFACE	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4b9, 0 , 1 ,&PSKEY_USB_VM_CONTROL) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_CONTROL	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4ba, 0 , 1 ,&PSKEY_DFU_ENABLE) != 1)
		m_Edt->ReplaceSel("Write PSKEY_DFU_ENABLE	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4bb, 0 , 1 ,&PSKEY_ONCHIP_HCI_CLIENT1) != 1)
		m_Edt->ReplaceSel("Write PSKEY_ONCHIP_HCI_CLIENT1	 error!\r\n");
	
	if(psWriteVerify(devHandle , 0x4bc, 0 , 1 ,&PSKEY_USB_VM_NUM_DESCRIPTORS ) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_NUM_DESCRIPTORS 	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4bd, 0 , 1 ,&USB_PID) != 1)
		m_Edt->ReplaceSel("Write USB_PID	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4be, 0 , 1 ,&USB_VENDOR_ID ) != 1)
		m_Edt->ReplaceSel("Write USB_VENDOR_ID	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4b0, 0 , 5 ,BOOTMODE_KEY_LIST_1) != 1)
		m_Edt->ReplaceSel("Write BOOTMODE_KEY_LIST_1 error!\r\n");

	if(psWriteVerify(devHandle , 0x4f8, 0 , 1 ,&HOST_INTERFACE  ) != 1)
		m_Edt->ReplaceSel("Write HOST_INTERFACE 	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4f9, 0 , 1 ,&PSKEY_VM_DISABLE  ) != 1)
		m_Edt->ReplaceSel("Write PSKEY_VM_DISABLE 	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4fa, 0 , 1 ,&PSKEY_USB_VM_CONTROL  ) != 1)
		m_Edt->ReplaceSel("Write PSKEY_USB_VM_CONTROL 	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4fb, 0 , 1 ,&USB_MAX_POWER   ) != 1)
		m_Edt->ReplaceSel("Write USB_MAX_POWER  	 error!\r\n");

	if(psWriteVerify(devHandle , 0x4fc, 0 , 1 ,&USB_ATTRIBUTES_POWER ) != 1)
		m_Edt->ReplaceSel("Write USB_ATTRIBUTES_POWER	 error!\r\n");
	*/

	pProgess->SetPos(100);

	//m_Edt->ReplaceSel("Complete!\r\n");
	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
    m_Edt->ReplaceSel("Successfully programmed device\r\n");
//*/
	return 0;
}

int CDIODlg::WriteDUT_BDaddress(CString BDAddress)
{
	//CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	TCHAR* unused; //to fit strtol function

	//get hex value (base 16) 
	PSkeyBD_nap = strtol((LPCTSTR) BDAddress.Left(4),&unused,16);
	PSkeyBD_uap = strtol((LPCTSTR) BDAddress.Mid(4,2),&unused,16);
	PSkeyBD_lap = strtol((LPCTSTR)BDAddress.Right(6),&unused,16);

	m_TestItemStr = "Write BD Address";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	m_ResultStr = "PASS";

	//m_Edt->GetWindowText(m_Str);
	//m_Edt->SetFocus();
	//m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	if (psWriteBdAddr(devHandle,PSkeyBD_lap, PSkeyBD_uap, PSkeyBD_nap) != TE_OK)
	{
		//AfxMessageBox("WriteDUT_FinalBDaddress error");
		//m_Edt->ReplaceSel("WriteDUT_BDaddress fail!\r\n");
		m_ResultStr = "FAIL";
		if(m_TestProcess != DebugSPITimer)
			AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
		return 1;
	}

	if(m_TestProcess != DebugSPITimer)
		AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	//m_Edt->ReplaceSel("WriteDUT_BDaddress pass\r\n");

	return 0;
}

void CDIODlg::PCProcessEnd()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16];
	CString tmp;

	m_Edt->ReplaceSel("Test!\r\n");
}

void CDIODlg::NFCTest()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	CString ReversedBD;
/*
	m_Str = GetINIData("T1.3" , "NFC_Tag_Verify");

	if(m_Str.Compare("true") != 0)
*/

/*
	//m_Edt->ReplaceSel("Read NFC tag image...\r\n");
	//m_Edt->ReplaceSel("Write NFC tag image...\r\n");

	if(m_NFCTag.HWInitialize() == 0)
	{
		m_Edt->ReplaceSel("Mf500InterfaceOpen fail\r\n");
		return;
	}

	//m_Str = m_NFCTag.RdNFCTagImage();
	//m_Edt->ReplaceSel(m_Str);

	m_NFCTag.WriteNFCTagImage();

	//m_NFCTag.MifareClose();
*/

///*
	CString m_str;
	CString m_tag;
	CString str1,str2;
	CString csExecute="NFCTagReader.exe -rr";

	//m_Edt->ReplaceSel("Verify NFC tag image.... ");

	if(m_TestProcess != NFCVerifyProcess)
	{
		GetDlgItemText(IDC_PASSFAIL , m_Str);

		if(m_Str.Compare("") != 0)
		{
			m_PASS.StopBlink(CC_BLINK_BOTH);
			SetDlgItemText(IDC_PASSFAIL , "");
			ClearListData();
			ListCtrlIndex = 0;
			ClearAllEditMessage();
										
			ReadBHC302INIData();
			TestResultFlag = true;
		}
	}

	TestResultFlag = true;

#ifndef WithoutPegoda
	char cCurrentDirectory[300]={0};
	GetCurrentDirectory( 256, cCurrentDirectory );
	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, csExecute);

	SECURITY_ATTRIBUTES secattr; 
	ZeroMemory(&secattr,sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data
	CreatePipe(&rPipe,&wPipe,&secattr,0);
	//
	STARTUPINFO sInfo; 
	ZeroMemory(&sInfo,sizeof(sInfo));
	PROCESS_INFORMATION pInfo; 
	ZeroMemory(&pInfo,sizeof(pInfo));
	sInfo.cb=sizeof(sInfo);
	sInfo.dwFlags=STARTF_USESTDHANDLES;
	sInfo.hStdInput=NULL; 
	sInfo.hStdOutput=wPipe; 
	sInfo.hStdError=wPipe;

	//char command[1024]; 
	//strcpy(command,csExecute.GetBuffer(csExecute.GetLength()));

	//now read the output pipe here.
    char buf[300];
    DWORD reDword; 
    CString m_csOutput = "";
    CString csTemp = "";
    BOOL res;

    //Create the process here.
    CreateProcess(0 ,cCurrentDirectory,0,0,TRUE,NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo);
    CloseHandle(wPipe);
#endif

#ifdef NFC_Tag_B31_  	
	res=::ReadFile(rPipe,buf,192,&reDword,0);
	m_str=buf;
	m_str = m_str.Left(192);
#endif
#ifdef NFC_Tag_B42_
	#ifndef WithoutPegoda
		res=::ReadFile(rPipe,buf,256,&reDword,0);
		m_str=buf;
	#else
		//debug
		m_str = "053400B90020475136000000E111100F01039014240203930504035791020A487312D102046163010130005A2023016170706C69636174696F6E2F766E642E626C7565746F6F74682E65702E6F6F6230210066554433221115094E6F6B6961204D442D35305720537065616B6572040D140420FE000000000000000000000000";
	#endif
	m_str = m_str.Left(256);
#endif

	//Get BT from string
#ifdef NFC_Tag_B31_
	ReversedBD = m_str.Mid(110,12);
#endif	
#ifdef NFC_Tag_B42_
	ReversedBD = m_str.Mid(164,12);
#endif	
	MPT_RealBT = "";
	MPT_RealBT += ReversedBD.Right(2);
	MPT_RealBT += ReversedBD.Mid(8,2);
	MPT_RealBT += ReversedBD.Mid(6,2);
	MPT_RealBT += ReversedBD.Mid(4,2);
	MPT_RealBT += ReversedBD.Mid(2,2);
	MPT_RealBT += ReversedBD.Left(2);

	SetDlgItemText(IDC_STATIC2 , "BD : " + MPT_RealBT);//BD

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	m_Edt->ReplaceSel(m_str+"\r\n");

#ifdef NFC_Tag_B31_
	str1 = m_str.Left(110);
	//m_Edt->ReplaceSel(str1 + "\r\n");//debug
	str1 = str1.Right(110-24);//Discard Page0/1/2(UID)
	m_Edt->ReplaceSel(str1 + "\r\n");//debug
	
	str2 = m_str.Right(70);
	m_Edt->ReplaceSel(str2 + "\r\n");//debug
	m_str = str1 + str2;
#endif
#ifdef NFC_Tag_B42_
	str1 = m_str.Left(164);
	//m_Edt->ReplaceSel(str1 + "\r\n");//debug
	str1 = str1.Right(164-24);//Discard Page0/1/2(UID)
	m_Edt->ReplaceSel(str1 + "\r\n");//debug
	
	str2 = m_str.Right(80);
	m_Edt->ReplaceSel(str2 + "\r\n");//debug
	m_str = str1 + str2;
#endif

	m_Str = GetINIData("T1.3" , "NFC_Tag_Verify");

	if(m_Str.Compare("true") == 0)
	{
		m_tag = GetINIData("T1.3" , "data");
		//m_Edt->ReplaceSel("****\r\n");
		//m_Edt->ReplaceSel(m_str + "\r\n");
		//m_Edt->ReplaceSel(m_tag +"\r\n");

		if(m_tag.Compare(m_str) == 0)	
		{
			m_Edt->ReplaceSel("Verify NFC Tag image.... pass!\r\n");
			m_ResultStr = "PASS";
		}
		else
		{
			m_Edt->ReplaceSel("Verify NFC Tag image.... fail!\r\n");
			TestResultFlag = false;
			m_ResultStr = "FAIL";
		}

		m_TestItemStr = "Verify NFC Tag image";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}

	if(m_TestStation.Compare("S4") != 0)
	{
		if(m_TestProcess != NFCVerifyProcess)
		{
			if(m_ResultStr.Compare("PASS") == 0)
			{
				rgbText = RGB_BLUE;
				SetDlgItemText(IDC_PASSFAIL , "PASS!");
			}
			else
			{
				rgbText = RGB_RED;
				SetDlgItemText(IDC_PASSFAIL , "FAIL!");
			}

			rgbBkgnd = RGB_GRAY1;
			m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			//m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
			m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_FAST);

			NFCVerifyCount = 0;
			SetTimer(NFCVerifyTimer , NFCVerifyTime , NULL);
		}
	}

	m_TestProcess = 0;
//*/
}

void CDIODlg::T13WriteBT()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16];
	CString tmp;
							
#ifdef MPT_Handshake_Message
	if(m_TestProcess != DebugSPITimer)
		m_Edt->ReplaceSel("Send Ack to MPT\r\n");
	else
	{
		ClearAllEditMessage();
		//m_Edt->ReplaceSel("\r\n");
	}
#endif

	m_TestItemStr = "OpenTestEngine";

	if(OpenTestEngine())
	{
		if(m_TestProcess == DebugSPITimer)
		{
			return;
		}
		else
		{
			//OpenTestEngine fail
			//Please check SPI cable or DUT Power

			CloseTestEngine();

			//F0 03 61 64 ==>Abort
			buff[0] = 0xf0;
			buff[1] = 0x03;

			buff[2] = PCMPT_Abort;
			buff[3] = 0x03+PCMPT_Abort;
			//m_TestProcess = PCMPT_Abort;		
																		
			MPTCOM.WriteBufferToPort(buff , 4);		

			m_TestProcess = PCMPT_Abort;

			SetDlgItemText(IDC_RETEST , "Re-Test");

			m_Edt->GetWindowText(m_Str);
			m_Edt->SetFocus();
			m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
			m_Edt->ReplaceSel("Check SPI cable and DUT Power\r\n");

			m_ResultStr = "FAIL";
			TestErrorCode++;
			m_Str.Format("%X",TestErrorCode);
			m_ErrorCodeStr = m_Str;
			AddListCtrlItem(m_List , ListCtrlIndex);
			ListCtrlIndex++;

			TestResultFlag = false;

			if(TestResultFlag == false && m_RFTestResult.Compare("") == 0)
				m_RFTestResult = m_ErrorCodeStr;

			SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

			rgbText = RGB_RED;
			rgbBkgnd = RGB_GRAY1;
			m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

			if(m_PromptMessage)
			{
				if(ToggleBitmap)
				{
					ToggleBitmap = false;
					pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
				}
				else
				{
					ToggleBitmap = true;
					pSplash->SetBitmap(IDB_SPLASH,255,0,255);
				}
				SplashScreenString = "CSR SPI fail!";
				pSplash->SetText(SplashScreenString);		
			}
			return;
		}
	}

	TestErrorCode++;

	m_ResultStr = "PASS";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	if(m_TestProcess != DebugSPITimer)
		AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	if(m_AppAction.Compare("debug") != 0)
	{							
		//Sleep(250);

		ReadDUT_BDaddress();

		m_Edt->ReplaceSel("Original BT = " + s_BDaddress);
		m_Edt->ReplaceSel("\r\n");

		Sleep(50);

		if(m_TestProcess == DebugSPITimer)
		{
			MPT_RealBT = "00025B00A5A5";
			s_BDaddress = "00025B00A5A5";
		}

		if(WriteDUT_BDaddress(MPT_RealBT))
		{
			SetDlgItemText(IDC_RETEST , "Re-Test");		
			m_Edt->ReplaceSel("WriteDUT_BDaddress fail!\r\n");
			rgbText = RGB_RED;
			SetDlgItemText(IDC_PASSFAIL , "Test FAIL!");
			TestResultFlag = false;
		}
		else
			m_Edt->ReplaceSel("WriteDUT_BDaddress pass!\r\n");
	}
	else
	{
		m_Edt->ReplaceSel("*****Debug...\r\n");
	}

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	if(TestResultFlag)
	{
		ReadDUT_BDaddress();

		Sleep(250);
								
		//BT Verification
		if(MPT_RealBT.Compare(s_BDaddress) == 0)
		{
			m_Edt->ReplaceSel("Compare BT pass!\r\n"); 
			m_ResultStr = "PASS";
			//TestResultFlag = true;

			//rgbText = RGB_BLUE;
			//SetDlgItemText(IDC_PASSFAIL , "Test PASS!");
		}
		else
		{
			m_Edt->ReplaceSel("Fail to compare BT !\r\n"); 
			m_ResultStr = "FAIL";
			TestResultFlag = false;

			SetDlgItemText(IDC_RETEST , "Re-Test");		
					
			//rgbText = RGB_RED;
			//SetDlgItemText(IDC_PASSFAIL , "Test FAIL!");
		}

		m_TestItemStr = "Verify Real BT";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		if(m_TestProcess != DebugSPITimer)
			AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;
	}

	CloseTestEngine();	
	
	if(m_TestProcess == DebugSPITimer)
		return;

	//Verify NFC Tag
	m_Str = GetINIData("T1.3" , "NFC_Tag_Verify");

	if(m_Str.Compare("true") == 0)
	{
		/*
		if(m_PromptMessage)
		{
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			SplashScreenString = "請將Pegoda Reader至於上蓋上方,之後按下綠色鈕";
			pSplash->SetText(SplashScreenString);		
		}		
		*/

		m_TestProcess = NFCVerifyProcess;
		m_Str = "請將待測物置於Pegoda Reader上方,之後按下確定鈕";
		AfxMessageBox(m_Str);
		NFCTest();
	}

	m_Edt->ReplaceSel("****************************************************\r\n");
	CTime t = CTime::GetCurrentTime();
	m_Str = "Current Time :";
	data.Format("%s    %d/%d/%d    %d:%d:%d\r\n" , m_Str , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond());
	m_Edt->ReplaceSel(data);
									
	if(TestResultFlag)
	{
		m_Edt->ReplaceSel("	T1.3 PASS!				                   \r\n");
		rgbText = RGB_BLUE;
		SetDlgItemText(IDC_PASSFAIL , "Test PASS!");
	}
	else
	{
		m_Edt->ReplaceSel("	T1.3 FAIL!				                   \r\n");
		rgbText = RGB_RED;
		SetDlgItemText(IDC_PASSFAIL , "Test FAIL!");
	}
	m_Edt->ReplaceSel("****************************************************\r\n");

	rgbBkgnd = RGB_GRAY1;
	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
	
	//TestResultFlag = false;

	if(!(T13RecordBT()))
		TestResultFlag = false;
								
	//F0 03 53 56 ==>PASS
	//F0 03 61 64 ==>Abort
	buff[0] = 0xf0;
	buff[1] = 0x03;

	if(TestResultFlag)
	{
		buff[2] = PC_ProcessEnd;
		buff[3] = 0x03+PC_ProcessEnd;
		//m_TestProcess = PC_ProcessEnd;
		MPTCOM.WriteBufferToPort(buff , 4);

		/*
		//Verify NFC Tag
		m_Str = GetINIData("T1.3" , "NFC_Tag_Verify");

		if(m_Str.Compare("true") == 0)
		{
			if(m_PromptMessage)
			{
				if(ToggleBitmap)
				{
					ToggleBitmap = false;
					pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
				}
				else
				{
					ToggleBitmap = true;
					pSplash->SetBitmap(IDB_SPLASH,255,0,255);
				}
				SplashScreenString = "請將Pegoda Reader至於上蓋上方,之後按下綠色鈕";
				pSplash->SetText(SplashScreenString);		
			}

			
		}
		else
		{
		*/
			m_TestProcess = MPT_Start;

			if(m_PromptMessage)
			{
				SplashScreenString = "BT address燒錄成功,請更換下一塊主板及NFC Tag";
				if(ToggleBitmap)
				{
					ToggleBitmap = false;
					pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
				}
				else
				{
					ToggleBitmap = true;
					pSplash->SetBitmap(IDB_SPLASH,255,0,255);
				}
				pSplash->SetText(SplashScreenString);
			}
		//}
	}
	else
	{
		buff[2] = PCMPT_Abort;
		buff[3] = 0x03+PCMPT_Abort;
		m_TestProcess = PCMPT_Abort;
		MPTCOM.WriteBufferToPort(buff , 4);

		m_TestItemStr = "Write DUT BT";
		m_ResultStr = "FAIL";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(TestResultFlag == false && m_RFTestResult.Compare("") == 0)
			m_RFTestResult = m_ErrorCodeStr;

		if(m_PromptMessage)
		{
			SplashScreenString = "BT address燒錄失敗,請更換下一塊主板及NFC Tag";
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			pSplash->SetText(SplashScreenString);
		}
	}
															
	//MPTCOM.WriteBufferToPort(buff , 4);
}

void CDIODlg::BTReverse()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	
	CString str,str1,str2,str3,str4,str5,str6,bt_export;

	CString RealBTName;

	CString ReverseBT = "";

	CString tmp;

	CFileFind FindFile;

	int len;

	CStdioFile RealBT;
	CStdioFile RealBTReverse;

	TCHAR cCurrentDirectory[300]={0};	

	CFileException e;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	//strcat(cCurrentDirectory, "TestData.txt");
	strcat(cCurrentDirectory, RealBTName);

	if(!(RealBT.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
	{
		m_Edt->ReplaceSel(RealBTName + " Open Error!\r\n");
		return;
	}

	len = RealBTName.GetLength();

	m_Str = RealBTName.Left(len-4);

	m_Str += "_reverse.txt";

	tmp = m_Str;

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, m_Str);

	if(FindFile.FindFile(cCurrentDirectory , 0)) 
		DeleteFile(cCurrentDirectory);

	Sleep(50);

	if(!RealBTReverse.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
	{
		m_Edt->ReplaceSel("Can't create " + m_Str);
		m_Edt->ReplaceSel("\r\n");
		return;
	}

	if(RealBT.GetLength() != 0)
	{
		while(RealBT.ReadString(m_Str))
		{
			str = m_Str.Left(12);
			str1 = str.Left(2);
			str2 = str.Mid(2,2);
			str3 = str.Mid(4,2);
			str4 = str.Mid(6,2);
			str5 = str.Mid(8,2);
			str6 = str.Right(2);

			ReverseBT += str6;
			ReverseBT += str5;
			ReverseBT += str4;
			ReverseBT += str3;
			ReverseBT += str2;
			ReverseBT += str1;
			ReverseBT += "\r\n";
			RealBTReverse.SeekToEnd();
			RealBTReverse.WriteString(ReverseBT);
			ReverseBT = "";
		}

		RealBT.Close();
		RealBTReverse.Close();

		m_Edt->ReplaceSel("BT reverse complete!\r\n");

		AfxMessageBox("Create file : " + tmp);

		CDialog::OnCancel();
	}
}

void CDIODlg::S4Clear()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CFileFind FindFile;
	TCHAR CurrentDir[300]={0};	
	GetCurrentDirectory( 256, CurrentDir );
	strcat(CurrentDir, "\\testlog.txt");

	if(FindFile.FindFile(CurrentDir , 0)) 
	{
		DeleteFile(CurrentDir);
		m_Edt->ReplaceSel("Delete testlog.txt\r\n");
	}
}

void CDIODlg::S4BTExport()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	
	bool FindBT;

	CString str,bt_export,logdat;

	CString RealBTName;

	CFileFind FindFile;

	CStdioFile BTExport;
	CStdioFile RealBT;
	CStdioFile logfile;

	TCHAR cCurrentDirectory[300]={0};	
	TCHAR CurrentDir[300]={0};

	CFileException e;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	bt_export = GetINIData("S4_NFC" , "export_name");//bt

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, bt_export);

	if(FindFile.FindFile(cCurrentDirectory , 0)) 
		DeleteFile(cCurrentDirectory);

	//Check testlog.txt 
	GetCurrentDirectory( 256, CurrentDir );
	strcat(CurrentDir, "\\testlog.txt");

	if((FindFile.FindFile(CurrentDir , 0))) 
	{
		if(!(logfile.Open(CurrentDir, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel("testlog.txt Open Error!\r\n");
			AfxMessageBox("testlog.txt open fail!");
			return;
		}
	}
	else
	{
		/*
		GetCurrentDirectory( 256, CurrentDir );

		strcat(CurrentDir, "\\");
		strcat(CurrentDir, bt_export);

		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		strcat(cCurrentDirectory, RealBTName);

		CopyFile(cCurrentDirectory, CurrentDir,FALSE);
		*/

		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		strcat(cCurrentDirectory, RealBTName);

		if(!(RealBT.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel(RealBTName + " Open Error!\r\n");
			//m_Edt->ReplaceSel("\r\n");
			AfxMessageBox(RealBTName + " Open Error!\r\n");
			return;
		}

		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		strcat(cCurrentDirectory, bt_export);

		if(!BTExport.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create " + BTExport);
			m_Edt->ReplaceSel("\r\n");
			AfxMessageBox("Failed to create file : " + BTExport);
			return;
		}

		while(RealBT.ReadString(m_Str))
		{
			BTExport.SeekToEnd();
			BTExport.WriteString(m_Str + "\r\n");
		}

		BTExport.Close();
		RealBT.Close();

		AfxMessageBox("Create file : " + bt_export);

		CDialog::OnCancel();

		return;
	}

	Sleep(50);

	if(!BTExport.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
	{
		m_Edt->ReplaceSel("Can't create " + BTExport);
		m_Edt->ReplaceSel("\r\n");
		AfxMessageBox("Create file error!");
		return;
	}

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	//strcat(cCurrentDirectory, "TestData.txt");
	strcat(cCurrentDirectory, RealBTName);

	if(!(RealBT.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
	{
		m_Edt->ReplaceSel(RealBTName + " Open Error!\r\n");
		//m_Edt->ReplaceSel("\r\n");
		AfxMessageBox(RealBTName + " Open Error!\r\n");
		return;
	}

	while(RealBT.ReadString(m_Str))
	{
		//str = "";
		//m_btmap.Lookup(m_Str , str);

		FindBT = false;
		
		while(logfile.ReadString(logdat))
		{
			//if(m_Str.Find(logdat) != -1 && logdat.Compare("") != 0 )
			if(logdat.Find(m_Str) != -1 && logdat.Compare("") != 0 )
			{
				FindBT = true;
				break;
			}
		}

		logfile.SeekToBegin();
		
		//if(str.Compare("") == 0)
		if(FindBT == false)
		{
			//m_Edt->ReplaceSel(m_Str + "\r\n");//debug
			BTExport.SeekToEnd();
			BTExport.WriteString(m_Str + "\r\n");
		}
	}

	logfile.Close();
	RealBT.Close();
	BTExport.Close();

	m_Edt->ReplaceSel("BT Export complete!\r\n");

	AfxMessageBox("Create file : " + bt_export);

	CDialog::OnCancel();
}

void CDIODlg::S4LoadData()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	bool btfileexist = false;
	bool snfileexist = false;
	
	CString str;
	CString key;

	CString RealBTName;

	TCHAR cCurrentDirectory[300]={0};	

	CFileException e;
	CFileFind FindFile;

	unsigned int btindex = 0;
	unsigned int snindex = 0;

	CString szFilter = "text|*.txt|*.*|*.*||";
	CFileDialog fd(TRUE,"txt","*.txt",OFN_HIDEREADONLY,szFilter,this);

	CStdioFile BTExport1,BTExport2;
	CString bt_export1,bt_export2;

	m_btmap.RemoveAll();
	m_snmap.RemoveAll();

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory,RealBTName);

	if(!(FindFile.FindFile(cCurrentDirectory , 0)))
	{
		//m_Edt->ReplaceSel("Error! Can't not find BT address import file\r\n");
		AfxMessageBox("Fail to Import BT file");
		CDialog::OnCancel();
	}
	else
	{
		m_Edt->ReplaceSel("Import BT file : ");
	}
 
	///*
	if(fd.DoModal() == IDOK)
	{
		//如果使用者開啟了一個檔案
		//CString szFileName = fd.GetPathName(); //取得開啟檔案的全名(包含路徑)
		CString szFileName = fd.GetFileName();
		//AfxMessageBox(szFileName);
		//ReadBHC302INIData();
		m_Edt->ReplaceSel(szFileName + "\r\n");
		if(szFileName.Compare(RealBTName) != 0)
		{
			AfxMessageBox("Import BT adress fail!");
			CDialog::OnCancel();
		}
	}
	else
	{
		//如果使用者取消
		CDialog::OnCancel();
	}
	//*/

	return;

	bt_export1 = GetINIData("S4_NFC" , "BT_Export1");//bt
	bt_export2 = GetINIData("S4_NFC" , "BT_Export2");//sn

	//Load BT&SN data from file
	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, bt_export1);

	if(FindFile.FindFile(cCurrentDirectory , 0)) 
	{
		if(!(BTExport1.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel(bt_export1 + " Open Error!");
			m_Edt->ReplaceSel("\r\n");
			return;
		}
		btfileexist = true;
	}
	else
	{
		if(!BTExport1.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create " + bt_export1);
			m_Edt->ReplaceSel("\r\n");
			return;
		}
	}

	//m_Edt->ReplaceSel("Load BT data\r\n");

	if(BTExport1.GetLength() != 0)
	{
		while(BTExport1.ReadString(m_Str))
		{
			//m_Edt->ReplaceSel(m_Str);

			btindex++;
			str.Format("%X",btindex);
			key = m_Str.Left(12);
			//m_btmap.SetAt(str , m_Str);
			m_btmap.SetAt(key , str);
		}
	}

	BTExport1.Close();

	//str.Format("Load BT data = %d\r\n",btindex);
	//m_Edt->ReplaceSel(str);

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, bt_export2);

	if(FindFile.FindFile(cCurrentDirectory , 0)) 
	{
		if(!(BTExport2.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel(bt_export2 + " Open Error!");
			m_Edt->ReplaceSel("\r\n");
			return;
		}
		snfileexist = true;
	}
	else
	{
		if(!BTExport2.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create " + bt_export2);
			m_Edt->ReplaceSel("\r\n");
			return;
		}
	}

	//m_Edt->ReplaceSel("Load SN data\r\n");

	if(BTExport2.GetLength() != 0)
	{
		while(BTExport2.ReadString(m_Str))
		{
			//m_Edt->ReplaceSel(m_Str);

			snindex++;
			str.Format("%X",snindex);
			key = m_Str.Left(19);
			//m_snmap.SetAt(str , m_Str);
			m_snmap.SetAt(key , str);
		}
	}

	BTExport2.Close();

	//str.Format("Load SN data = %d\r\n",snindex);
	//m_Edt->ReplaceSel(str);

	if(btindex != snindex)
	{
		m_Edt->ReplaceSel("Initial error\r\n");
		CDialog::OnCancel();
	}

	if(btfileexist == false && snfileexist == false)
	{
		CDialog::OnCancel();
	}

}

bool CDIODlg::T13RecordBT()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CFileFind FindFile;
	TCHAR cCurrentDirectory[300]={0};
	CFile file;
	CFileException e;
	bool datalog = true;
	CTime t;
	CString BuildNum;

	BuildNum = GetINIData("S4_NFC" , "BuildNumber");

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, "T13_testlog.txt");

	if(FindFile.FindFile(cCurrentDirectory , 0)) 
	{
		if(!(file.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel("Testlog File Open Error!");
			datalog = false;
		}
	}
	else
	{
		if(!file.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create testlog.txt\r\n");
			datalog = false;
		}
		else
		{
			file.SeekToBegin();

			m_Str = "BuildNumber	Date      Time            Dummy_BT                  Real_BT             \r\n";
			file.Write(m_Str , m_Str.GetLength());

			m_Str = "===================================================================================\r\n";
			file.Write(m_Str , m_Str.GetLength());

			m_Str = "\r\n";
			file.Write(m_Str , m_Str.GetLength());
		}
	}

	if(datalog == false)
	{
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
		m_Edt->ReplaceSel("Can't log T1.3 test data!\r\n");
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");

		m_ResultStr = "FAIL";
		m_TestItemStr = "Log Dummy BT";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		rgbText = RGB_RED;
		rgbBkgnd = RGB_GRAY1;
		SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
		
		return datalog;
	}

	file.SeekToEnd();

	t = CTime::GetCurrentTime();

	m_Str.Format("%-5s %-2d/%-2d/%-2d %-2d:%-2d:%-2d %-22s %-22s\r\n" , BuildNum , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond() , s_BDaddress , MPT_RealBT);
	//file.Write(m_Str , m_Str.GetLength()+1);
	file.Write(m_Str , m_Str.GetLength());

	file.Close();

	m_Edt->ReplaceSel("Log T1.3 dummy BT\r\n");

	return datalog;
}

void CDIODlg::S4RecordSN()
{
	//BHC302_B3.0 build:
	//SN:   1xxxxxxxxxxxxxxxx15 : 銀色鋁殼
	//		2xxxxxxxxxxxxxxxx11 : 黑色
	//		3xxxxxxxxxxxxxxxx15 : 藍色

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CFile file;
	//CFile BTfile;
	CStdioFile BTfile;
	CStdioFile Logfile;
	//CFile BTExport1,BTExport2;
	CStdioFile BTExport1,BTExport2;
	CString bt_export1,bt_export2;
	CString RealBTName;
	CFileException e;
	CFileFind FindFile;
	CTime t;
	CString str,tmp1,tmp2;
	CString strLine;
	CString BuildNum;
	TCHAR cCurrentDirectory[300]={0};	
	TCHAR BTfileDirectory[300]={0};
	char name[16];
	unsigned char buff[16];
	char pbuf[100];
	UINT nBytesRead;
	int k;
	int findpos = 0;
	bool btverify = true;
	bool FindBT = false;
	bool datalog = true;
	bool BTError = false;
	int BarcodeLen = 0;

	//m_btmap.RemoveAll();
	//m_snmap.RemoveAll();

	m_Str = GetINIData("S4_NFC" , "BarcodeLen");
	BarcodeLen = atoi(m_Str);

	bt_export1 = GetINIData("S4_NFC" , "BT_Export1");//bt
	bt_export2 = GetINIData("S4_NFC" , "BT_Export2");//sn

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	BuildNum = GetINIData("S4_NFC" , "BuildNumber");

	gethostname(name , 1);
	CString HostName = name;

	HostName = "MPT_Host";

#ifdef S4WithMPT
	//Send Ack to MPT
	buff[0] = 0xf0;
	buff[1] = 0x03;
	buff[2] = PCMPT_ACK;
	buff[3] = 0x03+PCMPT_ACK;
		
	MPTCOM.WriteBufferToPort(buff , 4);

	Sleep(250);
#endif

#ifdef S4_Debug
	MPT_RealBT = "";
#endif

	CBarcode dlg(this);

	dlg.m_Mac = MPT_RealBT;

	dlg.DoModal();

	m_Barcode = dlg.m_Bar;//Get Serial number

#ifdef S4_Debug
	MPT_RealBT = dlg.m_Mac;
#endif

	//if(m_Barcode.Compare("") == 0)
	if(m_Barcode.Compare("") == 0 && m_Barcode.GetLength() != BarcodeLen)
	{
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxx\r\n");
		m_Edt->ReplaceSel("Barcode fail!\r\n");
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxx\r\n");

		//AfxMessageBox("Barcode fail!");

		SetDlgItemText(IDC_PASSFAIL , "FAIL");

		rgbText = RGB_RED;
		rgbBkgnd = RGB_GRAY1;
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

		if(m_PromptMessage)
		{
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			SplashScreenString = "Barcode欄位資料不能是空的";
			pSplash->SetText(SplashScreenString);
		}

		m_TestProcess = 0;
		TestErrorCode = 0xF200;

		MPT_RealBT = m_Barcode = "";

		//CDialog::OnCancel();

		return;
	}

	SetDlgItemText(IDC_STATIC1 , "SN : " + m_Barcode);//SN
	SetDlgItemText(IDC_STATIC2 , "BD : " + MPT_RealBT);//BD

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	
	m_Edt->ReplaceSel("Read S/N : " + m_Barcode);
	m_Edt->ReplaceSel("\r\n");

	m_TestItemStr = "Read S/N";
	m_ResultStr = "PASS";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");
	strcat(cCurrentDirectory, "testlog.txt");
	
///*
	GetCurrentDirectory( 256, BTfileDirectory );

	strcat(BTfileDirectory, "\\");
	//strcat(BTfileDirectory, "TestData.txt");
	strcat(BTfileDirectory,RealBTName);

	if(!(FindFile.FindFile(BTfileDirectory , 0)))
	{
		m_Edt->ReplaceSel("Error! Can't not find BT address file\r\n");
		btverify = false;
		//return;
	}

	if(!(BTfile.Open(BTfileDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
	{
		//m_Edt->ReplaceSel("TestData.txt File Open Error!");
		m_Edt->ReplaceSel(RealBTName + " open fail!\r\n");
		btverify = false;
		//return;
	}
	else
	{
		m_Edt->ReplaceSel("Open file : " + RealBTName);
		m_Edt->ReplaceSel("\r\n");
	}

	if(btverify == false)
	{
		m_ResultStr = "FAIL";
		m_TestItemStr = "Check real BT";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);

		rgbText = RGB_RED;
		rgbBkgnd = RGB_GRAY1;
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

		if(m_PromptMessage)
		{
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			SplashScreenString = "BT Address檔案開啟失敗!";
			pSplash->SetText(SplashScreenString);
		}

#ifdef S4WithMPT
		SetTimer(PCAbortTimer , PCAbortTime , NULL);
#else
		m_TestProcess = 0;
		TestErrorCode = 0xF200;
#endif

		MPT_RealBT = m_Barcode = "";

		//CDialog::OnCancel();

		return;
	}

	m_Str = GetINIData("S4_NFC" , "SupportDB");
	
	if(m_Str.Compare("true") == 0)
	{
		if(DBRunQuery())
		{
			FindBT = true;
		}
	}
	else
	{
		///*
		while(BTfile.ReadString(m_Str))
		{
			//m_Edt->ReplaceSel(m_Str);
			
			if(m_Str.Find(MPT_RealBT) != -1)//Find
			{
				FindBT = true;
				break;
			}
		}
		//*/
	}

	m_TestItemStr = "Verify BT address";
	m_ResultStr = "PASS";	

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);

	//if(k == NumOfBT)
	if(FindBT == false)
	{
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
		m_Edt->ReplaceSel("Found invalid BT address!!!\r\n");
		m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");

		m_ResultStr = "FAIL";
		m_TestItemStr = "Verify BT address";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		rgbText = RGB_RED;
		rgbBkgnd = RGB_GRAY1;
		SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
		
#ifdef S4WithMPT
		SetTimer(PCAbortTimer , PCAbortTime , NULL);
		m_TestProcess = MPT_SentRealBAComd;
#else
		m_TestProcess = 0;
#endif
		TestErrorCode = 0xF200;

		BTfile.Close();

		//pSplash->Show();
		if(m_PromptMessage)
		{
			if(ToggleBitmap)
			{
				ToggleBitmap = false;
				pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
			}
			else
			{
				ToggleBitmap = true;
				pSplash->SetBitmap(IDB_SPLASH,255,0,255);
			}
			SplashScreenString = "不合法的BT address!   " + RealBTName;
			SplashScreenString += " 未定義";
			pSplash->SetText(SplashScreenString);
		}

		MPT_RealBT = m_Barcode = "";

		//CDialog::OnCancel();

		return;
	}

	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	BTfile.Close();

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	m_Edt->ReplaceSel("Find real BT in " + RealBTName);
	m_Edt->ReplaceSel("\r\n");

//*/
	if(FindFile.FindFile(cCurrentDirectory , 0)) 
	{
		//依據testlog.txt檢查BT address是否重覆

		if(!(Logfile.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			//m_Edt->ReplaceSel("TestData.txt File Open Error!");
			m_Edt->ReplaceSel("testlog.txt open fail!\r\n");
			//return;
		}
		else
		{
			//m_Edt->ReplaceSel("Open testlog.txt!\r\n");

			//Logfile.SeekToBegin();

			while(Logfile.ReadString(m_Str))
			{
				//m_Edt->ReplaceSel(m_Str + "\r\n");//debug
				///*

				strLine = m_Str;
				
				if(strLine.Compare("") != 0)
				{
					tmp1 = strLine.Mid(68 , 4);//68 ==> String position , 4 ==> String length
					tmp2 = strLine.Mid(69 , 4);
				}

				if(tmp1.Compare(BuildNum) == 0 || tmp2.Compare(BuildNum) == 0)
				{
					//m_Edt->ReplaceSel("Parse data : " + strLine);//debug
					//m_Edt->ReplaceSel("\r\n");

					tmp1 = strLine.Mid(52 , 12);
					tmp2 = strLine.Mid(53 , 12);

					if(tmp1.Compare(MPT_RealBT) == 0 || tmp2.Compare(MPT_RealBT) == 0)
					{
						//m_Edt->ReplaceSel("Find BT,then check S/N \r\n");//debug	
						
						tmp1 = strLine.Mid(30 , 19);
						tmp2 = strLine.Mid(29 , 19);

						if(tmp1.Compare(m_Barcode) != 0 && tmp2.Compare(m_Barcode) != 0)
						{
							//m_Edt->ReplaceSel("Duplicate BT\r\n");//debug
							BTError = true;
							break;
						}
						else
						{
							//m_Edt->ReplaceSel("New..\r\n");//debug
							break;
						}
					}
				}
				//*/
			}

			//m_Str.Format("Find position = %d\r\n",findpos);
			//m_Edt->ReplaceSel(m_Str);

			Logfile.Close();

			if(BTError)
			{
				m_Edt->GetWindowText(m_Str);
				m_Edt->SetFocus();
				m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
				m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
				m_Edt->ReplaceSel("Error! Duplicate BT Address!\r\n");
				m_Edt->ReplaceSel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");

				m_ResultStr = "FAIL";
				m_TestItemStr = "Check BT address";
				TestErrorCode++;
				m_Str.Format("%X",TestErrorCode);
				m_ErrorCodeStr = m_Str;
				AddListCtrlItem(m_List , ListCtrlIndex);

				ListCtrlIndex++;
				rgbText = RGB_RED;
				rgbBkgnd = RGB_GRAY1;
				SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_ErrorCodeStr);
				m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
				m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
					
				#ifdef S4WithMPT
					SetTimer(PCAbortTimer , PCAbortTime , NULL);
					m_TestProcess = MPT_SentRealBAComd;
				#else
					m_TestProcess = 0;
				#endif

				TestErrorCode = 0xF200;
									
				if(m_PromptMessage)
				{
					if(ToggleBitmap)
					{
						ToggleBitmap = false;
						pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
					}
					else
					{
						ToggleBitmap = true;
						pSplash->SetBitmap(IDB_SPLASH,255,0,255);
					}
					SplashScreenString = "重覆的BT adddress!(請參考testlog.txt)";						
					pSplash->SetText(SplashScreenString);
				}

				//Logfile.Close();

				MPT_RealBT = m_Barcode = "";
				
				//CDialog::OnCancel();

				return;
			}
		}

		if(!(file.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel("Testlog File Open Error!");
			datalog = false;
			//return;
		}
	}
	else
	{
		if(!file.Open(cCurrentDirectory, CFile::modeCreate | CFile::modeWrite , &e ))	 
		{
			m_Edt->ReplaceSel("Can't create testlog.txt\r\n");
			datalog = false;
			//return;
		}
		else
		{
			file.SeekToBegin();

			m_Str = "HOSTName  Date      Time            S/N                  BT             BuildNumber\r\n";
			file.Write(m_Str , m_Str.GetLength());

			m_Str = "===================================================================================\r\n";
			file.Write(m_Str , m_Str.GetLength());

			m_Str = "\r\n";
			file.Write(m_Str , m_Str.GetLength());
		}
	}

	file.SeekToEnd();

	t = CTime::GetCurrentTime();

	m_Str.Format("%-8s %-2d/%-2d/%-2d %-2d:%-2d:%-2d %-22s %-15s %-5s\r\n" , HostName , t.GetYear() , t.GetMonth() , t.GetDay() , t.GetHour() , t.GetMinute() , t.GetSecond() , m_Barcode , MPT_RealBT , BuildNum);
	//file.Write(m_Str , m_Str.GetLength()+1);
	file.Write(m_Str , m_Str.GetLength());

	file.Close();

	//Record data
	/*
	str = "";
	m_btmap.Lookup(MPT_RealBT , str);

	if(str.Compare("") == 0)
	{
		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		strcat(cCurrentDirectory, bt_export1);

		if(!(BTExport1.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel(bt_export1 + " Open Error!");
			m_Edt->ReplaceSel("\r\n");
			return;
		}

		BTExport1.SeekToEnd();

		//BTExport1.WriteString("\r\n" + MPT_RealBT);
		BTExport1.WriteString(MPT_RealBT + "\r\n");

		BTExport1.Close();
	}

	str = "";
	m_snmap.Lookup(m_Barcode , str);

	if(str.Compare("") == 0)
	{	
		GetCurrentDirectory( 256, cCurrentDirectory );

		strcat(cCurrentDirectory, "\\");
		strcat(cCurrentDirectory, bt_export2);

		if(!(BTExport2.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
		{
			m_Edt->ReplaceSel(bt_export2 + " Open Error!");
			m_Edt->ReplaceSel("\r\n");
			return;
		}

		BTExport2.SeekToEnd();

		//BTExport2.WriteString("\r\n" + m_Barcode);
		BTExport2.WriteString(m_Barcode + "\r\n");
		
		BTExport2.Close();
	}
	*/

	m_TestItemStr = "Record SN/BT";
	if(datalog)
		m_ResultStr = "PASS";
	else
		m_ResultStr = "FAIL";
	TestErrorCode++;
	m_Str.Format("%X",TestErrorCode);
	m_ErrorCodeStr = m_Str;
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

	if(datalog)
		m_Edt->ReplaceSel("Record SN and BT data....\r\n");

	//rgbText = RGB_BLUE;
	rgbBkgnd = RGB_GRAY1;
	if(datalog)
	{
		rgbText = RGB_BLUE;
		SetDlgItemText(IDC_PASSFAIL , "PASS");
	}
	else
	{
		rgbText = RGB_RED;
		SetDlgItemText(IDC_PASSFAIL , "FAIL");
	}
	m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
	m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

	if(m_PromptMessage)
	{
		if(ToggleBitmap)
		{
			ToggleBitmap = false;
			pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
		}
		else
		{
			ToggleBitmap = true;
			pSplash->SetBitmap(IDB_SPLASH,255,0,255);
		}
		SplashScreenString = "資料紀錄完成,請更換新的待測物";
		pSplash->SetText(SplashScreenString);
	}

#ifdef S4WithMPT
	//Restart...
	m_TestProcess = MPT_SentRealBAComd;
#else
	m_TestProcess = 0;
#endif

	MPT_RealBT = m_Barcode = "";

	TestErrorCode = 0xF200;

	m_Edt->ReplaceSel("\r\n");

	//CDialog::OnCancel();	
}

void CDIODlg::ProcessEnd()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16];

	buff[0] = 0xf0;
	buff[1] = 0x03;

	buff[2] = PC_ProcessEnd;
	buff[3] = 0x03+PC_ProcessEnd;

	MPTCOM.WriteBufferToPort(buff , 4);

	//Sleep(500);

	m_TestProcess = PC_ProcessEnd;

	/*
	if(TestResultFlag)
	{
		SetDlgItemText(IDC_PASSFAIL , "PASS");

		rgbText = RGB_BLUE;
		rgbBkgnd = RGB_GRAY1;
		
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
	}
	*/

	if(m_TestStation.Compare("T2") == 0)
	{
		m_Edt->ReplaceSel("T2 Process End!\r\n");

		if(TestResultFlag)
		{
			SetDlgItemText(IDC_PASSFAIL , "PASS");

			rgbText = RGB_BLUE;
			rgbBkgnd = RGB_GRAY1;
			
			m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
		}
		else
		{
			SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_RFTestResult);

			rgbText = RGB_RED;
			rgbBkgnd = RGB_GRAY1;
			
			m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
			m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );
		}

		m_TestProcess = MPT_Start;

		ShowWindow(SW_SHOWNORMAL);

		if(m_PromptMessage)
		{
		ToggleBitmap = false;

		if(ToggleBitmap)
		{
			ToggleBitmap = false;
			pSplash->SetBitmap(IDB_SPLASH1,255,0,255);
		}
		else
		{
			ToggleBitmap = true;
			pSplash->SetBitmap(IDB_SPLASH,255,0,255);
		}
		
		SplashScreenString = "更新待測物,插入USB接頭,安裝電池,按壓綠色鍵";
		pSplash->SetText(SplashScreenString);
		}

		m_Edt->ReplaceSel("T2 restart!\r\n");

		//ToggleBitmap = false;
		TestResultFlag = true;
		//m_RFTestResult = "";
	}
	else if(m_TestStation.Compare("T1") == 0)
	{
		m_Edt->ReplaceSel("T1 Process End!\r\n");

		if(TestResultFlag)
		{
			rgbText = RGB_BLUE;
			SetDlgItemText(IDC_PASSFAIL , "PASS");
		}
		else	
		{
			rgbText = RGB_RED;
			SetDlgItemText(IDC_PASSFAIL , "FAIL " + m_RFTestResult);
		}
			
		rgbBkgnd = RGB_GRAY1;
		m_PASS.SetTextBlinkColors(rgbText , rgbBkgnd );
		m_PASS.StartBlink(CC_BLINK_TEXT ,CC_BLINK_NORMAL );

		m_TestProcess = MPT_Start;
		TestResultFlag = true;
	}

	ExportFile();

	m_RFTestResult = "";

	MPT_DataStream = "";

	m_clock.StopTimer();
	//m_clock.ClearDate();
}

void CDIODlg::T1WriteDummyBT()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16];
	CString tmp;
	int i;

	m_TestItemStr = "Write dummy BT";
	m_ErrorCodeStr = "";
	m_ResultStr = "";
	AddListCtrlItem(m_List , ListCtrlIndex);
	ListCtrlIndex++;

#ifdef T1_SPI_Debug
	if(OpenTestEngine())
	{
		//OpenTestEngine fail
		//Please check SPI cable or DUT Power

		//F0 03 61 64 ==>Abort
		buff[0] = 0xf0;
		buff[1] = 0x03;

		buff[2] = PCMPT_Abort;
		buff[3] = 0x03+PCMPT_Abort;
		m_TestProcess = PCMPT_Abort;		
																
		MPTCOM.WriteBufferToPort(buff , 4);

		TestResultFlag = false;

		SetDlgItemText(IDC_RETEST , "Re-Test");

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("Check SPI cable and DUT Power\r\n");

		m_TestItemStr = "OpenTestEngine";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(TestResultFlag == false && m_RFTestResult.Compare("") == 0)
			m_RFTestResult = m_ErrorCodeStr;

		return;
	}
#endif

	Sleep(50);

	//TestResultFlag = true;

	i = WriteDUT_BDaddress(DummyBD);

	if(i == 1)
	{
		SetDlgItemText(IDC_RETEST , "Re-Test");		
		rgbText = RGB_RED;
		//SetDlgItemText(IDC_PASSFAIL , "Test FAIL!");
		m_Edt->ReplaceSel("WriteDUT_BDaddress fail!!\r\n");

		TestResultFlag = false;

		m_TestItemStr = "Write DUT BT";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(TestResultFlag == false && m_RFTestResult.Compare("") ==0)
			m_RFTestResult = m_ErrorCodeStr;

		SetTimer(PCAbortTimer , PCAbortTime , NULL);
	}

	if(i == 0)
	{
		ReadDUT_BDaddress();

		Sleep(100);
								
		//BT Verification
		if(DummyBD.Compare(s_BDaddress) == 0)
		{
			m_Edt->ReplaceSel("Write dummy BT success!\r\n"); 
			m_ResultStr = "PASS";
			i = 1;

			SetDlgItemText(IDC_STATIC2 , "BD : " + DummyBD);//BD

			//TestResultFlag = true;
			//rgbText = RGB_BLUE;
			//SetDlgItemText(IDC_PASSFAIL , "Test PASS!");
		}
		else
		{
			m_Edt->ReplaceSel("Fail to write dummy BT!\r\n"); 
			m_ResultStr = "FAIL";
			TestResultFlag = false;
			i = 0;

			SetDlgItemText(IDC_RETEST , "Re-Test");		
			SetDlgItemText(ID_BTN_EXIT , "Exit");

			SetTimer(PCAbortTimer , PCAbortTime , NULL);
					
			//rgbText = RGB_RED;
			//SetDlgItemText(IDC_PASSFAIL , "Test FAIL!");
		}

		m_TestItemStr = "Verify dummy BT";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(i == 0 && m_RFTestResult.Compare("") ==0)
			m_RFTestResult = m_ErrorCodeStr;

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("****************************************************\r\n");
		if(i == 1)
			m_Edt->ReplaceSel("Write dummy BT pass\r\n");
		else
			m_Edt->ReplaceSel("Write dummy BT fail!\r\n");
		m_Edt->ReplaceSel("****************************************************\r\n");
	}

	//else
	//{
	//	m_Edt->GetWindowText(m_Str);
	//	m_Edt->SetFocus();
	//	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	//	m_Edt->ReplaceSel("****************************************************\r\n");
	//	m_Edt->ReplaceSel("Write dummy BT fail!\r\n");
	//	m_Edt->ReplaceSel("****************************************************\r\n");
	//}

#ifdef T1_SPI_Debug
	CloseTestEngine();
#endif

	if(TestResultFlag)
	{
		//F0 03 50 53 ==>PC_RFEnd
		buff[0] = 0xf0;
		buff[1] = 0x03;

		buff[2] = PC_RFEnd;
		buff[3] = 0x03+PC_RFEnd;

		m_TestProcess = PCRFTestEnd;		
																
		MPTCOM.WriteBufferToPort(buff , 4);
	}
}

void CDIODlg::CSRPIOTest()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	unsigned char buff[16];

	Count_Volumn_up = 0;
	Count_Volumn_down = 0;
	Count_One_pair = 0;
///*
	//F0 03 51 53
	//PIO Test Request
	buff[0] = 0xf0;
	buff[1] = 0x03;

	buff[2] = PC_PIOReq;
	buff[3] = 0x03+PC_PIOReq;
																
	MPTCOM.WriteBufferToPort(buff , 4);

	Sleep(100);

	m_Edt->ReplaceSel("Sent PIO_Req to MPT\r\n");
//*/
#ifdef MPT_Process_debug
	#ifdef MPT_Handshake_Message
		m_Edt->ReplaceSel("Debug,skip PIO test\r\n");
	#endif
					
	Sleep(3000);
					
	//F0 03 50 53 ==>PC_RFEnd
	buff[0] = 0xf0;
	buff[1] = 0x03;

	buff[2] = PC_PIOEnd;
	buff[3] = 0x03+PC_PIOEnd;

	MPTCOM.WriteBufferToPort(buff , 4);

	return;
#endif

	//Detect CSR PIO
#ifdef T1_SPI_Debug
	if(OpenTestEngine())
	{
		TestResultFlag = false;

		SetDlgItemText(IDC_RETEST , "Re-Test");

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel("Check SPI cable and DUT Power\r\n");

		m_TestItemStr = "OpenTestEngine";
		TestErrorCode++;
		m_Str.Format("%X",TestErrorCode);
		m_ErrorCodeStr = m_Str;
		m_ResultStr = "FAIL";
		AddListCtrlItem(m_List , ListCtrlIndex);
		ListCtrlIndex++;

		if(TestResultFlag == false && m_RFTestResult.Compare("") == 0)
			m_RFTestResult = m_ErrorCodeStr;

		SetTimer(PCAbortTimer , PCAbortTime , NULL);

		return;
	}
#endif

	if(SetDUTMode())
	{
		return;
	}

	//m_Edt->ReplaceSel("TEST!\r\n");

	Sleep(150);
		
	bccmdSetPio(devHandle , PIOMask , PIOValue);

	Sleep(150);

	Buttonstate = 0xf8; 

	timeCount = 0;

	//m_Edt->ReplaceSel("*****PIO Test*****\r\n");

	//SetTimer(DetectPIOTimer , 500 , 0);

	SetTimer(DetectPIOTimer , 10 , 0);
}

void CDIODlg::SetEUT_CW_OPMD_To8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SetICSingleTestCmd[] = "OPMD CWMEAS\r\n";
	M8852EchoCmd_ptr = "OPMD?\r\n";
	MT8852_Op_str.Format(_T("OPMD CWMEAS"));
	MT8852COM.WriteToPort(SetICSingleTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetICSingleTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,200,NULL); 
}

void CDIODlg::SetEUT_SCRIPT_OPMD_To8852()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	TCHAR SetICSingleTestCmd[] = "OPMD SCRIPT\r\n";
	M8852EchoCmd_ptr = "OPMD?\r\n";
	MT8852_Op_str.Format(_T("OPMD SCRIPT"));
	MT8852COM.WriteToPort(SetICSingleTestCmd);
#ifdef Debug_RS232_Output_Message
	m_Edt->ReplaceSel(SetICSingleTestCmd);//Debug
	m_Edt->ReplaceSel("..w\r\n");
#endif
	SetTimer(M8852INStimer,200,NULL); 
}

void CDIODlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	//CPoint   pt;
	//::GetCursorPos(&pt);

	//m_Str.Format("x : %d, y : %d\r\n",pt.x,pt.y);
	//m_Edt->ReplaceSel(m_Str);

	CRect rect;
	rect.left = 563;
	rect.top = 263;
	rect.right = 1135;
	rect.bottom = 664;

	if(rect.PtInRect(point))
	{
		//m_Edt->ReplaceSel("*");
		::SetCursorPos(1135,700);
	}
	*/
	
	CDialog::OnMouseMove(nFlags, point);
}

bool CDIODlg::ConnectToMySQLServer()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	CString Host = GetINIData("Database" , "host");
	CString User = GetINIData("Database" , "user");
	CString Password = GetINIData("Database" , "password");

	//CString Host = "192.168.1.5";
	//CString Host = "10.7.1.32";
	//CString Host = "localhost";
	//CString User = "bhc302";
	//CString User = "root";
	//CString Password = "kensin123";
	//CString Password = "a754965a";
	
	UINT Port = 3306;

	IsConnected = false;
	
	if ((myData = mysql_init(NULL)) && mysql_real_connect( myData,Host,User,Password,NULL,Port,NULL,0))
	{
		m_Edt->ReplaceSel("Connect to MySQL Server successfully!\r\n");
		IsConnected = true;
		return TRUE;
	}
	else
	{
		CString msg;
		msg.Format("Error : %d , %s\r\n",mysql_errno(myData),mysql_error(myData));
		m_Edt->ReplaceSel(msg);
		m_Edt->ReplaceSel("Fail to connect to database\r\n");
		return FALSE;
	}
	
}

bool CDIODlg::GetDatabases()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if (!IsConnected)
		return FALSE;

	res = mysql_list_dbs(myData, NULL);
	
	while((row = mysql_fetch_row(res)))
	{
		//m_Databases.AddString(row[0]);
		m_Edt->ReplaceSel(row[0]);
		m_Edt->ReplaceSel("\r\n");
	}

	mysql_free_result(res);
	return TRUE;
}

bool CDIODlg::ChangeDB(CString &dbname)
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	if (!IsConnected)
		return FALSE;

	int Err = mysql_select_db(myData, dbname);
	
	if (Err == 0)
	{
		m_Edt->ReplaceSel("Select DB : " + dbname);
		m_Edt->ReplaceSel("\r\n");
		return TRUE;
	}
	else
	{
		CString msg;
		msg.Format("ChangeDB Error : %d , %s\r\n", mysql_errno(myData),mysql_error(myData));
		
		return FALSE;
	}
}

void CDIODlg::ShowDBTables()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	//CString currDB = "bhc302";
	CString currDB = "BHC302";
	MYSQL_RES *res;
	MYSQL_ROW row;
	bool newtable = true;
	CString query;
	CString RealBTName;
	int len;

	if (!IsConnected)
		return;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	len = RealBTName.GetLength();

	m_Str = RealBTName.Left(len-4);

	RealBTName = m_Str;

	if (!ChangeDB(currDB))
		return;

	res = mysql_list_tables(myData, NULL);

	while((row = mysql_fetch_row(res)))
	{
		//m_Tables.AddString(row[0]);
		m_Edt->ReplaceSel(row[0]);
		m_Edt->ReplaceSel("\r\n");

		m_Str = row[0];

		//if(m_Str.Compare("TestData") == 0)
		if(m_Str.Compare(RealBTName) == 0)
		{
			newtable = false;
			m_Edt->ReplaceSel("Table already exist\r\n");
		}
	}
	mysql_free_result(res);

	if(!newtable)
	{
		//len = RealBTName.GetLength();

		//m_Str = RealBTName.Left(len-4);

		//RealBTName = m_Str;

		query = "drop table " + RealBTName;

		if((mysql_query(myData, query) == 0))
		{
			m_Edt->ReplaceSel("drop table " + RealBTName);
			m_Edt->ReplaceSel("\r\n");
		}
		else
		{
			m_Edt->ReplaceSel("drop table fail!\r\n");
		}

		Sleep(50);

		newtable = true;
	}

	if(newtable)
	{
		//m_Edt->ReplaceSel("New table\r\n");

		if(!CreateDBTable())
			return;
	}
}

BOOL CDIODlg::DBRunQuery()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString query;
	MYSQL_FIELD *fd;
	UINT num_fields;
	MYSQL_RES *res;
	MYSQL_ROW row;
	UINT i = 0;
	UINT j = 0;
	CString bt = "";
	CString RealBTName;
	int len;
	CString realbt;

	if (!IsConnected)
		return FALSE;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	len = RealBTName.GetLength();

	m_Str = RealBTName.Left(len-4);

	RealBTName = m_Str;

	//query = "SELECT * FROM info";
	//query = "SELECT * FROM BD_ADDR";
	//query = "select * from BD_ADDR where Address='102030405060'";
	//query = "select * from BD_ADDR where Address='111111222222'";
	//query = "select * from BD_ADDR where Address='333032333009'";//查詢BD_ADDR資料表Address欄位是否有一筆資料為333032333009

	//Construct query command
	//realbt = "123456789DEF";
	m_Str = "select * from ";
	m_Str += RealBTName;
	m_Str += " where bt='";
	//m_Str += "123456789DEF'";
	m_Str += MPT_RealBT;
	m_Str += "'";
	query = m_Str;

	// Run the query
	if (( mysql_query(myData, query) == 0))
	{
		res = mysql_store_result(myData);
		num_fields = mysql_num_fields(res);
		fd = mysql_fetch_fields(res);

		for(i = 0 ; i < num_fields ; i++)
		{	
			m_Edt->ReplaceSel(fd[i].name);
			m_Edt->ReplaceSel("\r\n");
		}

		// Get all the rows in the result set
		while((row = mysql_fetch_row(res)))
		{
			for (j = 0; j < num_fields; j++)
			{
				m_Edt->ReplaceSel(row[j]);
				m_Edt->ReplaceSel("\r\n");
				bt = row[j];
			} 
		}
	}
	else
	{
		m_Str.Format("Error %d in query %s %s\r\n",mysql_errno(myData),query,mysql_error(myData));
		m_Edt->ReplaceSel(m_Str);
	}

	mysql_free_result(res);

	if(bt.Compare("") == 0)
	{
		m_Edt->ReplaceSel("Invalid BT address!\r\n");
		return FALSE;
	}
	else if(bt.Compare(realbt) ==0)
	{
		m_Edt->ReplaceSel("Check RealBT pass!\r\n");
		return TRUE;
	}
	else
		return FALSE;
}

void CDIODlg::DBInsertData()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString query;
	MYSQL_FIELD *fd;
	UINT num_fields;
	MYSQL_RES *res;
	MYSQL_ROW row;
	UINT i = 0;
	UINT j = 0;

	if (!IsConnected)
		return;

	//query = "INSERT INTO info VALUES('102030405060')";//Table : info(新增一筆資料到資料表)
	query = "insert into BD_ADDR values('34','gghhiijjkkll')";
	//query = "select * from BD_ADDR where Address='102030405060'";
		
	if (( mysql_query(myData, query) == 0))
	{
		m_Edt->ReplaceSel("Run mysql_query\r\n");
	}
	else
	{
		m_Edt->ReplaceSel("Insert data fail!\r\n");
	}

	//mysql_free_result(res);
}

BOOL CDIODlg::CreateDBTable()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	CString RealBTName;

	CStdioFile RealBT;

	CString tmp,str;

	TCHAR cCurrentDirectory[300]={0};	

	CString query;

	int len;

	if (!IsConnected)
		return FALSE;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	len = RealBTName.GetLength();

	m_Str = RealBTName.Left(len-4);

	RealBTName = m_Str;

	//Create table : TestData(bt char(12))

	m_Str = "create table ";

	m_Str += RealBTName;

	m_Str += "(bt char(12))";

	//query = "create table TestData(bt char(12))";

	query = m_Str;

	if((mysql_query(myData, query) == 0))
	{
		m_Edt->ReplaceSel("Create table\r\n");
	}
	else
		m_Edt->ReplaceSel("Create table fail!\r\n");

	m_Str = RealBTName;

	RealBTName = GetINIData("S4_NFC" , "RealBTInput");

	GetCurrentDirectory( 256, cCurrentDirectory );

	strcat(cCurrentDirectory, "\\");

	strcat(cCurrentDirectory, RealBTName);

	if(!(RealBT.Open(cCurrentDirectory, CFile::modeReadWrite | CFile::shareDenyNone)))
	{
		m_Edt->ReplaceSel("TestData Open Error!");
		m_Edt->ReplaceSel("\r\n");
		return FALSE;
	}

	RealBTName = m_Str;

	if(RealBT.GetLength() != 0)
	{
		//tmp = "insert into TestData values('";
		tmp = "insert into ";
		tmp += RealBTName;
		tmp += " values('";

		//Insert data
		while(RealBT.ReadString(m_Str))
		{
			str = m_Str.Left(12);

			query = tmp + str;

			query += "')";

			if (( mysql_query(myData, query) != 0))
			{
				m_Edt->ReplaceSel("Insert data fail!\r\n");
				break;
			}
		}

		RealBT.Close();

		m_Edt->ReplaceSel("Insert RealBT complete!\r\n");
	}

	return TRUE;
}

void CDIODlg::BHC302_DEBUG_CMD()
{
	DWORD status;
	char DeviveString[256];

	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	m_Edt->ReplaceSel("bhc302 debug..\r\n");

	//Dump USB Config
	//m_Str = m_FriendlyARMUSBDev.dumpUsbConfig();

	//m_Edt->ReplaceSel(m_Str);

///*
	status = m_USBDev.GetDeviceFromRegistry();

	if(status > 0)
	{
		m_Edt->ReplaceSel("Device found!\r\n");		

		m_USBDev.GetDeviceStrings();
		
		if(m_USBDev.OpenUSBDevice())
		{
			m_Edt->ReplaceSel("OpenUSBDevice success\r\n");

			//m_Edt->ReplaceSel("USB_Tx_Rx_Test! \r\n");	

			//SetTimer(USB_TEST_Timer , USB_TEST_Time , NULL);
		}
		else
			m_Edt->ReplaceSel("OpenUSBDevice fail\r\n");
	}
	else
		m_Edt->ReplaceSel("Device not found!\r\n");
//*/
}

void CDIODlg::DNW_USBDownload()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	CString szFilter = "text|*.txt|*.bin|*.*|*.*||";
	CFileDialog fd(TRUE,"txt","*.txt",OFN_HIDEREADONLY,szFilter,this);

	if(fd.DoModal() == IDOK)
	{
		//如果使用者開啟了一個檔案
		CString szFileName = fd.GetPathName(); //取得開啟檔案的全名(包含路徑)

		m_Edt->GetWindowText(m_Str);
		m_Edt->SetFocus();
		m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
		m_Edt->ReplaceSel(szFileName + "\r\n");

		m_FriendlyARMUSBDev.DNWUSBDownload(szFileName);
	}
}

void CDIODlg::BHC302_USB_TX()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	int i;

	BYTE buf[64];

	/*
	buf[0] = 0x46;
	buf[1] = 0x47;
	buf[2] = 0x48;
	buf[3] = 0x49;
	buf[4] = 0x4a;
	*/

	for(i = 0 ; i < 64 ; i++)
		buf[i] = i;

	//m_USBDev.WriteFileData(&buf[0] , 5);

	m_FriendlyARMUSBDev.UsbTransmit();
	
	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	m_Edt->ReplaceSel("USB Tx\r\n");
}

void CDIODlg::BHC302_USB_RX()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);
	
	BYTE buf[64];

	//m_Edt->ReplaceSel("USB Rx\r\n");

	//m_USBDev.ReadFileData(&buf[0] , 64);

	m_FriendlyARMUSBDev.UsbReceive(&buf[0] , 64);

	m_Edt->GetWindowText(m_Str);
	m_Edt->SetFocus();
	m_Edt->SetSel(m_Str.GetLength(),m_Str.GetLength(),false);
	
	m_Str.Format("USB Rx,%X %X %X\r\n",buf[0],buf[1],buf[2]);

	m_Edt->ReplaceSel(m_Str);
}

void CDIODlg::ConnectDB()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	CString Host = GetINIData("Database" , "host");
	CString User = GetINIData("Database" , "user");
	CString Password = GetINIData("Database" , "password");

	m_Edt->ReplaceSel("Connect to Database..\r\n");
	
	CMySQL dlg;
	
	dlg.Host = Host;
	dlg.User = User;
	dlg.Password = Password;

	dlg.DoModal();
}


void CDIODlg::CSRSPILoopTest()
{
	CEdit *m_Edt = (CEdit*)GetDlgItem(IDC_DSPMESS);

	m_Edt->ReplaceSel("CSR SPI Test!\r\n");

	m_TestProcess = DebugSPITimer;

	SetTimer(DebugSPITimer , DebugSPITime , NULL);
}

void CDIODlg::CSRSPITestStop()
{
	KillTimer(DebugSPITimer);

	m_TestProcess = 0;
}



