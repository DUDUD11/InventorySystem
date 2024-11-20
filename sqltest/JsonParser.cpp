#include "JsonParser.h"

void JsonParser::JsonFileOpen(const char* file_url)
{
	if (!SetFile(file_url))
	{
		__debugbreak();
	}

	fseek(File_ptr, 0, SEEK_END);

	int file_size = ftell(File_ptr);
	int buffer_size = file_size + 1;

	fseek(File_ptr, 0, SEEK_SET);

	int pos = ftell(File_ptr);

	json = new char[buffer_size];

	int read_cnt = fread_s(json, buffer_size, file_size, 1, File_ptr);

	if (read_cnt != 1)
	{
		__debugbreak();
	}

	fclose(File_ptr);

}

JsonParser::~JsonParser()
{
	if (json != NULL)
	{
		delete[] json;
	}
}


bool JsonParser::SetFile(const char* _file_ptr)
{
	
	fopen_s(&File_ptr, _file_ptr, "r");
	if (File_ptr == NULL)
	{
		return false;
	}

	return true;
}
