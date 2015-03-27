#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include "RPGWorld.h"
#include "PacketStruct.h"

DWORD WINAPI CreatePlayerSocket(LPVOID arg);
DWORD WINAPI PlayerSoketThread(LPVOID arg);
DWORD WINAPI WorldDataBroadCastThread(LPVOID arg);

static HANDLE hLogInOutEvent;

void err_quit(char *msg);
void err_display(char *msg);