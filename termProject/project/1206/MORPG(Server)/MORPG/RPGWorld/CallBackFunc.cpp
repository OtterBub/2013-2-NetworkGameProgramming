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
}

GLvoid drawScene ( GLvoid )
{
	init();

	//glClearColor( 0.0f, 0.3f, 0.3f, 1.0f ); (Blue Black)
	glClearColor( g_r, g_g, g_b, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	_point CharCenter = myChar->GetPoint();

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

GLvoid timerFunc(int val)
{
	WORLD->Update();

	glutPostRedisplay();
	glutTimerFunc( 16, timerFunc, 1 );
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