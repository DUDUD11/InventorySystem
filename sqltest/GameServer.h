#pragma once
#include "NetworkServer.h"
#include "Content.h"
#include "Proxy.h"
#include "ProxyCaller.h"
#include "DBManager.h"

#define dburl "127.0.0.1:3306"
#define dbid "root"
#define dbpassword "1234"
#define jsonurl "D:\\New Unity Project (1)\\Inventory System\\Assets\\item.json"


class GameServer : public NetworkServer, public ProxyCaller
{
private:
	Content m_Content;
	
public:

	friend class Proxy;

	GameServer()
	{

	}
	~GameServer()
	{
		Stop();
		DBManager::GetInstance()->Close();
	}

	BYTE Start(const WCHAR* ip, SHORT port, INT workerThreadNum, INT sendThreadNum, BYTE nagleOption, INT maxClientNum)
	{
		BYTE ret = NetworkServer::Start(ip, port, workerThreadNum, sendThreadNum, nagleOption, maxClientNum);
		if (ret == 0)
		{
			__debugbreak();
		}
		
		Proxy::GetInstance()->Set_Server(this);

		DBManager* dbManager = DBManager::GetInstance();
		dbManager->Init(dburl, dbid, dbpassword);
		dbManager->test_Init(jsonurl);

		m_Content.GetInitItem_Data(dbManager->GetItemManager());
		m_Content.GetDBInstance();


		/*

	for (auto it = _itemManager.itemList.begin(); it != _itemManager.itemList.end(); it++)
	{
		if (strcmp(it->id, "26ab8444-cbbc-40b3-8883-13a501508b9b") == 0)
		{
			_databaseQuery.eat_Item(&dbHandler, &player, *it);
			break;
		}
	}		
		*/

		return ret;
	}

	virtual VOID OnRecv(UINT64 SessionID, CPacket_TLS* packet)
	{

		unsigned short packet_len;
		*packet >> packet_len;

		unsigned short packet_type;
		*packet >> packet_type;

		PlayerManager::Player* player;
		if (!m_Content.SessionToPlayer.at(SessionID, player))
		{
			packet->Clear();
			return;
		}

		switch (packet_type)
		{
		case 0:

			m_Content.eat_Item(player,packet);
			break;
		case 1:
			m_Content.player_getItemtoBackPack(player, packet);
			//id�� ���
			break;
		case 2:
			m_Content.player_getItemtoEquip(player, packet);
			// id�� ���
			break;
		case 3:
			m_Content.player_equipfromBackPack(player, packet);
			// id�����
			break;

		case 4:
			m_Content.player_equiptoBackPack(player, packet);
			// id�����
			break;

		case 5:
			m_Content.player_dropItemtoGround(player, packet);
			//x,y,z rotation x,y,z
			break;
		case 6:
			m_Content.player_dropItemtoContainer(player, packet);
			//container id �ʿ�
			break;
		default:
			__debugbreak();
			break;
		}
		packet->Clear();

	}

	virtual VOID OnClientJoin(UINT64 SessionID)
	{
		PlayerManager::Player* player = m_Content.New_UserAdd(SessionID);
		m_Content.SessionToPlayer.insert(SessionID, player);
	}

	//Release�� ȣ��
	virtual VOID OnClientLeave(UINT64 SessionID)
	{
		//m_Content.disconnectFromServer(SessionID);
		//m_Content.Delete_Player(SessionID);
	}

	//accept����, return false, return true;
	//false �� Ŭ���̾�Ʈ �ź�, true �� ���� ���
	virtual bool OnConnectionRequest(PWCHAR, SHORT)
	{
		return true;
	}

	//��Ŷ ���� �Ϸ� ��


	//��Ŷ �۽� �Ϸ� ��
	virtual VOID OnSend(UINT64)
	{
	}
	//
	//��Ŀ������ GQCS �ٷ� �ϴܿ��� ȣ��
	virtual VOID OnWorkerThreadBegin(UINT64, DWORD, BYTE)
	{

	}
	//
	//��Ŀ������ 1���� ���� ��
	virtual VOID OnWorkerThreadEnd()
	{
	}

	virtual VOID OnSendThreadLooP()
	{
		//m_Content.Update();
	}


	virtual CPacket* Alloc_Packet(UINT64 SessionID)
	{
		CPacket* packet = NetworkServer::Alloc_Packet(SessionID);

		if (packet == nullptr)
		{
			return nullptr;

		}
		return packet;

	};

	virtual BYTE SendPacket(UINT64 SessionID, CPacket* packet)
	{
		return NetworkServer::SendPacket(SessionID, packet);
	};

	/*
		VOID Make_Header(int type, HEADER& header)
		{



		}
		*/




};