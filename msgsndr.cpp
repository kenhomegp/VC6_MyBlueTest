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


#include "stdafx.h"
#include "msgsndr.h"
#include "resource.h"

// create shared block of a fixed size
// for the most common data
//////////////////////////////////////
#pragma data_seg("Shared")
const UINT g_unSharedDataSize = 1024;					// bytes
#pragma data_seg()

#pragma comment(linker, "/SECTION:Shared,RWS")

__declspec(allocate("Shared")) volatile char g_SharedData[g_unSharedDataSize];


HHOOK		g_hook;
HINSTANCE	g_hDllInst;
bool		g_bFirstCall		= true;

char *		g_pchDataBuffer		= 0;					// pointer to the data to be returned
UINT		g_BuffSize			= 0;					// size of the above buffer
HANDLE		g_nMapFile			= 0;					// handle to a mapped file (if any)
void *		g_pMappedContent	= 0;
const char	g_ClientWndName[]	= "MessageSender";		// MessageSender main window
const char	g_HiddenWndName[]	= "DI inserted dialog";	// Hidden (injected) window name
const char	g_FileMappingName[]	= "MessageSenderFM";	// Hidden (injected) window name
const char	DIALOG_WND_CLASS[]	= "#32770";





BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hDllInst = (HINSTANCE)hModule;
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

HHOOK SetHook(HWND hWnd)
{
	DWORD	dwThread = GetWindowThreadProcessId(hWnd, 0);
	char * pName = "msgsndr.dll";
	g_hook = SetWindowsHookEx(	WH_GETMESSAGE, 
								&fnMsgSndrHookProc, 
								GetModuleHandle(pName),
								dwThread) ;

	if (!g_hook )	return 0;

	if (  ! PostThreadMessage(dwThread, WM_NULL, 0, 0) )	// make thread call fnMsgSndrHookProc()
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = 0;
	}
		
	return g_hook;
}

void RemoveHook(HHOOK hHook)
{
	HWND hwndHidden = FindWindow(DIALOG_WND_CLASS, g_HiddenWndName);
	if (hwndHidden)	
		SendMessage(hwndHidden, WM_CLOSE, 0, 0);	// do it before the library will be unloaded !
	UnhookWindowsHookEx(hHook);
	g_hook = 0;
}

void GetRetValSend(UINT * pRetVal)
{
	// WARNING !
	// Specific values are subject to change in the nearest future!
	*pRetVal	= ((UINT*)(g_SharedData))[0];
}

void GetRetSendTimeout(UINT * pRetVal, UINT * pExitCode, UINT * pErrorCode)
{
	// WARNING !
	// Specific values are subject to change in the nearest future!
	*pRetVal	= ((UINT*)(g_SharedData))[0];
	*pExitCode	= ((UINT*)(g_SharedData))[1];
	*pErrorCode	= ((UINT*)(g_SharedData))[2];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	Not exported functions

LRESULT CALLBACK fnMsgSndrHookProc(	int code,       // hook code
									WPARAM wParam,  // removal flag
									LPARAM lParam   // address of structure with message
									)
{
	if (g_bFirstCall)
	{
		HWND hDlg = CreateDialog(g_hDllInst, MAKEINTRESOURCE(IDD_DIALOG_INSERTED), NULL, InsertedDialogProc);
		HWND hwndSender = FindWindow(DIALOG_WND_CLASS, g_ClientWndName);
		PostMessage(hwndSender, (hwndSender != NULL) ? MSG_READY : MSG_FAILED, (WPARAM)hDlg, 0);
	}
	g_bFirstCall = false;
	return CallNextHookEx(g_hook, code, wParam, lParam);
}

BOOL CALLBACK InsertedDialogProc( HWND hwndDlg,  // handle to dialog box
								  UINT uMsg,     // message
								  WPARAM wParam, // first message parameter
								  LPARAM lParam  // second message parameter
								)
{
	switch (uMsg)
	{
		case WM_COPYDATA:
		{
			MSG		Msg;
			int		nExtraSize;
			char *	pExtraData = 0;


			//////////////////////////////////////////////////////////////////////////
			// UNPACKING THE MESSAGE DATA ...
			//
			// structure:
			// DWORD 0:	pSendingMsg->m_unID
			// DWORD 1:	Method
			// DWORD 2:	Timeout for SendMessageTimeout()
			// DWORD 3: Flags for SendMessageTimeout()
			// DWORD 4: pSendingMsg->m_bDataGet (size depends on a particular message,
			//			all preparations will be done in msgsndr.dll)
			// .......	MSG
			// .......	data buffer to send
			//
			// Total:	20 + sizeof(MSG) + buffer size
			//////////////////////////////////////////////////////////////////////////
			
			COPYDATASTRUCT *	pcdsData = (COPYDATASTRUCT *) lParam;
			if (pcdsData->cbData < sizeof(MSG) + 20)	break;
			UINT unID			= ((UINT*) (pcdsData->lpData))[0];
			UINT unMethod		= ((UINT*) (pcdsData->lpData))[1];
			UINT unSMTTimeout	= ((UINT*) (pcdsData->lpData))[2];
			UINT unSendingFlags	= ((UINT*) (pcdsData->lpData))[3];
			UINT bDataGet		= ((UINT*) (pcdsData->lpData))[4];

			memcpy(	&Msg, 
					(char*)(pcdsData->lpData) + 20, 
					sizeof(MSG));
			nExtraSize = pcdsData->cbData - sizeof(MSG) - 20;
			if (nExtraSize)	pExtraData = new char [nExtraSize];
			memcpy(	pExtraData, 
					(char*)(pcdsData->lpData) + sizeof(MSG) + 20, 
					nExtraSize);

			switch (Msg.message)
			{
				case WM_SETFONT:
				{
					if (nExtraSize < sizeof(LOGFONT) )	break;
					LOGFONT lf;
					memcpy(&lf, pExtraData, sizeof(LOGFONT));
					Msg.wParam = (WPARAM)CreateFontIndirect(&lf);
					break;
				}
				case WM_GETTEXT:
				{
					g_BuffSize		= Msg.wParam + 1;
					g_pchDataBuffer =new char [g_BuffSize];
					ZeroMemory(g_pchDataBuffer, g_BuffSize);
					Msg.lParam = (LPARAM)(g_pchDataBuffer);
					break;
				}
				case WM_SETTEXT:
				{
					Msg.lParam = (LPARAM)(pExtraData);
					break;			

				}
				// TODO: add other customizable messages here
				// break;
			}

			switch (unMethod)
			{
				case MT_SEND:
					((UINT*)(g_SharedData))[0] = SendMessage( Msg.hwnd,
															Msg.message, 
															Msg.wParam,
															Msg.lParam);
					break;
				case MT_POST:
					PostMessage(Msg.hwnd,
								Msg.message, 
								Msg.wParam,
								Msg.lParam);
					break;
				case MT_SENDTIMEOUT:
				{
					UINT unRes = SendMessageTimeout(Msg.hwnd,
													Msg.message, 
													Msg.wParam,
													Msg.lParam,
													unSendingFlags,
													unSMTTimeout,
													(ULONG*)(&(((UINT*)(g_SharedData))[0]))	);
					((UINT*)(g_SharedData))[1] = unRes;
					((UINT*)(g_SharedData))[2] = unRes ? 0 : GetLastError();

					break;
				}
			}

			if (bDataGet)
			{
				g_nMapFile = CreateFileMapping(	(HANDLE)0xFFFFFFFF, 
												NULL,
												PAGE_READWRITE,
												0,
												g_BuffSize + 4,
												g_FileMappingName	);
				g_pMappedContent = MapViewOfFile(g_nMapFile, 
												FILE_MAP_ALL_ACCESS,
												0,
												0,
												0);
				((UINT*)g_pMappedContent)[0] = g_BuffSize;
				memcpy((char*)g_pMappedContent + 4, g_pchDataBuffer, g_BuffSize);
				if (g_pchDataBuffer)	delete [] g_pchDataBuffer;
				g_pchDataBuffer = 0;

				SendMessage(FindWindow(DIALOG_WND_CLASS, g_ClientWndName), 
							MSG_READY, 
							g_BuffSize, 
							g_BuffSize	);
				g_BuffSize = 0;

			}

			if (pExtraData)	delete [] pExtraData;
			pExtraData = 0;

			break;
		}

		case WM_CLOSE:
		{
			if (g_nMapFile)
			{
				if (g_pMappedContent)	
				{
					UnmapViewOfFile(g_pMappedContent);
					g_pMappedContent = 0;
				}
				CloseHandle(g_nMapFile);
			}
			g_bFirstCall		= true;

			DestroyWindow(hwndDlg);
			break;
		}
	}
	return TRUE;
}
