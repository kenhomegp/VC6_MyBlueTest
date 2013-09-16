#if !defined(AFX_SENDINGMESSAGE_H__0B3153DF_940C_4DC5_8066_13DBAC7E799D__INCLUDED_)
#define AFX_SENDINGMESSAGE_H__0B3153DF_940C_4DC5_8066_13DBAC7E799D__INCLUDED_

// Sorry, lates .h file is unavailable
#if !defined 
#define	SMTO_NOTIMEOUTIFNOTHUNG 0x0008
#endif

#ifdef EXPLICIT_LOADING
enum MethodType
{
	MT_SEND,		// SendMessage()
	MT_POST,		// PostMessage()
	MT_SENDTIMEOUT,	// SendMessageTimeout()
};
#else
#include "msgsndr.h"
#endif	// EXPLICIT_LOADING


struct SM
{
	UINT	m_unID;				// ID of a structure
	MSG		m_msg;				// message to be send; Practically used: hwnd, message, lParam and wParam.
								// Extra handling required if lParam and/or wParam are pointers; 
								// add "special case" whereever necessary instead.
								// Depending on a message, value of Param or wParam should be 
								// replaced by a proper value
	UINT	m_unBufferSizeIn;	// size of the allocated m_pchBuffIn
	UINT	m_unBufferSizeOut;	// size of the allocated m_pchBuffOut
	char *	m_pchBuffIn;		// buffer to store data to get
	char *	m_pchBuffOut;		// buffer to store data to send
	UINT	m_unMethod;			// sending method ( = function)
	UINT	m_unSendingFlags;	// flags for SendMessageTimeout(); valid if m_unMethod == MT_SENDTIMEOUT
	UINT	m_unSMTTimeout;		// value of timeout parameter of SendMessageTimeout(); valid if m_unMethod == MT_SENDTIMEOUT
	UINT	m_unSMTError;		// Error code for SendMessageTimeout()
								// 0 : timeout
								// -1: no error
	UINT	m_unInitialDelay;	// initial delay before sending the (first) message (ms)
	UINT	m_unPeriod;			// time interval between periodic messages (ms)
	UINT	m_unTimes;			// number of times the message to be sent
	UINT	m_unResult;			// value returned by the last call to SendMessage()
	bool	m_bLocalContext;	// true if MessageSender context is requested
	bool	m_bDataGet;			// true if buffer will be used to get data
								// false in any other case (including no buffer allocation)

};

// Next class is provided for easier handling	//
// of the above structure. NOTE that some		//
// operations can be unsafe	(see comment below)	//
//////////////////////////////////////////////////
class SendingMessage : public SM
{
public:
	SendingMessage()
	{
		m_unID				= CreateID();
		m_pchBuffIn			= 0;
		m_pchBuffOut		= 0;
		m_unBufferSizeIn	= 0;
		m_unBufferSizeOut	= 0;
		Initialize();
	}

	~SendingMessage()	
	{
		if (m_pchBuffIn)	delete [] m_pchBuffIn;
		if (m_pchBuffOut)	delete [] m_pchBuffOut;
	}

	void Initialize()
	{
		m_msg.lParam		= 0;
		m_msg.wParam		= 0;
		m_unInitialDelay	= 0;
		m_unTimes			= 1;
		m_unPeriod			= -1;
		m_unResult			= -1;
		m_bDataGet			= false;
		m_bLocalContext		= true;
		
		m_unMethod			= MT_POST;
	}

	// WARNING:	
	// following operator is unsafe with respect 
	// to possible pointers in wParam and lParam
	// of m_msg member
	////////////////////////////////////////////
	SendingMessage & operator = (SendingMessage & sm)
	{
		m_unInitialDelay	= sm.m_unInitialDelay;
		m_unPeriod			= sm.m_unPeriod;
		m_unTimes			= sm.m_unTimes;
		m_unResult			= sm.m_unResult;
		m_bLocalContext		= sm.m_bLocalContext;
		m_bDataGet			= sm.m_bDataGet;
		
		m_unMethod			= sm.m_unMethod;
		m_unSendingFlags	= sm.m_unSendingFlags;
		m_unSMTTimeout		= sm.m_unSMTTimeout;
		m_unSMTError		= sm.m_unSMTError;
		memcpy(&m_msg, &(sm.m_msg), sizeof(MSG) );
		FillBufferIn(sm.m_unBufferSizeIn, sm.m_pchBuffIn);
		FillBufferOut(sm.m_unBufferSizeOut, sm.m_pchBuffOut);
		return *this;
	}

	operator SM* ()			// WARNING: this conversion is unsafe; m_pchBuff should be ignored !
	{
		return (SM*)this;
	}

	void AllocateBufferIn(UINT nSize)	
	{
		if (m_pchBuffIn && m_unBufferSizeIn != nSize)
			delete [] m_pchBuffIn;
		m_pchBuffIn		= new char [nSize];
		m_unBufferSizeIn	= nSize;
	}

	void AllocateBufferOut(UINT nSize)	
	{
		if (m_pchBuffOut && m_unBufferSizeOut != nSize)
			delete [] m_pchBuffOut;
		m_pchBuffOut		= new char [nSize];
		m_unBufferSizeOut	= nSize;
	}

	void FillBufferIn(UINT nSize, void * pContent)
	{
		AllocateBufferIn(nSize);
		memcpy(m_pchBuffIn, pContent, nSize);
	}

	void FillBufferOut(UINT nSize, void * pContent)
	{
		AllocateBufferOut(nSize);
		memcpy(m_pchBuffOut, pContent, nSize);
	}

private:
	static UINT m_unIDcount;	// base to generate ID's
	UINT	CreateID()	{return m_unIDcount ++;}
};


#endif // AFX_SENDINGMESSAGE_H__0B3153DF_940C_4DC5_8066_13DBAC7E799D__INCLUDED_