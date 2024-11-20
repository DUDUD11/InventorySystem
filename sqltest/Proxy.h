#pragma once
#include "ProxyCaller.h"

class Proxy
{

private:
	Proxy();
	~Proxy();

	ProxyCaller* Obj;

public:

	static inline Proxy* GetInstance()
	{
		static Proxy proxy;
		return &proxy;
	}


	void Set_Server(ProxyCaller* server)
	{
		this->Obj = server;

	}


	void Example(unsigned __int64   SessionID, int type, int ID, unsigned char   Direction, unsigned short X, unsigned short Y);

	
};