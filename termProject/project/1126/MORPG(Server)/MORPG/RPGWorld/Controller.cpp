#include "Controller.h"

#define CAMERA_EYE_SPEED 10

GLvoid keyBoardFunc(unsigned char key, int x, int y)
{

	static int CHAR_SPEED = 3;
	switch(key)
	{
	case '1':
		break;
	case 'a':
		myChar->ObjectControlPosition(-CHAR_SPEED, 0, 0);
		break;
	case 'd':
		myChar->ObjectControlPosition(+CHAR_SPEED, 0, 0);
		break;
	case 'w':
		myChar->ObjectControlPosition(0, 0, -CHAR_SPEED);
		break;
	case 's':
		myChar->ObjectControlPosition(0, 0, CHAR_SPEED);
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
	printf("mouse x : %d, y : %d\n", x, y);

	if(state == GLUT_DOWN) {
		switch (button)	{
		case GLUT_LEFT_BUTTON:
			{
				if(x >= 500)
					WORLD->Attack(Player::RIGHT);
				else if (x <= 300)
					WORLD->Attack(Player::LEFT);
				else if (y <= 200)
					WORLD->Attack(Player::UP);
				else if (y >= 400)
					WORLD->Attack(Player::DOWN);
			}
			//WORLD->Attack();
			break;

		case GLUT_RIGHT_BUTTON:
			break;		

		default:
			break;
		}
	}
}

GLvoid mouseMove(int x, int y)
{
	static int pre_x = 0, pre_y = 0;

	//printf("%d, %d \n", x - pre_x, y - pre_y);

	pre_x = x;
	pre_y = y;
}