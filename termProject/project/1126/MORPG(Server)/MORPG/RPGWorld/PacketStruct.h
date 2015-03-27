#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

enum attackDir {
		RIGHT = 1000,
		LEFT,
		UP,
		DOWN
	};

struct _point{
	float x, y, z;
};

struct PlayerData {
	_point UserPosition;
	int attackDir;
};

struct MonsterData {
	_point MobPosition;
};

struct WorldData {
	int PlayerNum;
	PlayerData playerInfo[10];
};

#endif // PACKETSTRUCT_H