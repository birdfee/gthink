//------------------------------------------
//author : fly snack
//date : 2009/10/05
//email : 358657363@qq.com
//------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
#include "../common/proto.h"
#include <iostream>

#pragma comment(lib, "wsock32.lib")

typedef struct tagMsgReqInfo
{
	int unknow;		//because client struct MsgReqxxx first field is {int client;int msgid;...;}, so here need this field
	int msgid;
}MsgReqInfo;

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

void split_s(std::vector<std::string>& vecStrs, char* pStr, char partSymbol)
{
	if (pStr == NULL)
	{
		return;
	}

	std::string str = pStr;
	std::string strItem = "";

	for (int i = 0; i < strlen(str.c_str()); i++)
	{
		if (str[i] != partSymbol)
		{
			strItem += str[i];
		}
		else if (str[i] == partSymbol)
		{
			vecStrs.push_back(strItem);
			strItem = "";
		}

		if (i == strlen(str.c_str()) - 1)
		{
			//std::cout << "split string strItem with end i value : " << i << std::endl;
			vecStrs.push_back(strItem);
			strItem = "";
		}
	}

	//std::cout << "split string strItem with last strItem : " << strItem << std::endl;
}

DWORD WINAPI JoinThread(LPVOID lpParam)
{
	int result = 0;
	ClientConnectInfo* pClientConnInfo = (ClientConnectInfo*)lpParam;
	SOCKET client = pClientConnInfo->client;
	SOCKET solider = pClientConnInfo->solider;
	char buf[64];
	char msg[64];
	memset(buf, 0, 64);
	memset(msg, 0, 64);
	int sendFlags = 0;
	int sendServerFlags = 0;
	int recvFlags = 0;
	std::string str = "";
	/*std::string strClient = "";*/
	char strClient[16];
	memset(strClient, 0, 16);
	std::string strClientInfo = "";

	MsgReqInfo msgReqInfo;
	MsgRspInfo msgRspInfo;
	MsgClientClosed msgClientClosed;
	MsgClientClosedRsp msgClientClosedRsp;
	MsgReqRegister msgReqRegister;
	MsgRspRegister msgRspRegister;
	MsgReqLogin msgReqLogin;
	MsgRspLogin msgRspLogin;
	MsgReqRoleRegister msgReqRoleRegister;
	MsgRspRoleRegister msgRspRoleRegister;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp;

	RoleInfo roleInfoRegister;

	char soliderRespon[1024];
	memset(soliderRespon, 0, 1024);

	int msgLength;
	//std::string strSoliderRespon = "";
	//std::vector<char> vecSoliderRespon;
	MsgRspInfo msgRspSoliderRespon;
	MsgRspInfo msgRspSoliderRespon2;

	int recvRspFlags = 0;
	int sendRspFlags = 0;

	char soliderResponse[64];		
	memset(soliderResponse, 0, 64);

	std::vector<std::string> vecRespon;

	char strMsgID[6];
	char strMsgSex[6];
	int clientInteger = 0;
	//char strMsgRspRegisterMsgID[6];
	//char strMsgRspRegisterResult[6];

	while(true)
	{
		result = recv(client, buf, 64, recvFlags);		//not say recv(solider, ..);
		std::cout << "server gate recv data from client result : " << result << ", error : " << WSAGetLastError() << ", client : " << client << std::endl;
		if (result != 0)
		{
			//std::cout << "server gate recv data from client result : " << result << ", error : " << WSAGetLastError() << ", client : " << client << std::endl;
			if (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)
			{
				msgClientClosed.msgid = MSG_CLIENT_CLOSED;
				itoa(msgClientClosed.msgid, strMsgID, 10);
				str = strMsgID;
				clientInteger = client;
				itoa(clientInteger, strClient, 10);
				strClientInfo = strClient;
				str = "gate : client : " + strClientInfo + " : " + str;
				strcpy(msg, str.c_str());
				send(solider, msg, 64, sendServerFlags);
				str = "";

				shutdown(client, 0x02);
				closesocket(client);
				Sleep(10000);	//sleep the close client process to gate thread, other client's thread not sleep
				continue;
				//WSACleanup();
				//closesocket(client);
			}
			else if (WSAGetLastError() == WSAENOTSOCK || WSAGetLastError() == WSAEINTR)
			{
				shutdown(client, 0x02);
				closesocket(client);
				Sleep(20000);
				continue;
			}
		}

		memcpy(msg, buf, 64);
		memcpy(&msgReqInfo, buf, sizeof(msgReqInfo));

		switch(msgReqInfo.msgid)
		{
		case MSG_REQ_REGISTER:
			memcpy(&msgReqRegister, buf, sizeof(msgReqRegister));
			//str += msgRegister.msgid;
			
			itoa(msgReqRegister.msgid, strMsgID, 10);
			str += strMsgID;
			str += ",";
			str += msgReqRegister.account;
			str += ",";
			//std::string strPwd = msgRegister.pwd;
			str = str + msgReqRegister.pwd;
			break;
		//case MSG_RSP_REGISTER:
		//	memcpy(&msgRspRegister, buf, sizeof(msgRspRegister));
		//	//direct return to client, parse from client, display with it, use case way to
		//	str += msgRspRegister.msgid;
		//	str += ",";
		//	str += msgRspRegister.result;
		//	break;
		case MSG_REQ_LOGIN:
			memcpy(&msgReqLogin, buf, sizeof(msgReqLogin));
			itoa(msgReqLogin.msgid, strMsgID, 10);
			str += strMsgID;
			str += ",";
			str += msgReqLogin.account;
			str += ",";
			str = str + msgReqLogin.pwd;
			break;
		case MSG_REQ_ROLE_REGISTER:
			memcpy(&msgReqRoleRegister, buf, sizeof(msgReqRoleRegister));
			itoa(msgReqRoleRegister.msgid, strMsgID, 10);
			str += strMsgID;
			str += ",";
			str += msgReqRoleRegister.nickname;
			str += ",";
			itoa(msgReqRoleRegister.sex, strMsgSex, 10);
			//str += msgReqRoleRegister.sex;
			str += strMsgSex;
			break;
		//default:
		//	shutdown(client, 0x02);
		//	closesocket(client);
		//	Sleep(30000);
		//	break;
		}

		clientInteger = client;
		itoa(clientInteger, strClient, 10);

		//str = msg;	//for truncate with '0', part of value

		strClientInfo = strClient;
		str = "gate : client : " + strClientInfo + "," + str;	//gate : client : 164,301,750857753,123456
		strcpy(msg, str.c_str());
		//send(client, msg, 64, sendFlags);		//return msg with game process, for the front solider:gate:client
		send(solider, msg, 64, sendServerFlags);	//send the msg from client to solider
		str = "";

		//-------------------------recv solider response start-------------------------//
		//result = recv(solider, soliderRespon, 128, recvRspFlags);
		result = recv(solider, soliderRespon, 1024, MSG_PEEK);	
		memcpy(&msgRspSoliderRespon, soliderRespon, sizeof(msgRspSoliderRespon));
		msgLength = result;
		std::cout << "server gate recv from solider process response msg : msgLength ------ : " << msgLength << ", error : " << WSAGetLastError() << ", solider : " << solider << std::endl;
		result = recv(solider, soliderRespon, msgLength, recvRspFlags);		//to transport param for sizeof in recv and send, msg length value : 1634561906; soliderRespon.sizeof() not enough the reason
		memcpy(&msgRspSoliderRespon2, soliderRespon, sizeof(msgRspSoliderRespon2));
		//result = recv(solider, &vecSoliderRespon[0], vecSoliderRespon.size(), recvRspFlags);
		std::cout << "server gate recv from solider process response msg : " << result << ", error : " << WSAGetLastError() << ", solider : " << solider << std::endl;
		if (result != 0)
		{
			if (WSAGetLastError() == WSAECONNABORTED || WSAGetLastError() == WSAECONNRESET)
			{
				Sleep(10000);
				continue;
			}
		}

		memcpy(&msgRspInfo, soliderRespon, sizeof(msgRspInfo));

		switch(msgRspInfo.msgid)
		{
		case MSG_CLIENT_CLOSED_RSP:
			memcpy(&msgClientClosedRsp, soliderRespon, sizeof(msgClientClosedRsp));
			break;
		case MSG_RSP_REGISTER:
			memcpy(&msgRspRegister, soliderRespon, sizeof(msgRspRegister));		//parse the msg from recv(solider,msg,..) to struct
			//direct return to client, parse by client, for display, use statement MSG_REQ_REGISTER case to; already to use this way, determine except the way to transport net data
			//itoa(msgRspRegister.msgid, strMsgRspRegisterMsgID, 10);
			//itoa(msgRspRegister.result, strMsgRspRegisterResult, 10);
			//str += strMsgRspRegisterMsgID;
			//str += ",";
			//str += strMsgRspRegisterResult;

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(msgRspInfo.client, soliderRespon, sizeof(msgRspRegister), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			client = msgRspInfo.client;		//
			break;
		case MSG_RSP_LOGIN:
			memcpy(&msgRspLogin, soliderRespon, sizeof(msgRspLogin));
			if (msgRspLogin.needCloseClient > 0)
			{
				shutdown(msgRspLogin.needCloseClient, 0x02);
				closesocket(msgRspLogin.needCloseClient);
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(msgRspInfo.client, soliderRespon, sizeof(msgRspLogin), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			client = msgRspInfo.client;		//
			break;
		case MSG_RSP_ROLE_REGISTER:
			memcpy(&msgRspRoleRegister, soliderRespon, sizeof(msgRspRoleRegister));
			memcpy(&msgRspRoleRegisterTemp, soliderRespon, sizeof(msgRspRoleRegisterTemp));
			for (int i = 0; i < msgRspRoleRegisterTemp.roleCount; i++)
			{
				memcpy(&roleInfoRegister, soliderRespon + sizeof(msgRspRoleRegisterTemp) + i * sizeof(RoleInfo), sizeof(RoleInfo));
			}

			std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
			//send(client, soliderResponse, 64, sendRspFlags);
			result = send(msgRspInfo.client, soliderRespon, sizeof(msgRspRoleRegisterTemp) + msgRspRoleRegisterTemp.roleCount * sizeof(RoleInfo), sendRspFlags);
			std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
			client = msgRspInfo.client;		//
			break;
		//default:
		//	break;
		}

		//memcpy(soliderResponse, soliderRespon, 64);
		//if (msgRspInfo.msgid != MSG_CLIENT_CLOSED_RSP)	//not to clear the comment for the statement
		//{
		//	std::cout << "server gate send to msgRspInfo.client : " << msgRspInfo.client << std::endl;
		//	//send(client, soliderResponse, 64, sendRspFlags);
		//	result = send(msgRspInfo.client, soliderRespon, msgLength, sendRspFlags);
		//	std::cout << "server gate send to client solider process response msg : " << result << ", error : " << WSAGetLastError() << ", client : " << msgRspInfo.client << ", msgLength : " << msgLength << std::endl;
		//	client = msgRspInfo.client;		//
		//}

		//str = "gate : client : " + strClientInfo + " : " + str;
		//not only to use string to transport package msg
		//strcpy(soliderResponse, str.c_str());
		//send(client, soliderResponse, 64, sendRspFlags);
		//str = "";

		//the comment of 3 statement replace by the above 3 sentences
		//split_s(vecRespon, soliderRespon, ':');
		//strcpy(soliderResponse, vecRespon[vecRespon.size() - 1].c_str());
		//send(client, soliderResponse, 64, sendRspFlags);		//clear solider:gate:client from msg
		//-------------------------recv solider response end-------------------------//
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int result = 0;
	WORD wVersion;
	wVersion = MAKEWORD(1,1);

	WSADATA wsaData;
	wsaData.wVersion = wVersion;
	result = WSAStartup(wVersion, &wsaData);
	if (result == WSAEWOULDBLOCK)
	{
		std::cout << "server gate wsa start up failed, error : " << WSAGetLastError() << std::endl;
		return -1;
	}

	SOCKET server, client;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		std::cout << "server gate create socket failed" << std::endl;
		return -1;
	}

	SOCKADDR_IN serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddress.sin_port = 9902;
	result = bind(server, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server gate bind socket failed" << std::endl;
		return -1;
	}

	int blacklog = 1;
	result = listen(server, blacklog);
	if (result == SOCKET_ERROR)
	{
		std::cout << "server gate listen socket failed " << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server gate listen socket success port : ---------------------- 9902" << std::endl;
	}

	//-----------------------connect solider start----------------------//
	SOCKET serverSolider;
	serverSolider = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSolider == INVALID_SOCKET)
	{
		std::cout << "server gate create server solider socket failed" << std::endl;
		return -1;
	}

	SOCKADDR_IN soliderAddress;
	soliderAddress.sin_family = AF_INET;
	soliderAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//INADDR_ANY;	
	soliderAddress.sin_port = 9901;
	//about need bind SOCKET serverSolider..., if bind, then connect will to make gate connect gate's bind SOCKET
	//it's not process solider's bind 9901 port, so can't to bind, would make solider accept() to gate faild, breakpoint could not to the process solider, then connect success
	//result = bind(serverSolider, (sockaddr*)&soliderAddress, sizeof(soliderAddress));
	//if (result == SOCKET_ERROR)
	//{
	//	std::cout << "server gate bind solider socket failed" << std::endl;
	//	return -1;
	//}

	result = connect(serverSolider, (sockaddr*)&soliderAddress, sizeof(soliderAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server gate connect solider failed" << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server gate connect solider success port : -------------------- 9901" << std::endl;
	}
	//-----------------------connect solider end----------------------//


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
	while(true)
	{
		client = accept(server, (sockaddr*)&clientAddress, &len);
		clientConnInfo.client = client;
		clientConnInfo.solider = serverSolider;
		handleRemote = CreateThread(&threadAttrs, sizeof(threadAttrs), JoinThread, /*(LPVOID)client*/(LPVOID)&clientConnInfo, dwCreationFlags, &threadId);
	}

	return 0;
}

