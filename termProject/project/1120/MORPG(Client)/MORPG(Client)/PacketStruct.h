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
	_point UserPosition;
	int attackDir;
};

struct PlayerDate {
	_point UserPosition;
	int attackDir;
};

#endif // PACKETSTRUCT_H