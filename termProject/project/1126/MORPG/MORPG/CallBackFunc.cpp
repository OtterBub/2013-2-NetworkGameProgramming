#include "CallBackFunc.h"
#define GROUND_SIZE 300.0f

int g_Shage = 0; // Wire
int g_x;
int g_y;
int g_z;

double g_eye_x = 0, g_eye_y = 100, g_eye_z = 100;
double g_center_x, g_center_y, g_center_z = -1;
double g_up_x, g_up_y, g_up_z;
double g_w, g_h;

double g_r = 0.3, g_g = 0.3, g_b = 0.5;

bool g_init = true;

Player* myChar = WORLD->GetMyChar();

// animation
inline void init(){
	if(!g_init)
		return;
	else
		g_init = false;

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	WORLD->AddObject(new Pyramid(250, 1, 250));
	WORLD->AddObject(new Pyramid(250, 1, -250));
	WORLD->AddObject(new Pyramid(-250, 1, -250));
	WORLD->AddObject(new Pyramid(-250, 1, 250, 2));
	WORLD->AddObject(new Pyramid(0, 0, -30, 0.5));
	WORLD->AddObject(new Monster(50, 0, 0));
	WORLD->AddObject(new Monster(70, 0, 0));
	WORLD->AddObject(new Monster(50, 0, 30));

	WORLD->AddObject(myChar);
}

GLvoid drawScene ( GLvoid )
{
	init();

	//glClearColor( 0.0f, 0.3f, 0.3f, 1.0f ); (Blue Black)
	glClearColor( g_r, g_g, g_b, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	_point CharCenter = myChar->GetPoint();

	char temp[100];
	_point tempPoint = RPGWorld::Instance()->GetMyChar()->GetPoint();

	// User Interface
	glPushMatrix();
	{
		gluLookAt(0, 0, 1,
			0, 0, 0,
			0.0, 1.0, 0.0);
		char temp[100];
		sprintf(temp, "HP : %d", myChar->GetHP());
		drawStrokeText(temp, 0, 0.3, 0, 0.0005);

		sprintf(temp, "Level : %d", myChar->GetLevel());
		drawStrokeText(temp, 0, 0.4, 0, 0.0005);

		sprintf(temp, "(%.2f, %.2f, %.2f)", tempPoint.x, tempPoint.y, tempPoint.z);
		drawStrokeText(temp, -0.2, 0.5, 0, 0.0005);
	}
	glPopMatrix();

	// Camera Control
	gluLookAt(g_eye_x + CharCenter.x, g_eye_y + CharCenter.y, g_eye_z + CharCenter.z,
		CharCenter.x, CharCenter.y, CharCenter.z,
		0.0, 1.0, 0.0);

	// Global View;
	//glTranslatef( g_eye_x, g_eye_y, g_eye_z );
	glTranslatef( 0, 0, 0 );
	glRotatef(g_x, 1.0, 0.0, 0.0);
	glRotatef(g_y, 0.0, 1.0, 0.0);
	glRotatef(g_z, 0.0, 0.0, 1.0);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-GROUND_SIZE, 0.0, -GROUND_SIZE);
	glVertex3f(-GROUND_SIZE, 0.0, GROUND_SIZE);
	glVertex3f(GROUND_SIZE, 0.0, GROUND_SIZE);
	glVertex3f(GROUND_SIZE, 0.0, -GROUND_SIZE);	
	glEnd();
	
	

	// Draw 3DWorld
	WORLD->Draw();
	glLoadIdentity();
	glutSwapBuffers();
}

GLvoid reshape ( int w, int h )
{
	g_w = w;
	g_h = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(0, w, 0, h, -h, h);
	gluPerspective(60, w/h, 1.0, 2000.0f);
	/*gluLookAt(0.0, 0.0, 500.0,
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0);*/

	glMatrixMode(GL_MODELVIEW);
}


GLvoid mouse(int button, int state, int x, int y)
{
	int r_y = HEIGHT - y;
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
	int r_y = HEIGHT - y;
	static int pre_x = 0, pre_y = 0;

	//printf("%d, %d \n", x - pre_x, y - pre_y);

	pre_x = x;
	pre_y = y;
}

GLvoid timerFunc(int val)
{
	WORLD->Update();

	glutPostRedisplay();
	glutTimerFunc( 16, timerFunc, 1 );
}

#define CAMERA_EYE_SPEED 10

GLvoid keyBoardFunc(unsigned char key, int x, int y)
{

	static int CHAR_SPEED = 3;
	switch(key)
	{
	case '1':
		WORLD->AddObject(new Pyramid(rand()%600 - 300, 0, rand()%600 - 300));
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

GLvoid MenuFuntion(int value)
{
	switch(value){
	case 1:
		glEnable(GL_DEPTH_TEST);
		break;
	case 2:
		glDisable(GL_DEPTH_TEST);
		break;

	case 11:
		glEnable(GL_CULL_FACE);
		break;
	case 22:
		glDisable(GL_CULL_FACE);
		break;

	case 111:
		glShadeModel(GL_FLAT);
		break;
	case 222:
		glShadeModel(GL_SMOOTH);
		break;

	default:
		break;
	}
}