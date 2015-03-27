#include "CallBackFunc.h"
#include "Controller.h"
#include"Client.h"
#include "UDPClient.h"

int main(int argc, char *argv[])
{


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

	int ViewModeMenu;
	int MainMenu;

	ViewModeMenu = glutCreateMenu( MenuFuntion );
	glutAddMenuEntry("2D View Front", 1111);
	glutAddMenuEntry("3D View", 2222);

	MainMenu = glutCreateMenu( MenuFuntion );
	glutAddSubMenu("View Mode", ViewModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutTimerFunc( 600, timerFunc, 1 );

	HANDLE hThread;

	char SERVERIP[20];
	printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );

	hLogInComplete = CreateEvent( NULL, TRUE, FALSE, NULL );
	if( hLogInComplete == NULL )	return -1;


	hThread = CreateThread( NULL, 0, ClientMain, &SERVERIP, 0, NULL );

	CloseHandle( hLogInComplete );

	WaitForSingleObject(hLogInComplete, INFINITE);

	glutMainLoop();
}

