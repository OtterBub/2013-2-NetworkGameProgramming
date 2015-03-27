#ifndef RPG_WORLD_H
#define RPG_WORLD_H

#include "Player.h"
#include "Monster.h"

#include "World3D.h"
#include "Util.h"

class RPGWorld : public World3D {
private:
	Player* m_MyChar;
	
	RPGWorld() { 
		m_update = true; 
		m_MyChar = new Player();
		AddObject(m_MyChar);
	}	

	~RPGWorld(){ m_obj.clear(); }
	RPGWorld(const RPGWorld&);
	RPGWorld& operator=(const RPGWorld&);

public:
	static RPGWorld* Instance() {
		static RPGWorld instance;
		return &instance;
	}
	void Draw();
	void Update();
	Player* GetMyChar() { return m_MyChar; }

	void Attack();

	void Attack(int dir);
};

#endif // RPG_WORLD_H