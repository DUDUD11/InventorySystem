#include "NetworkServer.h"



UINT WINAPI NetworkServer::_WorkerThread(PVOID p)
{
	NetworkServer* pObj = (NetworkServer*)p;
	pObj->WorkerThread();

	return 0;
}

UINT WINAPI NetworkServer::_AcceptThread(PVOID p)
{
	NetworkServer* pObj = (NetworkServer*)p;

	pObj->AcceptThread();

	return 0;
}

UINT WINAPI NetworkServer::_SendThread(PVOID p)
{
	NetworkServer* pObj = (NetworkServer*)p;

	pObj->SendThread();

	return 0;
}

VOID NetworkServer::SendThread()
{
	

	SHORT sendThreadIdx = InterlockedIncrement16(&mSendThreadIdx) - 1; // 0���� ����

	while (!ThreadExit)
	{
	
			int cur_time = (int)timeGetTime();

			for (int SessionIndex = sendThreadIdx; SessionIndex < mMaxClientNum; SessionIndex += mSendThreadNum-1)
			{
			

				stSESSION* pSession = (stSESSION*)mSessionPool->At(SessionIndex);

				if (pSession->socket == null_socket) continue;



				if (pSession->sendFlag)
				{
					continue;
				}
				pSession->sendFlag = true;

				int UseCnt = pSession->sendQ.GetUsingCount();
				if (UseCnt == 0)
				{
					pSession->sendFlag = false;
	
					continue;
				}

				WSABUF dataBuf[WSABUFSIZE];

				for (int i = 0; i < UseCnt; i++)
				{
					if (!pSession->sendQ.Dequeue(&pSession->SendBuf[i]))
					{
						break;
					}


					dataBuf[i].buf = pSession->SendBuf[i].Packet->GetBufferPtr();
					dataBuf[i].len = pSession->SendBuf[i].Packet_Size;
				}



				InterlockedIncrement16(&pSession->IOCount);

				pSession->SendCnt = UseCnt;

				INT sendRetval = WSASend(pSession->socket, dataBuf, UseCnt, NULL, NULL, &pSession->sendOverlapped, NULL);
		
				if (sendRetval == SOCKET_ERROR)
				{
					int err_code = WSAGetLastError();
					if (err_code != WSA_IO_PENDING)
					{
						TryLeaveSession(pSession);
					}
					else
					{
						if (pSession->socket == INVALID_SOCKET)
						{
							CancelIoEx((HANDLE)pSession->socket, NULL);
						}
					}
				}
			}

	}
}


VOID NetworkServer::WorkerThread()
{
	

	mSerialBuffer->Buffer_init();

	

	while (1)
	{
		stSESSION* pSession = nullptr;
		OVERLAPPED* pOverlapped = nullptr;
		DWORD transferredBytes = 0;
		BYTE abortedFlag = 0;

		bool signal = GetQueuedCompletionStatus(mIocpHandle, &transferredBytes, (PULONG_PTR)&pSession, &pOverlapped, INFINITE);
		
		if (!pOverlapped && transferredBytes == 0 && !pSession)
		{
			return;
		}

		if (pOverlapped == nullptr)
		{
			PostQueuedCompletionStatus(mIocpHandle, NULL, NULL, NULL);
			break;
		}

		if (transferredBytes == 0)
		{		
			TryLeaveSession(pSession);

			continue;
		}
		/*
		else if (pOverlapped->Internal == ERROR_OPERATION_ABORTED)
		{
		
		}
		*/
		else if (pOverlapped == &pSession->recvOverlapped)
		{
		
			RecvProc(transferredBytes, pSession);
		}
		else if (pOverlapped == &pSession->sendOverlapped)
		{			
			SendProc(pSession);
		}

		OnWorkerThreadEnd();

	}
	return;
}


VOID NetworkServer::RecvProc(SHORT transferredBytes, stSESSION* pSession)
{
	// recvQ�� transferredBytes ��ŭ ������ �Ǿ���.

	pSession->recvQ.MoveRear(transferredBytes);

	while (1)
	{


		INT useSize = pSession->recvQ.GetUseSize();

		//��Ʈ��ũ ����� ũ�⺸�� ���� ��� breakŸ�� �ٽ� ���

		

			stHEADER header;

			if (useSize < sizeof(header))
			{
				// ���� �پȿ�
				break;
			}

			pSession->recvQ.Peek((char*)&header, sizeof(header));

			// ���� �پȿ�
			if (useSize < header.length)
			{
				break;
			}

			char* SerialBufferInsertPoint = (char*)(mSerialBuffer->GetBufferPtr() + mSerialBuffer->GetDataSize());

			pSession->recvQ.Dequeue(SerialBufferInsertPoint, header.length);
			mSerialBuffer->MoveWritePos(header.length);

			OnRecv(pSession->sessionID, mSerialBuffer);
		}
	

	InterlockedIncrement64((LONG64*)&recvTPS);
	InterlockedDecrement16(&pSession->IOCount);
	RecvPost(pSession);
}

VOID NetworkServer::SendProc(stSESSION* pSession)
{
	// Send�� �Ϸ�Ǿ����� SendPacketpool�� �ִ� ���۸� �Ҵ����������ش�.

	
	for (INT i = 0; i < pSession->SendCnt; i++)
	{
		pSession->SendPacketPool->Free((CPacket*)pSession->SendBuf[i].Packet);
	}
	

	pSession->SendCnt = 0;
	pSession->sendFlag = false;

	InterlockedDecrement16(&pSession->IOCount);

	OnSend(pSession->sessionID);
}

VOID NetworkServer::AcceptThread()
{

	while (1)
	{

		SOCKADDR_IN clientAddr;

		INT size = sizeof(clientAddr);

		SOCKET clientSocket = accept(mListenSocket, (SOCKADDR*)&clientAddr, &size);
		
		
		{
			
			acceptCount++;
			acceptTPS++;

			if (clientSocket == INVALID_SOCKET)
			{
				INT errCode = WSAGetLastError();

				//���������� ����Ǿ���
				if (errCode == WSAECONNRESET)
					continue;
				else
				{
					break;
				}
			}

			WCHAR clientIP[46];

			InetNtop(AF_INET, (const VOID*)&clientAddr.sin_addr.s_addr, clientIP, sizeof(clientIP));

			BYTE acceptFlag = OnConnectionRequest(clientIP, clientAddr.sin_port);
			if (!acceptFlag)
			{
				continue;
			}

			if (mCurrentClientCount == mMaxClientNum)
			{
				closesocket(clientSocket);
				continue;
			}

			InterlockedIncrement(&mCurrentClientCount);
			stSESSION* pSession = mSessionPool->Alloc();

			CreateIoCompletionPort((HANDLE)clientSocket, mIocpHandle, (ULONG_PTR)pSession, NULL);
			//1���� ����
			pSession->sessionID = InterlockedIncrement(&mClientID);
			SessionMap.insert(pSession->sessionID, pSession);
			pSession->socket = clientSocket;
			
	
			if (pSession->SendPacketPool == nullptr)
			{
				pSession->SendPacketPool = new CMemoryPool<CPacket>(Packet_Per_Client);
			}

			memset(&pSession->recvOverlapped, 0, sizeof(OVERLAPPED));
			memset(&pSession->sendOverlapped, 0, sizeof(OVERLAPPED));
		
			pSession->IOCount = 0;
			memset(pSession->SendBuf, 0, WSABUFSIZE * sizeof(st_Packet));

			OnClientJoin(pSession->sessionID);
			RecvPost(pSession);
			pSession->sendFlag = false;
			pSession->SendCnt = 0;

		}
	}

	return;
}

VOID NetworkServer::RecvPost(stSESSION* pSession)
{

	DWORD recvQFreeSize = pSession->recvQ.GetFreeSize();
	DWORD recvQDirectEnqueueSize = pSession->recvQ.DirectEnqueueSize();

	INT wsaBufCount = 1;
	WSABUF dataBuf[2];

	dataBuf[0].buf = pSession->recvQ.GetRearBufferPtr();
	dataBuf[0].len = recvQDirectEnqueueSize;

	if (recvQFreeSize > recvQDirectEnqueueSize)
	{
		wsaBufCount++;
		dataBuf[1].buf = pSession->recvQ.GetFrontBufferPtr();
		dataBuf[1].len = recvQFreeSize - recvQDirectEnqueueSize;
	}

	DWORD flags = 0;
	ZeroMemory(&pSession->recvOverlapped, sizeof(pSession->recvOverlapped));
	InterlockedIncrement16(&pSession->IOCount);
	INT retval = WSARecv(pSession->socket, dataBuf, wsaBufCount, NULL, &flags, &pSession->recvOverlapped, NULL);



	if (retval == SOCKET_ERROR)
	{
		int p = WSAGetLastError();
		

		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			//WSA_IO_PENDING�̶��
			//�̹� Recv�� �ɷ��ִ�. Disconnect�� WSARecv�� �ɷ��ִ� ����
			//�ɷȴٸ� Recv�� ������������Ѵ�.
			if (pSession->socket == INVALID_SOCKET)
			{
				//I/Oī��Ʈ ���Ҵ� ABORTED���� ������ �ش�.
				CancelIoEx((HANDLE)pSession->socket, NULL);
			
			}
		}
		else
		{
			TryLeaveSession(pSession);

		}
	}
}




BYTE NetworkServer::SendPacket(UINT64 sessionID, CPacket* packet)
{

	

	stSESSION* pSession;
	st_Packet packet_wrapped;
	

	pSession = FindSession(sessionID);



	if (pSession->sessionID != sessionID)
	{
		__debugbreak();
	}


	if (pSession == NULL)
	{
		__debugbreak();
		return FALSE;
	}
	/*
	if (UserMap->find(pSession->sessionID))
	{
		return FALSE;
	}
	*/
	

	packet_wrapped.Packet = packet;
	packet_wrapped.Packet_Size = packet->GetDataSize();



	InterlockedIncrement64((LONG64*)&sendTPS);
	
	pSession->sendQ.Enqueue(packet_wrapped);

	return TRUE;
}


NetworkServer::stSESSION* NetworkServer::FindSession(UINT64 _sessionID)
{

	stSESSION* pSession;

	if (!SessionMap.at(_sessionID, pSession))
	{
		return nullptr;
	}
	return pSession;
}

/*
UINT64 NetworkServer::MakeSessionID(USHORT sessionArrayIndex, UINT64 sessionUniqueID)
{
UINT64 sessionID = 0;

sessionID = sessionUniqueID;

sessionID = sessionID << 16;

sessionID += sessionArrayIndex;

return sessionID;
}
*/




BYTE NetworkServer::ReleaseSession(stSESSION* pSession)
{
	

	OnClientLeave(pSession->sessionID);	

	pSession->recvQ.ClearBuffer();
	pSession->sendQ.Clear();

	pSession->SendPacketPool->Clear();

	
	pSession->sendFlag = true;

	

	InterlockedDecrement(&mCurrentClientCount);
	

	closesocket(pSession->socket);

	pSession->socket = null_socket;
	pSession->IOCount = 0;

	memset(pSession->SendBuf, 0, sizeof(st_Packet) * WSABUFSIZE);

	//UserMap->erase(pSession->sessionID);
	SessionMap.erase(pSession->sessionID);
	mSessionPool->Free(pSession);

	return TRUE;
}

void NetworkServer::TryLeaveSession(stSESSION* pSession)
{

	int ret = InterlockedDecrement16(&pSession->IOCount);

	if (ret == 0 && UserMap->insert_notfind(pSession->sessionID))
	{
		UserMap->insert(pSession->sessionID);
		ReleaseSession(pSession);
	}
	
}

CPacket* NetworkServer::Alloc_Packet(UINT64 SessionID)
{
	
	stSESSION* session = FindSession(SessionID);
	if (session == nullptr)
	{
		return nullptr;
	}

	else if (session->SendPacketPool == nullptr)
	{
		return nullptr;
	}
	else if (session->socket == null_socket)
	{
		return nullptr;
	}

	CPacket* ret = session->SendPacketPool->Alloc();

	return ret;
}

NetworkServer::NetworkServer()
	: acceptCount(0),
	acceptTPS(0),
	recvTPS(0),
	sendTPS(0),
	mListenSocket(INVALID_SOCKET),
	mServerPort(NULL),
	mMaxClientNum(0),
	mCurrentClientCount(0),
	mWorkerThreadNum(0),
	mSendThreadNum(0),
	mSendThreadIdx(0),
	mClientID(0),
	mIocpHandle(INVALID_HANDLE_VALUE),
	mThreadArr(nullptr),
	mSessionPool(nullptr),
	UserMap(nullptr),
	mSerialBuffer(nullptr),
	ThreadExit(false)
{}

VOID NetworkServer::Init(SHORT port, INT workerThreadNum, INT sendThreadNum, INT maxClientNum)
{
	
	//InitializeSRWLock(&Send_Lock);
	mServerPort = port;
	mWorkerThreadNum = workerThreadNum;
	mSendThreadNum = sendThreadNum;
	mMaxClientNum = maxClientNum;
	mThreadArr = new HANDLE[workerThreadNum + sendThreadNum + 1];
	mSessionPool = new CMemoryPool<NetworkServer::stSESSION>(maxClientNum, true);


	for (int i = 0; i < maxClientNum; i++)
	{
		stSESSION* Session = (stSESSION*)mSessionPool->At(i);
		Session->socket = null_socket;
		Session->SendPacketPool = nullptr;
		Session->sendFlag = true;
		
	}

	UserMap = new Concurrent_Unordered_set<UINT64>();
	mSerialBuffer = new CPacket_TLS(mWorkerThreadNum);


}



BYTE NetworkServer::Start(const WCHAR* ip, SHORT port, INT workerThreadNum, INT sendThreadNum, BYTE nagleOption, INT maxClientNum)
{
	Init(port, workerThreadNum, sendThreadNum, maxClientNum);

	INT retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return FALSE;
	}

	mListenSocket = socket(AF_INET, SOCK_STREAM, NULL);

	if (mListenSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, ip, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);

	retval = bind(mListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		return FALSE;
	}

	//���̱� �ɼ�
	BYTE optval = nagleOption;
	retval = setsockopt(mListenSocket, IPPROTO_TCP, TCP_NODELAY, (const CHAR*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		return FALSE;
	}

	//RST ����
	linger lingerOpt;
	lingerOpt.l_onoff = 1;  // ���ſɼ� 1 : on, 0 : off
	lingerOpt.l_linger = 0; // RST ������ ���� 0���� ����

	retval = setsockopt(mListenSocket, SOL_SOCKET, SO_LINGER, (const CHAR*)&lingerOpt, sizeof(lingerOpt));
	if (retval == SOCKET_ERROR)
	{

		return FALSE;
	}

	//�۽Ź��� 0����
	//INT bufSize = 0;
	//socklen_t len = sizeof(bufSize);
	//setsockopt(mListenSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&bufSize, sizeof(bufSize));

	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (mIocpHandle == NULL)
	{
		return FALSE;
	}

	retval = listen(mListenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		return FALSE;
	}

	INT threadCount = 0;

	for (threadCount = 0; threadCount < workerThreadNum; threadCount++)
	{
		mThreadArr[threadCount] = (HANDLE)_beginthreadex(NULL, NULL, _WorkerThread, (PVOID)this, NULL, NULL);
	}

	for (; threadCount < sendThreadNum + workerThreadNum; threadCount++)
	{
		mThreadArr[threadCount] = (HANDLE)_beginthreadex(NULL, NULL, _SendThread, (PVOID)this, NULL, NULL);
	}

	mThreadArr[threadCount] = (HANDLE)_beginthreadex(NULL, NULL, _AcceptThread, (PVOID)this, NULL, NULL);


	return true;
}

VOID NetworkServer::Stop()
{
	//���ο� �����ڸ� �������� �������� �ݱ�, ���⼭ AcceptThread�� �����ϰ� �ȴ�.
	closesocket(mListenSocket);

	for (int clientNum = 0; clientNum < mMaxClientNum; clientNum++)
	{
		stSESSION* pSession = (stSESSION*)mSessionPool->At(clientNum);

		if (pSession->socket != NULL)
		{
			Disconnect(pSession->sessionID);
		}
	}

	while (1)
	{
		//������ ���� �ݳ���
		if (mSessionPool->GetUseCount() == 0)
			break;

		Sleep(100);
	}

	//SendThread���� Signal
	ThreadExit = true;

	//WorkerThread�� ��������� ���� ������ �ɰ���
	PostQueuedCompletionStatus(mIocpHandle, NULL, NULL, NULL);

	//mWorkerThreadCreateNum + 1 = ��Ŀ�����尹�� + ���Ʈ������
	for (int threadIndex = 0; threadIndex < mWorkerThreadNum + mSendThreadNum + 1; threadIndex++)
	{
		DWORD retval = WaitForSingleObject(mThreadArr[threadIndex], 1000);
		if (retval == WAIT_TIMEOUT)
		{
			TerminateThread(mThreadArr[threadIndex], -1);
		}
	}

	//������迭 ����
	delete[] mThreadArr;
	//���ǹ迭 ����
	if (mSessionPool != NULL)
	{
		delete mSessionPool;
	}
	if (UserMap != NULL)
	{
		delete UserMap;
	}

	if (mSerialBuffer != NULL)
	{
		delete mSerialBuffer;
	}
}

INT NetworkServer::GetClientCount()
{
	return mCurrentClientCount;
}

UINT64 NetworkServer::GetAcceptCount()
{
	return acceptCount;
}

BYTE NetworkServer::Disconnect(UINT64 sessionID)
{

	//UserMap->insert(sessionID);

	stSESSION* pSession;

	pSession = FindSession(sessionID);
	if (pSession == nullptr)
	{
		//__debugbreak();
		return FALSE;
	}

	CancelIoEx((HANDLE)pSession->socket, NULL);

	TryLeaveSession(pSession);

	return TRUE;
}

VOID NetworkServer::OnSend(UINT64) {}

VOID NetworkServer::OnWorkerThreadBegin(UINT64, DWORD, BYTE) {}

VOID NetworkServer::OnWorkerThreadEnd() {}

VOID NetworkServer::OnSendThreadLooP()
{}
