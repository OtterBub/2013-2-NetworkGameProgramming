#include "Player.h"
#include <cmath>

void Player::Draw() 
{
	static int CHAR_SIZE = 10;
	Object3D::Draw();

	glTranslatef(0, (CHAR_SIZE/2) + 1, 0);
	
	glRotatef(m_angle, 0, 1, 0);

	// Body
	glBegin( GL_POLYGON );	// Àü¸é
		glColor3f(1, 1, 0.5);
		glVertex3f( 5, 7, 5 );
		glColor3f( 1, 0, 0 );
		glVertex3f( -5, 7, 5 );
		glColor3f( 0, 1, 0 );
		glVertex3f( -5, -3, 5 );
		glColor3f( 0, 0, 1 );
		glVertex3f( 5, -3, 5 );
	glEnd();
	glBegin( GL_POLYGON );	// ÁÂÃø¸é
		glColor3f( 1, 0, 0 );
		glVertex3f( -5, 7, 5 );
		glColor3f( 0, 0, 1 );
		glVertex3f( -5, 7, -5 );
		glColor3f( 1, 1, 0.5 );
		glVertex3f( -5, -3, -5 );
		glColor3f( 0, 1, 0 );
		glVertex3f( -5, -3, 5 );
	glEnd();
	glBegin( GL_POLYGON );	// µÞ¸é
		glColor3f( 0, 0, 1 );
		glVertex3f( -5, 7, -5 );
		glColor3f( 1, 0, 0 );
		glVertex3f( 5, 7, -5 );
		glColor3f( 0, 1, 0 );
		glVertex3f( 5, -3, -5 );
		glColor3f( 1, 1, 0.5 );
		glVertex3f( -5, -3, -5 );
	glEnd();
	glBegin( GL_POLYGON );	// ¿ìÃø¸é
		glColor3f( 1, 0, 0 );
		glVertex3f( 5, 7, -5 );
		glColor3f( 1, 1, 0.5 );
		glVertex3f( 5, 7, 5 );
		glColor3f( 0, 0, 1 );
		glVertex3f( 5, -3, 5 );
		glColor3f( 0, 1, 0 );
		glVertex3f( 5, -3, -5 );
	glEnd();
	glBegin( GL_POLYGON );	// À­¸é
		glColor3f( 1, 0, 0 );
		glVertex3f( 5, 7, -5 );
		glColor3f( 0, 0, 1 );
		glVertex3f( -5, 7, -5 );
		glColor3f( 1, 0, 0 );
		glVertex3f( -5, 7, 5 );
		glColor3f(1, 1, 0.5);
		glVertex3f( 5, 7, 5 );
	glEnd();
	glBegin( GL_POLYGON );	// ¹Ø¸é
		glColor3f( 1, 0, 0 );
		glVertex3f( 5, -3, -5 );
		glColor3f( 0, 0, 1 );
		glVertex3f( -5, -3, -5 );
		glColor3f( 1, 0, 0 );
		glVertex3f( -5, -3, 5 );
		glColor3f(1, 1, 0.5);
		glVertex3f( 5, -3, 5 );
	glEnd();

	// Head
	glBegin( GL_POLYGON );	// Àü¸é: À§
		glColor3f( 1, 1, 1 );
		glVertex3f( 2, 12, 3 );
		glVertex3f( -2, 12, 3 );
		glVertex3f( -2, 10, 4 );
		glVertex3f( 2, 10, 4 );
	glEnd();
	glBegin( GL_POLYGON );	// Àü¸é: ¾Æ·¡
		glColor3f( 0, 1, 0 );
		glVertex3f( 2, 10, 4 );
		glVertex3f( -2, 10, 4 );
		glVertex3f( -2, 7, 3 );
		glVertex3f( 2, 7, 3 );
	glEnd();
	glBegin( GL_POLYGON );// ÁÂ¸é
		glColor3f( 1, 0, 1 );
		glVertex3f( -2, 12, 3 );
		glVertex3f( -2, 12, -2 );
		glVertex3f( -2, 10, -1 );
		glVertex3f( -2, 7, -1 );
		glVertex3f( -2, 7, 3 );
		glVertex3f( -2, 10, 4 );
	glEnd();
	glBegin( GL_POLYGON );//µÞ¸é
		glColor3f( 1, 1, 0 );
		glVertex3f( -2, 12, -2 );
		glVertex3f( 2, 12, -2 );
		glVertex3f( 2, 10, -1 );
		glVertex3f( 2, 7, -1 );
		glVertex3f( -2, 7, -1 );
		glVertex3f( -2, 10, -1 );
	glEnd();
	glBegin( GL_POLYGON );// ¿ìÃø¸é
		glColor3f( 0, 1, 1 );
		glVertex3f( 2, 12, -2 );
		glVertex3f( 2, 12, 3 );
		glVertex3f( 2, 10, 4 );
		glVertex3f( 2, 7, 3 );
		glVertex3f( 2, 7, -1 );
		glVertex3f( 2, 10, -1 );
	glEnd();
	glBegin( GL_POLYGON );
		glColor3f( 0.5, 0.5, 0.5 );
		glVertex3f( 2, 12, -2 );
		glVertex3f( -2, 12, -2 );
		glVertex3f( -2, 12, 3 );
		glVertex3f( 2, 12, 3 );
	glEnd();

	switch(m_animationNum) {
	case 1:
		break;

	case 2:
		break;

	case 3:
		break;

	default:
		break;
	}

	char temp[100];
	sprintf(temp, "ID : %d", IdNum);
	drawStrokeText(temp, 0, 10, 0, 0.05);

	glPopMatrix();


	// new object
	if(m_attackState){
		glPushMatrix();
		{
			glColor3f(1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Max.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Min.z);
			glEnd();	

			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Min.z);
			glEnd();	

			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Min.z);
			glEnd();	

			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glEnd();

			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Max.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Min.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Max.y, m_CollisionBox.Min.z);
			glEnd();	

			glBegin(GL_LINE_LOOP);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Max.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Max.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Min.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glVertex3f(m_CollisionBox.Max.x, m_CollisionBox.Min.y, m_CollisionBox.Max.z);
			glEnd();
		}
		glPopMatrix();
	}
}

void Player::Update()
{
	if(m_attackState){
		if(500 < clock() - attackStartTime){
			m_attackState = false;
			m_atkDir = 0;
		}
	}
	ObjectControlPosition(m_velocity.x, m_velocity.y, m_velocity.z);
	m_angle = (atan2(m_velocity.x, m_velocity.z) * 180) / 3.14;
	m_velocity.x /= 1.05f;
	m_velocity.y /= 1.05f;
	m_velocity.z /= 1.05f;

	m_CollisionBox.Max.x = point.x + 5;
	m_CollisionBox.Max.y = point.y + 10;
	m_CollisionBox.Max.z = point.z + 5;

	m_CollisionBox.Min.x = point.x - 5;
	m_CollisionBox.Min.y = point.y - 10;
	m_CollisionBox.Min.z = point.z - 5;
}

void Player::Attack()
{
	static int width = 15, length = 15;
	if(!m_attackState)	{
		m_CollisionBox.Max.x = point.x + length;
		m_CollisionBox.Max.y = point.y + 5;
		m_CollisionBox.Max.z = point.z + width;

		m_CollisionBox.Min.x = point.x + 5;
		m_CollisionBox.Min.y = 1;
		m_CollisionBox.Min.z = point.z - width;

		m_attackState = true;
		attackStartTime = clock();
	}
}

void Player::Attack(int dir)
{
	m_CollisionBox.Max = point;
	m_CollisionBox.Min = point;
	
	static int width = 5, length = 20;
	if(!m_attackState)	{
		m_atkDir = dir;
		switch(m_atkDir)
		{
		case RIGHT:
			m_CollisionBox.Max.x += length;
			m_CollisionBox.Min.x += 5;
					
			m_CollisionBox.Max.y = 5;
			m_CollisionBox.Min.y = 1;

			m_CollisionBox.Max.z += width;
			m_CollisionBox.Min.z += -width;
			break;

		case LEFT:
			m_CollisionBox.Max.x += -5;			
			m_CollisionBox.Min.x += -length;

			m_CollisionBox.Max.y = 5;
			m_CollisionBox.Min.y = 1;

			m_CollisionBox.Max.z += width;
			m_CollisionBox.Min.z += -width;
			break;

		case UP:
			m_CollisionBox.Max.x += width;			
			m_CollisionBox.Min.x += -width;

			m_CollisionBox.Max.y = 5;
			m_CollisionBox.Min.y = 1;

			m_CollisionBox.Max.z += -5;
			m_CollisionBox.Min.z += -length;
			break;

		case DOWN:
			m_CollisionBox.Max.x += width;			
			m_CollisionBox.Min.x += -width;

			m_CollisionBox.Max.y = 5;
			m_CollisionBox.Min.y = 1;

			m_CollisionBox.Max.z += length;
			m_CollisionBox.Min.z += 5;
			break;

		default:
			break;
		}

		m_attackState = true;
		attackStartTime = clock();
	}
}