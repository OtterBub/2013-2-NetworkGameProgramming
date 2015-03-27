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

struct WorldData {
	int PlayerNum;
	_point OtherUserPos[10];
	int MonsterNum;
	_point MonsterPos[10];
};

struct PlayerDate {
	_point UserPosition;
	int attackDir;
};

#endif // PACKETSTRUCT_H