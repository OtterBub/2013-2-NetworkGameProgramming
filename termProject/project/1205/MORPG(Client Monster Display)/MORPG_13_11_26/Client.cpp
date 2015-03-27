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

	char readString[BUFSIZE+1];
	int len;
	int playerID = 0;

	retval = recv(sock, (char*)&playerID, sizeof(playerID), 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
	}

	printf("%d", playerID);
	//myChar->SetID(playerID);
	RPGWorld::Instance()->SetMyCharID(playerID);

	printf("my char id : %d \n", myChar->GetID());

	while( 1 ){

		////printf( "%d", inputKey);
		////if( inputKey == 0 ){
		////	SetEvent( hReadEvent );
		////	continue;
		////}

		//
		///*sprintf( readString, "%.0f, %.0f, %.0f", pointChar.x, pointChar.y, pointChar.z );

		//len = strlen( readString );

		//retval = send( sock, readString, len, 0 );
		//if( retval == SOCKET_ERROR ){
		//	err_display( "send()" );
		//	break;
		//}
		//printf( "%s를 보냈습니다\n", readString );*/

		//_point pointChar = RPGWorld::Instance()->GetMyChar()->GetPoint();
		//_point velocitychar = RPGWorld::Instance()->GetMyChar()->GetVelocity();

		//playerData.UserPosition.x = pointChar.x;
		//playerData.UserPosition.y = pointChar.y;
		//playerData.UserPosition.z = pointChar.z;

		//playerData.UserVelocity.x = velocitychar.x;
		//playerData.UserVelocity.y = velocitychar.y;
		//playerData.UserVelocity.z = velocitychar.z;
		//playerData.attackDir = inputKey;
		//
		//retval = send( sock, ( char* )&playerData, sizeof( playerData ), 0 );
		//if( retval == SOCKET_ERROR ){
		//	err_display( "send()" );
		//	break;
		//}
		////printf( "%.0f, %.0f, %.0f를 보냈습니다\n", playerDate.UserPosition.x, playerDate.UserPosition.y, playerDate.UserPosition.z );

		//SetEvent( hReadEvent );
		////inputKey = 0;
		//Sleep(1000);
	}
	closesocket( sock );

	WSACleanup();
	
	return 0;
}