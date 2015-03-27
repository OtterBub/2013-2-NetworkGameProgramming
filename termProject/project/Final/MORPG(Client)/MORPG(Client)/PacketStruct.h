#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

enum attackDir {
		RIGHT = 1000,
		LEFT,
		UP,
		DOWN
	};

enum moveDir {
	MOVE_RIGHT = 0x0001,
	MOVE_LEFT = 0x0002,
	MOVE_UP = 0x0004 ,
	MOVE_DOWN = 0x0008
};

struct _point{
	float x, y, z;
};

struct PlayerData {
	_point UserPosition;
	_point UserVelocity;
	int attackDir;
	int moveDir;
	int userHP;
	int userID;
	bool atkState;
};

struct MonsterData {
	_point MobPosition;
	int MonsterHP;
};

struct WorldObjectData {
	float angle;
};

struct ItemData {
	_point ItemPosition;
	int type;
};

struct WorldData {
	int PlayerNum;
	PlayerData playerInfo[10];
	int MonsterNum;
	MonsterData monsterInfo[20];
	int ObjectNum;
	WorldObjectData objectInfo[10];
	int ItemNum;
	ItemData itemInfo[10];
};

#endif // PACKETSTRUCT_H