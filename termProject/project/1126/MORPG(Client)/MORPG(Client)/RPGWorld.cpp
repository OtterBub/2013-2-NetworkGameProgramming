#include "RPGWorld.h"

void RPGWorld::Draw() 
{
	World3D::Draw();
}

void RPGWorld::Update()
{
	World3D::Update();

	int index = 0;

	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		if((*it)->GetType() == PLAYER){
			_point temp = World.playerInfo[index].UserPosition;
			(*it)->SetObjectPosition(temp.x, temp.y, temp.z);
			printf("index : %d (%.2f, %.2f, %.2f)\n", index, temp.x, temp.y, temp.z);
			World.playerInfo[index++].UserPosition = (*it)->GetPoint();
		}	
	}

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
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		if((*it)->GetType() == PLAYER)
			PlayerNum++;
	}
	World.PlayerNum = PlayerNum;
	return obj;
}

void RPGWorld::DeleteObject(Object3D* obj)
{
	World3D::DeleteObject(obj);
	PlayerNum = 0;
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		if((*it)->GetType() == PLAYER)
			PlayerNum++;
	}
	World.PlayerNum = PlayerNum;
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
	World = _data;
}