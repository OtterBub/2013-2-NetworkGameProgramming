#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 1024

#define FILEDATA	10
#define STRINGDATA	11
#define FILENAME	12
#define FILEEND		13

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

	//printf("%c %d _recv_ahead()\n", *p, *p);
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

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;

	int datatype = 0;

	FILE* f = NULL;
	char dir[100] = "Server/";

	int file_left;

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		printf("\n[TCP 서버 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d \n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		

		while(1){

			// datatype
			retval = recvn(client_sock, (char *)&datatype, sizeof(int), 0);
			if(retval == SOCKET_ERROR) {
				err_display("1 recv()");
				break;
			}
			else if(retval == 0)
				break;

			// identify datatype
			switch(datatype)
			{
			case STRINGDATA:
				printf("datatype :: STRINGDATA\n");
				break;
			case FILEDATA:
				printf("datatype :: FILEDATA\n");
				retval = recvn(client_sock, (char *)&file_left, sizeof(int), 0);
				if(retval == SOCKET_ERROR) {
					err_display("2 recv()");
					break;
				}
				break;
			case FILENAME:
				printf("datatype :: FILENAME\n");
				break;
			case FILEEND:
				printf("datatype :: FILEEND\n");
				break;
			default:
				printf("error : undefine dataType\n");
				getchar();
				return -1;
				break;
			}

			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR) {
				err_display("3 recv()");
				break;
			}
			else if(retval == 0)
				break;

			if((datatype == FILENAME) && (len == NULL)){
				printf("Client is file send failed! \n");
				continue;
			}
				
			retval = recvn(client_sock, buf, len, 0);
			if(retval == SOCKET_ERROR) {
				err_display("4 recv()");
				break;
			}

			else if(retval == 0)
				break;

			buf[retval] = '\0';

			switch(datatype)
			{
			case STRINGDATA:
				printf("[TCP/%s:%d][STRING] %s", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
				break;

			case FILEDATA:

				printf("[TCP/%s:%d][FILE] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

				printf("file left : %d \n", file_left);

				fwrite(buf, sizeof(char), len, f);
				
				fseek(f, len, SEEK_CUR);


				break;

			case FILEEND:
				fclose(f);
				break;

			case FILENAME:
				printf("[TCP/%s:%d][FILENAME] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

				strcpy(&dir[7], buf);
				f = fopen(dir, "wb");
				printf("copy path : %s \n", dir);
				if(f == NULL)
					printf("file open failed\n");
				break;

			default:
				break;
			}
			
		}

			closesocket(client_sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d \n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}