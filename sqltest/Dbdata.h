#pragma once
#include "JsonParser.h"
#include "DBHandler.h"
#include "itemManager.h"
class Dbdata : public JsonParser
{

private:
	DBHandler* m_dbHandler;

public:
	void JsonLoad(void* container);

	void SetDBHandler(DBHandler* handler_ptr);



};