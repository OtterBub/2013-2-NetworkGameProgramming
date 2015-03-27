#ifndef PLAYER_H
#define PLAYER_H

#include "Object3D.h"
#include "FMS.h"

class Player : public Object3D {
private:
	int m_hp;
	int m_level;
	int m_atk;

	bool m_attackState;
	int attackStartTime;

	int m_animationNum;
	
	FMS<Player>* m_fms;

public:
	enum attackDir {
		RIGHT = 1000,
		LEFT,
		UP,
		DOWN
	};

	Player() : 
		m_fms(new FMS<Player>(this)){ 
			m_hp = 100; 
			m_level = 1; 
			m_attackState = false; 
			m_atk = 10;
			type = 2001;
			m_animationNum = 0;
	}

	Player(int ID) : 
		Object3D(ID),
		m_fms(new FMS<Player>(this)){ 
			m_hp = 100; 
			m_level = 1; 
			m_attackState = false; 
			m_atk = 10; 
			type = 2001;
			m_animationNum = 0;
	}

	Player(float _x, float _y, float _z) : 
		Object3D(_x, _y, _z),
		m_fms(new FMS<Player>(this)){ 
			m_hp = 100; 
			m_level = 1; 
			m_attackState = true; 
			m_atk = 10; 
			type = 2001;
			m_animationNum = 0;
	}

	void Draw();
	void Update();
	void Attack();
	void Attack(int dir);
	
	FMS<Player>* GetFMS() { return m_fms; }
	int GetHP() { return m_hp; }
	int GetLevel() { return m_level; }
	bool GetAttackState() { return m_attackState; }
	int GetAttackStat() { return m_atk; }
	
};

#endif // PLAYER_H