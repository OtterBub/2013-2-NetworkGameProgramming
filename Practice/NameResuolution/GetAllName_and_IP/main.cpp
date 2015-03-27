#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#define TESTNAME "www.yahoo.co.kr"

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{

	if(argc <= 2 || argc >= 4){
		printf("name www.example.net, ip 123.123.123.123, ip6 1234:1234:1234:1234::1234 로 입력해주세요 \n");
		exit(1);
	}

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// 도메인 이름 -> IP 주소
	IN_ADDR addr;
	IN6_ADDR addrv6;
	HOSTENT *host;
	char addrstring[100];	

	struct addrinfo *result = NULL;
	struct addrinfo *next = NULL;
	struct addrinfo hints;
	void* ptr;

	ZeroMemory(&hints, sizeof(hints));
	ZeroMemory(addrstring, sizeof(addrstring));
	
	hints.ai_family = AF_UNSPEC;
	//hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	int select = 0;

	if(strcmp("name", argv[1]) == 0){ // domain name
		select = 1;		
	} else if (strcmp("ip", argv[1]) == 0){ // ipv4
		select = 2;
	} else if (strcmp("ip6", argv[1]) == 0){ // ipv6
		select = 3;
	} else {
		select = 100;
		printf("첫 번째 인자값이 잘못 되었습니다. name, ip, ip6 중 하나만 입력해 주세요.\n");
		exit(1);
	}

	switch(select)
	{
	case 1: // domain name
		host = gethostbyname(argv[2]);

		if(getaddrinfo(argv[2], NULL, &hints, &result) != 0){
			err_display("err : getaddrinfo()");
		}
		if(host == NULL){
			err_display("err : gethostbyname()");
			exit(1);
		}
		break;
	case 2: // ipv4
		//addr.s_addr = inet_addr(argv[2]);

		inet_pton(AF_INET, argv[2], &addr);
		host = gethostbyaddr((char*) &addr, 4, AF_INET);

		if(getaddrinfo(argv[2], NULL, &hints, &result) != 0){
			err_display("err : getaddrinfo()");
		}

		if(host == NULL){
			err_display("err : gethostbyaddr()");
			exit(1);
		}
		break;
	case 3: // ipv6
		inet_pton(AF_INET6, argv[2], &addrv6);

		host = gethostbyaddr((char*) &addrv6, 16, AF_INET6);

		if(getaddrinfo(argv[2], NULL, &hints, &result) != 0){
			err_display("err : getaddrinfo()");
		}

		if(host == NULL){
			err_display("err : gethostbyaddr()");
			exit(1);
		}
		break;
	default:
		break;
	}

	if(host == NULL){
		err_display("host == NULL");
		exit(1);
	}
	printf("-------- hostent -------------\n");
	printf("------- host name ------------\n");
	printf("%s\n\n", host->h_name);

	printf("-------- all aliases ------------\n");
	for(int i = 0; host->h_aliases[i]; i++){
		printf("%s \n", host->h_aliases[i]);
	}

	printf("\n");
	printf("-------- all addr ------------\n");
	for(int i = 0; host->h_addr_list[i]; i++){	
		inet_ntop(host->h_addrtype, host->h_addr_list[i], addrstring, 100);
		printf("%s \n", addrstring);
	}


	/*printf("\n");
	printf("-------- sizeof(struct) ------------\n");
	printf("sockaddr::%d sockaddr_in::%d sockaddr_in6::%d \n", sizeof(sockaddr), sizeof(sockaddr_in), sizeof(sockaddr_in6));
	printf("short::%d u_short::%d u_long::%d \n", sizeof(short), sizeof(u_short), sizeof(u_long));*/

	printf("\n");
	printf("-------- addrinfo ----------\n");
	printf("------- host name ------------\n");
	char hostname[100];

	if (getnameinfo(result->ai_addr, sizeof(struct sockaddr), hostname, sizeof(hostname), NULL,0,0) != 0) {
		err_display("error using getnameinfo");
	}
	printf("%s\n\n", hostname);

	printf("-------- getaddrinfo ----------\n");
	while(result){

		int current_family = result->ai_family;

		for(; result;  result = result->ai_next ){
			switch( result->ai_family )
			{
			case AF_INET:
				ptr = &((struct sockaddr_in *)result->ai_addr)->sin_addr;
				/*{
				char* temp = (char*)(result->ai_addr);
				ptr = temp + 4;
				}*/
				break;
			case AF_INET6:
				ptr = &((struct sockaddr_in6 *)result->ai_addr)->sin6_addr;
				/*{
				char* temp = (char*)(result->ai_addr);
				ptr = temp + 8;
				}*/
				break;
			}
			inet_ntop(result->ai_family, ptr, addrstring, 100);
			printf("canonname : %s\n", result->ai_canonname);
			printf("addr : %s\n\n", addrstring);
		}
		if(current_family == AF_INET){
			hints.ai_family = AF_INET6;
			if(getaddrinfo(argv[2], NULL, &hints, &result) != 0){
				err_display("err : getaddrinfo() (ipv6)");
			}
		}
	}

	WSACleanup();
	return 0;
}