// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "DebugDlg.h"
#include "DIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DebugDlg dialog


DebugDlg::DebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DebugDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DebugDlg)
	m_SerialSend = _T("");
	//}}AFX_DATA_INIT
	m_ptr = pParent;
}


void DebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DebugDlg)
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Text(pDX, IDC_SERIALSEND, m_SerialSend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DebugDlg, CDialog)
	//{{AFX_MSG_MAP(DebugDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnLoad)
	ON_BN_CLICKED(IDC_CHECK0, Boot0)
    ON_BN_CLICKED(IDC_CHECK1, Boot1)
	ON_BN_CLICKED(IDC_CHECK2, Boot2)
	ON_BN_CLICKED(IDC_CHECK3, Boot3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DebugDlg message handlers

BOOL DebugDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT);
	
	Boot = 0 ;

	m_item = 0 ;

    IniFileCtrl();

    InitListCtrl(m_ListCtrl);

    FillListCtrl(m_ListCtrl);

	return TRUE;
}

void DebugDlg::InitListCtrl(CXListCtrl &list)
{
	CRect rect;
	list.GetWindowRect(&rect);

	int w = rect.Width() - 2;

	int colwidths[2] = { 15, 20,  };	// sixty-fourths
    
	TCHAR * lpszHeaders[] = { _T("Enable"),
		           			  _T("Test Item"),
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
		if (i == 0)
			hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		else
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
		//lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		list.SetColumn(i, &lvcolumn);
	}
}

void DebugDlg::FillListCtrl(CXListCtrl &list)
{
	int nItem, nSubItem;

	// insert the items and subitems into the list
	for (nItem = 0 ; nItem < m_item ; nItem++)  
	{
		for (nSubItem = 0; nSubItem < 2; nSubItem++)
		{
			if (nSubItem == 0)
			{            
				str = m_Test[nItem];
				tmp = str.Left(1);

				if((tmp.Compare("#") == 0 ))
				{
						str = str.Left(12);
						str = str.Right(4); 
						list.InsertItem(nItem, str);
						list.SetCheckbox(nItem, nSubItem, 0);       // 1 : Enable , 0 : Disable
				}
				else
				{
						str = str.Left(11);
						str = str.Right(4); 
						list.InsertItem(nItem, str);
						list.SetCheckbox(nItem, nSubItem, 1);  
				}
			}
			if (nSubItem == 1)
			{
				str = m_Test[nItem];
                str = str.Right((str.GetLength() - 13));
				list.SetItemText(nItem, nSubItem, str);
			}
		}
	}
}

void DebugDlg::OnOK()
{
	UpdateData(TRUE);
	for(int k = 0 ; k < m_item ; k++)
	{
         if(!(m_ListCtrl.GetCheckbox(k , 0)))     // Disable
         {
			tmp = str = m_Test[k];
            str = str.Left(1);
			if(str.Compare("#") != 0)
			{
				tmp = "#" + tmp;
				str.Format("%X" , k);
				SetINIValue( tmp, str, "TEST");
			}
		 }
		 else if((m_ListCtrl.GetCheckbox(k , 0)))     // Enable
		 {
			tmp = str = m_Test[k];
            str = str.Left(1);
			if(str.Compare("#") == 0)
			{
				tmp = tmp.Right((tmp.GetLength()) -1);
				str.Format("%X" , k);
				SetINIValue( tmp, str, "TEST");
			}
		 }
	}

	CDialog::OnOK();
}

void DebugDlg::IniFileCtrl()
{
    m_Test.RemoveAll();

    m_IniReader.setINIFileName("D:\\USINOTE\\IM121.INI");

	m_IniReader.sectionExists("TEST");

	//for(int i=0 ; i <m_item ; i++)
	//{
    //   str.Format("%X" , i);
	//   m_Test.Add((m_IniReader.getKeyValue(str,"TEST")));
	//}
	
	CStringList* myOtherList = m_IniReader.getSectionData("TEST");

	POSITION position;
    int i = 0;

	for(position = myOtherList->GetHeadPosition(); position != NULL; ) 
	{
		myOtherList->GetNext(position);
        str.Format("%X" , i);
        m_Test.Add(m_IniReader.getKeyValue(str,"TEST"));
		m_item ++ ;
		i++;
	}

}
    /*
	m_Test.RemoveAll();
    m_Test.Add(_T("Boot 0 0000  RJ11 Loop back Test                   "));
    m_Test.Add(_T("Boot 0 0001  Wake Up Charge Test                   "));  // Boot0 Item0
    m_Test.Add(_T("Boot 0 0002  Quick Charge Test(System Off)         "));				
	m_Test.Add(_T("Boot 0 0003  Charge Stop Test                      "));
    m_Test.Add(_T("Boot 1 1001  Set MAC-Address                       "));  // Boot1
	m_Test.Add(_T("Boot 1 1002  LAN Copy Test                         "));
	m_Test.Add(_T("Boot 2 2001  DDR-RAM Timming Check                 "));  
	m_Test.Add(_T("Boot 2 2002  Memory Scan Test                      "));
	m_Test.Add(_T("Boot 2 2003  Power Status Check (AC)               "));
	m_Test.Add(_T("Boot 2 2004  Quick Charge Test(System On)          "));
	m_Test.Add(_T("Boot 2 2005  BIOS Updated                          "));
	m_Test.Add(_T("Boot 2 2006  EC Updated                            "));
	m_Test.Add(_T("Boot 3 3000  ALL LED and Audio Volume Test(Fn Key) "));
	m_Test.Add(_T("Boot 3 3001  BIOS Level Check                      "));  // Boot2
	m_Test.Add(_T("Boot 3 3002  EC Level Check                        "));
   	m_Test.Add(_T("Boot 3 3003  Suspend / Resume and Current Test     "));
	m_Test.Add(_T("Boot 3 3004  Keyboard Matrix Test                  "));
	m_Test.Add(_T("Boot 3 3005  CPU Fan Stop Test                     "));
	m_Test.Add(_T("Boot 3 3006  BMDC/CDC Modem ID Check               "));
    m_Test.Add(_T("Boot 3 3007  SSID/SSVID Configuration Check        ")); 
	m_Test.Add(_T("Boot 3 3008  Memory Size Check                     "));
	m_Test.Add(_T("Boot 3 3009  VGA Test and LCD/CRT Visual Check     "));
    m_Test.Add(_T("Boot 3 3010  USB Test(Speed and VBus Test)         "));
    m_Test.Add(_T("Boot 3 3011  Video RAM Size Check                  "));
	m_Test.Add(_T("Boot 3 3012  PCI Configuration Check for Mini-PCI  "));
	m_Test.Add(_T("Boot 3 3013  Line out to Mic in Test               "));
//  m_Test.Add(_T("Boot 3 3014  Speaker to Mic in Test                "));
	m_Test.Add(_T("Boot 3 3016  TouchPad & PS2 Mouse Test             "));
	m_Test.Add(_T("Boot 3 3017  LCD ID Test                           "));
	m_Test.Add(_T("Boot 3 3018  System Board test                     "));
	m_Test.Add(_T("Boot 3 3019  Power Status Check(BATT LED Test)     "));
    m_Test.Add(_T("Boot 3 3020  CPU Speed Step Test                   "));  //Item28
    m_Test.Add(_T("Boot 3 3021  Thermal ShutDown Test                 "));  //Item30
	m_Test.Add(_T("Boot 3 3022  Wake On Lan Test                      "));
	*/

/*	IBM ROME
	m_Test.RemoveAll();
    m_Test.Add(_T("BOOT0001 Burn in default mac address"));  //Boot0  //0
	m_Test.Add(_T("BOOT1001          Check code version"));  //Boot1  //1
	m_Test.Add(_T("BOOT1002           Memory speed test"));			  //2	
	m_Test.Add(_T("BOOT1003                 Memory test"));			  //3	
	m_Test.Add(_T("BOOT1004       EEPROM initialization"));           //4
	m_Test.Add(_T("BOOT1005   Mac eeprom initialization"));           //5
	m_Test.Add(_T("BOOT1006               Write barcode"));           //6
	m_Test.Add(_T("BOOT1007           Write mac address"));           //7
	m_Test.Add(_T("BOOT1008            EEPROM signature"));           //8
	m_Test.Add(_T("BOOT1009                 BIOS update"));           //9
	m_Test.Add(_T("BOOT1010                   H8 update"));           //10
    m_Test.Add(_T("BOOT2001               BIOS id check"));  //Boot2  //11 
	m_Test.Add(_T("BOOT2002            H8 version check"));           //12
	m_Test.Add(_T("BOOT2003                Check memory"));           //13
    m_Test.Add(_T("BOOT2004            Memory size test"));           //14
    m_Test.Add(_T("BOOT2005                CDC id check"));           //15
	m_Test.Add(_T("BOOT2006       Check ethernet device"));           //16
	m_Test.Add(_T("BOOT2007           Power state check"));           //17
	m_Test.Add(_T("BOOT2008               IR busy check"));           //18
	m_Test.Add(_T("BOOT2009       IR communication test"));           //19
	m_Test.Add(_T("BOOT2010                   POV3 test"));           //20
	m_Test.Add(_T("BOOT2011               LPT wrap test"));           //21
	m_Test.Add(_T("BOOT2012                Kbd id check"));           //22
	m_Test.Add(_T("BOOT2013           System board test"));           //23
	m_Test.Add(_T("BOOT2014                 LCD id test"));           //24
    m_Test.Add(_T("BOOT2015                Ubay id test"));           //25
	m_Test.Add(_T("BOOT2016               Crypt id test"));           //26
	m_Test.Add(_T("BOOT2017                    IPD test"));           //27
    m_Test.Add(_T("BOOT2018                Bay LED test"));           //28
    m_Test.Add(_T("BOOT2019             Key matrix test"));           //29
*/

void DebugDlg::OnLoad()
{
    ((CDIODlg*)m_ptr)->m_List.DeleteAllItems();

	CDialog::OnOK();
}

void DebugDlg::Boot0()
{
    Boot = 0 ; 
}

void DebugDlg::Boot1()
{
    Boot = 1 ; 
}

void DebugDlg::Boot2()
{
    Boot = 2 ; 
}

void DebugDlg::Boot3()
{
    Boot = 3 ; 
}

void DebugDlg::SetINIValue(CString m_keyvalue, CString m_key, CString m_section)
{
    m_IniReader.setINIFileName("D:\\USINOTE\\IM121.INI");

	m_IniReader.sectionExists(m_section);

	m_IniReader.setKey(m_keyvalue,m_key,m_section);

}


