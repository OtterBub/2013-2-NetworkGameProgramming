#include "CallBackFunc.h"
#include "Controller.h"
#include "UDPClient.h"

void main(int argc, char *argv[])
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

	HANDLE hThread;

	char SERVERIP[20];
	printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );

	hThread = CreateThread( NULL, 0, ClientMain, &SERVERIP, 0, NULL );

	hThread = CreateThread( NULL, 0, UDPClientMain, &SERVERIP, 0, NULL );
	//WaitForSingleObject( hThread, INFINITE );
	glutMainLoop();

}

