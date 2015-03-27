#include "Controller.h"

#define CAMERA_EYE_SPEED 10
int inputKey;

GLvoid keyBoardFunc(unsigned char key, int x, int y)
{

	static time_t keyInputTime = clock();

	if(500 < clock() - keyInputTime){
		keyInputTime = clock();
	} else {
		return;
	}

	static int CHAR_SPEED = 10;
	switch(key)
	{
	case '1':
		break;
	case 'a':
		//myChar->ObjectControlPosition(-CHAR_SPEED, 0, 0);
		//myChar->addVelocity(-CHAR_SPEED / 10.f, 0, 0);
		playerData.moveDir = moveDir::MOVE_LEFT;
		break;
	case 'd':
		//myChar->ObjectControlPosition(+CHAR_SPEED / 10.f, 0, 0);
		//myChar->addVelocity(+CHAR_SPEED / 10.f, 0, 0);
		playerData.moveDir = moveDir::MOVE_RIGHT;
		break;
	case 'w':
		//myChar->ObjectControlPosition(0, 0, -CHAR_SPEED / 10.f);
		//myChar->addVelocity(0, 0, -CHAR_SPEED / 10.f);
		playerData.moveDir = moveDir::MOVE_UP;
		break;
	case 's':
		//myChar->ObjectControlPosition(0, 0, CHAR_SPEED / 10.f);
		//myChar->addVelocity(0, 0, CHAR_SPEED / 10.f);
		playerData.moveDir = moveDir::MOVE_DOWN;
		break;
	case 'r':
		g_eye_y += CAMERA_EYE_SPEED;
		break;
	case 'f':
		g_eye_y -= CAMERA_EYE_SPEED;
		break;
	case 'q':
		g_eye_x -= CAMERA_EYE_SPEED;
		g_center_x -= CAMERA_EYE_SPEED;
		break;
	case 'e':
		g_eye_x += CAMERA_EYE_SPEED;
		g_center_x += CAMERA_EYE_SPEED;
		break;
	case 'x':
		g_x--;
		break;
	case 'X':
		g_x++;
		break;
	case 'y':
		g_y--;
		break;
	case 'Y':
		g_y++;
		break;
	case 'z':
		g_z--;
		break;
	case 'Z':
		g_z++;
		break;
	case 't':
		g_r-=0.05;
		break;
	case 'T':
		g_r+=0.05;
		break;
	case 'g':
		g_g-=0.05;
		break;
	case 'G':
		g_g+=0.05;
		break;
	case 'b':
		g_b-=0.05;
		break;
	case 'B':
		g_b+=0.05;
		break;
	case ' ':
		WORLD->AddFunction();
		break;
	default:
		break;
	}

	int retval;
	
	_point pointChar = RPGWorld::Instance()->GetMyChar()->GetPoint();
	_point velocitychar = RPGWorld::Instance()->GetMyChar()->GetVelocity();

	playerData.UserPosition.x = pointChar.x;
	playerData.UserPosition.y = pointChar.y;
	playerData.UserPosition.z = pointChar.z;

	playerData.UserVelocity.x = velocitychar.x;
	playerData.UserVelocity.y = velocitychar.y;
	playerData.UserVelocity.z = velocitychar.z;
	playerData.attackDir = inputKey;

	retval = send( sock, ( char* )&playerData, sizeof( playerData ), 0 );
	if( retval == SOCKET_ERROR ){
		err_display( "send()" );
		return;
	}
}

GLvoid keyUpFunc(unsigned char key, int x, int y) {
	int retval;
	
	_point pointChar = RPGWorld::Instance()->GetMyChar()->GetPoint();
	_point velocitychar = RPGWorld::Instance()->GetMyChar()->GetVelocity();

	playerData.UserPosition.x = pointChar.x;
	playerData.UserPosition.y = pointChar.y;
	playerData.UserPosition.z = pointChar.z;

	playerData.UserVelocity.x = velocitychar.x;
	playerData.UserVelocity.y = velocitychar.y;
	playerData.UserVelocity.z = velocitychar.z;
	playerData.attackDir = inputKey;
	playerData.moveDir = 0;

	retval = send( sock, ( char* )&playerData, sizeof( playerData ), 0 );
	if( retval == SOCKET_ERROR ){
		err_display( "send()" );
		return;
	}
}

GLvoid specialKeyBoardFunc(int key, int x, int y)
{
	// left : 100 / up : 101 / right : 102 / down : 103
	// page up : 104 / page down : 105
	switch(key)
	{
	case 100:
		g_center_x -= CAMERA_EYE_SPEED;
		break;
	case 102:
		g_center_x += CAMERA_EYE_SPEED;
		break;
	case 101:
		g_center_z -= CAMERA_EYE_SPEED;
		break;
	case 103:
		g_center_z += CAMERA_EYE_SPEED;
		break;
	case 104:
		g_center_y += CAMERA_EYE_SPEED;
		break;
	case 105:
		g_center_y -= CAMERA_EYE_SPEED;
		break;
	}
	printf("%d\n", key);
}

GLvoid mouse(int button, int state, int x, int y)
{
	//WaitForSingleObject( hReadEvent, INFINITE );
	printf("mouse x : %d, y : %d\n", x, y);

	if(state == GLUT_DOWN) {
		switch (button)	{
		case GLUT_LEFT_BUTTON:
			{
				if(x >= 500){
					WORLD->Attack(Player::RIGHT);
					inputKey = RIGHT;
				}
				else if (x <= 300){
					WORLD->Attack(Player::LEFT);
					inputKey = LEFT;
				}
				else if (y <= 200){
					WORLD->Attack(Player::UP);
					inputKey = UP;
				}
				else if (y >= 400){
					WORLD->Attack(Player::DOWN);
					inputKey = DOWN;
				}
			}
			//WORLD->Attack();
			break;

		case GLUT_RIGHT_BUTTON:
			break;		

		default:
			break;
		}
	}

	if(state == GLUT_UP) {
		switch (button)	{
		case GLUT_LEFT_BUTTON:
		{
			inputKey = 0;
		}
		break;

		case GLUT_RIGHT_BUTTON:
			break;		

		default:
			break;
		}
	}
	int retval;
	PlayerData playerData;
	_point pointChar = RPGWorld::Instance()->GetMyChar()->GetPoint();
	_point velocitychar = RPGWorld::Instance()->GetMyChar()->GetVelocity();

	playerData.UserPosition.x = pointChar.x;
	playerData.UserPosition.y = pointChar.y;
	playerData.UserPosition.z = pointChar.z;

	playerData.UserVelocity.x = velocitychar.x;
	playerData.UserVelocity.y = velocitychar.y;
	playerData.UserVelocity.z = velocitychar.z;
	playerData.attackDir = inputKey;

	retval = send( sock, ( char* )&playerData, sizeof( playerData ), 0 );
	if( retval == SOCKET_ERROR ){
		err_display( "send()" );
		return;
	}
}

GLvoid mouseMove(int x, int y)
{
	static int pre_x = 0, pre_y = 0;

	//printf("%d, %d \n", x - pre_x, y - pre_y);

	pre_x = x;
	pre_y = y;
}