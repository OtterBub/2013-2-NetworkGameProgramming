#ifndef __CALLBACKFUC_H__
#define __CALLBACKFUC_H__

#include <iostream>
#include <vector>
#include <algorithm>

#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdlib>
#include <ctime>

#include "World3D.h"
#include "RPGWorld.h"

#include "Pyramid.h"
#include "Player.h"
#include "Item.h"

#define HEIGHT 600
#define WIDTH 800

#define WORLD RPGWorld::Instance()

GLvoid drawScene( GLvoid );
GLvoid reshape( int w, int h );
GLvoid mouse(int button, int state, int x, int y);

GLvoid timerFunc(int val);

GLvoid MenuFuntion(int value);

extern int g_Shage; // Wire
extern int g_x;
extern int g_y;
extern int g_z;

extern double g_eye_x, g_eye_y, g_eye_z;
extern double g_center_x, g_center_y, g_center_z;
extern double g_up_x, g_up_y, g_up_z;
extern double g_w, g_h;

extern double g_r, g_g, g_b;

extern bool g_init;

extern Player* myChar;


#endif //__CALLBACKFUC_H__