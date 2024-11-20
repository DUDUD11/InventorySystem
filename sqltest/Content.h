#pragma once
#include "PlayerManager.h"
#include "Serial_Buffer.h"
#include "Proxy.h"
#include "ConCurrent_HashMap.h"
#include "itemManager.h"
#include "DBManager.h"

class Content {
private:


public:

	Concurrent_Unordered_map<uint64_t, PlayerManager::Player*> SessionToPlayer;

	Content();
	~Content();

	PlayerManager::Player* New_UserAdd(int SessionID);

	// 아이템 먹기
	void eat_Item(PlayerManager::Player* player, CPacket_TLS* packet);

	//map에 있던걸 가방에 넣음
	void player_getItemtoBackPack(PlayerManager::Player* player, CPacket_TLS* packet);

	//map에 있던걸 착용함
	void player_getItemtoEquip(PlayerManager::Player* player, CPacket_TLS* packet);

	//가방에 있던걸 착용함
	void player_equipfromBackPack(PlayerManager::Player* player, CPacket_TLS* packet);
	
	//착용한걸 가방에
	void player_equiptoBackPack(PlayerManager::Player* player, CPacket_TLS* packet);

	//땅에다 버림
	void player_dropItemtoGround(PlayerManager::Player* player, CPacket_TLS* packet);

	//컨테이너에 버림
	void player_dropItemtoContainer(PlayerManager::Player* player, CPacket_TLS* packet);



	void GetInitItem_Data(itemManager*);

	void GetDBInstance();

	//void disconnect(PlayerManager::Player* player);

	//void disconnectFromServer(UINT64 SessionID);

	//void Delete_Player(UINT64 SessionID);

private:
	PlayerManager* m_Players;
	itemManager* m_Items;
	databaseQuery* m_dbquery;
};