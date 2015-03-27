#include "Client.h"

// ���� �Լ� ���� ��� �� ����
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

// ����� ���� ������ ���� �Լ�
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

	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == INVALID_SOCKET )
		err_quit( "socket()" );

	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);
	/*printf( "���� ip �ּҸ� �Է��Ͻÿ�: " );
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

	PlayerData playerDate;

	while( 1 ){
		WaitForSingleObject( hWriteEvent, INFINITE );

		//printf( "%d", inputKey);
		//if( inputKey == 0 ){
		//	SetEvent( hReadEvent );
		//	continue;
		//}

		_point pointChar = RPGWorld::Instance()->GetMyChar()->GetPoint();
		/*sprintf( readString, "%.0f, %.0f, %.0f", pointChar.x, pointChar.y, pointChar.z );

		len = strlen( readString );

		retval = send( sock, readString, len, 0 );
		if( retval == SOCKET_ERROR ){
			err_display( "send()" );
			break;
		}
		printf( "%s�� ���½��ϴ�\n", readString );*/

		

		playerDate.UserPosition.x = pointChar.x;
		playerDate.UserPosition.y = pointChar.y;
		playerDate.UserPosition.z = pointChar.z;
		playerDate.attackDir = inputKey;
		
		retval = send( sock, ( char* )&playerDate, sizeof( playerDate ), 0 );
		if( retval == SOCKET_ERROR ){
			err_display( "send()" );
			break;
		}
		//printf( "%.0f, %.0f, %.0f�� ���½��ϴ�\n", playerDate.UserPosition.x, playerDate.UserPosition.y, playerDate.UserPosition.z );

		Sleep( 100 );

		SetEvent( hReadEvent );
		//inputKey = 0;
	}
	closesocket( sock );

	WSACleanup();
	
	return 0;
}