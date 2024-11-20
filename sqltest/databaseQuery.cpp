#include "databaseQuery.h"

//slot_panel 0착용 1 가방

databaseQuery::databaseQuery(DBHandler* dbhandle) :m_dbHandle(dbhandle)
{

}

databaseQuery::databaseQuery():m_dbHandle(nullptr)
{
    
}

void databaseQuery::Player_Inventory(std::list<itemManager::item>& BackPack, std::list<itemManager::item>& Equipment, int playerid)
{
    try {
        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("SELECT * FROM inventory.players_inventory where slot_panel= ?");
        pstmt->setInt(1, (int)itemManager::playeritemStatus::Equip);
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next())
        {
            itemManager::item _item;
            char* res_id = (char*)res->getString(2).c_str();
            memcpy(_item.id, res_id, strlen(res_id));
            _item.rotated = res->getBoolean(3);
            _item.matrix_pos.x = res->getInt(4);
            _item.matrix_pos.y = res->getInt(5);
            _item.slot_paneltype = (int)itemManager::playeritemStatus::Equip;

            Equipment.push_back(_item);
        }


        pstmt = m_dbHandle->con->prepareStatement("SELECT * FROM inventory.players_inventory where slot_panel= ?");
        pstmt->setInt(1, (int)itemManager::playeritemStatus::Backpack);
        res = pstmt->executeQuery();

        while (res->next())
        {
            itemManager::item _item;
            char* res_id = (char*)res->getString(2).c_str();
            memcpy(_item.id, res_id,strlen(res_id));
            _item.rotated = res->getBoolean(3);
            _item.matrix_pos.x = res->getInt(4);
            _item.matrix_pos.y = res->getInt(5);
            _item.slot_paneltype = (int)itemManager::playeritemStatus::Backpack;

            BackPack.push_back(_item);
        }

        delete pstmt;
        delete res;

    }

    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }


}

void databaseQuery::eat_Item(PlayerManager::Player* player, itemManager::item& item, bool player_has)
{
    try {
      
        int dummy_idx;

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;
        if (player_has)
        {
           
            pstmt = m_dbHandle->con->prepareStatement("DELETE FROM players_inventory WHERE user_id=? and item_id=?");
            pstmt->setInt(1, player->id);
            pstmt->setString(2, item.id);
            pstmt->executeUpdate();

        }

        else
        {
            pstmt = m_dbHandle->con->prepareStatement("DELETE FROM map_item WHERE item_id=?");
            pstmt->setString(1, item.id);
            pstmt->executeUpdate();
        }
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }

}

//map에 있던걸 가방에 넣음
void databaseQuery::player_getItemtoBackPack(PlayerManager::Player* player, itemManager::item& item)
{
    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("DELETE FROM map_item WHERE item_id=?");
        pstmt->setString(1, item.id);
        pstmt->executeUpdate();

        pstmt = m_dbHandle->con->prepareStatement("INSERT INTO players_inventory(user_id, item_id, isRotated, matrix_pos_x, matrix_pos_y, slot_panel) VALUES(?, ?, ?, ?, ?, ?)");
        pstmt->setInt(1, player->id);
        pstmt->setString(2, item.id);
        pstmt->setBoolean(3, item.rotated);
        pstmt->setInt(4, item.matrix_pos.x);
        pstmt->setInt(5, item.matrix_pos.y);
        pstmt->setInt(6, slot_panel::backpack); // 1

        pstmt->executeUpdate();
       
     
        
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }
}

//map에 있던걸 착용함
void databaseQuery::player_getItemtoEquip(PlayerManager::Player* player, itemManager::item& item)
{
    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("DELETE FROM map_item WHERE item_id=?");
        pstmt->setString(1, item.id);
        pstmt->executeUpdate();

        pstmt = m_dbHandle->con->prepareStatement("INSERT INTO players_inventory(user_id, item_id, isRotated, matrix_pos_x, matrix_pos_y, slot_panel) VALUES(?, ?, ?, ?, ?, ?)");
        pstmt->setInt(1, player->id);
        pstmt->setString(2, item.id);
        pstmt->setBoolean(3, item.rotated);
        pstmt->setInt(4, item.matrix_pos.x);
        pstmt->setInt(5, item.matrix_pos.y);
        pstmt->setInt(6, (int)slot_panel::equip); // 0

        pstmt->executeUpdate();



        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }

}

//가방에 있던걸 착용함
void databaseQuery::player_equipfromBackPack(PlayerManager::Player* player, itemManager::item& item)
{
    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("UPDATE players_inventory SET slot_panel = ? where user_id=? and item_id=?");
        
        pstmt->setInt(1, (int)slot_panel::equip); // 0
        pstmt->setInt(2, player->id);
        pstmt->setString(3, item.id);

        pstmt->executeUpdate();



        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }
}

void databaseQuery::player_equiptoBackPack(PlayerManager::Player* player, itemManager::item& item)
{
    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("UPDATE players_inventory SET slot_panel = ? where user_id=? and item_id=?");

        pstmt->setInt(1, (int)slot_panel::backpack); // 1
        pstmt->setInt(2, player->id);
        pstmt->setString(3, item.id);

        pstmt->executeUpdate();



        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }


}

//땅에다 버림
void databaseQuery::player_dropItemtoGround(PlayerManager::Player* player, itemManager::item& item)
{

    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;

        pstmt = m_dbHandle->con->prepareStatement("DELETE FROM players_inventory where user_id = ? and item_id = ?");
        pstmt->setInt(1, player->id);
        pstmt->setString(2, item.id);
        pstmt->executeUpdate();

        pstmt = m_dbHandle->con->prepareStatement("INSERT INTO map_item(item_id, isRotated, inContainer, container_id, matrix_pos_x, matrix_pos_y, world_pos_x, world_pos_y, world_pos_z, world_rot_x, world_rot_y, world_rot_z) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        pstmt->setString(1, item.id);
        pstmt->setBoolean(2, item.rotated);
        pstmt->setBoolean(3, false);
        pstmt->setString(4, "");
        pstmt->setInt(5, item.matrix_pos.x);
        pstmt->setInt(6, item.matrix_pos.y);
        pstmt->setDouble(7, item.position.x);
        pstmt->setDouble(8, item.position.y);
        pstmt->setDouble(9, item.position.z);
        pstmt->setDouble(10, item.rotation.x);
        pstmt->setDouble(11, item.rotation.y);
        pstmt->setDouble(12, item.rotation.z);
        pstmt->executeUpdate();


        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }




}



//컨테이너에 버림
void databaseQuery::player_dropItemtoContainer(PlayerManager::Player* player, itemManager::item& item)
{
    try {

        m_dbHandle->con->setSchema("inventory");
        sql::PreparedStatement* pstmt = NULL;


        pstmt = m_dbHandle->con->prepareStatement("DELETE FROM players_inventory where user_id = ? and item_id = ?");
        pstmt->setInt(1, player->id);
        pstmt->setString(2, item.id);
        pstmt->executeUpdate();

        pstmt = m_dbHandle->con->prepareStatement("INSERT INTO map_item(item_id, isRotated, inContainer, container_id, matrix_pos_x, matrix_pos_y, world_pos_x, world_pos_y, world_pos_z, world_rot_x, world_rot_y, world_rot_z) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        pstmt->setString(1, item.id);
        pstmt->setBoolean(2, item.rotated);
        pstmt->setBoolean(3, true);
        pstmt->setString(4, item.containerid);
        pstmt->setInt(5, item.matrix_pos.x);
        pstmt->setInt(6, item.matrix_pos.y);
        pstmt->setDouble(7, 0);
        pstmt->setDouble(8, 0);
        pstmt->setDouble(9, 0);
        pstmt->setDouble(10, 0);
        pstmt->setDouble(11, 0);
        pstmt->setDouble(12, 0);
        pstmt->executeUpdate();


        delete pstmt;
    }
    catch (sql::SQLException& e) {
        printf("sql error code %d\n", e.getErrorCode());
        __debugbreak();
    }

}





