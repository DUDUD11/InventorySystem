#include"Content.h"



PlayerManager::Player* Content::New_UserAdd(int SessionID)
{

    PlayerManager::Player* player = new PlayerManager::Player(SessionID);
    m_Players->Playerlist.push_back(player);
    
    m_dbquery->Player_Inventory(player->BackPack, player->Equipment, player->id);

    return player;
}

void Content::eat_Item(PlayerManager::Player* player, CPacket_TLS* packet)
{
    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];


    *packet >> st_id;


    int player_itemslottype;

    if (player->Item_exist((char*)st_id.pointer, player_itemslottype))
    {
        itemManager::item item = player->Item_Delete_atBackPack((char*)st_id.pointer);
      //  bool wres = m_Items->item_delete(item.id);
        m_dbquery->eat_Item(player, item,true);
    }

    else
    {
        itemManager::item item = m_Items->item_find((char*)st_id.pointer);
        bool wres = m_Items->item_delete(item.id);
        m_dbquery->eat_Item(player, item,false);
    }

    
    //player hp increase

}

void Content::player_getItemtoBackPack(PlayerManager::Player* player, CPacket_TLS* packet)
{
    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];

    *packet >> st_id;

    itemManager::item item = m_Items->item_find((char*)st_id.pointer);
    m_Items->item_delete(item.id);
    player->item_insert_atBackpack(item);


    m_dbquery->player_getItemtoBackPack(player, item);

}

void Content::player_getItemtoEquip(PlayerManager::Player* player, CPacket_TLS* packet)
{

    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];

    *packet >> st_id;

    itemManager::item item = m_Items->item_find((char*)st_id.pointer);
    m_Items->item_delete(item.id);
    player->item_insert_atEquip(item);

    m_dbquery->player_getItemtoEquip(player, item);
}

void Content::player_equipfromBackPack(PlayerManager::Player* player, CPacket_TLS* packet)
{
    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];

    *packet >> st_id;


    itemManager::item item = player->Item_Delete_atBackPack((char*)st_id.pointer);
    item.slot_paneltype = (int)itemManager::playeritemStatus::Equip;
    player->item_insert_atEquip(item);

    m_dbquery->player_equipfromBackPack(player, item);
}

void Content::player_equiptoBackPack(PlayerManager::Player* player, CPacket_TLS* packet)
{
    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];

    *packet >> st_id;

    itemManager::item item = m_Items->item_find((char*)st_id.pointer);
    player->Item_Delete_atEquip(item.id);
    player->item_insert_atBackpack(item);

    m_dbquery->player_equiptoBackPack(player, item);


}

void Content::player_dropItemtoGround(PlayerManager::Player* player, CPacket_TLS* packet)
{
    double pos_x;
    double pos_y;
    double pos_z;
    double rot_x;
    double rot_y;
    double rot_z;

    *packet >> pos_x >> pos_y >> pos_z >> rot_x >> rot_y >> rot_z;

    CPacket_TLS::D_Packet st_id;
    st_id.len = packet->GetDataSize() - packet->GetreadDataSize();
    st_id.pointer = new char[st_id.len];

    *packet >> st_id;

    itemManager::item item;

    int player_itemslottype;

    if (player->Item_exist((char*)st_id.pointer, player_itemslottype))
    {
        if (player_itemslottype == (int)itemManager::playeritemStatus::Backpack)
        {
            item = player->Item_Delete_atBackPack((char*)st_id.pointer);

        }

        else
        {
            item = player->Item_Delete_atEquip((char*)st_id.pointer);

        }

        item.position.x = pos_x;
        item.position.y = pos_y;
        item.position.z = pos_z;
        item.rotation.x = rot_x;
        item.rotation.x = rot_y;
        item.rotation.x = rot_z;

        m_Items->item_insert(item);
        m_dbquery->player_dropItemtoGround(player, item);
    }

    else
    {
        __debugbreak();
    }
}

void Content::player_dropItemtoContainer(PlayerManager::Player* player, CPacket_TLS* packet)
{
    unsigned short item_len;
    CPacket_TLS::D_Packet st_ItemId;
    unsigned short container_idlen;
    CPacket_TLS::D_Packet st_ContainerId;

    *packet >> item_len;
    st_ItemId.len = item_len;
    st_ItemId.pointer = new char[item_len];

    *packet >> st_ItemId;
    *packet >> container_idlen;
    st_ContainerId.len = container_idlen;
    st_ContainerId.pointer = new char[container_idlen];

    *packet >> st_ContainerId;

    itemManager::item item;
    int player_itemslottype;

    if (player->Item_exist((char*)st_ItemId.pointer, player_itemslottype))
    {
        if (player_itemslottype == (int)itemManager::playeritemStatus::Backpack)
        {
            item = player->Item_Delete_atBackPack((char*)st_ItemId.pointer);

        }

        else
        {
            item = player->Item_Delete_atEquip((char*)st_ItemId.pointer);

        }

        memcpy(item.containerid, st_ContainerId.pointer, st_ContainerId.len);

        m_Items->item_insert(item);
        m_dbquery->player_dropItemtoContainer(player, item);
    }
}

/*
void Content::disconnect(Players::Player* player)
{


    m_Players.PlayerRemove.insert(player->sessionID);
    Character_Delete(dfPACKET_SC_DELETE_CHARACTER, player);

}

void Content::disconnectFromServer(UINT64 SessionID)
{
    Profiler_Manager::Profile p(L"disfromser");

    Players::Player* tmp;

    {
        Profiler_Manager::Profile p(L"session at");

        if (!SessionToPlayer.at(SessionID, tmp))
        {
            __debugbreak();

        }
    }

    m_Players.PlayerRemove.insert(tmp->sessionID);


    Character_Delete(dfPACKET_SC_DELETE_CHARACTER, tmp);


    {
        sector.Write_Lock();

        // Profile p(L"delete user time", profile_N);


       // for (typename Concurrency::concurrent_unordered_set<Players::Player*>::iterator rm_iter = m_Players.PlayerRemove.GetHashSet()->begin(); rm_iter != m_Players.PlayerRemove.GetHashSet()->end();rm_iter++)
       // {
        //    int x = 0;



        int s_y = tmp->Y / Sector_Size;
        int s_x = tmp->X / Sector_Size;

        bool flag = false;

        for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[s_y][s_x].begin(); iter != sector.Sector_Map[s_y][s_x].end(); )
        {
            if ((Players::Player*)*iter == tmp)
            {

                iter = sector.Sector_Map[s_y][s_x].erase(iter);
                Delete_Player(SessionID);
                //rm_iter = m_Players.PlayerRemove.erase(rm_iter);

                delete(tmp);
                flag = true;
                sector.Write_UnLock();
                return;

                break;
            }

            else
            {

                iter++;
            }
        }
        if (!flag) {

            Profiler_Manager::Profile p(L"Sector SYNC Del");
            bool ff = false;

            for (int ii = 0; ii < dfRANGE_MOVE_BOTTOM / Sector_Size; ii++)
            {
                for (int jj = 0; jj < dfRANGE_MOVE_RIGHT / Sector_Size; jj++)
                {
                    for (std::list<Players::Player*>::iterator it = sector.Sector_Map[ii][jj].begin(); it != sector.Sector_Map[ii][jj].end(); it++)


                    {
                        if (*it == tmp)
                        {



                            sector.Sector_Map[ii][jj].erase(it);
                            Delete_Player(SessionID);
                            // rm_iter = m_Players.PlayerRemove.erase(rm_iter);
             //                __debugbreak();
                            delete(tmp);

                            ff = true;

                            break;
                        }

                    }

                    if (ff) break;
                }
                if (ff) break;

            }

            if (!ff)
            {

                // __debugbreak();
            }

        }

        //  }
        sector.Write_UnLock();
    }





}
*/

/*
void Content::Delete_Player(UINT64 SessionID)
{
    Players::Player* player;

    if (!SessionToPlayer.at(SessionID, player))
    {
        __debugbreak();
    }

    m_Players.Delete_Player(player);
    SessionToPlayer.erase(SessionID);

}
*/

Content::Content()
{
    m_Players = new PlayerManager();
    m_Items = new itemManager();
  
}

Content::~Content()
{

}



void Content::GetInitItem_Data(itemManager* _itemmanager)
{
    this->m_Items = _itemmanager;
}

void Content::GetDBInstance()
{
    m_dbquery = DBManager::GetInstance()->GetDBInstacneSet();
}
