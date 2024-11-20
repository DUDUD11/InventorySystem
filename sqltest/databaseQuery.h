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

	//item ���濡 �ִ��� �Դ���
	//�ۿ� �ִ��� �Դ��� 
	void eat_Item(PlayerManager::Player *player, itemManager::item& item, bool player_has);

	//map�� �ִ��� ���濡 ����
	void player_getItemtoBackPack(PlayerManager::Player* player, itemManager::item& item);

	//map�� �ִ��� ������
	void player_getItemtoEquip(PlayerManager::Player* player, itemManager::item& item);

	//���濡 �ִ��� ������
	void player_equipfromBackPack(PlayerManager::Player* player,itemManager::item& item);

	//�����Ѱ� ���濡
	void player_equiptoBackPack(PlayerManager::Player* player, itemManager::item& item);

	//������ ����
	void player_dropItemtoGround(PlayerManager::Player* player,itemManager::item& item);

	//�����̳ʿ� ����
	void player_dropItemtoContainer(PlayerManager::Player* player ,itemManager::item& item);

};