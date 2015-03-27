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

	int DepthMenu, CullingMenu, ShadingMenu;
	int MainMenu;

	DepthMenu = glutCreateMenu( MenuFuntion );
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	CullingMenu = glutCreateMenu( MenuFuntion );
	glutAddMenuEntry("On", 11);
	glutAddMenuEntry("Off", 22);

	ShadingMenu = glutCreateMenu( MenuFuntion );
	glutAddMenuEntry("Flat", 111);
	glutAddMenuEntry("Smooth", 222);

	MainMenu = glutCreateMenu( MenuFuntion );
	glutAddSubMenu("Depth", DepthMenu);
	glutAddSubMenu("Culling", CullingMenu);
	glutAddSubMenu("Shading", ShadingMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc( 600, timerFunc, 1 );

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

	glutMainLoop();
}

