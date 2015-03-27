#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <direct.h>

/*
send 함수의 리턴 시간도 재어보자
*/

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

#define BUFSIZE 50
#define FILEBUFSIZE 8192
#define FILENAMESIZE 50

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

	char dir[100] = "Client/";
	char fileName[FILENAMESIZE];
	char buff[FILEBUFSIZE];



	char buf[BUFSIZE];
	/*char *testdata[] = {
	"Hello",
	"nice to meet to you",
	"you're very nice",
	"thanks",
	"have a nice day",
	"bye bye~!"
	};*/
	char chatBuf[100];
	int len;
	int datatype;

	int unsent, free, ilen, tot;
	ilen = sizeof(int);

	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&tot, &ilen);

	printf("size :: %d\n", tot);

	for(int i = 0; ; i++){

		printf("입력하세요\n");
		gets(chatBuf);

		datatype = STRINGDATA;

		printf("datatype::\n");

		if(strcmp(chatBuf, "/file") == 0){
			printf("file send ready \n");
			datatype = FILEDATA;
		} else if(strcmp(chatBuf, "/exit") == 0) {
			printf("Client를 종료합니다.\n");
			break;
		}

		retval = send(sock, (char *)&datatype, sizeof(int), 0);
		if(retval == SOCKET_ERROR){
			printf("datatype ");
			err_display("send()");
			break;
		}

		switch(datatype){
		case STRINGDATA:			

			len = strlen(chatBuf);
			strncpy(buf, chatBuf, len);
			buf[len++] = '\n';

			retval = send(sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR){
				printf("strlen ");
				err_display("send()");
				break;
			}

			retval = send(sock, buf, len, 0);
			if(retval == SOCKET_ERROR){
				printf("data ");
				err_display("send()");
				break;
			}
			printf("[TCP 클라이언트] %d + %d 바이트를 보냈습니다. \n", sizeof(int), retval);
			break;
		case FILEDATA:
			{
				// send file name
				datatype = FILEDATA;

				printf("MAX File size %d \n", FILEBUFSIZE);
				printf("File name : ");

				gets(&dir[7]);
				printf("%s\n", dir);

				//dir[strlen(dir)] = '\0';

				FILE *f;
				f = fopen(dir, "rb");

				if(f == NULL){
					printf("don't found file\n");
					len = NULL;
					retval = send(sock, (char *)&len, sizeof(int), 0);
					if(retval == SOCKET_ERROR) {
						printf("file failed ");
						err_display("send()");
					}
					break;
				}

				fseek(f, 00, SEEK_END);
				int file_len = ftell(f);
				fseek(f, 00, SEEK_SET);

				int file_read_len = 0;
				fpos_t filepos;
				fgetpos(f, &filepos);

				strcpy(fileName, (strrchr(dir, '/'))+1);

				printf("file name : %s\n", fileName);
				printf("file size : %d\n", file_len);

				len = strlen(fileName);

				retval = send(sock, (char *)&len, sizeof(int), 0);
				if(retval == SOCKET_ERROR){
					printf("filelen ");
					err_display("send()");
				}

				retval = send(sock, fileName, len, 0);
				if(retval == SOCKET_ERROR){
					printf("fileName ");
					err_display("send()");
				}

				// send file data

				retval = send(sock, (char *)&file_len, sizeof(int), 0);
				if(retval == SOCKET_ERROR){
					printf("file_len ");
					err_display("send()");
				}

				time_t currentTime = clock();
				unsigned int time_max = 0, time_min = UINT_MAX;
				unsigned int pre_time_min = time_min;
				printf("file send....\n");

				while(file_len > 0){

					file_read_len = fread(buff, sizeof(char), FILEBUFSIZE ,f);

					if(file_len == 0)
						break;

					//fseek(f, file_read_len, SEEK_CUR);

					fgetpos(f, &filepos);

					//printf("file read len : %d\n", file_read_len);
					//printf("file len : %d\n", file_len);
					//printf("file pos  : %d\n", filepos);

					//Sleep(1000);
					//getchar();
					//fflush(stdin);					

					retval = send(sock, (char *)&file_read_len, sizeof(int), 0);
					if(retval == SOCKET_ERROR){
						printf("filelen ");
						err_display("send()");
					}

					if(file_len == FILEBUFSIZE)
						pre_time_min = time_min;

					time_t startTime = clock();
					time_t endTime = 0;
					retval = send(sock, buff, file_read_len, 0);
					if(retval == SOCKET_ERROR){
						printf("data ");
						err_display("send()");
					}

					
					endTime = clock() - startTime;

					if(time_min > endTime)
						time_min = endTime;
					if(time_max < endTime)
						time_max = endTime;
					
					

					//static int s_i = 0;

					//s_i ++;

					//printf("count : %d \n ", s_i);

					/*retval = send(sock, (char*)&s_i, sizeof(int), 0);
					if(retval == SOCKET_ERROR){
						printf("data ");
						err_display("send()");
					}*/

					file_len -= file_read_len;
				}				

				fclose(f);
				printf("file send complete\n");
				printf("total time %d\n", clock() - currentTime);
				printf("send return time pre_min::%d min::%d max::%d\n", pre_time_min, time_min, time_max);
			}
			break;
		}

	}

	closesocket(sock);
	WSACleanup();

	getchar();

	return 0;
}