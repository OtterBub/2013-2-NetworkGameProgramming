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

	//World3D::Update();

	int PlayerIndex = 0;
	int MonsterIndex = 0;
	int ObjectIndex = 0;
	int ItemIndex = 0;

	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end();){
		(*it)->Update();
		switch((*it)->GetType())
		{
		case PLAYER:
			World.playerInfo[PlayerIndex].UserPosition = (*it)->GetPoint();
			World.playerInfo[PlayerIndex].UserVelocity = (*it)->GetVelocity();
			World.playerInfo[PlayerIndex].userID = (*it)->getID();
			World.playerInfo[PlayerIndex].userHP = dynamic_cast<Player*>(*it)->GetHP();
			World.playerInfo[PlayerIndex].atkState = dynamic_cast<Player*>(*it)->GetAttackState();
			World.playerInfo[PlayerIndex].attackDir = dynamic_cast<Player*>(*it)->GetAttackDir();
			PlayerIndex++;
			break;
		case MONSTER:
			World.monsterInfo[MonsterIndex].MobPosition = (*it)->GetPoint();
			World.monsterInfo[MonsterIndex].MonsterHP = dynamic_cast<Monster*>(*it)->GetHP();
			MonsterIndex++;
			break;
		case PYRAMID:
			{
				float angle = dynamic_cast<Pyramid*>(*it)->getRotate();
				World.objectInfo[ObjectIndex].angle = angle;
				ObjectIndex++;
			}
			break;
		case ITEM:
			{
				World.itemInfo[ItemIndex].ItemPosition = (*it)->GetPoint();
				World.itemInfo[ItemIndex].type = dynamic_cast<Item*>(*it)->getItemType();
				ItemIndex++;
				bool collision = false;
				for(std::vector<Object3D*>::iterator tempit = m_obj.begin(); tempit != m_obj.end(); tempit++){
					if((*tempit)->GetType() == PLAYER){
						if(CollisionCheck((*tempit)->GetCollisionBox(), (*it)->GetCollisionBox())) {
							int itemType = dynamic_cast<Item*>(*it)->getItemType();
							switch(itemType) {
							case ItemType::HEAL :
								{
									int currentHP = dynamic_cast<Player*>(*tempit)->GetHP();
									dynamic_cast<Player*>(*tempit)->SetHP(currentHP + 10);
								}
								break;
							default:
								break;
							}
							collision = true;
							it = m_obj.erase(it);
							break;
						}
					}
				}
				if(collision == true)
					continue;
			}
			break;
		}	
		it++;
	}
	World.PlayerNum = PlayerIndex;
	World.MonsterNum = MonsterIndex;
	World.ObjectNum = ObjectIndex;
	World.ItemNum= ItemIndex;


	static int CreateClock = clock();

	if(2000 < clock() - CreateClock) {
		CreateClock = clock();
		if(World.ItemNum < 10) {
			AddObject(new Item(((rand()%600) - 300), 0, ((rand()%600) - 300), ItemType::HEAL));
		}

		if(World.MonsterNum < 20) {
			AddObject(new Monster(((rand()%600) - 300), 0, ((rand()%600) - 300)));
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
						DeleteObject((*it));
						break;
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
						DeleteObject((*it));
						break;
					}
				}
			}
			it++;
		}
	}
}

