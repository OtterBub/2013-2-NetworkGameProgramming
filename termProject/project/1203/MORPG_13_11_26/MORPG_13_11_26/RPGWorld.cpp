#include "RPGWorld.h"

void RPGWorld::Draw() 
{
	World3D::Draw();
}

void RPGWorld::Update()
{
	World3D::Update();

	
	int PlayerIndex = 0;
	int MonsterIndex = 0;
#ifdef CLIENT
	
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){

		switch((*it)->GetType())
		{
		case PLAYER:
			{
				_point temp = World.playerInfo[PlayerIndex].UserPosition;
				(*it)->SetObjectPosition(temp.x, temp.y, temp.z);
				printf("index : %d (%.2f, %.2f, %.2f)\n", PlayerIndex, temp.x, temp.y, temp.z);
				World.playerInfo[PlayerIndex++].UserPosition = (*it)->GetPoint();
			}
			break;

		case MONSTER:
			{
				_point temp = World.monsterInfo[MonsterIndex].MobPosition;
				(*it)->SetObjectPosition(temp.x, temp.y, temp.z);
				printf("index : %d (%.2f, %.2f, %.2f)\n", MonsterIndex, temp.x, temp.y, temp.z);
				World.monsterInfo[MonsterIndex++].MobPosition = (*it)->GetPoint();
			}
			break;
		}
	}

#else

	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		if((*it)->GetType() == PLAYER){
			World.playerInfo[index++].UserPosition = (*it)->GetPoint();
		}
			
	}

#endif

	/*std::vector<Object3D*>::iterator it;

	for( it = m_obj.begin(); it != m_obj.end(); it++){
		if((*it)->GetType() == MONSTER){
			if(CollisionCheck(m_MyChar->GetCollisionBox(), (*it)->GetCollisionBox())){
				printf("Collision!!");
			}
		}
	}*/
}

Object3D* RPGWorld::AddObject(Object3D* obj)
{
	World3D::AddObject(obj);
	PlayerNum = 0;
	MonsterNum = 0;
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		switch((*it)->GetType())
		{
		case PLAYER:
			PlayerNum++;
			break;
		case MONSTER:
			MonsterNum++;
			break;
		}
	}

	World.PlayerNum = PlayerNum;
	World.MonsterNum = MonsterNum;

	return obj;
}

void RPGWorld::DeleteObject(Object3D* obj)
{
	World3D::DeleteObject(obj);
	PlayerNum = 0;
	MonsterNum = 0;
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		switch((*it)->GetType())
		{
		case PLAYER:
			PlayerNum++;
			break;
		case MONSTER:
			MonsterNum++;
			break;
		}
	}

	World.PlayerNum = PlayerNum;
	World.MonsterNum = MonsterNum;
}


void RPGWorld::Attack()
{
	if(!(m_MyChar->GetAttackState())){
		m_MyChar->Attack();
		std::vector<Object3D*>::iterator it;

		for( it = m_obj.begin(); it != m_obj.end(); ){
			if((*it)->GetType() == MONSTER){
				if(CollisionCheck(m_MyChar->GetCollisionBox(), (*it)->GetCollisionBox())){
					printf("Collision!!");
					((Monster*)*it)->Damage(m_MyChar->GetAttackStat());
					if(((Monster*)*it)->GetHP() <= 0){
						it = m_obj.erase(it);
						continue;
					}
				}
			}
			it++;
		}
	}
}

void RPGWorld::Attack(int dir)
{
	if(!(m_MyChar->GetAttackState())){
		m_MyChar->Attack(dir);
		std::vector<Object3D*>::iterator it;

		for( it = m_obj.begin(); it != m_obj.end(); ){
			if((*it)->GetType() == MONSTER){
				if(CollisionCheck(m_MyChar->GetCollisionBox(), (*it)->GetCollisionBox())){
					printf("Collision!!");
					((Monster*)*it)->Damage(m_MyChar->GetAttackStat());
					if(((Monster*)*it)->GetHP() <= 0){
						it = m_obj.erase(it);
						continue;
					}
				}
			}
			it++;
		}
	}
}

void RPGWorld::Attack(int dir, Object3D* obj)
{
	if(dir == NULL)
		return ;

	if(!((Player*)(obj))->GetAttackState()){
		((Player*)(obj))->Attack(dir);
		std::vector<Object3D*>::iterator it;

		for( it = m_obj.begin(); it != m_obj.end(); ){
			if((*it)->GetType() == MONSTER){
				if(CollisionCheck(((Player*)(obj))->GetCollisionBox(), (*it)->GetCollisionBox())){
					printf("Collision!!");
					((Monster*)*it)->Damage(((Player*)(obj))->GetAttackStat());
					if(((Monster*)*it)->GetHP() <= 0){
						it = m_obj.erase(it);
						continue;
					}
				}
			}
			it++;
		}
	}
}

void RPGWorld::SetWorldData(WorldData _data)
{
	if(World.PlayerNum != _data.PlayerNum){
		for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end();){
			if((*it)->GetType() == PLAYER){
				it = m_obj.erase(it);
			} else {
				it++;
			}
		}

		for(int i = 0; i < _data.PlayerNum; i++){
			_point temp = _data.playerInfo[i].UserPosition;
			AddObject(new Player(temp.x, temp.y, temp.z));
		}
	}

	if(World.MonsterNum != _data.MonsterNum){
		for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end();){
			if((*it)->GetType() == MONSTER){
				it = m_obj.erase(it);
			} else {
				it++;
			}
		}

		for(int i = 0; i < _data.MonsterNum; i++){
			_point temp = _data.monsterInfo[i].MobPosition;
			AddObject(new Monster(temp.x, temp.y, temp.z));
		}
	}

	World = _data;
}