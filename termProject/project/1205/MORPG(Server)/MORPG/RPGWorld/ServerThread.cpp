#include "ServerThread.h"

#define BUFSIZE 512
#define MULTCASTADDR "235.1.200.9"

//std::vector<SOCKADDR_IN*> clientList;
struct clientListInfo
{
	SOCKADDR_IN addr;
	bool use;
};
int clientNum = 0;
clientListInfo clientAddList[50] = {0,};

DWORD WINAPI CreatePlayerSocket(LPVOID arg)
{
	int retval;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	DWORD ThreadId;
	while(1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if(client_sock == SOCKET_ERROR) {
			err_display("accept()");
			continue;
		}
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		hThread = CreateThread(NULL, 0, PlayerSoketThread,
			(LPVOID)client_sock, 0, &ThreadId);
		if(hThread == NULL)
			printf("[오류] 스레드 생성 실패!\r\n");
		else
			CloseHandle(hThread);
	}
	closesocket(listen_sock);
	
}

DWORD WINAPI PlayerSoketThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	char buf[BUFSIZE + 1];
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;
	int playerIndex;

	PlayerData playerPlayerData;
	Object3D* playerChar;
	

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);
	playerChar = RPGWorld::Instance()->AddObject(new Player((int)&playerChar));
	SetEvent(hLogInOutEvent);	

	int playerID = playerChar->getID();

	retval = send(client_sock, (char*)&playerID, sizeof(playerID), 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
	}

	while(1){
		retval = recv(client_sock, (char*)&playerPlayerData, sizeof(playerPlayerData), 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		} else if(retval == 0)
			break;
		else {
			//printf("(%.2f %.2f %.2f) atk::%d\n", playerPlayerData.UserPosition.x, playerPlayerData.UserPosition.y, playerPlayerData.UserPosition.z, playerPlayerData.attackDir);
			//playerChar->SetObjectPosition(playerPlayerData.UserPosition.x, playerPlayerData.UserPosition.y, playerPlayerData.UserPosition.z);
			//layerChar->setVelocity(playerPlayerData.UserVelocity.x, playerPlayerData.UserVelocity.y, playerPlayerData.UserVelocity.z);
			static int CHAR_SPEED = 10;
			switch(playerPlayerData.moveDir)
			{
			case MOVE_RIGHT:
				playerChar->addVelocity(+CHAR_SPEED / 10.f, 0, 0);
				break;
			case MOVE_LEFT:
				playerChar->addVelocity(-CHAR_SPEED / 10.f, 0, 0);
				break;
			case MOVE_UP:
				playerChar->addVelocity(0, 0, -CHAR_SPEED / 10.f);
				break;
			case MOVE_DOWN:
				playerChar->addVelocity(0, 0, +CHAR_SPEED / 10.f);
				break;
			default:
				break;
			}
			if(playerPlayerData.attackDir){
				RPGWorld::Instance()->Attack(playerPlayerData.attackDir, playerChar);
			}
		}
	}

	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);
	RPGWorld::Instance()->DeleteObject(playerChar);
	SetEvent(hLogInOutEvent);

	printf("client logout\n");
	

	closesocket(client_sock);

	return 0;
}

DWORD WINAPI WorldDataBroadCastThread(LPVOID arg)
{
	int retval;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	int ttl = 2;

	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = inet_addr(MULTCASTADDR);

	WorldData WorldInfo;
	SOCKADDR_IN clientaddr;

	int addrlen;
	int len;
	char buf[BUFSIZE + 1];

	while(1) {
		// 데이터 받기
		/*addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, 
			(SOCKADDR *)&clientaddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}*/

		// 받은 데이터 출력
		/*buf[retval] = '\0';*/
		//printf("[UDP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), 
			//ntohs(clientaddr.sin_port), buf);

		

		// 데이터 보내기
		ZeroMemory(&WorldInfo, sizeof(WorldInfo));
		WorldInfo = RPGWorld::Instance()->GetWorldData();

		
		retval = sendto(sock, reinterpret_cast<char*>(&WorldInfo), sizeof(WorldInfo), 0, 
			(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}

		_sleep(33);
		
	}

	closesocket(sock);

	return 0;
}

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
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