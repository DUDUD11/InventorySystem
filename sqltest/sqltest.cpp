#include <stdio.h>
#include <iostream>
#include "GameServer.h"

#define SERVERPORT 12345

using namespace std;

int main()
{
	GameServer GameServer;
	wprintf(L"MainThread Start\n");
	GameServer.Start(L"127.0.0.1", SERVERPORT, 1, 1, true, 100);

	while (true)
	{
		printf("########################################\n");
		printf("AcceptCount : %d\n", GameServer.acceptCount);
		printf("recvTPS : %d\n", GameServer.recvTPS);
		printf("########################################\n");
		

		Sleep(1000);
	}

	return 0;
}
