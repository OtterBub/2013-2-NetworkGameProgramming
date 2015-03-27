#include "ServerThread.h"

#define BUFSIZE 512
#define MULTCASTADDR "235.1.200.9"

//std::vector<SOCKADDR_IN*> clientList;
struct clientListInfo
{
	SOCKET* cliensSock;
	bool use;
};
int clientNum = 0;
clientListInfo clientAddList[50] = {0,};

DWORD WINAPI CreatePlayerSocket(LPVOID arg)
{
	int retval;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// -------------------- Non - Blocking --------------------------
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if(listen_sock == INVALID_SOCKET) err_quit("ioctlsocket()");

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
			if(WSAGetLastError() != WSAEWOULDBLOCK) {
				err_display("recv()");
				break;
			}
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
	return 0;
}

DWORD WINAPI PlayerSoketThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;

	BOOL bPotVal = TRUE;
	int bOptLen = sizeof(BOOL);

	setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bPotVal, bOptLen);

	char buf[BUFSIZE + 1];
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;
	int playerIndex;

	PlayerData playerPlayerData;
	Object3D* playerChar;
	

	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);

	playerChar = RPGWorld::Instance()->AddObject(new Player((int)&playerChar));

	for(int i = 0; i < 50; i++) {
		if (clientAddList[i].use == false) {
			clientAddList[i].use = true;
			clientAddList[i].cliensSock = &client_sock;
			playerIndex = i;

			break;
		}
	}

	int playerID = playerChar->getID();

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	retval = send(client_sock, (char*)&playerID, sizeof(playerID), 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
	}

	SetEvent(hLogInOutEvent);	

	

	while(1){
		retval = recv(client_sock, (char*)&playerPlayerData, sizeof(playerPlayerData), 0);
		if(retval == SOCKET_ERROR){
			if(WSAGetLastError() != WSAEWOULDBLOCK) {
				err_display("recv()");
				break;
			}
		} else if(retval == 0)
			break;
		else {

			static int CHAR_SPEED = 1;

			int dir = playerPlayerData.moveDir;

			if(dir & MOVE_RIGHT){
				playerChar->addVelocity(+CHAR_SPEED / 10.f, 0, 0);
			}
			if(dir & MOVE_LEFT){
				playerChar->addVelocity(-CHAR_SPEED / 10.f, 0, 0);
			}
			if(dir & MOVE_UP){
				playerChar->addVelocity(0, 0, -CHAR_SPEED / 10.f);
			}
			if(dir & MOVE_DOWN){
				playerChar->addVelocity(0, 0, +CHAR_SPEED / 10.f);
			}
			if(playerPlayerData.attackDir){
				RPGWorld::Instance()->Attack(playerPlayerData.attackDir, playerChar);
			}
		}
	}

	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);
	RPGWorld::Instance()->DeleteObject(playerChar);
	clientAddList[playerIndex].use = false;
	SetEvent(hLogInOutEvent);

	printf("client logout\n");

	closesocket(client_sock);

	return 0;
}

DWORD WINAPI WorldDataBroadCastThread(LPVOID arg)
{	
	int retval;
	int count = 0;
	while(1){
		WaitForSingleObject(hLogInOutEvent, INFINITE);
		WorldData serverWorld = RPGWorld::Instance()->GetWorldData();
		for(int i = 0; i < 20; i++){
			if(clientAddList[i].use) {
				retval = send(*(clientAddList[i].cliensSock), (char*)&serverWorld, sizeof(WorldData), 0);
				if(retval == SOCKET_ERROR) {
					if(WSAGetLastError() != WSAEWOULDBLOCK) {
						err_display("send()");
						continue;
					}
				}
			}
		}
		//printf("complete send packet %d\n", count++);
		_sleep(2);
	}


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