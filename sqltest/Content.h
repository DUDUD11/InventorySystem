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

	// ������ �Ա�
	void eat_Item(PlayerManager::Player* player, CPacket_TLS* packet);

	//map�� �ִ��� ���濡 ����
	void player_getItemtoBackPack(PlayerManager::Player* player, CPacket_TLS* packet);

	//map�� �ִ��� ������
	void player_getItemtoEquip(PlayerManager::Player* player, CPacket_TLS* packet);

	//���濡 �ִ��� ������
	void player_equipfromBackPack(PlayerManager::Player* player, CPacket_TLS* packet);
	
	//�����Ѱ� ���濡
	void player_equiptoBackPack(PlayerManager::Player* player, CPacket_TLS* packet);

	//������ ����
	void player_dropItemtoGround(PlayerManager::Player* player, CPacket_TLS* packet);

	//�����̳ʿ� ����
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