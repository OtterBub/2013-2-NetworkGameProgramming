#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <direct.h>

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

#define BUFSIZE 50
#define FILEBUFSIZE 1024
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


	char dir[100] = "Client/";
	char fileName[FILENAMESIZE];
	char buff[FILEBUFSIZE];
	char buf[FILEBUFSIZE];
	char chatBuf[100];
	int MaxTime = INT_MIN;
	int MinTime = INT_MAX;

	int len = 0;

	len = strlen(chatBuf);
	strncpy(buf, chatBuf, len);
	buf[len++] = '\n';

	printf("MAX read File size %d \n", FILEBUFSIZE);
	printf("File name : ");

	gets(&dir[7]);
	printf("%s\n", dir);

	//dir[strlen(dir)] = '\0';

	while(1){
		system("cls");
		FILE *f;
		f = fopen(dir, "rb");

		if(f == NULL){
			printf("don't found file\n");

			return -1;
		}


		fseek(f, 00, SEEK_END);
		int file_len = ftell(f);
		fseek(f, 00, SEEK_SET);

		int file_read_len = 0;
		fpos_t filepos;
		fgetpos(f, &filepos);

		strcpy(fileName, (strrchr(dir, '/'))+1);
		printf("MAX read File size %d \n", FILEBUFSIZE);
		printf("file name : %s\n", fileName);
		printf("file size : %d\n", file_len);

		len = strlen(fileName);

		printf("MinTime::%d / MaxTime::%d\n", MinTime, MaxTime);

		time_t currentTime = clock();

		while(file_len > 0){

			file_read_len = fread(buff, sizeof(char), FILEBUFSIZE ,f);

			if(file_len == 0)
				break;



			file_len -= file_read_len;
		}				

		fclose(f);
		printf("file send complete\n");

		int resultTime = clock() - currentTime;
		printf("total time %d\n", resultTime);
		if(MinTime > resultTime){
			MinTime = resultTime;
		}

		if(MaxTime < resultTime){
			MaxTime = resultTime;
		}

		
	}

	getchar();

	return 0;
}
