// Mifare.h: interface for the CMifare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIFARE_H__95BEEBD5_FF11_4F5B_9225_5D43A249D193__INCLUDED_)
#define AFX_MIFARE_H__95BEEBD5_FF11_4F5B_9225_5D43A249D193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMifare  
{
public:
	void WriteNFCTagImage();
	void MifareClose();
	CString m_TagImage;
	CString RdNFCTagImage();
	int HWInitialize();
	CMifare();
	virtual ~CMifare();

};

#endif // !defined(AFX_MIFARE_H__95BEEBD5_FF11_4F5B_9225_5D43A249D193__INCLUDED_)
