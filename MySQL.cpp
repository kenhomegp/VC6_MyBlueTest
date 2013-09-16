// MySQL.cpp : implementation file
//

#include "stdafx.h"
#include "dio.h"
#include "MySQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySQL dialog


CMySQL::CMySQL(CWnd* pParent /*=NULL*/)
	: CDialog(CMySQL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySQL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void CMySQL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySQL)
	DDX_Control(pDX, IDC_LV_DB, m_lvDbTable);
	DDX_Control(pDX, IDC_TABLES, m_Tables);
	DDX_Control(pDX, IDC_DATABASES, m_Databases);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STRUCTURE_VIEW, m_bStructureView);
	DDX_Control(pDX, IDC_DATA_VIEW, m_bDataView);
}


BEGIN_MESSAGE_MAP(CMySQL, CDialog)
	//{{AFX_MSG_MAP(CMySQL)
	ON_CBN_SELCHANGE(IDC_DATABASES, OnSelchangeDatabases)
	ON_CBN_SELCHANGE(IDC_TABLES, OnSelchangeTables)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DATA_VIEW, OnDataView)
	ON_BN_CLICKED(IDC_STRUCTURE_VIEW, OnStructureView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySQL message handlers

BOOL CMySQL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_bDataView.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMySQL::OnSelchangeDatabases() 
{
	// TODO: Add your control notification handler code here
	if (!IsConnected)
		return;
	
	CString currDB;

	int nIndex = m_Databases.GetCurSel();
	m_Databases.GetLBText(nIndex, currDB);

	if (!ChangeDB(currDB))
		return;

	m_Tables.ResetContent();
	
	MYSQL_RES *res;
	res = mysql_list_tables(myData, NULL);
	MYSQL_ROW row;
	while ( ( row = mysql_fetch_row(res)))
	{
		m_Tables.AddString(row[0]);
	}
	mysql_free_result(res);	
}

void CMySQL::OnSelchangeTables() 
{
	// TODO: Add your control notification handler code here
	CString query;
	CString currTable;
	LVITEM lvItem;
	MYSQL_RES *res;
	MYSQL_FIELD *fd;
	UINT num_fields;
	MYSQL_ROW row;
	UINT i = 0;
	UINT j = 0;

	// Clear any previous display of data
	ClearListView();

	// Get the select text in the combo box
	int nIndex = m_Tables.GetCurSel();
	m_Tables.GetLBText(nIndex, currTable);

	// Check whether Structure View or Data View is checked
	// and format the query accordingly
	if (m_bDataView.GetCheck() == 1)
		query.Format("SELECT * FROM %s", currTable);
	else if (m_bStructureView.GetCheck() == 1)
		query.Format("DESCRIBE %s", currTable);

	// Run the query
	if (( mysql_query(myData, query) == 0))
	{
		res = mysql_store_result(myData);
		num_fields = mysql_num_fields(res);
		fd = mysql_fetch_fields(res);
		
		// Build the listview headers based on the number of columns returned
		if (!BuildListView(num_fields, fd))
			return;

		// Set some nice grid lines and effect to the listview
		m_lvDbTable.SetExtendedStyle(LVS_EX_FULLROWSELECT | 
									 LVS_EX_ONECLICKACTIVATE |
									 LVS_EX_GRIDLINES |
									 LVS_EX_INFOTIP);


		// Get all the rows in the result set
		while ( (row = mysql_fetch_row(res)))
		{				
			for (j = 0; j < num_fields; j++)
			{
				// Fill in the listview items and subitems
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.iSubItem = j;
				lvItem.pszText = row[j];
				m_lvDbTable.InsertItem(&lvItem);
				m_lvDbTable.SetItem(&lvItem);
			}
			i++; // Next Item
		}		
	}
	else  // Something went wrong
	{
		CString msg;
		msg.Format("Error %d in query %s\n%s", mysql_errno(myData), query, mysql_error(myData));
		MessageBox(msg, "Error in Query", MB_ICONERROR);
	}
	// Now free the resources
	mysql_free_result(res);	
}

void CMySQL::OnConnect() 
{
	// TODO: Add your control notification handler code here
	bool ConnectToDBServer = true;
	CString msg;

	//CString Host = "localhost";
	//CString Host = "192.168.1.5";
	//CString Host = "10.7.1.32";
	//CString Password = "kensin123";
	//CString User = "root";

	//(耕毅桌上電腦)MySQL Server
	//CString Host = "10.1.13.17";
	//CString User = "bhc302";
	//CString Password = "a754965a";

	//(桌上測試電腦)MySQL Server
	//CString Host = "10.1.13.10";
	//CString User = "test";
	//CString Password = "test123";

	UINT Port = 3306;

	IsConnected = false;
	
	if ((myData = mysql_init(NULL)) && mysql_real_connect( myData,Host,User,Password,NULL,Port,NULL,0))
	{
		ConnectToDBServer = true;
		//m_Edt->ReplaceSel("Connect to MySQL Server successfully!\r\n");
		AfxMessageBox("Connect to MySQL Server successfully!\r\n");
		IsConnected = true;
		
		//return TRUE;
	}
	else
	{
		ConnectToDBServer = false;
		msg.Format("Error : %d , %s\r\n",mysql_errno(myData),mysql_error(myData));
		//m_Edt->ReplaceSel(msg);
		//m_Edt->ReplaceSel("Fail to connect to database\r\n");
		AfxMessageBox(msg);
		//return FALSE;
	}	

	if(!ConnectToDBServer)
		return;

	if(!GetDatabases())
	{
		AfxMessageBox("Unable to get databases list from server");
		return;
	}
}

void CMySQL::OnDisconnect() 
{
	// TODO: Add your control notification handler code here
	if (IsConnected)
	{
		IsConnected = FALSE;
		mysql_close(myData);
	}
}

BOOL CMySQL::GetDatabases()
{
	MYSQL_RES *res;
	MYSQL_ROW row;

	if (!IsConnected)
		return FALSE;

	res = mysql_list_dbs(myData, NULL);

	while ( (row = mysql_fetch_row(res)))
	{
		m_Databases.AddString(row[0]);
	}
	mysql_free_result(res);
	return TRUE;
}

BOOL CMySQL::ChangeDB(CString &db_name)
{
	if (!IsConnected)
		return FALSE;

	int Err = mysql_select_db(myData, db_name);
	if (Err == 0)
		return TRUE;
	else
	{
		CString msg;
		msg.Format("Database %s selection failed with error %d\n"
			       "Error Message : %s", db_name, mysql_errno(myData), mysql_error(myData));
		MessageBox(msg, "Change database Error", MB_ICONERROR);		
		return FALSE;
	}
}

void CMySQL::ClearListView()
{
	m_lvDbTable.DeleteAllItems();
	int numCols = m_lvDbTable.GetHeaderCtrl()->GetItemCount();
	if (numCols)
	{
		for (int nIndex = 0; nIndex < numCols; nIndex++)
		{
			m_lvDbTable.DeleteColumn(0);
		}
	}
}

BOOL CMySQL::BuildListView(UINT num_Cols, MYSQL_FIELD *fd)
{
	UINT i;
	LVCOLUMN m_pCol;

	for (i = 0; i < num_Cols; i++)
	{
		m_pCol.pszText = fd[i].name;
		m_lvDbTable.InsertColumn(i, m_pCol.pszText, LVCFMT_LEFT, 100);
	}
	return TRUE;
}

void CMySQL::OnStructureView()
{
	int nIndex = m_Tables.GetCurSel();
	if (nIndex != CB_ERR)
		OnSelchangeTables();
}

void CMySQL::OnDataView()
{
	int nIndex = m_Tables.GetCurSel();
	if (nIndex != CB_ERR)
		OnSelchangeTables();
}
