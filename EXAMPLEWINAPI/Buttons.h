#pragma once
#include "windows.h"
#include "vector"


TCHAR ip_addr[255] = "";
int port = 25565;

extern HWND hRichEditLogs = NULL;
extern HWND hRichEditChat = NULL;

extern HWND hStaticOnline = NULL;
extern HWND hButtonStart = NULL;
extern HWND hButtonStop = NULL;


extern HWND hEditIp = NULL;
extern HWND hEditPort = NULL;

extern HANDLE ThreadIdListener = 0;


