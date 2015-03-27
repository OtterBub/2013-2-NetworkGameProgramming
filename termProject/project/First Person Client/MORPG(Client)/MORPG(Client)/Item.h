#ifndef ITEM_H
#define ITEM_H

#include "Object3D.h"
#include "FMS.h"
#include "Util.h"

#include <list>

enum ItemType{
	HEAL
};

class Item : public Object3D {
private:
	int itemType;

public:
	Item()
	{ 
		type = ITEM; 
		itemType = rand() % 1;
	}

	Item(float _x, float _y, float _z) : 
		Object3D(_x, _y, _z)
	{
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
		type = ITEM;
		itemType = rand() % 1;
	}

	Item(float _x, float _y, float _z, int setType) : 
		Object3D(_x, _y, _z)
	{
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
		type = ITEM;
		itemType = setType;
	}

	Item(float _x, float _y, float _z, float _scale) : 
		Object3D(_x, _y, _z, _scale)
	{
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
		type = ITEM;
		itemType = rand() % 1;
	}

	int getItemType() { return itemType; }

	void Draw();
	void Update();
	
};

#endif // ITEM_H