#include "Client.h"

// 소켓 함수 오류 출력 후 종료

SOCKET sock;
PlayerData playerData;

void err_quit( char *msg )
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		( LPTSTR )&lpMsgBuf, 0, NULL );
	MessageBox( NULL, ( LPCTSTR )lpMsgBuf, ( LPCSTR )msg, MB_ICONERROR );
	LocalFree( lpMsgBuf );
	exit( 1 );
}

void err_display( char *msg ){
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&lpMsgBuf, 0, NULL 
		);
	printf( "[%s] %s", msg, ( char * )lpMsgBuf );
	LocalFree( &lpMsgBuf );
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
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

DWORD WINAPI ClientMain(LPVOID arg){
	int retval;

	WSADATA wsa;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return 1;

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == INVALID_SOCKET )
		err_quit( "socket()" );

	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);
	/*printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );
	fflush( stdin );*/
	/*printf("\n");
	printf(SERVERIP);*/

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr( SERVERIP );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = connect( sock, ( SOCKADDR * )&serveraddr, sizeof( serveraddr ) );
	if( retval == SOCKET_ERROR )
		err_quit( "connect()" );
	else {
		printf("Login \n");
		_sleep(1000);
	}

	char readString[BUFSIZE+1];
	int len;
	int playerID = 0;

	retval = recv(sock, (char*)&playerID, sizeof(playerID), 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
	}

	//myChar->SetID(playerID);
	RPGWorld::Instance()->SetMyCharID(playerID);

	printf("my char id : %d \n", myChar->GetID());

	WorldData WorldInfo;

	ZeroMemory(&WorldInfo, sizeof(WorldInfo));

	while( 1 ){
		retval = recvn(sock, (char*)&WorldInfo, sizeof(WorldData), 0);
		if(retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		} else if(retval == 0)
			break;
		
		RPGWorld::Instance()->SetWorldData(WorldInfo);
		SetEvent(hLogInComplete);
	}
	closesocket( sock );

	WSACleanup();
	
	return 0;
}