#include "itemManager.h"

bool itemManager::item_delete(char* item_id)
{
	for (auto it = itemList.begin(); it != itemList.end(); it++)
	{
		item tmp = *it;

		if (strcmp(item_id, tmp.id) == 0)
		{
			it = itemList.erase(it);
			return true;
		}

	}


	return false;
}

void itemManager::item_insert(item& item)
{
	itemList.push_back(item);
}

itemManager::item& itemManager::item_find(const char* item_id)
{
	for (auto it = itemList.begin(); it != itemList.end(); it++)
	{
		item* tmp = &(*it);

		if (strcmp(item_id, tmp->id) == 0)
		{
			return *it;
		}

	}

	__debugbreak();

	return *(new item());
}