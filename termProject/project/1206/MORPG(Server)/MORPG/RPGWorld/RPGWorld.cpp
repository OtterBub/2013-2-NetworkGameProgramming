#include "RPGWorld.h"
#include "ServerThread.h"

void RPGWorld::Draw() 
{	
	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);
	
	World3D::Draw();

	SetEvent(hLogInOutEvent);
}

void RPGWorld::Update()
{
	WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);

	World3D::Update();

	int PlayerIndex = 0;
	int MonsterIndex = 0;

	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){
		switch((*it)->GetType())
		{
		case PLAYER:
			World.playerInfo[PlayerIndex].UserPosition = (*it)->GetPoint();
			World.playerInfo[PlayerIndex].UserVelocity = (*it)->GetVelocity();
			World.playerInfo[PlayerIndex].userID = (*it)->getID();
			PlayerIndex++;
			break;
		case MONSTER:
			World.monsterInfo[MonsterIndex++].MobPosition= (*it)->GetPoint();
			break;
		}
			
	}
	SetEvent(hLogInOutEvent);
	
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

