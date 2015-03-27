#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

enum attackDir {
		RIGHT = 1000,
		LEFT,
		UP,
		DOWN
	};

enum moveDir {
		MOVE_RIGHT = 2000,
		MOVE_LEFT ,
		MOVE_UP ,
		MOVE_DOWN
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

struct WorldData {
	int PlayerNum;
	PlayerData playerInfo[10];
	int MonsterNum;
	MonsterData monsterInfo[20];
};

#endif // PACKETSTRUCT_H