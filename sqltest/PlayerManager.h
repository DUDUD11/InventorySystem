#pragma once
#include "itemManager.h"
#include <list>


class PlayerManager
{

	typedef itemManager::Vector2d vector2;
	typedef itemManager::Vector3d vector3;

public:

	PlayerManager();
	~PlayerManager();

	struct Player
	{
		int id;

		std::list<itemManager::item> Equipment;
		std::list<itemManager::item> BackPack;

		Player(int _id):id(_id)
		{
		}

		bool Item_exist(const char* item_id, int& itemslotidx);

		itemManager::item Item_Delete_atBackPack(char* item_id);

		itemManager::item Item_Delete_atEquip(char* item_id);

		void item_insert_atBackpack(itemManager::item& item);

		void item_insert_atEquip(itemManager::item& item);

	};




	std::list<Player*> Playerlist;

};