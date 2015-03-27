#ifndef MONSTER_H
#define MONSTER_H

#include "Object3D.h"
#include "FMS.h"
#include "Util.h"

enum MonsterState {
	MOB_RIGHT = 10,
	MOB_LEFT,
	MOB_UP,
	MOB_DOWN,
	MOB_STOP
};

class Monster : public Object3D {
private:
	FMS<Monster>* m_fms;
	
	int m_hp;
	int m_state;

	time_t m_StateChangeClock;

public:
	Monster() : 
		m_fms(new FMS<Monster>(this))
	{ type = MONSTER; }

	Monster(float _x, float _y, float _z) : 
		Object3D(_x, _y, _z),
		m_fms(new FMS<Monster>(this))
	{
		type = MONSTER;
		m_hp = 100;
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
		m_StateChangeClock = clock();
	}

	Monster(float _x, float _y, float _z, float _scale) : 
		Object3D(_x, _y, _z, _scale),
		m_fms(new FMS<Monster>(this))
	{
		type = MONSTER;
		m_hp = 100;
		m_CollisionBox.Max = point;
		m_CollisionBox.Min = point;
		m_StateChangeClock = clock();
	}

	void Draw();
	void Update();

	void Damage(int _Dmg) { m_hp = m_hp - _Dmg; }

	FMS<Monster>* GetFMS() { return m_fms; }
	int GetHP() { return m_hp; }
	
};

#endif // MONSTER_H