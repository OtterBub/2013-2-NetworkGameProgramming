#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include <iostream>
#include <vector>

#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdlib>
#include <ctime>

#include "Util.h"

#define MONSTER	2000
#define PLAYER	2001

class Object3D {
protected:
	struct _point m_velocity;
	struct _point point;
	CollisionBox m_CollisionBox;
	float angle;
	float scale;
	int type;
	int IdNum;

public:
	Object3D(){point.x = 0; point.y = 0; point.z = 0; angle = 0; type = 0; scale = 1; IdNum = 0; m_velocity.x = 0; m_velocity.y = 0; m_velocity.z = 0;}
	Object3D(int _ID){point.x = 0; point.y = 0; point.z = 0; angle = 0; type = 0; scale = 1; IdNum = _ID; m_velocity.x = 0; m_velocity.y = 0; m_velocity.z = 0;}
	Object3D(float _x, float _y, float _z) {
		point.x = _x;
		point.y = _y;
		point.z = _z;
		angle = 0.0;
		type = 0;
		scale = 1;
		IdNum = 0;

		m_velocity.x = 0; m_velocity.y = 0; m_velocity.z = 0;
	}

	Object3D(float _x, float _y, float _z, float _scale) {
		point.x = _x;
		point.y = _y;
		point.z = _z;
		angle = 0.0;
		type = 0;
		scale = _scale;
		IdNum = 0;
		m_velocity.x = 0; m_velocity.y = 0; m_velocity.z = 0;
	}

	virtual ~Object3D() {};
	
	virtual void Draw();
	virtual void Update() = 0;
	virtual void AddFuntion() {}

	void setVelocity(float _x, float _y, float _z) { m_velocity.x = _x; m_velocity.y = _y; m_velocity.z = _z;}
	void addVelocity(float _x, float _y, float _z) { m_velocity.x += _x; m_velocity.y += _y; m_velocity.z += _z;}


	void ObjectControlPosition(float add_x, float add_y, float add_z);

	void ObjectControlAngle(float add_angle);

	void ObjectSeek(_point pos, float move);
	void ObjectFlee(_point pos, float move);

	void SetObjectPosition(float add_x, float add_y, float add_z);

	_point GetDist(_point pos);

	_point GetVelocity() { return m_velocity; }
	_point GetPoint() {return point;}
	int GetType() {return type;}
	CollisionBox GetCollisionBox() { return m_CollisionBox; }
	int getID() {return IdNum;}
};


#endif //__OBJECT3D_H__