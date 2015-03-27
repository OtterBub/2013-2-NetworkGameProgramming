#include "UDPClient.h"

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);

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
		
		strcpy(buf, " ");

		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\0';
		if(strlen(buf) == 0)
			break;

		// ������ ������
		retval = sendto(sock, buf, strlen(buf), 0, 
			(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}
		//DisplayText(hEdit6, "[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, (char*)&WorldInfo, sizeof(WorldInfo), 0, 
			(SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}

		// �۽����� IP �ּ� üũ
		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("[����] �߸��� �������Դϴ�!\n");
			continue;
		}

		//if((count % 500) == 0)
			//SetWindowText(hEdit6, "");

		// ���� ������ ���
		/*	printf("[Info] ::%d \n", count++);
		for(int i = 0; i < WorldInfo.PlayerNum; i++){
			
			printf("[UserInfo] Num::%d \n Pos x::%.2f y::%.2f z::%.2f \n", 
				i, 
				WorldInfo.playerInfo[i].UserPosition.x, WorldInfo.playerInfo[i].UserPosition.y, WorldInfo.playerInfo[i].UserPosition.z, 
				count++);
		}*/
		
		WORLD->SetWorldData(WorldInfo);

		_sleep(33);
	}

	// closesocket()
	closesocket(sock);

	return 0;
}