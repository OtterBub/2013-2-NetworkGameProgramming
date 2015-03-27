#include "RPGWorld.h"
#include "Client.h"

void RPGWorld::Draw() 
{
	World3D::Draw();
}

void RPGWorld::Update()
{
	World3D::Update();

	int PlayerIndex = 0;
	int MonsterIndex = 0;
	int ObjectIndex = 0;
#ifdef CLIENT
	
	for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end(); it++){

		switch((*it)->GetType())
		{
		case PLAYER:
			{
				_point temp = World.playerInfo[PlayerIndex].UserPosition;
				_point tmpeVel = World.playerInfo[PlayerIndex].UserVelocity;
				int atkDir = World.playerInfo[PlayerIndex].attackDir;
				int playerID = World.playerInfo[PlayerIndex].userID;
				int playerHP = World.playerInfo[PlayerIndex].userHP;
				bool atkState = World.playerInfo[PlayerIndex].atkState;



				(*it)->SetObjectPosition(temp.x, temp.y, temp.z);
				(*it)->setVelocity(tmpeVel.x, tmpeVel.y, tmpeVel.z);
				(*it)->SetID(playerID);
				dynamic_cast<Player*>(*it)->SetHP(playerHP);
				dynamic_cast<Player*>(*it)->SetAttackDir(atkDir);
				dynamic_cast<Player*>(*it)->SetAttackState(atkState);
				
				
				//printf("index : %d (%.2f, %.2f, %.2f)\n", PlayerIndex, temp.x, temp.y, temp.z);
				World.playerInfo[PlayerIndex].UserPosition = (*it)->GetPoint();
				if(World.playerInfo[PlayerIndex].userID == m_MyChar.GetID()){
					m_MyChar.SetObjectPosition(temp.x, temp.y, temp.z);
					m_MyChar.setVelocity(tmpeVel.x, tmpeVel.y, tmpeVel.z);
					m_MyChar.SetHP(playerHP);
				}
				PlayerIndex++;
			}
			break;

		case MONSTER:
			{
				_point temp = World.monsterInfo[MonsterIndex].MobPosition;
				int mobHp = World.monsterInfo[MonsterIndex].MonsterHP;
				(*it)->SetObjectPosition(temp.x, temp.y, temp.z);
				dynamic_cast<Monster*>(*it)->SetHP(mobHp);
				//printf("index : %d (%.2f, %.2f, %.2f)\n", MonsterIndex, temp.x, temp.y, temp.z);
				World.monsterInfo[MonsterIndex++].MobPosition = (*it)->GetPoint();

			}
			break;
		case PYRAMID:
			{
				float angle = 0;
				angle = World.objectInfo[ObjectIndex].angle;
				dynamic_cast<Pyramid*>(*it)->setRotate(angle);

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
	
}

void RPGWorld::Attack(int dir)
{
	
}

void RPGWorld::Attack(int dir, Object3D* obj)
{
	
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
			int ID = _data.playerInfo[i].userID;
			AddObject(new Player(temp.x, temp.y, temp.z, ID));
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
	if(World.ItemNum != _data.ItemNum){
		for(std::vector<Object3D*>::iterator it = m_obj.begin(); it != m_obj.end();){
			if((*it)->GetType() == ITEM){
				it = m_obj.erase(it);
			} else {
				it++;
			}
		}

		for(int i = 0; i < _data.ItemNum; i++){
			_point temp = _data.itemInfo[i].ItemPosition;
			int type = _data.itemInfo[i].type;
			AddObject(new Item(temp.x, temp.y, temp.z, type));
		}
	}

	World = _data;
}