#pragma once

#include "DBHandler.h"
#include "PlayerManager.h"

class databaseQuery
{
private:

	DBHandler* m_dbHandle;

	enum slot_panel
	{
		equip = 0,
		backpack = 1

	};

public:

	databaseQuery(DBHandler* dbhandle);
	databaseQuery();
	
	void Player_Inventory(std::list<itemManager::item>& BackPack, std::list<itemManager::item>& Equipment, int playerid);

	//item 가방에 있던걸 먹던가
	//밖에 있던걸 먹던가 
	void eat_Item(PlayerManager::Player *player, itemManager::item& item, bool player_has);

	//map에 있던걸 가방에 넣음
	void player_getItemtoBackPack(PlayerManager::Player* player, itemManager::item& item);

	//map에 있던걸 착용함
	void player_getItemtoEquip(PlayerManager::Player* player, itemManager::item& item);

	//가방에 있던걸 착용함
	void player_equipfromBackPack(PlayerManager::Player* player,itemManager::item& item);

	//착용한걸 가방에
	void player_equiptoBackPack(PlayerManager::Player* player, itemManager::item& item);

	//땅에다 버림
	void player_dropItemtoGround(PlayerManager::Player* player,itemManager::item& item);

	//컨테이너에 버림
	void player_dropItemtoContainer(PlayerManager::Player* player ,itemManager::item& item);

};