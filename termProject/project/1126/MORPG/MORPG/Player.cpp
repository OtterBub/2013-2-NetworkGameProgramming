#include "Player.h"

void Player::Draw() 
{
	static int CHAR_SIZE = 10;
	Object3D::Draw();
	glTranslatef(0, (CHAR_SIZE/2) + 1, 0);
	glColor3f(1, 1, 0.5);
	glutSolidCube(CHAR_SIZE);

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
		if(500 < clock() - attackStartTime)
			m_attackState = false;
	}
}

void Player::Attack()
{
	static int width = 10, length = 15;
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
		switch(dir)
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