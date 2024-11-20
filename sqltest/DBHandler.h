#pragma once

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <queue>
#include "itemManager.h"

#define item_num 39

class DBHandler
{
	friend class databaseQuery;

	class test_dataobject
	{
	private:
		enum class len {

			item_id = 50,
			description = 40,
			money_type = 10,
			money_symbol = 4,
			weapon_type = 10,
			cloth_type = 15,
			ammo_type = 10,
			food_type =10

		};

	public:

		struct Bullet_Magazine_idx
		{
			int ammo_556_idx;
			int ammo_762_idx;
			int ammo_919_idx;
			int magazine_556_idx;
			int magazine_762_idx;
			int magazine_919_idx;

		};

		struct special
		{
			char description[(int)len::description];

			special(const char* _description)
			{
				memset(description, 0, (int)len::description);
				memcpy(description, _description, strlen(_description));
				
	
			}
		};

		struct money
		{
			char type[(int)len::money_type];
			char symbol[(int)len::money_symbol];
			int amount;

			money(const char* _type, const char* _symbol, int _amount) :amount(_amount)
			{

				memset(type, 0, (int)len::money_type);
				memset(symbol, 0, (int)len::money_symbol);

				memcpy(type, _type, strlen(_type));
				memcpy(symbol, _symbol, strlen(_symbol));

			}

		};
		
		struct weapon
		{
			char type[(int)len::weapon_type];
			char magazine_id[(int)len::item_id];
			int effective_range;

			weapon(const char* _type, const char* _magazine_id, int _effective_range) :effective_range(_effective_range)
			{

				memset(type, 0, (int)len::weapon_type);
				memset(magazine_id, 0, (int)len::item_id);

				memcpy(type, _type, strlen(_type));
				memcpy(magazine_id, _magazine_id, strlen(_magazine_id));

			}

		};

		struct cloth
		{
			char type[(int)len::cloth_type];
			int durability;
			int armor;

			cloth(const char* _type, int _durability, int _armor) :durability(_durability), armor(_armor)
			{
				memset(type, 0, (int)len::cloth_type);
				memcpy(type, _type, strlen(_type));
				
			}


		};

		struct ammo
		{
			char type[(int)len::ammo_type];
			int damage;

			ammo(const char* _type, int _damage) :damage(_damage)
			{
				memset(type, 0, (int)len::ammo_type);
				memcpy(type, _type, strlen(_type));
			}

		};

		struct magazine
		{
			char ammo_id[(int)len::item_id];
			int capacity;

			magazine(const char* _ammo_id, int _capacity) :capacity(_capacity)
			{
				memset(ammo_id, 0, (int)len::item_id);
				memcpy(ammo_id, _ammo_id, strlen(_ammo_id));
			}

		};

		struct food
		{
			char type[(int)len::food_type];
			int energy;

			food(const char* _type, int _energy) :energy(_energy)
			{
				memset(type, 0, (int)len::food_type);
				memcpy(type, _type, strlen(_type));
			}
		};

	};


private:

	sql::Driver* driver = NULL;
	sql::Connection* con = NULL;

	std::queue<void*> extradata_queue;

	void test_extradata_generate(test_dataobject::Bullet_Magazine_idx& idx);

public:
	
	bool Connect_mysql(const char* sqlurl, const  char* sql_id, const char* sql_password);

	void Init_testmysql();

	void testdata_insert();

	void JsonMapData_insert(itemManager* _itemManager);

	void JsonUserInvetory_insert(itemManager* _itemManager, int userid = 1);

	void Close();

	~DBHandler();

};