#include "UDPClient.h"

#define MULTCASTADDR "235.1.200.9"

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET udpSock = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpSock == INVALID_SOCKET) err_quit("socket()");

	BOOL optval = TRUE;
	retval = setsockopt(udpSock, SOL_SOCKET, SO_REUSEADDR,
		(char *)&optval, sizeof(optval));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");
	
	// 소켓 주소 구조체 초기화
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(udpSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTCASTADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(udpSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");
	

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	WorldData WorldInfo;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;
	int count = 0;
	// 서버와 데이터 통신
	while(1){
		// 데이터 입력
		
		//strcpy(buf, " ");

		//// '\n' 문자 제거
		//len = strlen(buf);
		//if(buf[len-1] == '\n')
		//	buf[len-1] = '\0';
		//if(strlen(buf) == 0)
		//	break;

		//// 데이터 보내기
		//retval = sendto(sock, buf, strlen(buf), 0, 
		//	(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		//if(retval == SOCKET_ERROR){
		//	err_display("sendto()");
		//	continue;
		//}
		//DisplayText(hEdit6, "[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		/*addrlen = sizeof(peeraddr);
		retval = recvfrom(udpSock, (char*)&WorldInfo, sizeof(WorldInfo), 0, 
			(SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}*/

		//RPGWorld::Instance()->SetWorldData(WorldInfo);
		
	}

	retval = setsockopt(udpSock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");

	// closesocket()
	closesocket(udpSock);

	return 0;
}