//#ifndef CLIENT_H
//#define CLIENT_H
#pragma comment( lib, "ws2_32" )

#include<iostream>
#include<WinSock2.h>
#include<stdio.h>
#include"CallBackFunc.h"

#define WORLD RPGWorld::Instance()
#define SERVERPORT 9000
#define BUFSIZE 512

void err_quit( char *msg);
void err_display( char *msg);
int recvn(SOCKET s, char *buf, int len, int flags);

DWORD WINAPI ClientMain( LPVOID arg );

//#endif //CLIENT_H