#pragma once
#include "windows.h"
#include "winsock.h"
#include "string"
#include "vector"
#include "Buttons.h"
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;
vector<SOCKET>connections;
vector<HANDLE>ThreadIdHandler;

SOCKET sock = NULL;


void append_text(HWND hwnd, char * buf)
{
	SendMessage(hwnd, EM_SETSEL, -1, -1);
	SendMessage(hwnd, EM_REPLACESEL, FALSE, LPARAM("\n"));
	SendMessage(hwnd, EM_REPLACESEL, FALSE, LPARAM(buf));
}

void Handler(SOCKET sock)
{
	while (1)
	{
		
		
		
		char buf[2048] = "";
		int res = recv(sock, buf, sizeof(buf), 0);
		if (res>0){
			for (int i = 0; i < connections.size(); i++)
			{

				send(connections[i], buf, sizeof(buf), 0);
				
			}
			if (buf[0] == '\0' && buf[1] == '\0')continue;
			append_text(hRichEditChat, buf);
		}
		if (res <= 0)
		{// Уменьшить онлайн
			for (int i = 0; i < connections.size(); i++)
			{
				if (connections[i] == sock)
				{
					char buffer[] = "client disconnected";
					append_text(hRichEditChat, buffer);
					connections.erase(connections.begin()+i);
					break;
				}
			}
			
			
			  closesocket(sock);
			ExitThread(0);
		}
		
		
	}
}

void check_online()
{
	while (1) {
		Sleep(2000);
		char buf_counter[2048] = ""; char out_counter[2048] = "ONLINE: ";

		itoa(connections.size(), buf_counter, 10);
		strcat(out_counter, buf_counter);

		SetWindowText(hStaticOnline, out_counter);
	}
}


void Listener()
{

	WSAData WSD;
	WSAStartup(MAKEWORD(2, 1), &WSD);

	sockaddr_in addr;
	
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr);
	addr.sin_family = (AF_INET);
	addr.sin_port = htons(port);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	bind(sock, (sockaddr*)&addr, sizeof(addr));

	listen(sock, SOMAXCONN);

	int size = sizeof(addr);
	sockaddr_in client_addr;
	while (1)
	{
		SOCKET socket_;
		socket_ = accept(sock, (sockaddr*)&addr, &size);

		connections.push_back(socket_);
		char buffer[] = "client connected";
		append_text(hRichEditLogs, buffer);
		
		HANDLE ThreadHandlerr = CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(Handler), LPVOID(socket_), NULL, NULL);
		ThreadIdHandler.push_back(ThreadHandlerr);

		//HOSTENT *hst;
		//hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		//hst->h_name;
		//inet_ntoa(client_addr.sin_addr);
		
	}
}