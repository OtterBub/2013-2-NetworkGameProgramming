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

#define HEIGHT 600
#define WIDTH 800

#define WORLD RPGWorld::Instance()

GLvoid drawScene( GLvoid );
GLvoid reshape( int w, int h );
GLvoid mouse(int button, int state, int x, int y);
GLvoid timerFunc(int val);
GLvoid mouseMove(int x, int y);
GLvoid keyBoardFunc(unsigned char key, int x, int y);
GLvoid specialKeyBoardFunc(int key, int x, int y);
GLvoid MenuFuntion(int value);


#endif //__CALLBACKFUC_H__