#pragma once
#include "Serial_Buffer.h"

class ProxyCaller
{
public:

	virtual ~ProxyCaller() {};

	virtual CPacket* Alloc_Packet(UINT64) = 0;

	virtual BYTE SendPacket(UINT64, CPacket*) = 0;

};
