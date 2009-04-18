/*
 * Zune Now Playing - A Zune Plugin to store the metadata of current track
 * being played in the registry
 * Copyright (C) 2009 - Sanrag Sood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


// ZuneNowPlaying.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ZuneNowPlaying.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL AppIsAlreadyRunning()
{
    BOOL bRunning=FALSE;
    CString strAppName;
    strAppName = "ZuneNowPlaying.exe";
    DWORD dwOwnPID = GetProcessId(GetCurrentProcess());
    HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    PROCESSENTRY32* processInfo=new PROCESSENTRY32;
    processInfo->dwSize=sizeof(PROCESSENTRY32);
    int index=0;
    while(Process32Next(hSnapShot,processInfo)!=FALSE)
    {
		CString pName = (LPCWSTR)processInfo->szExeFile;
		if (pName.CompareNoCase(strAppName) == 0)
        {
            if (processInfo->th32ProcessID != dwOwnPID)
            {
                bRunning=TRUE;
                break;
            }
        }
    }
    CloseHandle(hSnapShot);
    delete processInfo;
    return bRunning;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	if (AppIsAlreadyRunning())
		return -1;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ZUNENOWPLAYING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZUNENOWPLAYING));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZUNENOWPLAYING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ZUNENOWPLAYING);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable


   // The class for the windows should be the same as Windows Live Messenger
   // so that it receives the message sent by Zune Software
   hWnd = CreateWindow(L"MsnMsgrUIManager", L"", WS_DISABLED,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // Our process would run in Background
   //ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	std::string ZMesg="";
	CString title="", artist="", album="";
	int slashes = 0,found = -1,ctr,start;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_COPYDATA:
		COPYDATASTRUCT *cds;
		cds = (COPYDATASTRUCT *)lParam;
		char *x;
		x = (char *)cds->lpData;
		int step;
		step = sizeof(char);
		if (cds->cbData > 2 && x[1]==0)
			step*=2;
		 //Don't know why I have to multiply step by 2
		 //The messages I am getting are in the form of characters seperated by 0
		 //Therefore, removing null characters first
		for (int i=0;i<cds->cbData;i+=step)
		{
			ZMesg += x[i];
			if (x[i] == '\\')
				slashes++;
		}
		if (ZMesg.substr(0,4) == "ZUNE")
		{
			
			if (slashes == 4 )
			{

				HKEY hkey;
				DWORD dwDisp = 0;
				LPDWORD lpdwDisp = &dwDisp;
				CString ZNPKey = "SOFTWARE\\ZuneNowPlaying";
				CString Playing = "Playing";
				CString Album = "Album";
				CString Title = "Title";
				CString Artist = "Artist";
				bool bVal = false;
				LONG iSuccess = RegCreateKeyEx(HKEY_CURRENT_USER, ZNPKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, lpdwDisp);
				if (iSuccess == ERROR_SUCCESS)
				{
					DWORD dwVal = 0;
					RegSetValueEx(hkey,Playing,0L,REG_DWORD,(CONST BYTE*)&dwVal, sizeof(DWORD));
					RegSetValueEx(hkey,Artist,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)artist,(artist.GetLength()+1)*sizeof(TCHAR));
					RegSetValueEx(hkey,Title,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)title,(title.GetLength()+1)*sizeof(TCHAR));
					RegSetValueEx(hkey,Album,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)album,(album.GetLength()+1)*sizeof(TCHAR));
					RegFlushKey(hkey);
					RegCloseKey(hkey);
				}
			}
			else
			{
				// Skipping first three '\'
				ctr = 0;
				do 
				{
					found = ZMesg.find("\\0",found+1);
					ctr++;
				} while (found > 0 && ctr < 4);

				start = found+2;
				found = ZMesg.find("\\0",found+1);
				title += ZMesg.substr(start,found-start).c_str();

				start = found +2;
				found = ZMesg.find("\\0",found+1);
				artist += ZMesg.substr(start,found-start).c_str();

				start = found + 2;
				found = ZMesg.find("\\0",found+1);
				album += ZMesg.substr(start,found-start).c_str();

				HKEY hkey;
				DWORD dwDisp = 0;
				LPDWORD lpdwDisp = &dwDisp;
				CString ZNPKey = "SOFTWARE\\ZuneNowPlaying";
				CString Playing = "Playing";
				CString Artist = "Artist";
				CString Title = "Title";
				CString Album = "Album";
				
				bool bVal = false;
				LONG iSuccess = RegCreateKeyEx(HKEY_CURRENT_USER, ZNPKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, lpdwDisp);
				if (iSuccess == ERROR_SUCCESS)
				{
					DWORD dwVal = 1;
					RegSetValueEx(hkey,Artist,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)artist,(artist.GetLength()+1)*sizeof(TCHAR));
					RegSetValueEx(hkey,Title,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)title,(title.GetLength()+1)*sizeof(TCHAR));
					RegSetValueEx(hkey,Album,0L,REG_SZ,(CONST BYTE *)(LPCTSTR)album,(album.GetLength()+1)*sizeof(TCHAR));
					RegSetValueEx(hkey,Playing,0L,REG_DWORD,(CONST BYTE*)&dwVal, sizeof(DWORD));
					RegFlushKey(hkey);
					RegCloseKey(hkey);
				}

			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
