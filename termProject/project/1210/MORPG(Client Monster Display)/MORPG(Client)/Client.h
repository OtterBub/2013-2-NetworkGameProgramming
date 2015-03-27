#ifndef CLIENT_H
#define CLIENT_H
#pragma comment( lib, "ws2_32" )

#include<iostream>
#include<WinSock2.h>
#include <WS2tcpip.h>
#include<stdio.h>
#include"RPGWorld.h"

#define SERVERPORT 9000
#define BUFSIZE 512

void err_quit( char *msg);
void err_display( char *msg);
int recvn(SOCKET s, char *buf, int len, int flags);


static HANDLE hReadEvent, hWriteEvent;
extern SOCKET sock;
extern int inputKey;
extern PlayerData playerData;
extern Player* myChar;

DWORD WINAPI ClientMain( LPVOID arg );

#endif //CLIENT_H