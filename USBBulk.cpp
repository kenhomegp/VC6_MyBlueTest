// USBBulk.cpp: implementation of the CUSBBulk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dio.h"
#include "USBBulk.h"

#include <stdio.h>
#include <stdlib.h>
//#include <setupapi.h>
#include <initguid.h>
#include <string>
#include <process.h>

//#include "usbdi.h"
//#include "devioctl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HANDLE hWrite = INVALID_HANDLE_VALUE;
HANDLE hRead = INVALID_HANDLE_VALUE;

char inPipe[32] = "PIPE00";     // pipe name for bulk input pipe on our test board
char outPipe[32] = "PIPE01";    // pipe name for bulk output pipe on our test board
char completeDeviceName[256] = "";
volatile int isConnected=FALSE;

volatile char *txBuf;
volatile DWORD iTxBuf;
static DWORD txBufSize;
void UsbTxFile(void *args);

GUID m_GUID;
HANDLE m_hUSBDevice;
HANDLE m_hUSBWrite;
HANDLE m_hUSBRead;
CString m_friendlyname	= "";
CString m_linkname		= "";
CString m_serialnumber	= "";

//FriendlyARM USB Device
DEFINE_GUID(GUID_CLASS_I82930_BULK, 
0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8);

//SiliconLab USB Device
//DEFINE_GUID(GUID_INTERFACE_SILABS_BULK, 
//0x37538c66, 0x9584, 0x42d3, 0x96, 0x32, 0xeb, 0xad, 0xa, 0x23, 0xd, 0x13);

DEFINE_GUID(GUID_INTERFACE_SILABS_BULK, 
0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8);

#define SILABS_BULK_WRITEPIPE	"PIPE01"
#define SILABS_BULK_READPIPE	"PIPE00"

#define BULKUSB_IOCTL_INDEX  0x0000

#define IOCTL_BULKUSB_GET_CONFIG_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
						   BULKUSB_IOCTL_INDEX,\
						   METHOD_BUFFERED,  \
						   FILE_ANY_ACCESS)
						   
#define IOCTL_BULKUSB_RESET_DEVICE   CTL_CODE(FILE_DEVICE_UNKNOWN,  \
						   BULKUSB_IOCTL_INDEX+1,\
						   METHOD_BUFFERED,  \
						   FILE_ANY_ACCESS)                                                              
						   
#define IOCTL_BULKUSB_RESET_PIPE  CTL_CODE(FILE_DEVICE_UNKNOWN,  \
						   BULKUSB_IOCTL_INDEX+2,\
						   METHOD_BUFFERED,  \
						   FILE_ANY_ACCESS)               

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSBBulk::CUSBBulk()
{

}

CUSBBulk::~CUSBBulk()
{

}

DWORD CUSBBulk::GetDeviceFromRegistry()
{
	DWORD numDevices = 0;

	m_GUID = GUID_INTERFACE_SILABS_BULK;

	// Retrieve device list for GUID that has been specified.
	HDEVINFO hDevInfoList = SetupDiGetClassDevs (&m_GUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	//HDEVINFO hDevInfoList = SetupDiGetClassDevs (&m_GUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_INTERFACEDEVICE));

	if (hDevInfoList != NULL)
	{
		SP_DEVICE_INTERFACE_DATA deviceInfoData;

		for (int index = 0; index < 127;index++)
		{
			// Clear data structure
			ZeroMemory(&deviceInfoData, sizeof(deviceInfoData));
			deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

			// retrieves a context structure for a device interface of a device information set.
			if (SetupDiEnumDeviceInterfaces (hDevInfoList, 0, &m_GUID, index, &deviceInfoData)) 
			{
				numDevices++;
			}
			else
			{
				if ( GetLastError() == ERROR_NO_MORE_ITEMS ) 
					break;
			}
		}
	}

	// SetupDiDestroyDeviceInfoList() destroys a device information set
	// and frees all associated memory.
	SetupDiDestroyDeviceInfoList (hDevInfoList);

	return numDevices;
}

bool CUSBBulk::OpenUSBDevice()
{
	m_hUSBDevice = INVALID_HANDLE_VALUE;

	m_hUSBDevice = CreateFile(m_linkname,GENERIC_WRITE | GENERIC_READ,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

	if(m_hUSBDevice != INVALID_HANDLE_VALUE)
	{
		m_linkname += "\\";
		m_linkname += SILABS_BULK_WRITEPIPE;

		m_hUSBWrite = CreateFile(m_linkname,GENERIC_WRITE | GENERIC_READ,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if (m_hUSBWrite == INVALID_HANDLE_VALUE)
		{
			//CString sMessage;
			//sMessage.Format("Error opening Write device: %s\n\nApplication is aborting.\nReset hardware and try again.",SILABS_BULK_WRITEPIPE);
			//AfxMessageBox(sMessage,MB_OK|MB_ICONEXCLAMATION);

			//AfxMessageBox("Can not open USB write pipe");
			return false;
		}

		m_linkname += "\\";
		m_linkname += SILABS_BULK_READPIPE;

		m_hUSBRead = CreateFile(m_linkname,GENERIC_WRITE | GENERIC_READ,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if (m_hUSBRead == INVALID_HANDLE_VALUE)
		{
			//AfxMessageBox("Can not open USB read pipe");

			return false;
		}	

		return true;
	}
	else
		return false;
}

bool CUSBBulk::ReadFileData(BYTE* ptr , int len)
{
	DWORD size			= 0;
	DWORD counterPkts	= 0;
	DWORD numPkts		= 0;
	//BYTE  buf[64];
	DWORD dwBytesRead	= 0;
	DWORD dwReadLength	= 0;
	DWORD dwBytesWritten = 0;
	CString m_Str;
	
	//memset(buf, 0, len);

	//if (DeviceRead(buf, len, &dwBytesRead))
	if (DeviceRead(ptr, len, &dwBytesRead))
	{
		//m_Edt->ReplaceSel(m_Str);

		if(ptr[0] == 0xff && dwBytesRead == 1)
		{
			//memset(buf, 0, len);

			//if(DeviceRead(buf, len, &dwBytesRead))
			if(DeviceRead(ptr, len, &dwBytesRead))
			{
				//m_Str.Format("USB Rx %X %X %X\r\n",buf[0],buf[1],buf[2]);
				//m_Edt->ReplaceSel(m_Str);
				//AfxMessageBox(m_Str);

				if(dwBytesRead == len)
					return true;
			}	
			else
				return false;
		}
		else if(dwBytesRead == len)
		{
			//m_Str.Format("USB Rx %X %X %X\r\n",buf[0],buf[1],buf[2]);
			//m_Edt->ReplaceSel(m_Str);
			//AfxMessageBox(m_Str);
			return true;
		}
	}
	else
	{
		//m_Edt->ReplaceSel("USB read info fail\r\n");
		//AfxMessageBox(m_Str);
		return false;
	}
}

bool CUSBBulk::WriteFileData(BYTE* ptr , int len)
{
	DWORD		dwBytesWritten	= 0;
	DWORD		dwBytesRead		= 0;
	DWORD		dwWriteLength	= 0;
	BYTE		buf[64];
	CString		m_Str;
	int			i = 0;

	buf[0] = 0x01;
	buf[1] = 0x05;
	buf[2] = 0x00;

	if(DeviceWrite(buf, 3, &dwBytesWritten))
	{
		//m_Edt->ReplaceSel("USB_Write info pass\r\n");

		for(i = 0 ; i < len ; i++)
			buf[i] = *(ptr+i);

		dwWriteLength = len;

		if(DeviceWrite(ptr, dwWriteLength, &dwBytesWritten))
		{
			//m_Edt->ReplaceSel("USB_Write data pass\r\n");
			if(dwBytesWritten == len)
			{
				//m_Str.Format("USB Tx,%X %X %X %X %X\r\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
				//m_Edt->ReplaceSel(m_Str);
				//AfxMessageBox(m_Str);
				return true;
			}
		}
		else
			return false;
	}
	else
	{
		//m_Edt->ReplaceSel("USB_Write info fail\r\n");
		//m_Str = "USB_Write info fail\r\n";
		//AfxMessageBox(m_Str);
		return false;
	}
}

bool CUSBBulk::DeviceRead(BYTE *buffer, DWORD dwSize, DWORD *lpdwBytesRead)
{
	int timeout = 65536;

	while(!ReadFile(m_hUSBRead, buffer, dwSize, lpdwBytesRead, NULL))
	{	// Device IO failed.
		//status = F32x_READ_ERROR;
		return 0;
	} 
	return 1;
}

bool CUSBBulk::DeviceWrite(BYTE *buffer, DWORD dwSize, DWORD *lpdwBytesWritten)
{
	if(WriteFile(m_hUSBWrite, buffer, dwSize, lpdwBytesWritten, NULL))
	{
		//status = F32x_SUCCESS;	// Write succeeded after > 1 attempts.
		return 1;
	}
	else
		return 0;
}

void CUSBBulk::GetDeviceStrings()
{
	CString m_tmp;

	// Retrieve device list for GUID that has been specified.
	HDEVINFO hDevInfoList = SetupDiGetClassDevs (&m_GUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)); 

	if (hDevInfoList != NULL)
	{
		SP_DEVICE_INTERFACE_DATA deviceInfoData;

		// Clear data structure
		ZeroMemory(&deviceInfoData, sizeof(deviceInfoData));
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		// retrieves a context structure for a device interface of a device information set.
		//if (SetupDiEnumDeviceInterfaces (hDevInfoList, 0, &m_GUID, dwDeviceNum, &deviceInfoData)) 
		if (SetupDiEnumDeviceInterfaces (hDevInfoList, 0, &m_GUID, 0, &deviceInfoData)) 
		{
			// Must get the detailed information in two steps
			// First get the length of the detailed information and allocate the buffer
			// retrieves detailed information about a specified device interface.
			PSP_DEVICE_INTERFACE_DETAIL_DATA     functionClassDeviceData = NULL;
			ULONG  predictedLength, requiredLength;

			predictedLength = requiredLength = 0;
			SetupDiGetDeviceInterfaceDetail (	hDevInfoList,
												&deviceInfoData,
												NULL,			// Not yet allocated
												0,				// Set output buffer length to zero 
												&requiredLength,// Find out memory requirement
												NULL);			

			predictedLength = requiredLength;
			functionClassDeviceData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc (predictedLength);
			functionClassDeviceData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			
			SP_DEVINFO_DATA did = {sizeof(SP_DEVINFO_DATA)};
	
			// Second, get the detailed information
			if ( SetupDiGetDeviceInterfaceDetail (	hDevInfoList,
													&deviceInfoData,
													functionClassDeviceData,
													predictedLength,
													&requiredLength,
													&did)) 
			{
				TCHAR fname[256];

				// Try by friendly name first.
				if (!SetupDiGetDeviceRegistryProperty(hDevInfoList, &did, SPDRP_FRIENDLYNAME, NULL, (PBYTE) fname, sizeof(fname), NULL))
				{	// Try by device description if friendly name fails.
					if (!SetupDiGetDeviceRegistryProperty(hDevInfoList, &did, SPDRP_DEVICEDESC, NULL, (PBYTE) fname, sizeof(fname), NULL))
					{	// Use the raw path information for linkname and friendlyname
						strncpy(fname, functionClassDeviceData->DevicePath, 256);
					}
				}
					
				m_friendlyname	= fname;
				m_linkname		= functionClassDeviceData->DevicePath;

				//m_tmp = m_linkname;
				std::string temp = m_linkname;
				temp = temp.substr(temp.find_first_of('#') + 1, temp.find_last_of('#') - temp.find_first_of('#'));				
				temp = temp.substr(temp.find_first_of('#') + 1, (temp.find_last_of('#') - temp.find_first_of('#')) - 1);

				m_serialnumber = temp.c_str();

				free( functionClassDeviceData );
			}
		}
	}
	SetupDiDestroyDeviceInfoList (hDevInfoList);
}

HANDLE CUSBBulk::OpenUsbDevice(LPGUID pGuid, char *outNameBuf)
{
	//Routine Description:

	//   Do the required PnP things in order to find
	//   the next available proper device in the system at this time.

	//Arguments:

	//	pGuid:      ptr to GUID registered by the driver itself
	//	outNameBuf: the generated name for this device

	//Return Value:

	//	return HANDLE if the open and initialization was successful,
	//	else INVLAID_HANDLE_VALUE.
	
   ULONG					NumberDevices;
   HANDLE					hOut = INVALID_HANDLE_VALUE;
   HDEVINFO                 hardwareDeviceInfo;
   SP_INTERFACE_DEVICE_DATA deviceInfoData;
   ULONG                    i;
   BOOLEAN                  done;
   PUSB_DEVICE_DESCRIPTOR   usbDeviceInst;
   PUSB_DEVICE_DESCRIPTOR   *UsbDevices = &usbDeviceInst;

   *UsbDevices = NULL;
   NumberDevices = 0;

   //
   // Open a handle to the plug and play dev node.
   // SetupDiGetClassDevs() returns a device information set that contains info on all
   // installed devices of a specified class.
   //
   hardwareDeviceInfo = SetupDiGetClassDevs (pGuid,
			   NULL, // Define no enumerator (global)
			   NULL, // Define no
			   (DIGCF_PRESENT | // Only Devices present
			    DIGCF_INTERFACEDEVICE)); // Function class devices.

   //
   // Take a wild guess at the number of devices we have;
   // Be prepared to realloc and retry if there are more than we guessed
   //
   NumberDevices = 4;
   done = FALSE;
   deviceInfoData.cbSize = sizeof (SP_INTERFACE_DEVICE_DATA);

   i=0;
   while (!done) 
   {
      NumberDevices *= 2;

      if (*UsbDevices) 
	  {
		*UsbDevices = (PUSB_DEVICE_DESCRIPTOR)realloc (*UsbDevices, (NumberDevices * sizeof (USB_DEVICE_DESCRIPTOR)));
      } 
	  else 
	  {
		*UsbDevices = (PUSB_DEVICE_DESCRIPTOR)calloc (NumberDevices, sizeof (USB_DEVICE_DESCRIPTOR));
      }

      if (NULL == *UsbDevices) 
	  {
		// SetupDiDestroyDeviceInfoList destroys a device information set
		// and frees all associated memory.

		SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
		return INVALID_HANDLE_VALUE;
      }

      usbDeviceInst = *UsbDevices + i;

      for (; i < NumberDevices; i++) 
	  {
		// SetupDiEnumDeviceInterfaces() returns information about device interfaces
		// exposed by one or more devices. Each call returns information about one interface;
		// the routine can be called repeatedly to get information about several interfaces
		// exposed by one or more devices.

		if (SetupDiEnumDeviceInterfaces (hardwareDeviceInfo,0, pGuid,i,&deviceInfoData))// We don't care about specific PDOs
		{
			hOut = OpenOneDevice (hardwareDeviceInfo, &deviceInfoData, outNameBuf);
			if(hOut != INVALID_HANDLE_VALUE) 
			{
				done = TRUE;
				break;
			}
		} 
		else 
		{
			if(ERROR_NO_MORE_ITEMS == GetLastError()) 
			{
				done = TRUE;
				break;
			}
		}
      }
   }

   NumberDevices = i;

   // SetupDiDestroyDeviceInfoList() destroys a device information set
   // and frees all associated memory.

   SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
   free ( *UsbDevices );
   return hOut;	
}

HANDLE CUSBBulk::OpenOneDevice(HDEVINFO HardwareDeviceInfo, PSP_INTERFACE_DEVICE_DATA DeviceInfoData, char *devName)
{
	//Routine Description:

	//	Given the HardwareDeviceInfo, representing a handle to the plug and
	//	play information, and deviceInfoData, representing a specific usb device,
	//	open that device and fill in all the relevant information in the given
	//	USB_DEVICE_DESCRIPTOR structure.

	//Arguments:

	//	HardwareDeviceInfo:  handle to info obtained from Pnp mgr via SetupDiGetClassDevs()
	//	DeviceInfoData:      ptr to info obtained via SetupDiEnumInterfaceDevice()

	//Return Value:

	//	return HANDLE if the open and initialization was successfull,
	//	else INVLAID_HANDLE_VALUE.
	
    PSP_INTERFACE_DEVICE_DETAIL_DATA     functionClassDeviceData = NULL;
    ULONG                                predictedLength = 0;
    ULONG                                requiredLength = 0;
    HANDLE                               hOut = INVALID_HANDLE_VALUE;

    //
    // allocate a function class device data structure to receive the
    // goods about this particular device.
    //
    SetupDiGetInterfaceDeviceDetail (
	    HardwareDeviceInfo,
	    DeviceInfoData,
	    NULL, // probing so no output buffer yet
	    0, // probing so output buffer length of zero
	    &requiredLength,
	    NULL); // not interested in the specific dev-node


    predictedLength = requiredLength;
    // sizeof (SP_FNCLASS_DEVICE_DATA) + 512;

    functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc (predictedLength);
    functionClassDeviceData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);

    //
    // Retrieve the information from Plug and Play.
    //
    if (! SetupDiGetInterfaceDeviceDetail (
	       HardwareDeviceInfo,
	       DeviceInfoData,
	       functionClassDeviceData,
	       predictedLength,
	       &requiredLength,
	       NULL)) {
	free( functionClassDeviceData );
	return INVALID_HANDLE_VALUE;
    }

    strcpy( devName,functionClassDeviceData->DevicePath) ;
    //EB_Printf( "Attempting to open %s\n", devName );

    hOut = CreateFile (
		  functionClassDeviceData->DevicePath,
		  GENERIC_READ | GENERIC_WRITE,
		  FILE_SHARE_READ | FILE_SHARE_WRITE,
		  NULL, // no SECURITY_ATTRIBUTES structure
		  OPEN_EXISTING, // No special create flags
		  0, // No special attributes
		  NULL); // No template file

    if (INVALID_HANDLE_VALUE == hOut) 
	{
		//EB_Printf( "FAILED to open %s\n", devName );
		CString str = devName;
		AfxMessageBox("Fail to open device!\r\n");
    }
	
    free( functionClassDeviceData );
    return hOut;
}

bool CUSBBulk::SearchFriendlyARMUSB()
{
	HANDLE hDEV;

	hDEV = OpenUsbDevice( (LPGUID)&GUID_CLASS_I82930_BULK, completeDeviceName);
	if (hDEV == INVALID_HANDLE_VALUE) 	
	{				
		return false;
	} 
	else 
	{				
		CloseHandle(hDEV);

		return true;
	}	       
}

HANDLE CUSBBulk::open_dev()
{
    HANDLE hDEV = OpenUsbDevice( (LPGUID)&GUID_CLASS_I82930_BULK, completeDeviceName);

    if (hDEV == INVALID_HANDLE_VALUE) 
	{
		//EB_Printf("Failed to open (%s) = %d", completeDeviceName, GetLastError());
    } else 
	{
		//EB_Printf("DeviceName = (%s)\n", completeDeviceName);
    }       

    return hDEV;
}

HANDLE CUSBBulk::open_file(char *filename)
{
    int success = 1;
    HANDLE h;

    if ( !GetUsbDeviceFileName((LPGUID) &GUID_CLASS_I82930_BULK,completeDeviceName))
    {
		//NOISY(("Failed to GetUsbDeviceFileName:%d\n", GetLastError()));
		return  INVALID_HANDLE_VALUE;
    }

    strcat (completeDeviceName,"\\");          

    strcat (completeDeviceName,filename);                  

    //EB_Printf("completeDeviceName = (%s)\n", completeDeviceName);

    h = CreateFile(completeDeviceName,
	GENERIC_WRITE | GENERIC_READ,
	FILE_SHARE_WRITE | FILE_SHARE_READ,
	NULL,
	OPEN_EXISTING,
	0,
	NULL);

    if (h == INVALID_HANDLE_VALUE) 
	{
		//NOISY(("Failed to open (%s) = %d", completeDeviceName, GetLastError()));
		success = 0;
    } else 
	{
	    //NOISY(("Opened successfully.\n"));
    }       

    return h;
}

bool CUSBBulk::GetUsbDeviceFileName(LPGUID pGuid, char *outNameBuf)
{
    HANDLE hDev = OpenUsbDevice( pGuid, outNameBuf );
    if ( hDev != INVALID_HANDLE_VALUE )
    {
		CloseHandle( hDev );
		return TRUE;
    }
		
	return FALSE;
}

CString CUSBBulk::dumpUsbConfig()
{
    HANDLE hDEV = open_dev();

    if ( hDEV )
    {
		rw_dev( hDEV );
		CloseHandle(hDEV);
		return m_USBInfo;
    }
}

void CUSBBulk::rw_dev(HANDLE hDEV)
{
    UINT success;
    //int siz, nBytes;
	int siz;
	unsigned long nBytes;
    char buf[256];
    PUSB_CONFIGURATION_DESCRIPTOR cd;
    PUSB_INTERFACE_DESCRIPTOR id;
    PUSB_ENDPOINT_DESCRIPTOR ed;

    siz = sizeof(buf);

    if (hDEV == INVALID_HANDLE_VALUE) {
	//NOISY(("DEV not open"));
	return;
    }

	m_USBInfo = "";
    
    success = DeviceIoControl(hDEV,IOCTL_BULKUSB_GET_CONFIG_DESCRIPTOR,buf,siz,buf,siz,&nBytes,NULL);

    //NOISY(("request complete, success = %d nBytes = %d\n", success, nBytes));
    
    if (success) 
	{
		ULONG i;
		UINT  j, n;
		char *pch;

		pch = buf;
		n = 0;

		cd = (PUSB_CONFIGURATION_DESCRIPTOR) pch;

		print_USB_CONFIGURATION_DESCRIPTOR( cd );

		pch += cd->bLength;

		do 
		{
			id = (PUSB_INTERFACE_DESCRIPTOR) pch;

			print_USB_INTERFACE_DESCRIPTOR(id, n++);

			pch += id->bLength;
			for (j=0; j<id->bNumEndpoints; j++) 
			{
				ed = (PUSB_ENDPOINT_DESCRIPTOR) pch;

				print_USB_ENDPOINT_DESCRIPTOR(ed,j);

				pch += ed->bLength;
			}
			i = (ULONG)(pch - buf);
		} while (i<cd->wTotalLength);
    }
}

void CUSBBulk::print_USB_CONFIGURATION_DESCRIPTOR(PUSB_CONFIGURATION_DESCRIPTOR cd)
{
	CString m_Str;

	m_Str = "===== USB DEVICE STATUS =====\r\nUSB_CONFIGURATION_DESCRIPTOR\r\n";
	m_USBInfo += m_Str;

	m_Str.Format("bLength = 0x%x, decimal %d\r\n", cd->bLength, cd->bLength);
	m_USBInfo += m_Str;

	m_Str.Format("bDescriptorType = 0x%x ( %s )\n", cd->bDescriptorType, usbDescriptorTypeString( cd->bDescriptorType ));
	m_USBInfo += m_Str;

	m_Str.Format("wTotalLength = 0x%x, decimal %d\r\n", cd->wTotalLength, cd->wTotalLength);
	m_USBInfo += m_Str;

	m_Str.Format("bNumInterfaces = 0x%x, decimal %d\r\n", cd->bNumInterfaces, cd->bNumInterfaces);
	m_USBInfo += m_Str;

	m_Str.Format("bConfigurationValue = 0x%x, decimal %d\r\n", cd->bConfigurationValue, cd->bConfigurationValue);
	m_USBInfo += m_Str;

	m_Str.Format("iConfiguration = 0x%x, decimal %d\r\n", cd->iConfiguration, cd->iConfiguration);
	m_USBInfo += m_Str;

	m_Str.Format("bmAttributes = 0x%x ( %s )\r\n", cd->bmAttributes, usbConfigAttributesString( cd->bmAttributes ));
	m_USBInfo += m_Str;

	m_Str.Format("MaxPower = 0x%x, decimal %d\r\n", cd->MaxPower, cd->MaxPower);
	m_USBInfo += m_Str;
}

void CUSBBulk::print_USB_INTERFACE_DESCRIPTOR(PUSB_INTERFACE_DESCRIPTOR id, UINT ix)
{
	CString m_Str;

	m_Str.Format("USB_INTERFACE_DESCRIPTOR #%d\r\n", ix);
	m_USBInfo += m_Str;

	m_Str.Format("bLength = 0x%x\r\n", id->bLength);
	m_USBInfo += m_Str;

	m_Str.Format("bDescriptorType = 0x%x ( %s )\r\n", id->bDescriptorType, usbDescriptorTypeString( id->bDescriptorType ));
	m_USBInfo += m_Str;

	m_Str.Format("bInterfaceNumber = 0x%x\r\n", id->bInterfaceNumber);
    m_USBInfo += m_Str;

	m_Str.Format("bAlternateSetting = 0x%x\r\n", id->bAlternateSetting);
	m_USBInfo += m_Str;

	m_Str.Format("bNumEndpoints = 0x%x\r\n", id->bNumEndpoints);
	m_USBInfo += m_Str;

	m_Str.Format("bInterfaceClass = 0x%x\r\n", id->bInterfaceClass);
	m_USBInfo += m_Str;

	m_Str.Format("bInterfaceSubClass = 0x%x\r\n", id->bInterfaceSubClass);
	m_USBInfo += m_Str;

	m_Str.Format("bInterfaceProtocol = 0x%x\r\n", id->bInterfaceProtocol);
	m_USBInfo += m_Str;

	m_Str.Format("bInterface = 0x%x\r\n", id->iInterface);
	m_USBInfo += m_Str;

	m_Str.Format("-----------------------------\r\n");
	m_USBInfo += m_Str;
}

void CUSBBulk::print_USB_ENDPOINT_DESCRIPTOR(PUSB_ENDPOINT_DESCRIPTOR ed, int i)
{
	CString m_Str;

	m_Str.Format("USB_ENDPOINT_DESCRIPTOR for Pipe%02d\r\n", i);
	m_USBInfo += m_Str;

	m_Str.Format("bLength = 0x%x\r\n", ed->bLength);
	m_USBInfo += m_Str;

	m_Str.Format("bDescriptorType = 0x%x ( %s )\r\n", ed->bDescriptorType, usbDescriptorTypeString( ed->bDescriptorType ));
	m_USBInfo += m_Str;

    if ( USB_ENDPOINT_DIRECTION_IN( ed->bEndpointAddress)) 
	{
		m_Str.Format("bEndpointAddress= 0x%x ( INPUT )\r\n", ed->bEndpointAddress);
		m_USBInfo += m_Str;
    } 
	else 
	{
		m_Str.Format("bEndpointAddress= 0x%x ( OUTPUT )\n", ed->bEndpointAddress);
		m_USBInfo += m_Str;
    }

	m_Str.Format("bmAttributes= 0x%x ( %s )\r\n", ed->bmAttributes, usbEndPointTypeString ( ed->bmAttributes ));
	m_USBInfo += m_Str;

	m_Str.Format("wMaxPacketSize= 0x%x, decimal %d\r\n", ed->wMaxPacketSize, ed->wMaxPacketSize);
	m_USBInfo += m_Str;

	m_Str.Format("bInterval = 0x%x, decimal %d\r\n", ed->bInterval, ed->bInterval);
	m_USBInfo += m_Str;

	m_Str.Format("-----------------------------\r\n");
	m_USBInfo += m_Str;
}

char* CUSBBulk::usbDescriptorTypeString(UCHAR bDescriptorType)
{
    switch(bDescriptorType) 
	{
		case USB_DEVICE_DESCRIPTOR_TYPE:
			return "USB_DEVICE_DESCRIPTOR_TYPE";

		case USB_CONFIGURATION_DESCRIPTOR_TYPE:
			return "USB_CONFIGURATION_DESCRIPTOR_TYPE";
	
		case USB_STRING_DESCRIPTOR_TYPE:
			return "USB_STRING_DESCRIPTOR_TYPE";
	
		case USB_INTERFACE_DESCRIPTOR_TYPE:
			return "USB_INTERFACE_DESCRIPTOR_TYPE";
	
		case USB_ENDPOINT_DESCRIPTOR_TYPE:
			return "USB_ENDPOINT_DESCRIPTOR_TYPE";
	
#ifdef USB_POWER_DESCRIPTOR_TYPE // this is the older definintion which is actually obsolete
    // workaround for temporary bug in 98ddk, older USB100.h file
		case USB_POWER_DESCRIPTOR_TYPE:
			return "USB_POWER_DESCRIPTOR_TYPE";
#endif
	
#ifdef USB_RESERVED_DESCRIPTOR_TYPE  // this is the current version of USB100.h as in NT5DDK

		case USB_RESERVED_DESCRIPTOR_TYPE:
			return "USB_RESERVED_DESCRIPTOR_TYPE";

		case USB_CONFIG_POWER_DESCRIPTOR_TYPE:
			return "USB_CONFIG_POWER_DESCRIPTOR_TYPE";

		case USB_INTERFACE_POWER_DESCRIPTOR_TYPE:
			return "USB_INTERFACE_POWER_DESCRIPTOR_TYPE";
#endif // for current nt5ddk version of USB100.h

    default:
		return "??? UNKNOWN!!"; 
    }

	return "";
}

char* CUSBBulk::usbEndPointTypeString(UCHAR bmAttributes)
{
    UINT typ = bmAttributes & USB_ENDPOINT_TYPE_MASK;

    switch(typ) 
	{
		case USB_ENDPOINT_TYPE_INTERRUPT:
			return "USB_ENDPOINT_TYPE_INTERRUPT";

		case USB_ENDPOINT_TYPE_BULK:
			return "USB_ENDPOINT_TYPE_BULK";    

		case USB_ENDPOINT_TYPE_ISOCHRONOUS:
			return "USB_ENDPOINT_TYPE_ISOCHRONOUS"; 
	
		case USB_ENDPOINT_TYPE_CONTROL:
			return "USB_ENDPOINT_TYPE_CONTROL"; 
	
		default:
			return "??? UNKNOWN!!"; 
    }

	return "";
}

char* CUSBBulk::usbConfigAttributesString(UCHAR bmAttributes)
{
    UINT typ = bmAttributes & USB_CONFIG_POWERED_MASK;

    switch(typ) 
	{
		case USB_CONFIG_BUS_POWERED:
			return "USB_CONFIG_BUS_POWERED";

		case USB_CONFIG_SELF_POWERED:
			return "USB_CONFIG_SELF_POWERED";
	
		case USB_CONFIG_REMOTE_WAKEUP:
			return "USB_CONFIG_REMOTE_WAKEUP";

		default:
			return "??? UNKNOWN!!"; 
    }

	return "";
}

void CUSBBulk::UsbReceive(BYTE *ptr, int len)
{
    ULONG i;    
    UINT success;
    //UINT rxLength=128;
	UINT rxLength=64;
    ULONG nBytesRead;
    //char *rxBuf;
	void *rxBuf;

    //rxBuf = (char *)malloc(rxLength);
    //if(rxBuf==NULL)return; 

	rxBuf = (void *)ptr;

	hRead = open_file(inPipe);

    success = ReadFile(hRead,
     	               rxBuf,
                       rxLength,
	               &nBytesRead,
	               NULL);
    if(success)
    {

    }
    else
    {
		//EB_Printf("Error: can't receive data from the USBD\n");
    }

	//if (rxBuf)free(rxBuf);
	
    // close devices if needed
    if(hRead != INVALID_HANDLE_VALUE)
  		CloseHandle(hRead);
}

void CUSBBulk::UsbTransmit()
{
	int i;
	int BuffSize = 64;
	//int BuffSize = 4;
	void *txBlk;
	ULONG txBlkSize;
	DWORD nBytesWrite;

	/*
	if(!isConnected)
    {
		AfxMessageBox("[ERROR:USB not Connected]\n");
		return;
    }
	*/

    hWrite = open_file( outPipe);
    
    if(hWrite==INVALID_HANDLE_VALUE)
    {
		//MessageBox(hwnd,TEXT("Can't open USB device.\n"),TEXT("Error"),MB_OK | MB_ICONINFORMATION );
		AfxMessageBox("Can't open USB device");
		return;
    }

	txBuf=(char *)malloc(BuffSize); 

	if(txBuf == 0)
	{
		AfxMessageBox("Memory allocate fail!");
		return;
	}

	//Prepare USB TxBuff
	for(i = 0 ; i < BuffSize; i++)
		txBuf[i] = i+0x31;

	txBlk=(void *)(txBuf);

	txBlkSize = BuffSize;

	WriteFile(hWrite,txBlk,txBlkSize,&nBytesWrite,NULL);

	free((void *)txBuf);

    CloseHandle(hWrite);    	
}

void CUSBBulk::DNWUSBDownload(CString file)
{
	//USB Tx format : addr(4)+size(4)+data(n)+cs(2)
	ULONG i;
	USHORT cs=0;
	HANDLE hFile;
	ULONG fileSize;
	unsigned long threadResult;

	hWrite = open_file( outPipe);
    
    if(hWrite==INVALID_HANDLE_VALUE)
    {
		//MessageBox(hwnd,TEXT("Can't open USB device.\n"),TEXT("Error"),MB_OK | MB_ICONINFORMATION );
		AfxMessageBox("Can't open USB device");
		return;
    }

    hFile=CreateFile(file,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
    
	if(hFile==INVALID_HANDLE_VALUE)
    {
		return;
    }

	fileSize=GetFileSize(hFile,NULL);

	txBuf=(char *)malloc(fileSize+10); 

    if(txBuf==0)
    {
		return;
    }

    ReadFile(hFile,(void *)(txBuf+8),fileSize,&txBufSize,NULL);
    if(txBufSize!=fileSize)    
    {
		return;
    }

	//*((DWORD *)txBuf+0)=downloadAddress;
	*((DWORD *)txBuf+0)=0x31000000;
    *((DWORD *)txBuf+1)=fileSize+10;   //attach filesize(n+6+4) 
    for(i=8;i<(fileSize+8);i++)
	cs+=(BYTE)(txBuf[i]);
    *((WORD *)(txBuf+8+fileSize))=cs;   //attach checksum 

	txBufSize+=10;
	iTxBuf=0;

    CloseHandle(hFile);
    
    threadResult=_beginthread( (void (*)(void *))UsbTxFile,0x2000,(void *)0);

	if(threadResult!=-1)
    {
		//Create the download progress dialogbox.
		//CreateDialog(_hInstance,MAKEINTRESOURCE(IDD_DIALOG1),hwnd,DownloadProgressProc); //modaless
		
		//ShowWindow(_hDlgDownloadProgress,SW_SHOW); 
    	//isn't needed because the dialog box already has WS_VISIBLE attribute.
    }
    else
    {
		//EB_Printf("[ERROR:Can't creat a thread. Memory is not sufficient]\n");
    }
}

#define TX_SIZE 2048
void UsbTxFile(void *args)
{
    void *txBlk;
    ULONG txBlkSize;
    DWORD nBytesWrite;
    DWORD txBufSize100;
    DWORD temp;
    DWORD count;

    txBufSize100=txBufSize/100;
    if(txBufSize100==0)txBufSize100=1;
    while(1)
    {
        if((txBufSize-iTxBuf) > TX_SIZE)
			txBlkSize=TX_SIZE;
		else
			txBlkSize=txBufSize-iTxBuf;
	
		txBlk=(void *)(txBuf+iTxBuf);
 
		WriteFile(hWrite,txBlk,txBlkSize,&nBytesWrite,NULL);

		//assert(nBytesWrite == WriteLen);

		iTxBuf+=TX_SIZE;
		
		//if(downloadCanceled==1)break;	//download is canceled by user.

		if(iTxBuf>=txBufSize)break;
    }

    free((void *)txBuf);

    CloseHandle(hWrite);    

    _endthread();
}
