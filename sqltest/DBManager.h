#pragma once

#include "databaseQuery.h"
#include "Dbdata.h"
#include "DBHandler.h"
#include "JsonParser.h"

class DBManager
{
private:
	DBManager() {};
	~DBManager() {};
public:

	static inline DBManager* GetInstance()
	{
		static DBManager dbmanager;
		return &dbmanager;
	}

	void Init(const char* mysqlurl, const char* id, const char* password);
	void test_Init(const char* jsonurl);
	void Close();

	struct Db_Set
	{
		databaseQuery* st_databasequery;
		DBHandler* st_dbhandler;

		Db_Set(const Db_Set& dbset):st_databasequery(dbset.st_databasequery),st_dbhandler(dbset.st_dbhandler)
		{

		}

		Db_Set() :st_databasequery(nullptr), st_dbhandler(nullptr)
		{

		}

	};


	databaseQuery* GetDBInstacneSet()
	{
		if (m_DbSetList.empty())
		{
			Db_Set dbset;

		
			dbset.st_dbhandler = new DBHandler();
			dbset.st_databasequery = new databaseQuery(dbset.st_dbhandler);

			dbset.st_dbhandler->Connect_mysql(m_mysqlurl, m_id, m_password);

			m_DbSetList.push(dbset);

			//connect 새로연결해서 주면됨
			return dbset.st_databasequery;
		}

		else
		{
			Db_Set dbset = m_DbSetList.front();
			m_DbSetList.pop();
			return dbset.st_databasequery;

		}
	}

	void ReleaseDBInstance(databaseQuery* _databaseQuery)
	{
		Db_Set* dbset = reinterpret_cast<Db_Set*>(_databaseQuery);
		m_DbSetList.push(*dbset);

	}

	itemManager* GetItemManager();

private:

	std::queue<Db_Set> m_DbSetList;
	
	DBHandler m_dbHandler;
	databaseQuery m_databasequery;
	Dbdata m_dbdata;
	itemManager m_itemManager;

	const char* m_mysqlurl;
	const char* m_id; 
	const char* m_password;


	/*
Player player;
player.id = 1;

for (auto it = _itemManager.itemList.begin(); it != _itemManager.itemList.end(); it++)
{
	if (strcmp(it->id, "26ab8444-cbbc-40b3-8883-13a501508b9b") == 0)
	{
		_databaseQuery.eat_Item(&dbHandler, &player, *it);
		break;
	}
}
	*/



};