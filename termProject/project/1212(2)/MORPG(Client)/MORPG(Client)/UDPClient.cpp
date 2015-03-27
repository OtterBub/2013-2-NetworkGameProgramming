#include "UDPClient.h"

#define MULTCASTADDR "235.1.200.9"

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// ���� �ʱ�ȭ
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
	
	// ���� �ּ� ����ü �ʱ�ȭ
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
	

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peeraddr;
	WorldData WorldInfo;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;
	int count = 0;
	// ������ ������ ���
	while(1){
		// ������ �Է�
		
		//strcpy(buf, " ");

		//// '\n' ���� ����
		//len = strlen(buf);
		//if(buf[len-1] == '\n')
		//	buf[len-1] = '\0';
		//if(strlen(buf) == 0)
		//	break;

		//// ������ ������
		//retval = sendto(sock, buf, strlen(buf), 0, 
		//	(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		//if(retval == SOCKET_ERROR){
		//	err_display("sendto()");
		//	continue;
		//}
		//DisplayText(hEdit6, "[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
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