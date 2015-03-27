#include "UDPClient.h"

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);

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
		
		strcpy(buf, " ");

		// '\n' 문자 제거
		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\0';
		if(strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = sendto(sock, buf, strlen(buf), 0, 
			(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}
		//DisplayText(hEdit6, "[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, (char*)&WorldInfo, sizeof(WorldInfo), 0, 
			(SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}

		// 송신자의 IP 주소 체크
		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("[오류] 잘못된 데이터입니다!\n");
			continue;
		}

		//if((count % 500) == 0)
			//SetWindowText(hEdit6, "");

		// 받은 데이터 출력
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