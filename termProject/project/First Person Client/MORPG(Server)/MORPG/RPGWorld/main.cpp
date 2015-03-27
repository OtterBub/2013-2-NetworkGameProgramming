#include "Controller.h"
#include "CallBackFunc.h"
#include "ServerThread.h"




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

	hLogInOutEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	WSADATA wsa;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return -1;

	HANDLE hThread;
	DWORD ThreadId;

	hThread = CreateThread(NULL, 0, CreatePlayerSocket,
		NULL, 0, &ThreadId);
	if(hThread == NULL)
		printf("[오류] 스레드 생성 실패!\r\n");
	else
		CloseHandle(hThread);

	hThread = CreateThread(NULL, 0, WorldDataBroadCastThread,
		NULL, 0, &ThreadId);
	if(hThread == NULL)
		printf("[오류] 스레드 생성 실패!\r\n");
	else
		CloseHandle(hThread);	

	CloseHandle(hLogInOutEvent);

	glutMainLoop();
}

