#pragma once

#include <stdio.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class JsonParser
{

private:
	bool SetFile(const char* file_url);

protected:
	FILE* File_ptr;
	char* json = NULL;

public:
	void JsonFileOpen(const char* file_url);
	virtual void JsonLoad(void* container) = 0;
	virtual ~JsonParser();


};