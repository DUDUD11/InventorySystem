#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
	for (auto it = Playerlist.begin(); it != Playerlist.end();)
	{
		Player* temp = *it;
		it = Playerlist.erase(it);
		delete(temp);
	}
}

bool PlayerManager::Player::Item_exist(const char* item_id,int& itemslot)
{
	
		for (auto it = this->Equipment.begin(); it != this->Equipment.end(); it++)
		{
			if (strcmp(item_id, it->id) == 0)
			{
				itemslot = (int)itemManager::playeritemStatus::Equip;
				return true;

			}

		}

		for (auto it = this->BackPack.begin(); it != this->BackPack.end(); it++)
		{
			if (strcmp(item_id, it->id) == 0)
			{
				itemslot = (int)itemManager::playeritemStatus::Backpack;
				return true;
			}

		}


		return false;
	
}

itemManager::item PlayerManager::Player::Item_Delete_atBackPack(char* item_id)
{
	for (auto it = this->BackPack.begin(); it != this->BackPack.end(); it++)
	{
		if (strcmp(item_id, it->id) == 0)
		{
			itemManager::item tmp = *it;
			this->BackPack.erase(it);

			return tmp;
		}

	}

	return *new itemManager::item();
}

itemManager::item PlayerManager::Player::Item_Delete_atEquip(char* item_id)
{
	for (auto it = this->Equipment.begin(); it != this->Equipment.end(); it++)
	{
		if (strcmp(item_id, it->id) == 0)
		{
			itemManager::item tmp = *it;
			this->Equipment.erase(it);

			return tmp;

		}

	}

	return  *new itemManager::item();;
}

void PlayerManager::Player::item_insert_atBackpack(itemManager::item& item)
{
	BackPack.push_back(item);
}

void PlayerManager::Player::item_insert_atEquip(itemManager::item& item)
{
	Equipment.push_back(item);
}
