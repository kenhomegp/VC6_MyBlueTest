// DIODlg.h : header file
//

#if !defined(AFX_DIODLG_H__271981A7_6E24_4EFC_8511_DB81575214DE__INCLUDED_)
#define AFX_DIODLG_H__271981A7_6E24_4EFC_8511_DB81575214DE__INCLUDED_

#include "ClockST.h"	// Added by ClassView
#include "MyStatic.h"
#include "MyEdit.h"
#include "ColorCtrl.h"
#include "FontCtrl.h"
#include "Barcode.h"
#include "Password.h"
#include "XListCtrl.h"
#include "INI.h"		// Added by ClassView
#include "MySQL.h"

#pragma once

#include "ModelessDialogTracker.h"
#include "TrackDialogC.h"

#include "TestEngine.h"
#include "SerialPort.h"

#include "SplashScreenEx.h"
#include "USBBulk.h"

#include "Wave.h"

#include <mysql.h>

#include "Mifare.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MESSAGE 100
/////////////////////////////////////////////////////////////////////////////
// CDIODlg dialog
class SerialThread;

#define WM_MY_MESSAGE_M8852_GET_RESULT 100
#define WM_MY_MESSAGE_G8246_GET_RESULT 200

class CDIODlg : public CDialog
{
// Construction
public:
	CString m_T21GateWidth;
	CString m_T21Freq;
	CString m_T21TestLoop;
	CString m_T21PowerMin;
	CString m_T21PowerMax;
	void SetEUT_CW_OPMD_To8852();
	void DNW_USBDownload();
	bool T13RecordBT();
	void CSRSPITestStop();
	void CSRSPILoopTest();
	void S4Clear();
	void NFCTest();
	CMifare m_NFCTag;
	void ConnectDB();
	CUSBBulk m_FriendlyARMUSBDev;
	CUSBBulk m_USBDev;
	void BHC302_USB_RX();
	void BHC302_USB_TX();
	void BHC302_DEBUG_CMD();
	CMenu m_menu;
	CString m_BHC302_Fw_Version;
	CString T3_file;
	bool m_PromptMessage;
	BOOL CreateDBTable();
	void DBInsertData();
	BOOL DBRunQuery();
	void ShowDBTables();
	bool ChangeDB(CString& dbname);
	bool IsConnected;
	bool GetDatabases();
	bool ConnectToMySQLServer();
	MYSQL * myData;
	void BTReverse();
	void S4BTExport();
	void S4LoadData();
	CMapStringToString m_snmap;
	CMapStringToString m_btmap;
	void SetEUT_SCRIPT_OPMD_To8852();
	void BTA2DPConnectFail();
	void GenerateDummyBT();
	CString SplashScreenString;
	CSplashScreenEx *pSplash;
	CString ExportFolder;
	void T1WriteDummyBT();
	void CreateGUID();
	CString m_RFTestResult;
	void ProcessEnd();
	int MPT_DataStreamLen;
	CString MPT_DataStream;
	int NumOfBT;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void S4RecordSN();
	CString MPT_ErrorCode;
	void ReadBHC302INIData();
	void T13WriteBT();
	CString MPT_RealBT;
	void PCProcessEnd();
	int WriteDUT_BDaddress(CString BDAddress);
	int m_TestProcess;
	CString m_AppAction;
	uint16 CrystalTrimOldValue;
	void CSRPIOTest();
	int CSRBlueFlash();
	void MT8852Script3Test();
	void ReadEUT_SSTestTo8852();
	void ReadEUT_MITestTo8852();
	void ReadEUT_OPTestTo8852();
	int Query8852StatusCount;
	void BHC302_GO_TEST();
	void ClearAllEditMessage();
	int SetDUTMode();
	CString m_AudioFile;
	CString m_TestStation;
	void PlayAudio();
	static UINT BTThreadfun(LPVOID pParam);
	CWave m_Wave1;
	void ExecuteFile();
	void PlayMusic();
	CString DummyBD;
	CString m_Autorun;
	CString m_finalBDAddress;
	void ExportFile();
	int ListCtrlIndex;
	void AddListCtrlItem(CXListCtrl &list , int nItem);
	CString m_ResultStr;
	CString m_ErrorCodeStr;
	CString m_TestItemStr;
	int m_activeCurrenttimeDelay;
	float m_MinActiveCurrent;
	float m_MaxActiveCurrent;
	void SetMIF1F2Max();
	void SetMIF2MaxLim();
	void SetMIAvgMax();
	void SetMIAvgMin();
	void SetMIHTxFreq();
	void SetMIHRxFreq();
	void SetMIHFreqSel();
	void SetMImaxTxFreq();
	void SetMIRxmediumFreq();
	void SetMImedium();
	void SetMILTxFreq();
	void SetMILRxFreq();
	void SetMILFreqSel();
	void SetMItogglePad();
	void SetMIpkgType();
	void SetMItestType();
	void SetMIpkgNum();
	void SetSSFerLimit();
	void SetSSBerLimit();
	void SetSSTxHighFreq();
	void SetSSRxHighFreq();
	void SetSSHighFreq();
	void SetSSEutTx();
	void SetSSEutRx();
	void SetSSMedium();
	void SetSSTxFreq();
	void SetSSmodIndex();
	void SetSSLRXFREQ();
	void SetSSLFreqSel();
	void SetSSsymtTable();
	void SetSSoffsetTable();
	void SetSSDirty();
	void SetSSHopping();
	void SetSSTxPayload();
	double SSTxPower;
	void SetSSTxPower();
	void SetSSpkgNum();
	void SetOPTestType();
	void SetOPpkgNum();
	void SetOPHopMode();
	void SetOPTestPeak();
	double OPAverageMin;
	void SetOPAverageMin();
	double OPAverageMax;
	void SetOPAverageMax();
	void SetOPPkgType();
	void SetOPHopping();
	void SetICMaxOffset();
	void SetICTestType();
	void SetICHopMode();
	void SetICHopping();
	void SetICpkgNum();
	uint8 config_status;
	void SetEUT_SCRIPT3configTo8852();
	double Fixedoffset;
	void SetEUT_ManualtTo8852();
	void SetEUT_SCRIPT3PathOffsetTo8852();
	void SetEUT_SCRIPT3OffsetvalueTo8852();
	void ResetMT8852();
	void SetEUT_SCRIPT3TxPowerTo8852();
	CString model_name;
	void SetEUT_SCRIPT3NameTo8852();
	CString m_MT8852Action;
	uint8 i_MT8852Script;
	uint16 i_MT8852Baudrate;
	uint16 i_MPTBaudrate;
	uint16 i_MPTComPort;
	uint16 i_MT8852ComPort;
	void SetEUT_SELECT_SCRIPT1To8852();
	void QuickTest();
	double Second_IC_averageoffset;
	double First_IC_averageoffset;
	uint8 crystaltrim_diff;
	uint16 SecondCrystalTrim;
	uint16 FirstCrystalTrim;
	CString m_strCrystaltrimReport;
	void CloseTestEngine();
	double IC_averageoffset;
	uint8 IC_test_states;
	uint16 DefPSkeyCrystalTrim;
	uint16 CrystalTrimLowerlimit;
	uint16 CrystalTrimUpperlimit;
	void SET_SINGLE_InitCarrier_RUN_TEST();
	void GetValueFromString(TCHAR *chararray, uint16 start_point, uint16 find_counter);
	TCHAR TRICO[60];
	void ReadEUT_ICSingleTestTo8852();
	uint8 I8852InsTimerOut;
	void SetEUT_RunTestTo8852();
	void SetEUT_ICSingleTestTo8852();
	uint8 run_step;
	void SetEUT_SELECT_SCRIPT3To8852();
	TCHAR M8852_echo_BD[14];
	void SetEUT_BDaddressTo8852();
	void radiotestDUT_CrystalFrequencyTrim(uint16 XtalFtrim);
	uint16 PSkeyCrystalTrim;
	void WriteDUT_CrystalFrequencyTrim();
	int OpenTestEngine();
	void CrystalTrimTest();
	CString DUT_8852TestReport;
	UINT Msg;
	LONG OnMyMessageM8852_GetResult(WPARAM wParam, LPARAM lParam);
	bool m_M8852IsHandShakingFlag;
	CString MT8852_Op_str;
	TCHAR * M8852EchoCmd_ptr;
	CString m_M8852StrReceived;
	CSerialPort MT8852COM;
	TCHAR TBDaddress[13];
	CString s_BDaddress;
	uint16 PSkeyBD_nap;
	uint8 PSkeyBD_uap;
	uint32 PSkeyBD_lap;
	uint32 devHandle;
	void ReadDUT_BDaddress();
	CSerialPort MPTCOM;
	CString MPT_Recv_string;
	TCHAR * MPT_EchoCmd_ptr;
	afx_msg LONG OnCommunication(WPARAM ch , LPARAM port);
	void SetUUT1();
	BOOL CheckLAN();
	void OpenSFIS1();
	TrackDialogC TrackSFIS1;
	//TrackDialogC TrackSFIS2;
	//TrackDialogC TrackSFIS3;
	//TrackDialogC TrackSFIS4;
	//TrackDialogC TrackSFIS5;
	//TrackDialogC TrackSFIS6;
	void RunTest();
	void OpenSixUUT();
	TrackDialogC TrackUUT6;
	TrackDialogC TrackUUT5;
	void ClearListData();
	void DisplayResult(int nItem , int result);
	CFile m_FileCtrl;
	void DisableKbd();
	TrackDialogC TrackUUT4;
	TrackDialogC TrackUUT3;
	TrackDialogC TrackUUT2;
	TrackDialogC TrackUUT1;
	void ScanBarcode();
	void OnUUTClosed();
	//ModelessDialogTracker TrackUUT;
	short LPTBaseIO;
	CString m_LptBaseIO;
	static BOOL ActiveTest;
	CString m_Data;
	static UINT SFISSocketThread(LPVOID pParam);
	CWinThread *ConnectSFIS;
	static UINT IrDAThread(LPVOID pParam);
	CWinThread *m_LPTThread;
	BOOL ActiveIrDA;
	void DisplayTest(int nItem);
	void ShowTEST(CString &Str);
	CString GetFileData(CString Filename , CString section , CString key);
	BOOL m_SwitchCRT;
	static BOOL ActiveSerial;
	HANDLE handlePort_;
	BOOL Openport();
	SerialThread* serialProcess ;
	DCB configSerial_;
	void OnFileSharing();
	void UpdateINIValue(CString m_keyvalue , CString m_key , CString m_section);
	double low;
	double high;
	CString GetINIData(CString section, CString key);
	void TestTimer();
	void SendFAIL();
	void SendPASS();
	void SendData();
	BOOL ConnectToServer();
	BOOL InitWinSock1();
	CString m_szPort;
	CString m_szIP;
	void INIFileTest();
	CIniReader m_IniReader;
	void EndTHread();
	void DispProgress(int n);
	void TestLog();
	void ResumeThread();
	void SuspendThread();
	int post;
	void Winwake();
	static UINT TestThread(LPVOID pParam);
	void ThreadTest();
	void CMOSError();
	BOOL ChkLanCopy();
	BOOL CheckPost();
	BOOL Chkpassfail();
	void ClearString();
	void DisablePost();
	void ShowItem();
	void GetLocalName();
	void Reset();
	void Stop();
	void Start();
	BOOL Find();
	int Loop;
	void LoopTest();
	void DIO96Ctrl();
	void DispPostCode();
	void Set0V();
	BOOL RJ11LoopTest();
	BOOL WakeUpTest();
	void EnableRJ11();
	void TIPOFF();
	void TIPON();
	void RINGOFF();
	void RINGON();
	void TurnOffHDD();
	void TurnOnHDD();
	void SetWOL();
	void Set12V();
	void Set10V();
	void Set9V();
	BOOL ChkRJ11();
	BOOL ChkPowerOff();
	BOOL ChkPowerOn();
	BOOL WakeOnLan();
	void USBOff();
	void USBOn();
	CFile FileCtrl;
	CString data;
	CString index;
	double current;
	void ThermalShutdown();
	void SetBIOS3();
	void SetBIOS2();
	void SetBIOS1();
	int BootSequence;
	int Item;
	void EnablePost();
	void LidSwitchOff();
	CFileFind findfile;
	BOOL ChkRange(double  i  , double j);
	double RdGPIBCurrent();
	void LidSwitchOn();
	BOOL ChkDIOStatus();
	void FixturePower();
	void DisableSMBUS();
	void EnableSMBUS();
	void DisableBattery();
	void EnableBattery();
	CString m_Str;
	int  m_Time2;
	int  m_Time1;
	void GotoNextPage();
	CMapStringToString m_MapCtrl;
	void DispXList(int nItem , int passfail);
	void OnAllPASS();
	void TestCtrl();
	CMapStringToString m_Map;
	CStringArray m_File;
	void FillListCtrl(CXListCtrl& list);
	void InitListCtrl(CXListCtrl& list);
	void InitialGUI();
	int  m_Password;
	void DispPost(CString &post);
	void DispString(CString &str);
	static BOOL ActiveProcess;
	void KbdCtrl(int i);
	void SetEnv();
	void SWtoHOST();
	void SWtoUUT();
	void AC_Off();
	void AC_On();
	BOOL PMTest();
	COLORREF rgbBkgnd;
	COLORREF rgbText;
	BOOL m_CheckMac;
	BOOL IniDIO96();
	BOOL m_Initial;
	BOOL m_CheckBar;
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnUUTFail();
	static UINT DIO96Threadfun(LPVOID pParam);
	CWinThread *m_pLptThrd;
	void Sendkey(int keyvalue);
	void Sendkeymatrix();
	BOOL ReadFixtureID();
	void Power_off();
	void SetBIOS();
	void Power_on();
	BOOL SetGPIB(double V , double I);
	CClockST m_clock;
	CDIODlg(CWnd* pParent = NULL);	// standard constructor
	CFontCtrl <CColorCtrl<CStatic> > m_Boot;
	CFontCtrl <CColorCtrl<CStatic> > m_Status;
    CFontCtrl <CColorCtrl<CStatic> > m_Mac;
    CFontCtrl <CColorCtrl<CStatic> > m_Bar;
	CColorCtrl<CItalicCtrl<CButton> > m_Start;
	CColorCtrl<CItalicCtrl<CButton> > m_Stop;
	CColorCtrl<CItalicCtrl<CButton> > m_ReTest;
    CColorCtrl<CItalicCtrl<CButton> > m_End;
	//CColorCtrl<CBoldCtrl < CRichEditCtrl > > m_Edit;
	CFontCtrl <CColorCtrl<CStatic> > m_ComputerName;
	CFontCtrl <CColorCtrl<CStatic> > m_PASS;
	CXListCtrl		m_List;
	CRichEditCtrl	m_Edit;

// Dialog Data
	//{{AFX_DATA(CDIODlg)
	enum { IDD = IDD_DIO_DIALOG };
	CString	m_Barcode;
	CString	m_Maccode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnBtnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDebug();
	afx_msg void OnKeytest();
	afx_msg void OnClosefile();
	afx_msg void OnSerialCtrl();
	afx_msg void CRTSwitch();
	afx_msg void CloseSerial();
	afx_msg void OnRs232send();
	afx_msg void OnLpttest();
	afx_msg void OnUUT();
	afx_msg void OnNotifyUUT();
	afx_msg void OnBattTest();
	afx_msg void OnNextpage();
	afx_msg void OnReTest();
	afx_msg void OnFCTExit();
	afx_msg void CloseUUT1();
	afx_msg void CloseUUT2();
	afx_msg void CloseUUT3();
	afx_msg void CloseUUT4();
	afx_msg void CloseUUT5();
	afx_msg void CloseUUT6();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIODLG_H__271981A7_6E24_4EFC_8511_DB81575214DE__INCLUDED_)
