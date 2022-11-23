//------------------------------------------
//author : fly snack
//date : 2009/10/08
//email : 358657363@qq.com
//------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "../common/proto.h"

#pragma comment(lib, "wsock32.lib")

typedef struct tagMsgRspInfo
{
	int client;
	int msgid;
}MsgRspInfo;

typedef struct tagClientConnectInfo
{
	SOCKET client;
	SOCKET solider;
}ClientConnectInfo;

DWORD WINAPI JoinThread(LPVOID lpParam)
{
	int result = 0;

	ClientConnectInfo* pClientConnInfo = (ClientConnectInfo*)lpParam;
	SOCKET client = pClientConnInfo->client;
	SOCKET solider = pClientConnInfo->solider;

	char msg[1024];
	memset(msg, 0, 1024);
	int msgLength = 0;
	int recvFlags = 0;
	int sendServerFlags = 0;

	MsgRspInfo msgRspInfo;
	MsgClientClosedRsp msgClientClosedRsp;
	MsgRspRegister msgRspRegister;
	MsgRspLogin msgRspLogin;
	MsgRspRoleRegister msgRspRoleRegister;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp;

	RoleInfo roleInfoRegister;

	int sendRspFlags = 0;

	while(true)
	{
		//Sleep(1000);
		//send(serverSolider, "server recv gate", 20, sendServerFlags);

		result = recv(solider, msg, 1024, MSG_PEEK);
		msgLength = result;
		result = recv(solider, msg, msgLength, recvFlags);
		std::cout << "server recv gate recv data from server solider result : " << result << ", error : " << WSAGetLastError() << ", solider : " << solider << std::endl;
		if (result != 0)
		{
			if (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)
			{
				Sleep(10000);
				continue;
			}
		}

		memcpy(&msgRspInfo, msg, sizeof(msgRspInfo));

		switch(msgRspInfo.msgid)
		{
		case MSG_CLIENT_CLOSED_RSP:
			memcpy(&msgClientClosedRsp, msg, sizeof(msgClientClosedRsp));
			break;
		case MSG_RSP_REGISTER:
			memcpy(&msgRspRegister, msg, sizeof(msgRspRegister));		//parse the msg from recv(solider,msg,..) to struct
			//direct return to client, parse by client, for display, use statement MSG_REQ_REGISTER case to; already to use this way, determine except the way to transport net data
			//itoa(msgRspRegister.msgid, strMsgRspRegisterMsgID, 10);
			//itoa(msgRspRegister.result, strMsgRspRegisterResult, 10);
			//str += strMsgRspRegisterMsgID;
			//str += ",";
			//str += strMsgRspRegisterResult;

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspRegister), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
		case MSG_RSP_LOGIN:
			memcpy(&msgRspLogin, msg, sizeof(msgRspLogin));
			if (msgRspLogin.needCloseClient > 0)
			{
				shutdown(msgRspLogin.needCloseClient, 0x02);
				closesocket(msgRspLogin.needCloseClient);
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspLogin), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
		case MSG_RSP_ROLE_REGISTER:
			memcpy(&msgRspRoleRegister, msg, sizeof(msgRspRoleRegister));
			memcpy(&msgRspRoleRegisterTemp, msg, sizeof(msgRspRoleRegisterTemp));
			for (int i = 0; i < msgRspRoleRegisterTemp.roleCount; i++)
			{
				memcpy(&roleInfoRegister, msg + sizeof(msgRspRoleRegisterTemp) + i * sizeof(RoleInfo), sizeof(RoleInfo));
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspRoleRegisterTemp) + msgRspRoleRegisterTemp.roleCount * sizeof(RoleInfo), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
			//default:
			//	break;
		}
	}

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int result = 0;
	char msg[1024];
	memset(msg, 0, 1024);
	int msgLength = 0;
	int recvFlags = 0;
	int sendServerFlags = 0;

	MsgRspInfo msgRspInfo;
	MsgClientClosedRsp msgClientClosedRsp;
	MsgRspRegister msgRspRegister;
	MsgRspLogin msgRspLogin;
	MsgRspRoleRegister msgRspRoleRegister;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp;
	MsgRspGameEnter msgRspGameEnter;

	RoleInfo roleInfoRegister;

	int sendRspFlags = 0;

	WORD wVersion;
	wVersion = MAKEWORD(1,1);

	WSAData wsaData;
	wsaData.wVersion = wVersion;

	result = WSAStartup(wVersion, &wsaData);

	SOCKET server, client;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		std::cout << "server recv gate create socket failed" << std::endl;
		return -1;
	}

	std::cout << "server recv gate create socket value : " << server << std::endl;

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddress.sin_port = 9903;
	result = bind(server, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server recv gate bind socket failed" << std::endl;
		return -1;
	}

	int blacklog = 1;
	result = listen(server, blacklog);
	if (result == SOCKET_ERROR)
	{
		std::cout << "server recv gate listen socket failed " << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server recv gate listen socket success port : ---------------------- 9903" << std::endl;
	}

	//-----------------------connect solider start----------------------//
	SOCKET serverSolider;
	serverSolider = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		std::cout << "server recv create server solider failed" << std::endl;
		return -1;
	}
	//else
	//{
	//	std::cout << "server recv create server solider success : " << serverSolider << std::endl;
	//}

	sockaddr_in soliderAddress;
	soliderAddress.sin_family = AF_INET;
	soliderAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	soliderAddress.sin_port = 9901;

	result = connect(serverSolider, (sockaddr*)&soliderAddress, sizeof(soliderAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server recv gate connect server solider failed" << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server recv gate connect solider success port : -------------------- 9901" << std::endl;
	}

	//-----------------------connect solider start----------------------//

	SOCKADDR_IN clientAddress;
	int len = sizeof(clientAddress);
	HANDLE handleRemote;
	SECURITY_ATTRIBUTES threadAttrs;
	threadAttrs.bInheritHandle = true;
	threadAttrs.lpSecurityDescriptor = NULL;
	threadAttrs.nLength = 0;
	DWORD dwCreationFlags = 0;
	DWORD threadId = 0;
	ClientConnectInfo clientConnInfo;

	client = accept(server, (sockaddr*)&clientAddress, &len);

	while (true)
	{
		

		//Sleep(1000);
		//send(serverSolider, "server recv gate", 20, sendServerFlags);

		result = recv(serverSolider, msg, 1024, MSG_PEEK);
		msgLength = result;
		result = recv(serverSolider, msg, msgLength, recvFlags);
		std::cout << "server recv gate recv data from server solider result : " << result << ", error : " << WSAGetLastError() << ", solider : " << serverSolider << std::endl;
		if (result != 0)
		{
			if (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)
			{
				Sleep(10000);
				continue;
			}
		}

		memcpy(&msgRspInfo, msg, sizeof(msgRspInfo));

		switch(msgRspInfo.msgid)
		{
		case MSG_CLIENT_CLOSED_RSP:
			memcpy(&msgClientClosedRsp, msg, sizeof(msgClientClosedRsp));
			break;
		case MSG_RSP_REGISTER:
			memcpy(&msgRspRegister, msg, sizeof(msgRspRegister));		//parse the msg from recv(solider,msg,..) to struct
			//direct return to client, parse by client, for display, use statement MSG_REQ_REGISTER case to; already to use this way, determine except the way to transport net data
			//itoa(msgRspRegister.msgid, strMsgRspRegisterMsgID, 10);
			//itoa(msgRspRegister.result, strMsgRspRegisterResult, 10);
			//str += strMsgRspRegisterMsgID;
			//str += ",";
			//str += strMsgRspRegisterResult;

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspRegister), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
		case MSG_RSP_LOGIN:
			memcpy(&msgRspLogin, msg, sizeof(msgRspLogin));
			if (msgRspLogin.needCloseClient > 0)
			{
				shutdown(msgRspLogin.needCloseClient, 0x02);
				closesocket(msgRspLogin.needCloseClient);
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspLogin), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
		case MSG_RSP_ROLE_REGISTER:
			memcpy(&msgRspRoleRegister, msg, sizeof(msgRspRoleRegister));
			memcpy(&msgRspRoleRegisterTemp, msg, sizeof(msgRspRoleRegisterTemp));
			for (int i = 0; i < msgRspRoleRegisterTemp.roleCount; i++)
			{
				memcpy(&roleInfoRegister, msg + sizeof(msgRspRoleRegisterTemp) + i * sizeof(RoleInfo), sizeof(RoleInfo));
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(client, msg, sizeof(msgRspRoleRegisterTemp) + msgRspRoleRegisterTemp.roleCount * sizeof(RoleInfo), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
		case MSG_RSP_GAME_ENTER:
			memcpy(&msgRspGameEnter, msg, sizeof(msgRspGameEnter));
			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			result = send(client, msg, sizeof(msgRspGameEnter), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			//client = msgRspInfo.client;		//
			break;
			//default:
			//	break;
		}

		//handleRemote = CreateThread(&threadAttrs, sizeof(threadAttrs), JoinThread, (LPVOID)&clientConnInfo, dwCreationFlags, &threadId);	//all params need set correct can join to the thread func
	}

	//send(serverSolider, msg, 1024, sendServerFlags);



	return 0;
}

