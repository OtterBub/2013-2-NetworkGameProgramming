#pragma comment(lib, "ws2_32")

//#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include "RPGWorld.h"
#include "PacketStruct.h"

DWORD WINAPI CreatePlayerSocket(LPVOID arg);
DWORD WINAPI PlayerSoketThread(LPVOID arg);
DWORD WINAPI WorldDataBroadCastThread(LPVOID arg);

void err_quit(char *msg);
void err_display(char *msg);