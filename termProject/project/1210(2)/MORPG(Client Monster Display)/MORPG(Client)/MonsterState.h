#ifndef MONSTER_STATE_H
#define MONSTER_STATE_H

#include "State.h"
#include "Monster.h"

class MonsterMoveRand : public State<Monster> {
private:
	MonsterMoveRand();
	~MonsterMoveRand();
	MonsterMoveRand(const MonsterMoveRand&);
	MonsterMoveRand& operator=(const MonsterMoveRand&);

public:
	void Enter(Monster* obj);
	void Execute(Monster* obj);
	void Exit(Monster* obj);
	
};


#endif // MONSTER_STATE_H