// SerialThread.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "SerialThread.h"
#include "DIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SerialThread

IMPLEMENT_DYNCREATE(SerialThread, CWinThread)

SerialThread::SerialThread()
{

}

SerialThread::~SerialThread()
{
}

BOOL SerialThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int SerialThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(SerialThread, CWinThread)
	//{{AFX_MSG_MAP(SerialThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SerialThread message handlers

int SerialThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

    char	buff[80];
	memset(buff , 0 , 80);  

	CString  m_Str;

	m_Str = ptrDlg->m_Barcode;
	m_Str = m_Str.Left(16);
	m_Str = m_Str.Right(13);

	m_Header = m_Str.Right(5);
	m_SerialNumber = m_Str.Left(7);

	m_File.Open("d:\\usinote\\barcode.usi" , CFile::modeRead);
	FileCtrl2.Open("d:\\usinote\\uut.log", CFile::modeReadWrite | CFile::shareDenyNone );

	while(m_File.ReadString(buff , 80))
	{
		m_Str = buff;
		m_Str = m_Str.Left(14);
		
		if(m_Header.Compare(m_Str.Right(5)) == 0 && m_SerialNumber.Compare(m_Str.Left(7)) == 0)
		{
			m_Str = buff;
			m_PINFO = m_Str.Right(m_Str.GetLength() - 17 );
			//AfxMessageBox(m_PINFO);
			break;
		}
	}

	m_File.Close();

	CEdit *m_Edt = (CEdit*)(ptrDlg->GetDlgItem(IDC_DSPMESS));

	COMSTAT     cs;
	DWORD       dwError;
	DWORD       nBytesRead;
    char		mess[1024];
	int			StateFlag = 0;
	int         IrSendFlag= 0;
	int			RxByteFlag = 0;
	unsigned char Data = 0;
	unsigned char   *PtrData = &Data;
	
	memset( mess , NULL , 1024);

	CString output = "";
	CString str;
	LONG	seeklen = -80;
	unsigned long		len;

	if(ptrDlg->handlePort_ == INVALID_HANDLE_VALUE) return FALSE;

	memset(buff , 0 , 80);  

	while(ptrDlg->ActiveSerial)
	{
    	ClearCommError(ptrDlg->handlePort_ , &dwError , &cs);

		if(cs.cbInQue > sizeof(mess))
		{
			PurgeComm(ptrDlg->handlePort_ , PURGE_RXCLEAR);
			return FALSE;
		}
		else if(cs.cbInQue > 0)
		{
			ReadFile(ptrDlg->handlePort_ , mess , cs.cbInQue , &nBytesRead , NULL);
			mess[cs.cbInQue]='\0';
			m_Str = mess ; 
			m_Edt->ReplaceSel( mess );
			memset( mess , NULL , nBytesRead );	

			FileCtrl2.Write(m_Str , m_Str.GetLength());

		}
		else if(cs.cbInQue  == 0)
		{
			if(FileCtrl2.GetLength() > 0)
			{
                seeklen = FileCtrl2.GetLength();

				if(seeklen >  80 )
					seeklen = 80;

				FileCtrl2.Seek(-(seeklen) , CFile::end);

				FileCtrl2.Read(buff , 80);  
				str = buff;

				if(str.Find("Barcode ?" , 0) >= 0 )
				{
					//ptrDlg->SetDlgItemText(IDC_PASSFAIL , "find");
					//ptrDlg->ActiveSerial = FALSE;

					str = ptrDlg->m_Barcode;
					str = str + "\r\n" ;
					WriteFile(ptrDlg->handlePort_, str , str.GetLength() , &len , NULL);
					Sleep(1000);
					str = "Y";
					str = str + "\r\n";
					WriteFile(ptrDlg->handlePort_, str , str.GetLength() , &len , NULL);
				}
				else if(str.Find("PINFO?" , 0) >= 0 )
				{
					str = m_PINFO + "\r\n";
					WriteFile(ptrDlg->handlePort_, str , str.GetLength() , &len , NULL);
				}
				else if(str.Find("GETMAC?" , 0) >= 0 )
				{
					str = ptrDlg->m_Maccode;
					str = str + "\r\n" ;
					WriteFile(ptrDlg->handlePort_, str , str.GetLength() , &len , NULL);
					Sleep(1000);
					str = "Y";
					str = str + "\r\n";
					WriteFile(ptrDlg->handlePort_, str , str.GetLength() , &len , NULL);
				}
			}
		}

		Sleep(5);

  	}

	FileCtrl2.Close();

	return TRUE;
	//return CWinThread::Run();
}

void SerialThread::setOwner(CDIODlg *m_ptrDialog)
{
    ptrDlg = m_ptrDialog;
}
