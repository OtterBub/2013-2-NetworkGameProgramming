#ifndef RPG_WORLD_H
#define RPG_WORLD_H

#include "Player.h"
#include "Monster.h"
#include "Pyramid.h"

#include "World3D.h"
#include "Util.h"
#include "PacketStruct.h"

class RPGWorld : public World3D {
private:
	Player m_MyChar;
	int PlayerNum;
	int MonsterNum;

	bool init;
	
	RPGWorld() { 
		m_update = true; 
		init = true;

		World.MonsterNum = 0;
		World.PlayerNum = 0;
		World.ObjectNum = 0;
		
		memset(&World, 0, sizeof(World));
	}

	~RPGWorld(){ 
		m_obj.clear(); 		
	}
	RPGWorld(const RPGWorld&);
	RPGWorld& operator=(const RPGWorld&);

	WorldData World;

public:
	static RPGWorld* Instance() {
		static RPGWorld instance;
		return &instance;
	}
	void Draw();
	void Update();

	Object3D* AddObject(Object3D* obj);
	void DeleteObject(Object3D* obj);
	void Attack();
	void Attack(int dir);
	void Attack(int dir, Object3D* obj);
	void SetWorldData(WorldData _data);
	void SetMyCharID(int num) { m_MyChar.SetID(num); }

	Player* GetMyChar() { return &m_MyChar; }
	int GetPlayerNum() { return PlayerNum; }
	WorldData GetWorldData() { return World; }
};

#endif // RPG_WORLD_H