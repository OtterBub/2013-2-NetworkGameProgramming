#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <errno.h>
#include <io.h>

CRITICAL_SECTION cs;

#define SERVERPORT 9000
#define BUFSIZE 5000

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

bool FileCheck(char* path){
	_finddatai64_t c_file;
	intptr_t hFile;

	if( (hFile = _findfirsti64(path, &c_file)) == -1) {
		switch (errno) {
		case ENOENT:
			printf(" 파일 없음 \n"); break;
		case EINVAL:
			fprintf(stderr, "Invalid path name.\n"); break;
		case ENOMEM:
			fprintf(stderr, "Not Enough memory or file name too long.\n"); break;
		default:
			fprintf(stderr, "Unknown error.\n"); break;
		}
		return false;
	}
	else {
		printf(" 파일 중복 \n");
		_findclose(hFile);
		return true;
	}
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	char buf[BUFSIZE + 1];
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;
	int len;

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("\n[TCP 서버 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d \n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	int datatype = 0;

	FILE* f = NULL;
	char dir[100] = "Server/";

	int file_length;
	int recv_file_length;

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

			printf("[TCP/%s:%d][STRING] %s", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
			break;

		case FILEDATA:
			printf("datatype :: FILEDATA\n");

			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR) {
				err_display("3 recv()");
				break;
			}
			else if(retval == 0)
				break;

			if((len == NULL)){
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

			printf("[TCP/%s:%d][FILENAME] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			strcpy(&dir[7], buf);

			EnterCriticalSection(&cs);
			if(FileCheck(dir)) {
				int overlapCount = 1;
				char tempName[50];
				while(FileCheck(dir)){
					sprintf(tempName, "(%d) ", overlapCount++);
					strcpy(&tempName[strlen(tempName)], buf);
					strcpy(&dir[7], tempName);
				}
			}
			LeaveCriticalSection(&cs);

			f = fopen(dir, "wb");
			printf("copy path : %s \n", dir);
			_sleep(3000);
			if(f == NULL){
				printf("file open failed\n");
				break;
			}

			retval = recvn(client_sock, (char *)&file_length, sizeof(int), 0);
			if(retval == SOCKET_ERROR) {
				err_display("2 recv()");
				break;
			}
			printf("file_length : %d \n", file_length);
			recv_file_length = 0;
			printf("file recv....\n");
			{
				time_t currentTime = clock();
				int file_complete_percent = 0;
				int currentPercent = 0;

				while(recv_file_length != file_length){
					retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
					if(retval == SOCKET_ERROR) {
						err_display("recv()");
						break;
					}
					else if(retval == 0)
						break;

					retval = recvn(client_sock, buf, len, 0);
					if(retval == SOCKET_ERROR) {
						err_display("recv()");
						break;
					}
					else if(retval == 0)
						break;

					fwrite(buf, sizeof(char), len, f);

					recv_file_length += len;

					if(recv_file_length > file_length){
						printf("file is crupted\n");
						break;
					}

					file_complete_percent = ((float)recv_file_length / file_length) * 100;
					if(currentPercent != file_complete_percent) {
						currentPercent  = file_complete_percent;
						printf("File Recv %d%% Complete\n", currentPercent);
					}
				}
				fclose(f);
				printf("file write complete\n");
				printf("total time : %fSec\n", (float)(clock() - currentTime)/1000);
			}

			break;

		default:
			printf("error : undefine dataType\n");
			//getchar();
			break;
		}
	}
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char** argv){
	int retval;

	InitializeCriticalSection(&cs);

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	DWORD ThreadId;

	while(1){
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if(hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	closesocket(listen_sock);

	DeleteCriticalSection(&cs);

	WSACleanup();
	return 0;
}