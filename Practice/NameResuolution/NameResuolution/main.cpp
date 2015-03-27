#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

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

BOOL GetIPAddr(char *name, IN_ADDR *addr)
{
	HOSTENT *ptr = gethostbyname(name);
	if(ptr == NULL){
		err_display("gethostbyname()");
		return FALSE;
	}
	if(ptr->h_addrtype != AF_INET)
		return FALSE;
	memcpy(addr, ptr->h_addr, ptr->h_length);

	return TRUE;
}

BOOL GetIPAddr6(char *name, IN6_ADDR *addr6)
{

	struct addrinfo *result = NULL;
	struct addrinfo *next = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	if(getaddrinfo(name, NULL, &hints, &result) != 0){
		err_display("err : getaddrinfo()");
		exit(1);
	}

	if(result->ai_family != AF_INET6){
		return FALSE;
	}

	addr6 = &((sockaddr_in6*)result->ai_addr)->sin6_addr;

	return TRUE;
}

BOOL GetDomainName(IN_ADDR addr, char *name, int namelen)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
	if(ptr == NULL){
		err_display("gethostbyaddr()");
		return FALSE;
	}
	if(ptr->h_addrtype != AF_INET)
		return FALSE;
	strncpy(name, ptr->h_name, namelen);
	return TRUE;
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	
	char addrstring[100];
	in6_addr addr6;

	GetIPAddr6(TESTNAME, &addr6);
	inet_pton(AF_INET6, addrstring, &addr6);
	printf("IP 주소 = %s\n", addrstring);

	// 도메인 이름 -> IP 주소
	IN_ADDR addr;
	if(GetIPAddr(TESTNAME, &addr)){
		// 성공이면 결과 출력
		printf("IP 주소 = %s\n", inet_ntoa(addr));
	
		// IP 주소 -> 도메인 이름
		char name[256];
		if(GetDomainName(addr, name, sizeof(name))){
			// 성공이면 결과 출력
			printf("도메인 이름 = %s\n", name);
		}
	}

	WSACleanup();
	return 0;
}