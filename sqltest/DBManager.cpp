#include "DBManager.h"

void DBManager::Init(const char* mysqlurl, const char* id, const char* password)
{
	m_dbHandler.Connect_mysql(mysqlurl, id, password);
	m_dbdata.SetDBHandler(&m_dbHandler);

	m_mysqlurl = new char[strlen(mysqlurl) + 1];
	m_id = new char[strlen(id) + 1];
	m_password = new char[strlen(password) + 1];

	memset((void*)m_mysqlurl, 0, strlen(mysqlurl) + 1);
	memset((void*)m_id, 0, strlen(id) + 1);
	memset((void*)m_password, 0, strlen(password) + 1);

	memcpy((void*)m_mysqlurl, mysqlurl, strlen(mysqlurl));
	memcpy((void*)m_id, id, strlen(id));
	memcpy((void*)m_password, password, strlen(password));

	

}

void DBManager::test_Init(const char* jsonurl)
{
	m_dbHandler.Init_testmysql();
	m_dbHandler.testdata_insert();

	m_dbdata.JsonFileOpen(jsonurl);
	m_dbdata.JsonLoad(&m_itemManager);

}

void DBManager::Close()
{
	while (!m_DbSetList.empty())
	{
		Db_Set tmp = m_DbSetList.front();
		m_DbSetList.pop();
		tmp.st_dbhandler->Close();

		delete(tmp.st_databasequery);
		delete(tmp.st_dbhandler);
	}

	if (m_mysqlurl != NULL)
	{
		delete(m_mysqlurl);
		delete(m_id);
		delete(m_password);
	}

}

itemManager* DBManager::GetItemManager()
{
	return &this->m_itemManager;
}

