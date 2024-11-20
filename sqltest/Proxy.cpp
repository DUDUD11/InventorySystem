#include "Proxy.h"

Proxy::~Proxy()
{

}

Proxy::Proxy():Obj(nullptr)
{

}
/*
void Proxy::Example(unsigned __int64   SessionID, unsigned short type, int ID, unsigned char   Direction, unsigned short X, unsigned short Y)
{
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	short pack_len = sizeof(ID) + sizeof(Direction);
	*pack << pack_len << type << ID << Direction << X << Y;
	Obj->SendPacket(SessionID, pack);
}
*/