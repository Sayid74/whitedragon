#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "content.h"

#pragma comment(lib, "ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 1860

typedef struct
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	CHAR Buffer[DEFAULT_BUFLEN];
	DWORD BytesRECV;
}PER_IO_DATA, *LPPER_IO_DATA;

typedef struct
{
	SOCKET Socket;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;


DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortIO)
{
	HANDLE CompletionPort = (HANDLE)CompletionPortIO;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD SendOuted;
	DWORD Flags;

	while (TRUE)
	{
		if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred
			, (LPDWORD)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE) == 0)
		{
			printf("GetQueueCompletionStatus failed with error %d\n", GetLastError());
			return 0;
		}
		if (BytesTransferred == 0)
		{
			printf("Closing socket %d", PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("ClosingSocket() failed with error %d\n", WSAGetLastError());
				return 0;
			}
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}
		if (PerIoData->BytesRECV == 0)
		{
			PerIoData->BytesRECV = BytesTransferred;
		}
		printf("\nBytes received: %d\n", BytesTransferred);

		OVERLAPPED Overlapped = PerIoData->Overlapped;
		DWORD transfered = 0;
		DWORD flags = 0;
		WSAGetOverlappedResult(PerHandleData->Socket, &Overlapped, &transfered, TRUE, &flags);

		PerIoData->BytesRECV = 0;
		Flags = 0;
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->DataBuf.len = DEFAULT_BUFLEN;
		PerIoData->DataBuf.buf = new char[DEFAULT_BUFLEN] {"hello world!"};

		//if (WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendOuted, 0, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		//{
		//	if (WSAGetLastError() != ERROR_IO_PENDING)
		//	{
		//		printf("WSARecv() failed with error %d\n", WSAGetLastError());
		//	}
		//}

	}
}

int _main(int argc, char** argv[])
{
	SOCKADDR_IN InternetAddr;
	SOCKET ServerSocket = INVALID_SOCKET;
	SOCKET AcceptSocket = INVALID_SOCKET;
	HANDLE CompletionPort;
	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD ThreadID;
	WSADATA wsaData;
	DWORD Ret;

	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		printf("wSAStartup failed with error %d \n", Ret);
		return -1;
	}
	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
		return -1;
	}
	GetSystemInfo(&SystemInfo);
	for (DWORD i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE ThreadHandle;
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompletionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d\n", GetLastError());
			return -1;
		}
		CloseHandle(ThreadHandle);
	}
	if ((ServerSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocketW() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(DEFAULT_PORT);
	if (bind(ServerSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		printf("", WSAGetLastError());
		return -1;
	}
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		printf("listen() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	printf("TCP server starting\n");
	sockaddr_in addrClient;
	int addrClientlen = sizeof(sockaddr_in);
	while (TRUE)
	{
		if ((AcceptSocket = WSAAccept(ServerSocket, (sockaddr FAR*)&addrClient, &addrClientlen, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept() failed with error %d\n", WSAGetLastError());
			return -1;
		}
		char _addr0[17] = {0};
		PCTSTR _ret = inet_ntop(AF_INET, &(addrClient.sin_addr)  , _addr0, 17);
		printf("\n recieve new connection: %s \n", _ret);
//		printf("\n recieve new connection: %s \n", inet_ntoa(addrClient.sin_addr));
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
			return -1;
		}
		PerHandleData->Socket = AcceptSocket;

		if (CreateIoCompletionPort((HANDLE)AcceptSocket, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			printf("CreateIoCompletionProt() failed with error %d\n", GetLastError());
			return -1;
		}
		if ((PerIoData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
			return -1;
		}
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DEFAULT_BUFLEN;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		Flags = 0;

		if (WSARecv(AcceptSocket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
			&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				return -1;
			}
		}
		else
		{
			printf("WSARecv ocurred! \n");
		}
	}
	return 0;
}