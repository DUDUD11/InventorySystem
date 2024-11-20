#include "Dbdata.h"

void Dbdata::JsonLoad(void* container)
{
	itemManager* _itemManager = reinterpret_cast<itemManager*> (container);

	Document Doc;
	Doc.Parse(json);

	Value& panelListArray = Doc["panelList"];
	//panel 정보는 pass

	for (SizeType i = 0; i < panelListArray.Size(); i++)
	{
		Value& PanelListData = panelListArray[i];

		Value& PanelItemList = PanelListData["itemList"].GetArray();

		for (SizeType j = 0; j < PanelItemList.Size(); j++)
		{
			
			Value& PanelItemData = PanelItemList[j];
			
			itemManager::item _item;

			memcpy(_item.id, PanelItemData["itemId"].GetString(), PanelItemData["itemId"].GetStringLength());

			_item.rotated = PanelItemData["isRotated"].GetBool();
			_item.matrix_pos.x = PanelItemData["matrixPosition"]["x"].GetDouble();
			_item.matrix_pos.y = PanelItemData["matrixPosition"]["y"].GetDouble();
			_item.slot_pos.x = PanelItemData["slotPosition"]["x"].GetDouble();
			_item.slot_pos.y = PanelItemData["slotPosition"]["y"].GetDouble();
			_item.slot_paneltype = PanelItemData["slotPanelTypeId"].GetInt();
			_item.user_assigned = true;
			_item.containered = false;
			//panel에는 container가 없다.
			//memcpy(_item.containerid, PanelItemData["myLootContainerId"].GetString(), PanelItemData["itemId"].GetStringLength());
			//panel에는 position과 rotation도 없다

			_itemManager->itemList.push_back(_item);
	

		}

	}

	Value& ContainerListArray = Doc["lootDataList"];

	for (SizeType i = 0; i < ContainerListArray.Size(); i++)
	{
		Value& ContainerListData = ContainerListArray[i];

		const char* container_id = ContainerListData["id"].GetString();

		Value& ContainerItemList = ContainerListData["itemList"].GetArray();

		for (SizeType j = 0; j < ContainerItemList.Size(); j++)
		{
			Value& ContainerItemData = ContainerItemList[j];

			itemManager::item _item;

			memcpy(_item.id, ContainerItemData["itemId"].GetString(), ContainerItemData["itemId"].GetStringLength());
			memcpy(_item.containerid, container_id, strlen(container_id));
			_item.containerid[strlen(container_id)] = '\0';

			_item.rotated = ContainerItemData["isRotated"].GetBool();
			_item.matrix_pos.x = ContainerItemData["matrixPosition"]["x"].GetDouble();
			_item.matrix_pos.y = ContainerItemData["matrixPosition"]["y"].GetDouble();
			_item.slot_pos.x = ContainerItemData["slotPosition"]["x"].GetDouble();
			_item.slot_pos.y = ContainerItemData["slotPosition"]["y"].GetDouble();
			_item.slot_paneltype = ContainerItemData["slotPanelTypeId"].GetInt();
		
			_item.user_assigned = false;
			_item.containered = true;
			_itemManager->itemList.push_back(_item);
		}

		
	}

	Value& GroundListArray = Doc["itemDataListOnGround"];

	for (SizeType i = 0; i < GroundListArray.Size(); i++)
	{
		Value& GroundItemData = GroundListArray[i];

		itemManager::item _item;

		memcpy(_item.id, GroundItemData["itemId"].GetString(), GroundItemData["itemId"].GetStringLength());

		_item.rotated = GroundItemData["isRotated"].GetBool();
		_item.matrix_pos.x = GroundItemData["matrixPosition"]["x"].GetDouble();
		_item.matrix_pos.y = GroundItemData["matrixPosition"]["y"].GetDouble();
		_item.slot_pos.x = GroundItemData["slotPosition"]["x"].GetDouble();
		_item.slot_pos.y = GroundItemData["slotPosition"]["y"].GetDouble();
		_item.slot_paneltype = GroundItemData["slotPanelTypeId"].GetInt();
		_item.position.x = GroundItemData["position"]["x"].GetDouble();
		_item.position.y = GroundItemData["position"]["y"].GetDouble();
		_item.position.z = GroundItemData["position"]["z"].GetDouble();
		_item.rotation.x = GroundItemData["rotation"]["x"].GetDouble();
		_item.rotation.y = GroundItemData["rotation"]["y"].GetDouble();
		_item.rotation.z = GroundItemData["rotation"]["z"].GetDouble();


		_item.user_assigned = false;
		_item.containered = false;
		_itemManager->itemList.push_back(_item);

	}

	m_dbHandler->JsonMapData_insert(_itemManager);
	m_dbHandler->JsonUserInvetory_insert(_itemManager);

	for (auto it = _itemManager->itemList.begin(); it != _itemManager->itemList.end();)
	{
		itemManager::item _item = *it;

		if (!_item.user_assigned)
		{
			++it;
			continue;
		}

		it = _itemManager->itemList.erase(it);
	}


}

void Dbdata::SetDBHandler(DBHandler* handler_ptr)
{
	this->m_dbHandler = handler_ptr;
}
