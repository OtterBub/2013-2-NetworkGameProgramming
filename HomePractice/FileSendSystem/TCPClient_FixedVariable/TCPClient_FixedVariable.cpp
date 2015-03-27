#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 50
#define SERVERIP "127.0.0.1"

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0) {
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

int _recv_ahead(SOCKET s, char *p)
{
	__declspec(thread) static int nbytes = 0;
	__declspec(thread) static char buf[1024];
	__declspec(thread) static char *ptr;

	if(nbytes == 0 || nbytes == SOCKET_ERROR){
		nbytes = recv(s, buf, sizeof(buf), 0);
		if(nbytes == SOCKET_ERROR){
			return SOCKET_ERROR;
		}
		else if(nbytes == 0)
			return 0;
		ptr = buf;
	}

	--nbytes;
	*p = *ptr++;
	return 1;
}

int recvline(SOCKET s, char *buf, int maxlen)
{
	int n, nbytes;
	char c, *ptr = buf;

	for(n = 1; n < maxlen; n++){
		nbytes = _recv_ahead(s, &c);
		if(nbytes == 1){
			*ptr++ = c;
			if(c == '\n')
				break;
		}
		else if(nbytes == 0){
			*ptr = 0;
			return n-1;
		}
		else
			return SOCKET_ERROR;
	}

	*ptr = 0;
	return n;
}

int main(int argc, char** argv)
{
	int retval;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];
	char *testdata[] = {
		"Hello",
		"nice to meet to you",
		"you're very nice",
		"thanks",
		"have a nice day",
		"bye bye~!"
	};
	int len;

	for(int j = 0; j < 5; j++){
		for(int i = 0; i < 6; i++){
		
			len = strlen(testdata[i]);
			strncpy(buf, testdata[i], len);
			buf[len++] = '\n';

			retval = send(sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR){
				printf("1 ");
				err_display("send()");
			
				break;
			}

			retval = send(sock, buf, len, 0);
			if(retval == SOCKET_ERROR){
				printf("2 ");
				err_display("send()");
				break;
			}
			printf("[TCP 클라이언트] %d + %d 바이트를 보냈습니다. \n", sizeof(int), retval);
			Sleep(1000);
		}
	}


	closesocket(sock);

	WSACleanup();

	return 0;
}