#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include "ConCurrent_HashMap.h"
#include "ConCurrent_HashSet.h"
#include "RingBuffer.h"
#include "LockFreeQueue.h"
#include "Serial_Buffer.h"
#include "MemoryPoolTLS.h"


#define WSABUFSIZE 200
#define TimeOut 30000
#define Packet_Length_Limit 1000
#define Packet_Per_Client 200
#define null_socket -1


class NetworkServer
{
protected:


#pragma pack(1)
	struct stHEADER
	{
		USHORT length;
	};
#pragma pack(pop)


	CPacket* Alloc_Packet(UINT64);

public:
	UINT64 mCurrentClientCount; // ���� Ŭ���̾�Ʈ ������

	NetworkServer();

	VOID Init(SHORT port, INT workerThreadNum, INT sendThreadNum, INT maxClientNum);

	BYTE Start(const WCHAR* ip, SHORT port, INT workerThreadNum, INT sendThreadNum, BYTE nagleOption, INT maxClientNum);

	BYTE SendPacket(UINT64, CPacket*);

	BYTE Disconnect(UINT64);

	INT GetClientCount();

	UINT64 GetAcceptCount();

	VOID Stop();

	//Accept�� ����ó�� �Ϸ� �� ȣ��
	virtual VOID OnClientJoin(UINT64) = 0;

	//Release�� ȣ��
	virtual VOID OnClientLeave(UINT64) = 0;

	//accept����, return false, return true;
	//false �� Ŭ���̾�Ʈ �ź�, true �� ���� ���
	virtual bool OnConnectionRequest(PWCHAR, SHORT) = 0;

	//��Ŷ ���� �Ϸ� ��
	virtual VOID OnRecv(UINT64, CPacket_TLS*) = 0;

	//��Ŷ �۽� �Ϸ� ��
	virtual VOID OnSend(UINT64);
	//
	//��Ŀ������ GQCS �ٷ� �ϴܿ��� ȣ��
	virtual VOID OnWorkerThreadBegin(UINT64, DWORD, BYTE);
	//
	//��Ŀ������ 1���� ���� ��
	virtual VOID OnWorkerThreadEnd();

	//OnSendThreadLooP();
	virtual VOID OnSendThreadLooP();

	UINT64 acceptCount = 0; // accept �ϸ� �����ϳ�
	UINT64 acceptTPS = 0; // accept �ϸ� �����ϳ�
	UINT64 recvTPS = 0; // recvproc ����
	UINT64 sendTPS = 0; // sendpacket ����

private:


	struct st_Packet
	{
		CPacket* Packet;
		int Packet_Size;
	};


	__declspec(align(64))struct stSESSION
	{
		UINT64 sessionID; // ���� ID
		SOCKET socket; // ����

		__declspec(align(64))OVERLAPPED recvOverlapped;
		OVERLAPPED sendOverlapped;
	
		RingBuffer recvQ;

		CLockFreeQueue<st_Packet> sendQ;
		CMemoryPool<CPacket>* SendPacketPool;
		st_Packet SendBuf[WSABUFSIZE];
		volatile BYTE sendFlag;
		volatile SHORT SendCnt;

		SHORT IOCount;
		

	};




private:

	SOCKET mListenSocket; // ��������

	UINT64 mClientID; // �ϳ��� ���������
	INT mMaxClientNum; // ���� Ŭ���̾�Ʈ ��
	INT mWorkerThreadNum; // worker ������ ����
	INT mSendThreadNum;
	SHORT mSendThreadIdx;
	SHORT mServerPort; // ������Ʈ
	HANDLE mIocpHandle; // �ڵ�
	HANDLE* mThreadArr; // �������ڵ� �����
	CPacket_TLS* mSerialBuffer; // TLS
	BOOL ThreadExit;



	//SRWLOCK Send_Lock;


	CMemoryPool<stSESSION>* mSessionPool;
	//stSESSION* mSessionArray;
	Concurrent_Unordered_set<UINT64>* UserMap;
	Concurrent_Unordered_map<UINT64, stSESSION*> SessionMap;

	static UINT WINAPI _WorkerThread(PVOID p);
	VOID WorkerThread();

	static UINT WINAPI _AcceptThread(PVOID p);
	VOID AcceptThread();

	static UINT WINAPI _SendThread(PVOID p);
	VOID SendThread();

	VOID RecvProc(SHORT, stSESSION*);
	VOID SendProc(stSESSION*);

	VOID RecvPost(stSESSION*);

	stSESSION* FindSession(UINT64);
	//	UINT64 MakeSessionID(USHORT, UINT64);

	BYTE ReleaseSession(stSESSION*);

	//	BYTE AcquireSession(UINT64, stSESSION*);

	VOID TryLeaveSession(stSESSION*);




};

