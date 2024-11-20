#pragma once
#include <list>


class itemManager
{
public:

	struct Vector2d
	{
		double x;
		double y;

		Vector2d(double _x,double  _y) :x(_x), y(_y)
		{
		}

		Vector2d() :x(0.0f), y(0.0f)
		{
		}

	};

	struct Vector3d
	{
		double x;
		double y;
		double z;
		Vector3d(double _x, double  _y, double _z) :x(_x), y(_y),z(_z)
		{
		}

		Vector3d():x(0.0f),y(0.0f),z(0.0f)
		{
		}

	};

	enum class playeritemStatus
	{
		Equip = 0,
		Backpack = 1
	};

	enum item_type_len {
		id = 50,
		type = 10,
		slot_type = 20,

	};

	enum item_type
	{
		item_Weapon, // Weapon
		Food,
		Cloth,
		Money,
		Ammo,
		Magazine,
		Special


	};

	enum slot_type
	{
		General,
		Headset,
		Headwear,
		FaceCover,
		TacticalRig,
		BodyArmor,
		Backpack,
		Pistol,
		knife,
		slot_Weapon // Weapon

	};

	enum spawn_frequency
	{
		one = 1,
		five = 5,
		ten = 10,
		twenty_five = 25,
		fifty = 50


	};

	struct item
	{
		char id[item_type_len::id];
		char type[item_type_len::type];
		char slot_type[item_type_len::slot_type];
		char containerid[item_type_len::id];
		
		Vector2d slot_pos;
		Vector2d matrix_pos;
		Vector3d position;
		Vector3d rotation;

		int slot_paneltype;
		bool rotated;
		bool user_assigned;
		bool containered;

		item():slot_paneltype(0),rotated(false),user_assigned(false),containered(false)
		{
			memset(id, 0, item_type_len::id);
			memset(type, 0, item_type_len::type);
			memset(slot_type, 0, item_type_len::slot_type);
			memset(containerid, 0, item_type_len::id);

		}
		

	};

	std::list<item> itemList;

	bool item_delete(char* item_id);
	void item_insert(item& item);
	item& item_find(const char* item_id);


};