#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT	9000
#define BUFSIZE		512

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_KOREAN, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);	
	exit(1);
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_KOREAN, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);	
}

int main(int argc, char **argv)
{
	int retval;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("sock()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr		= htonl(INADDR_ANY);
	serveraddr.sin_port		= htons(SERVERPORT);
	
	retval = bind(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	
	while(1){
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&clientaddr, &addrlen);

		if(retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			continue;
		}

		buf[retval] = '\0';

		printf("[UDP / %s : %d], %s", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		retval = sendto(sock, buf, retval, 0, (SOCKADDR *)&clientaddr, sizeof(clientaddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}
