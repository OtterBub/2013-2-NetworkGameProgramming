#include "Controller.h"

#define CAMERA_EYE_SPEED 10
int inputKey;

static bool keyDown[256] = {0, };
static time_t keyInputTime = clock();

GLvoid keyBoardCalculation() {

	if(16 < clock() - keyInputTime){
		keyInputTime = clock();
	} else {
		return;
	}
	
	if(keyDown['a'])
		playerData.moveDir |= moveDir::MOVE_LEFT;
	if(keyDown['d'])
		playerData.moveDir |= moveDir::MOVE_RIGHT;
	if(keyDown['w'])
		playerData.moveDir |= moveDir::MOVE_UP;
	if(keyDown['s'])
		playerData.moveDir |= moveDir::MOVE_DOWN;

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

	playerData.moveDir = 0x0000;
}

GLvoid keyBoardFunc(unsigned char key, int x, int y)
{

	keyDown[key] = true;
}

GLvoid keyUpFunc(unsigned char key, int x, int y) {

	keyDown[key] = false;

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
	static int mouseInputClock = 0;
	if(16 < clock() - mouseInputClock){
		mouseInputClock = clock();
	} else {
		return;
	}


	//WaitForSingleObject( hReadEvent, INFINITE );
	printf("mouse x : %d, y : %d\n", x, y);

	if(state == GLUT_DOWN) {
		switch (button)	{
		case GLUT_LEFT_BUTTON:
			{
				if(x >= 500) {
					WORLD->Attack(Player::RIGHT);
					inputKey = RIGHT;
				}
				else if (x <= 300) {
					WORLD->Attack(Player::LEFT);
					inputKey = LEFT;
				}
				else if (y <= 200) {
					WORLD->Attack(Player::UP);
					inputKey = UP;
				}
				else if (y >= 400) {
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
	playerData.moveDir = 0;

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