#include "DBHandler.h"

void DBHandler::test_extradata_generate(test_dataobject::Bullet_Magazine_idx& idx_data)
{
	typedef DBHandler::test_dataobject obj;

	obj::special* black_card = new obj::special("Access card for high security facility.");
	obj::special* blue_card = new obj::special("Access card for high security facility.");
	obj::special* red_card = new obj::special("Access card for high security facility.");
	obj::special* graphic_card = new obj::special("Access card for high security facility.");
	obj::special* green_card = new obj::special("Access card for high security facility.");
	obj::special* violet_card = new obj::special("Access card for high security facility.");
	obj::special* yellow_card = new obj::special("Access card for high security facility.");

	extradata_queue.push(black_card);
	extradata_queue.push(blue_card);
	extradata_queue.push(red_card);
	extradata_queue.push(graphic_card);
	extradata_queue.push(green_card);
	extradata_queue.push(violet_card);
	extradata_queue.push(yellow_card);

	obj::money* dollar100 = new obj::money("Dollar","$",100);
	obj::special* flash_drive = new obj::special("flash_drive for encrypted file");
	obj::special* Gold = new obj::special("Gold Bar");
	obj::special* Key_A306 = new obj::special("Key A-306");
	obj::special* Key_A308 = new obj::special("Key A-308");
	obj::special* Key_B102 = new obj::special("Key B-102");
	obj::special* Key_B104 = new obj::special("Key B-104");
	obj::special* Key_C203 = new obj::special("Key C-203");
	obj::special* Key_C205 = new obj::special("Key C-205");

	extradata_queue.push(dollar100);
	extradata_queue.push(flash_drive);
	extradata_queue.push(Gold);
	extradata_queue.push(Key_A306);
	extradata_queue.push(Key_A308);
	extradata_queue.push(Key_B102);
	extradata_queue.push(Key_B104);
	extradata_queue.push(Key_C203);
	extradata_queue.push(Key_C205);

	obj::weapon* ak47 = new obj::weapon("Assault","M_762",400);
	obj::cloth* body_armor = new obj::cloth("Body Armor",70,30);
	obj::special* gas = new obj::special("gas");
	obj::weapon* glock = new obj::weapon("Pistol","M_919",50);
	obj::cloth* headwear = new obj::cloth("Headwear", 20, 25);
	obj::weapon* mp7a2 = new obj::weapon("Assault","M_919", 200);
	obj::weapon* revolver = new obj::weapon("Pistol","M_919", 75);
	obj::weapon* wd_400 = new obj::weapon("Sniper","M_762", 1000);

	extradata_queue.push(ak47);
	extradata_queue.push(body_armor);
	extradata_queue.push(gas);
	extradata_queue.push(glock);
	extradata_queue.push(headwear);
	extradata_queue.push(mp7a2);
	extradata_queue.push(revolver);
	extradata_queue.push(wd_400);

	obj::ammo* ammo_556 = new obj::ammo("Ammo_556", 40);
	obj::ammo* ammo_762 = new obj::ammo("Ammo_762", 40);
	obj::ammo* ammo_919 = new obj::ammo("Ammo_919",25);
	obj::cloth* backpack = new obj::cloth("backpack", 30,0);
	obj::cloth* headset = new obj::cloth("Head Set", 30, 0);
	obj::magazine* Magazine_556 = new obj::magazine("Ammo_556", 20);
	obj::magazine* Magazine_762 = new obj::magazine("Ammo_762", 20);
	obj::magazine* Magazine_919 = new obj::magazine("Ammo_919", 15);

	idx_data.ammo_556_idx = extradata_queue.size();
	extradata_queue.push(ammo_556);

	idx_data.ammo_762_idx = extradata_queue.size();
	extradata_queue.push(ammo_762);

	idx_data.ammo_919_idx = extradata_queue.size();
	extradata_queue.push(ammo_919);

	extradata_queue.push(backpack);
	extradata_queue.push(headset);

	idx_data.magazine_556_idx = extradata_queue.size();
	extradata_queue.push(Magazine_556);

	idx_data.magazine_762_idx = extradata_queue.size();
	extradata_queue.push(Magazine_762);

	idx_data.magazine_919_idx = extradata_queue.size();
	extradata_queue.push(Magazine_919);

	obj::food* canned_beans = new obj::food("Food",40);
	obj::food* canned_corn = new obj::food("Food", 35);
	obj::food* canned_fish = new obj::food("Food", 38);
	obj::food* canned_meat = new obj::food("Food", 45);
	obj::food* canned_pasta = new obj::food("Food", 43);
	obj::food* canned_soup = new obj::food("Food", 50);
	obj::food* water = new obj::food("Drink", 45);

	extradata_queue.push(canned_beans);
	extradata_queue.push(canned_corn);
	extradata_queue.push(canned_fish);
	extradata_queue.push(canned_meat);
	extradata_queue.push(canned_pasta);
	extradata_queue.push(canned_soup);
	extradata_queue.push(water);

}

bool DBHandler::Connect_mysql(const char* sqlurl, const char* sql_id, const char * sql_password)
{
	driver = get_driver_instance();
	try
	{
		con = driver->connect(sqlurl, sql_id, sql_password);
	}

	catch (sql::SQLException e)
	{
		__debugbreak();
	}

	return true;
}

void DBHandler::Init_testmysql()
{

	sql::Statement* stmt = con->createStatement();

	stmt->execute("use inventory");
	stmt->execute("drop table if exists savefile");
	stmt->execute("create table savefile(savedata json , savetime TIMESTAMP NOT NULL DEFAULT Current_timestamp)");

	stmt->execute("drop table if exists item");
	stmt->execute("create table item(item_id varchar(50) unique not null primary key,item_type varchar(10),slot_type varchar(20),spawn_frequency int,size_x int,size_y int)");

	stmt->execute("drop table if exists players_inventory");
	stmt->execute("create table players_inventory(user_id int not null,item_id varchar(50),isRotated bool,matrix_pos_x int,matrix_pos_y int,slot_panel int)");

	stmt->execute("drop table if exists map_item");
	stmt->execute("create table map_item(id int auto_increment primary key,item_id varchar(50),isRotated bool,inContainer bool,container_id varchar(50),matrix_pos_x int,matrix_pos_y int,world_pos_x double,world_pos_y double,world_pos_z double,world_rot_x double,world_rot_y double,world_rot_z double)");

	stmt->execute("drop table if exists item_special");
	stmt->execute("create table item_special(item_id varchar(50) unique not null primary key, item_description varchar(40))");

	stmt->execute("drop table if exists item_money");
	stmt->execute("create table item_money(item_id varchar(50) unique not null primary key, money_type varchar(10), symbol varchar(4), amount int)");
	
	stmt->execute("drop table if exists item_weapon");
	stmt->execute("create table item_weapon(item_id varchar(50) unique not null primary key, weapon_type varchar(10), magazine_id varchar(50), effective_range int)");

	stmt->execute("drop table if exists item_cloth");
	stmt->execute("create table item_cloth(item_id varchar(50) unique not null primary key, cloth_type varchar(15), durability int, armor int)");

	stmt->execute("drop table if exists item_ammo");
	stmt->execute("create table item_ammo(item_id varchar(50) unique not null primary key, ammo_type varchar(10), damage int)");

	stmt->execute("drop table if exists item_magazine");
	stmt->execute("create table item_magazine(item_id varchar(50) unique not null primary key, ammo_id varchar(50), ammo_capacity int)");

	stmt->execute("drop table if exists item_food");
	stmt->execute("create table item_food(item_id varchar(50) unique not null primary key, food_type varchar(10), energy int)");

	delete stmt;

}

void DBHandler::testdata_insert()
{

	int ammo_556_num;
	int ammo_762_num;
	int ammo_919_num;


	const char* test_id_array[item_num] = { "1e558e3d - 6228 - 4f0f - b267 - 25929d1c1001" , "1e558e3d-6228-4f0f-b267-25929d1c1002", "1e558e3d-6228-4f0f-b267-25929d1c1003",
		"1e558e3d-6228-4f0f-b267-25929d1c1004","1e558e3d-6228-4f0f-b267-25929d1c1005","1e558e3d-6228-4f0f-b267-25929d1c1006","1e558e3d-6228-4f0f-b267-25929d1c1007",
		"90a032ef-6a71-46a0-9425-24398d34d58e", "17350475-6e4f-4966-90a9-979c00a3ce3b", "c54e4a81-876b-4d16-853e-958360ccf6d2", "56847c42-8628-4acc-bfcb-2b0c77453001", 
		"56847c42-8628-4acc-bfcb-2b0c77453002", "56847c42-8628-4acc-bfcb-2b0c77453003", "56847c42-8628-4acc-bfcb-2b0c77453004",
		"23fe8ea5-601a-4018-8eeb-f1a778d3c772", "20350dd3-b1db-4f1e-a5d7-02ab03a7e462",
		"0aeeb5f9-12e9-4e22-b38e-aa6a443006f8", "23fd0d34-7dcc-4316-906b-96dc239f60c4", "ff11aa39-96e3-49cf-9ccf-904ea71993ec", 
		"446502e0-180a-49f5-8e15-078311f94429", "dc2a4226-bdd3-4123-b0fd-639628e6c8c3", "bf31be4a-2e6a-49ab-ba01-ad7029d15fc8",
		"2f6f1e4d-0634-4ea4-9b9d-29e42cf4ed5a", "304c5d1a-f7ff-4bf4-89ca-3d80f5c4268c", 
		"07fbf64f-40bd-462a-98bf-866eab9ed17f", "4201bb40-7e4e-479f-9b08-338e6d75d12d", "a6a408ab-25ee-4113-a469-61c053be1df7", "67b07fdf-6f25-4ec9-ae3a-8f07a6d08776",
		"b60b3139-6452-44aa-8f9d-fc06016c2a05", "0c0fd58c-5a6c-4fae-bfac-6cc5c83913a3", "5d81d8bb-a7f9-436d-94bc-6f100381d3a3", "38a40b04-c79d-483d-99ba-842b74ff5c0f",
		"c7f7f5e6-9e97-4e06-b621-bb08956bd9a0", "b114b72d-c98d-4f43-b2c6-30658803758b", "fa2fd9d2-5067-4f49-8ac4-db6d07431e88", "58664caf-9d5d-47cb-b4a7-c93d37ae0f6a",
		"db982e2d-7858-49a5-bf3e-5f4c72ea227f", "9048fdc7-1490-4179-8b3b-0b06a3881c7e", "26ab8444-cbbc-40b3-8883-13a501508b9b"


	};
	const char* test_type_array[item_num] = { "Special","Special", "Special", "Special", "Special", "Special", "Special", 
"Money","Special", "Special", "Special","Special","Special","Special","Special","Special",
"Weapon","Cloth","Special","Weapon","Cloth","Weapon","Weapon","Weapon",
"Ammo","Ammo","Ammo","Cloth","Cloth","Magazine","Magazine","Magazine",
"Food","Food","Food","Food","Food","Food","Food"
	};
	const char* test_slottype_array[item_num] = {"General","General", "General", "General", "General", "General", "General", 
		"General","General", "General", "General", "General", "General", "General","General", "General",
		"Weapon","Body Armor","General","Pistol","Headwear","Weapon","Pistol","Weapon",
		"General","General","General","Backpack","Headset","General","General","General",
		"General","General", "General", "General","General","General", "General"

	};
	int test_spawnfrequency_array[item_num] = {1,1,1,1,1,1,1,
		5,5,5,5,5,5,5,5,5,
		10,10,10,10,10,10,10,10,
		25,25,25,25,25,25,25,25,
		50,50,50,50,50,50,50



	};
	int test_sizeofx_array[item_num] = { 1,1,1,1,1,1,1, 
		 1,1,1,1,1,1,1,1,1,
		1,2,2,1,2,1,1,1,
		1,1,1,2,2,1,1,1,
		1,1,1,1,1,1,1


	};
	int test_sizeofy_array[item_num] = { 1,1,1,1,1,1,1, 
		 1,1,1,1,1,1,1,1,1,
		4,2,2,2,2,3,2,5,
		1,1,1,2,2,1,1,1,
		1,1,1,1,1,1,1


	};

	struct test_dataobject::Bullet_Magazine_idx idx_data;
	test_extradata_generate(idx_data);

	try
	{
		sql::PreparedStatement* pstmt;

		con->setSchema("inventory");
	
		pstmt = con->prepareStatement("INSERT INTO item(item_id,item_type,slot_type,spawn_frequency,size_x,size_y) VALUES (?,?,?,?,?,?)");
		for (int i = 0; i < item_num; i++) {

			pstmt->setString(1, test_id_array[i]);
			pstmt->setString(2, test_type_array[i]);
			pstmt->setString(3, test_slottype_array[i]);
			pstmt->setInt(4, test_spawnfrequency_array[i]);
			pstmt->setInt(5, test_sizeofx_array[i]);
			pstmt->setInt(6, test_sizeofy_array[i]);
			pstmt->executeUpdate();
		}

		delete pstmt;
	}

	catch (sql::SQLException& e) {
		printf("sql error code %d\n", e.getErrorCode());
		__debugbreak();
	}

	
	try
	{

		con->setSchema("inventory");

		sql::PreparedStatement* pstmt = NULL;

		for (int i = 0; i < item_num; i++) {

			void* tmp_data = extradata_queue.front();
			extradata_queue.pop();


			if (strcmp(test_type_array[i], "Special") == 0)
			{
				test_dataobject::special* t_special = reinterpret_cast<test_dataobject::special*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_special(item_id, item_description) VALUES(? , ? )");
				pstmt->setString(1, test_id_array[i]);

				pstmt->setString(2, t_special->description);

				pstmt->executeUpdate();
				
			}

			else if (strcmp(test_type_array[i], "Money") == 0)
			{
				test_dataobject::money* t_money = reinterpret_cast<test_dataobject::money*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_money(item_id, money_type,symbol,amount) VALUES(? , ? , ?, ?)");
				pstmt->setString(1, test_id_array[i]);
				pstmt->setString(2, t_money->type);
				pstmt->setString(3, t_money->symbol);
				pstmt->setInt(4, t_money->amount);
				pstmt->executeUpdate();

			}
			else if (strcmp(test_type_array[i], "Weapon") == 0)
			{
				test_dataobject::weapon* t_weapon = reinterpret_cast<test_dataobject::weapon*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_weapon(item_id, weapon_type,magazine_id,effective_range) VALUES(? , ? , ?, ?)");
				
			
			
				pstmt->setString(1, test_id_array[i]);
				pstmt->setString(2, t_weapon->type);

				if (strcmp(t_weapon->magazine_id, "M_919") == 0)
				{

					pstmt->setString(3, test_id_array[idx_data.magazine_919_idx]);
				}

				else if (strcmp(t_weapon->magazine_id, "M_556") == 0)
				{
					pstmt->setString(3, test_id_array[idx_data.magazine_556_idx]);

				}

				else if (strcmp(t_weapon->magazine_id, "M_762") == 0)
				{
					pstmt->setString(3, test_id_array[idx_data.magazine_762_idx]);

				}

				pstmt->setInt(4, t_weapon->effective_range);
				pstmt->executeUpdate();

			}
			else if (strcmp(test_type_array[i], "Cloth") == 0)
			{
				test_dataobject::cloth* t_cloth = reinterpret_cast<test_dataobject::cloth*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_cloth(item_id, cloth_type,durability,armor) VALUES(? , ? , ?, ?)");
				pstmt->setString(1, test_id_array[i]);
				pstmt->setString(2, t_cloth->type);
				pstmt->setInt(3, t_cloth->durability);
				pstmt->setInt(4, t_cloth->armor);
				pstmt->executeUpdate();


			}
			else if (strcmp(test_type_array[i], "Ammo") == 0)
			{
				test_dataobject::ammo* t_ammo = reinterpret_cast<test_dataobject::ammo*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_ammo(item_id, ammo_type,damage) VALUES(? , ? , ?)");
				pstmt->setString(1, test_id_array[i]);
				pstmt->setString(2, t_ammo->type);
				pstmt->setInt(3, t_ammo->damage);
				pstmt->executeUpdate();

			}
			else if (strcmp(test_type_array[i], "Magazine") == 0)
			{
				test_dataobject::magazine* t_magazine = reinterpret_cast<test_dataobject::magazine*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_magazine(item_id, ammo_id,ammo_capacity) VALUES(? , ? , ?)");

				pstmt->setString(1, test_id_array[i]);

				if (strcmp(t_magazine->ammo_id, "Ammo_556") == 0)
				{

					pstmt->setString(2, test_id_array[idx_data.ammo_556_idx]);
				}

				else if (strcmp(t_magazine->ammo_id, "Ammo_762") == 0)
				{

					pstmt->setString(2, test_id_array[idx_data.ammo_762_idx]);
				}

				else if (strcmp(t_magazine->ammo_id, "Ammo_919") == 0)
				{
					pstmt->setString(2, test_id_array[idx_data.ammo_919_idx]);
				}

				pstmt->setInt(3, t_magazine->capacity);
				pstmt->executeUpdate();

			}

			else if (strcmp(test_type_array[i], "Food") == 0)
			{
				test_dataobject::food* t_food = reinterpret_cast<test_dataobject::food*>(tmp_data);

				pstmt = con->prepareStatement("INSERT INTO item_food(item_id, food_type,energy) VALUES(? , ? , ?)");
				pstmt->setString(1, test_id_array[i]);
				pstmt->setString(2, t_food->type);
				pstmt->setInt(3, t_food->energy);
				pstmt->executeUpdate();

			}

			else
			{
				__debugbreak();
			}
		}
		if (pstmt != NULL)
		{
			delete pstmt;
		}
	}

	catch (sql::SQLException& e) {
		printf("sql error code %d\n", e.getErrorCode());
		__debugbreak();
	}


}

void DBHandler::JsonMapData_insert(itemManager* _itemManager)
{
	con->setSchema("inventory");
	sql::PreparedStatement* pstmt = NULL;
	
	try
	{

		for (std::list<itemManager::item>::iterator iter = _itemManager->itemList.begin(); iter != _itemManager->itemList.end(); iter++)
		{
			if (iter->user_assigned)
			{
				continue;
			}

				pstmt = con->prepareStatement("INSERT INTO map_item(item_id, isRotated, inContainer, container_id, matrix_pos_x, matrix_pos_y, world_pos_x, world_pos_y, world_pos_z, world_rot_x, world_rot_y, world_rot_z) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
				pstmt->setString(1, iter->id);
				pstmt->setBoolean(2, iter->rotated);
				pstmt->setBoolean(3, iter->containered);
				pstmt->setString(4, iter->containerid);
				pstmt->setInt(5, iter->matrix_pos.x);
				pstmt->setInt(6, iter->matrix_pos.y);
				pstmt->setDouble(7, iter->position.x);
				pstmt->setDouble(8, iter->position.y);
				pstmt->setDouble(9, iter->position.z);
				pstmt->setDouble(10, iter->rotation.x);
				pstmt->setDouble(11, iter->rotation.y);
				pstmt->setDouble(12, iter->rotation.z);
				
				pstmt->executeUpdate();

		}

		if (pstmt != NULL)
		{
			delete pstmt;
		}
	}

	catch (sql::SQLException& e) {
		printf("sql error code %d\n", e.getErrorCode());
		__debugbreak();
	}


}

void DBHandler::JsonUserInvetory_insert(itemManager* _itemManager, int param_userid)
{

	con->setSchema("inventory");
	sql::PreparedStatement* pstmt = NULL;

	try
	{

		for (std::list<itemManager::item>::iterator iter = _itemManager->itemList.begin(); iter != _itemManager->itemList.end(); iter++)
		{
			if (!iter->user_assigned)
			{
				continue;
			}

		

			pstmt = con->prepareStatement("INSERT INTO players_inventory(user_id, item_id, isRotated, matrix_pos_x, matrix_pos_y, slot_panel) VALUES(?, ?, ?, ?, ?, ?)");
			pstmt->setInt(1, param_userid);
			pstmt->setString(2, iter->id);
			pstmt->setBoolean(3, iter->rotated);
			pstmt->setInt(4, iter->matrix_pos.x);
			pstmt->setInt(5, iter->matrix_pos.y);
			pstmt->setInt(6, iter->slot_paneltype);


			pstmt->executeUpdate();

		}

		if (pstmt != NULL)
		{
			delete pstmt;
		}
	}

	catch (sql::SQLException& e) {
		printf("sql error code %d\n", e.getErrorCode());
		__debugbreak();
	}




}

void DBHandler::Close()
{
	if (con != NULL)
	{
		try {

			con->close();
			driver->threadEnd();
			delete con;

		}

		catch (sql::SQLException& e) {
			printf("sql error code %d\n", e.getErrorCode());
			__debugbreak();
		}
	}

	for (int i = 0; i < extradata_queue.size(); i++)
	{
		void* data_ptr = extradata_queue.front();
		extradata_queue.pop();

		delete data_ptr;

	}
}


DBHandler::~DBHandler()
{
	
	
}
