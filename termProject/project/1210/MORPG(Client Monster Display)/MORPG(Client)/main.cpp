#include "CallBackFunc.h"
#include "Controller.h"
#include"Client.h"
#include "UDPClient.h"

int main(int argc, char *argv[])
{
	hReadEvent = CreateEvent( NULL, FALSE, TRUE, NULL );
	if( hReadEvent == NULL )	return -1;
	hWriteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( hWriteEvent == NULL )	return -1;

	srand((unsigned)time(NULL));
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( WIDTH, HEIGHT );
	glutCreateWindow( "MORPG" );
	glutDisplayFunc( drawScene );
	glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
	glutMotionFunc( mouseMove );
	glutKeyboardFunc( keyBoardFunc );
	glutKeyboardUpFunc( keyUpFunc );
	glutSpecialFunc( specialKeyBoardFunc );
	glutTimerFunc( 600, timerFunc, 1 );

	HANDLE hThread;

	char SERVERIP[20];
	printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );

	hThread = CreateThread( NULL, 0, ClientMain, &SERVERIP, 0, NULL );

	CloseHandle( hReadEvent );
	CloseHandle( hWriteEvent );

	glutMainLoop();
}

