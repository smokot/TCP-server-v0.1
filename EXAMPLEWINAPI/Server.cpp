#include "windows.h"
#include "winsock.h"
#include "Buttons.h"
#include "Func.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE savehinst = NULL;




LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{

		
			LoadLibrary("Riched32.dll");

			hEditIp = CreateWindow("edit", "127.0.0.1",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0, 150, 20, hwnd, 0, savehinst, NULL);

			hEditPort = CreateWindow("edit", "25565",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 22, 150, 20, hwnd, 0, savehinst, NULL);

			hButtonStart = CreateWindow("BUTTON","Запустить сервер",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_ICON,
				200, 0, 150, 30, hwnd, reinterpret_cast<HMENU>(1), savehinst, NULL);


			hButtonStop = CreateWindow("BUTTON", "Остановить сервер",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
				400, 0, 150, 30, hwnd, reinterpret_cast<HMENU>(2), savehinst, NULL);
		

			hRichEditLogs = CreateWindow("richedit", "logs\n",
				WS_CHILD | WS_VISIBLE|ES_READONLY|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL,
				0, 50, 200, 400, hwnd, 0, savehinst, NULL);

			hRichEditChat = CreateWindow("richedit", "chat\n",
				WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
				250, 50, 300, 400, hwnd, 0, savehinst, NULL);


			hStaticOnline = CreateWindow("STATIC", "ONLINE:",
				WS_CHILD | WS_VISIBLE |  WS_BORDER,
				0, 450, 200, 20, hwnd, 0, savehinst, NULL);

			EnableWindow(hButtonStop, false);

			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(1);
			break;
		}
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case 1:
				{
					
					EnableWindow(hButtonStart, false);
					EnableWindow(hButtonStop, true);

					GetWindowText(hEditIp, ip_addr, 255);
					char buf[256] = "";
					GetWindowText(hEditPort, buf, 255);
					port = atoi(buf);
					ThreadIdListener = CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(Listener),NULL,NULL,NULL);
					CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(check_online), NULL, NULL, NULL);

					break;
				}
				case 2:
				{
					int res = MessageBox(NULL, "Вы уверены что хотите остановить сервер?", "Внимание!", MB_OKCANCEL);
					if (res == 1) {//При выключении сервера

						EnableWindow(hButtonStart, true);
						EnableWindow(hButtonStop, false);
						TerminateThread(ThreadIdListener, 0);
						
					
						
						
						
						for (int i = 0; i < connections.size(); i++)
						{
							
							closesocket(connections[i]);
							TerminateThread((HANDLE)ThreadIdHandler[i],0);
						}
						closesocket(sock);
						connections.clear();
						ThreadIdHandler.clear();
					}
					else if (res == 2) {


						
					}
					break;
				}
			}
			break;
		}
	}



	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}








int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = "server";
	RegisterClassEx(&wc);

	savehinst = hInstance;

	hwnd = CreateWindow(wc.lpszClassName, "server",
		WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX, 300, 300, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}



	return static_cast<int>(msg.wParam);
}