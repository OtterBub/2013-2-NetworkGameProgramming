#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT	9000
#define BUFSIZE		512
#define SERVERIP	"127.0.0.1"

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

int main(int argc, char** argv)
{
	int retval;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;
	char temp[BUFSIZE + 1];

	while(1){
		for(int i = 0; i < 10; i++){
			buf[i] = 'a' + (i % 20);
		}
		buf[10] = '\n';
		//strcpy(
		printf("\n[send data] %s", buf);
		//if(fgets(buf, BUFSIZE + 1, stdin) == NULL)
			//break;

		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\n';
		if(strlen(buf) == 0)
			break;

		retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR) {
			err_display("sendto()");
			continue;
		}

		printf("[UDP 클라이언트] %d바이트를 보냈습니다. \n", retval);

		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			continue;
		}

		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("[오류] 잘못된 데이터입니다!\n\a");
			continue;
		}

		if(strcmp(buf, temp) == 0){
		}

		buf[retval] = '\0';
		printf("[UDP 클라이언트] %d바이트를 받았습니다. \n", retval);
		printf("[받은 데이터] %s\n", buf);

				
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}