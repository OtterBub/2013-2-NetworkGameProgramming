#include "Monster.h"

void Monster::Draw() {
	Object3D::Draw();
	glColor3f(1, 0, 0);
	glutWireSphere(10, 10, 10);

	char temp[100];
	sprintf(temp, "HP : %d", m_hp);
	drawStrokeText(temp, 0, 10, 0, 0.05);

	glPopMatrix();

	// new object
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
void Monster::Update() {

	static float MONSTER_SPEED = 0.1;

	switch(m_state)
	{	
	
	case MonsterState::MOB_RIGHT :
		ObjectControlPosition(MONSTER_SPEED, 0, 0);
		break;
	
	case MonsterState::MOB_LEFT :
		ObjectControlPosition(-MONSTER_SPEED, 0, 0);
		break;
	
	case MonsterState::MOB_UP :
		ObjectControlPosition(0, 0, -MONSTER_SPEED);
		break;
	
	case MonsterState::MOB_DOWN :
		ObjectControlPosition(0, 0, MONSTER_SPEED);
		break;

	case MonsterState::MOB_STOP :
		break;

	default:
		break;
	}

	if( 1000 < clock() - m_StateChangeClock){
		m_StateChangeClock = clock();

		m_state = (10 + (rand() % 5));
	}


	m_CollisionBox.Max.x = point.x + 10;
	m_CollisionBox.Max.y = point.y + 10;
	m_CollisionBox.Max.z = point.z + 10;

	m_CollisionBox.Min.x = point.x - 10;
	m_CollisionBox.Min.y = point.y - 10;
	m_CollisionBox.Min.z = point.z - 10;
}

