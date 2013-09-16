// USBBulk.h: interface for the CUSBBulk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBBULK_H__8E8997F9_4CCF_49BA_B6DB_C069918E7EA2__INCLUDED_)
#define AFX_USBBULK_H__8E8997F9_4CCF_49BA_B6DB_C069918E7EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <setupapi.h>
//#include <initguid.h>

#include "usbdi.h"
#include "devioctl.h"

class CUSBBulk  
{
public:
	void DNWUSBDownload(CString file);
	void UsbReceive(BYTE* ptr , int len);
	void UsbTransmit();
	CString m_USBInfo;
	char* usbConfigAttributesString(UCHAR bmAttributes);
	char* usbEndPointTypeString(UCHAR bmAttributes);
	char* usbDescriptorTypeString(UCHAR bDescriptorType);
	void print_USB_ENDPOINT_DESCRIPTOR(PUSB_ENDPOINT_DESCRIPTOR ed, int i);
	void print_USB_INTERFACE_DESCRIPTOR(PUSB_INTERFACE_DESCRIPTOR id, UINT ix);
	void print_USB_CONFIGURATION_DESCRIPTOR(PUSB_CONFIGURATION_DESCRIPTOR cd);
	void rw_dev(HANDLE hDEV);
	CString dumpUsbConfig();
	bool GetUsbDeviceFileName( LPGUID  pGuid, char *outNameBuf);
	HANDLE open_file( char *filename);
	HANDLE open_dev();
	bool SearchFriendlyARMUSB();
	HANDLE OpenOneDevice (IN HDEVINFO HardwareDeviceInfo,IN PSP_INTERFACE_DEVICE_DATA DeviceInfoData,IN char *devName);
	HANDLE OpenUsbDevice(LPGUID pGuid, char *outNameBuf);
	void GetDeviceStrings();
	bool DeviceWrite(BYTE* buffer, DWORD dwSize, DWORD* lpdwBytesWritten);
	bool DeviceRead(BYTE* buffer , DWORD dwSize, DWORD* lpdwBytesRead);
	bool WriteFileData(BYTE* ptr , int len);
	bool ReadFileData(BYTE* ptr , int len);
	bool OpenUSBDevice();
	DWORD GetDeviceFromRegistry();
	CUSBBulk();
	virtual ~CUSBBulk();

};

#endif // !defined(AFX_USBBULK_H__8E8997F9_4CCF_49BA_B6DB_C069918E7EA2__INCLUDED_)
