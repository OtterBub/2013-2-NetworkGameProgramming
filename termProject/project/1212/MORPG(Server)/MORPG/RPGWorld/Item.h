#ifndef ITEM_H
#define ITEM_H

#include "Object3D.h"
#include "FMS.h"
#include "Util.h"

#include <list>

enum ItemType{
	HEAL
};

class Monster : public Object3D {
private:
	int itemType;

public:
	Monster() : 
		m_fms(new FMS<Monster>(this))
	{ type = MONSTER; }

	Monster(float _x, float _y, float _z) : 
		Object3D(_x, _y, _z),
		m_fms(new FMS<Monster>(this))
	{
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
	}

	Monster(float _x, float _y, float _z, float _scale) : 
		Object3D(_x, _y, _z, _scale),
		m_fms(new FMS<Monster>(this))
	{
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
	}

	void Draw();
	void Update();
	
};

#endif // ITEM_H