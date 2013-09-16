////////////////////////////////////////////////////////////////////////////
// File:	msgsndr.cpp
// Version:	1.02
// Created:	Sep 06, 2003
//
// Author:	Dmytro Ivanchykhin
// E-mail:	D_Ivanchykhin@hotmail.com
//
// Defines the entry point for the dll injecting to the address space of
// a target process
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// Copyright (c) 2003 Dmytro Ivanchykhin
//
////////////////////////////////////////////////////////////////////////////
//
// HISTORY:
//
// Version 1.01		Aug 22, 2003	Initial release
//
// Version 1.02		Sep 06, 2003	(Current)
//					Added:	"Shared" section
//							void GetRetValSend(UINT*)
//							GetRetSendTimeout(UINT*, UINT*, UINT*)
//					Changed:InsertedDialogProc()
//					Result:	Message sending results are now returnable.
//
////////////////////////////////////////////////////////////////////////////
//
// ---   PLEASE LEAVE THIS HEADER INTACT   ---
//
////////////////////////////////////////////////////////////////////////////






#if !defined MSGSNDR_H
#define MSGSNDR_H
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MSGSNDR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MSGSNDR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef MSGSNDR_EXPORTS
#define MSGSNDR_API __declspec(dllexport)
#else
#define MSGSNDR_API __declspec(dllimport)
#endif

#define	MSG_SENDBACK	WM_APP + 1
#define	MSG_READY		WM_USER + 2
#define	MSG_FAILED		WM_USER + 3

enum MethodType
{
	MT_SEND,		// SendMessage()
	MT_POST,		// PostMessage()
	MT_SENDTIMEOUT,	// SendMessageTimeout()
};


extern "C" MSGSNDR_API HHOOK SetHook(HWND hWnd);
extern "C" MSGSNDR_API void RemoveHook(HHOOK hHook);
extern "C" MSGSNDR_API void GetRetValSend(UINT * pRetVal);
extern "C" MSGSNDR_API void GetRetSendTimeout(UINT * pRetVal,		// value returned through 7th parameter of SendMessageTimeout()
											  UINT * pExitCode,		// value returned by SendMessageTimeout() 
											  UINT * pErrorCode		// value returned by GetLastError() if pExitCode == 0 or 0
											  );

LRESULT CALLBACK fnMsgSndrHookProc(	int code,       // hook code
									WPARAM wParam,  // removal flag
									LPARAM lParam   // address of structure with message
									);
BOOL CALLBACK InsertedDialogProc( HWND hwndDlg,  // handle to dialog box
								  UINT uMsg,     // message
								  WPARAM wParam, // first message parameter
								  LPARAM lParam  // second message parameter
								);
 
#endif // MSGSNDR_H
