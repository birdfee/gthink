//------------------------------------------
//author : fly snack
//date : 2009/10/02
//email : 358657363@qq.com
//------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include "../common/proto.h"
#include <iostream>

#pragma comment(lib, "wsock32.lib")

typedef struct tagMsgRspInfo
{
	int client;
	int msgid;
}MsgRspInfo;

int _tmain(int argc, _TCHAR* argv[])
{
	int result = 0;
	char buf[64];
	char msg[1024];
	std::string strMsg = "";
	int msgid = 0;
	int msgLength = 0;

	std::vector<RoleInfo> vecRoleInfosLogin;
	std::vector<RoleInfo> vecRoleInfosRegister;
	RoleInfo roleInfoLogin;
	RoleInfo roleInfoRegister;
	//char role[64];

	MsgRspInfo msgRspInfo;
	MsgReqRegister msgReqRegister;
	MsgRspRegister msgRspRegister;
	MsgReqLogin msgReqLogin;
	MsgRspLogin msgRspLogin;
	MsgRspLoginTemp msgRspLoginTemp;
	MsgReqRoleRegister msgReqRoleRegister;
	MsgRspRoleRegister msgRspRoleRegister;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp;
	MsgReqGameEnter msgReqGameEnter;
	MsgRspGameEnter msgRspGameEnter;

	WORD wVersion = 0;
	wVersion = MAKEWORD(1,1);

	WSADATA wsaData;
	wsaData.wVersion = wVersion;

	result = WSAStartup(wVersion, &wsaData);
	if (result == WSAEWOULDBLOCK)
	{
		std::cout << "server client wsa start up failed" << std::endl;
		return -1;
	}

	SOCKET serverGate;
	serverGate = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverGate == INVALID_SOCKET)
	{
		std::cout << "server client create socket failed" << std::endl;
		return -1;
	}

	std::cout << "server client create socket server gate value : " << serverGate << std::endl;

	//int mode = 1;
	//result = ioctlsocket(serverGate, FIONBIO, (u_long*)&mode);	//need & or set ioctl socket failed
	//if (result == SOCKET_ERROR)
	//{
	//	std::cout << "server client set ioctlsocket failed" << std::endl;
	//	WSACleanup();
	//	return -1;
	//}

	SOCKADDR_IN gateAddress;
	gateAddress.sin_family = AF_INET;
	gateAddress.sin_addr.S_un.S_addr = inet_addr("192.168.1.6");
	gateAddress.sin_port = 9902;
	//result = bind(serverGate, (sockaddr*)&gateAddress, sizeof(gateAddress));
	//if (result == SOCKET_ERROR)
	//{
	//	std::cout << "server client bind socket failed" << std::endl;
	//	return -1;
	//}

	result = connect(serverGate, (sockaddr*)&gateAddress, sizeof(gateAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server client connect gate failed" << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server client connect gate success port : -------------------- 9902" << std::endl;
	}

	int sendFlags = 0;
	int recvFlags = 0;

	while(true)
	{
		std::cout << "please input a msgid : " << std::endl;
		std::cin>> msgid;
		switch(msgid)
		{
		case MSG_REQ_REGISTER:
			msgReqRegister.msgid = msgid;
			std::cout << "please in put a char account[20] : " << std::endl;
			std::cin>> msgReqRegister.account;
			std::cout << "please in put a char pwd[16] : " << std::endl;
			std::cin >> msgReqRegister.pwd;
			memcpy(buf, &msgReqRegister, sizeof(msgReqRegister));
			break;

		case MSG_REQ_LOGIN:
			msgReqLogin.msgid = msgid;
			std::cout << "please in put a char account[20] : " << std::endl;
			std::cin >> msgReqLogin.account;
			std::cout << "please in put a char pwd[16] : " << std::endl;
			std::cin >> msgReqLogin.pwd;
			memcpy(buf, &msgReqLogin, sizeof(msgReqLogin));
			break;
		case MSG_REQ_ROLE_REGISTER:
			msgReqRoleRegister.msgid = msgid;
			std::cout << "please in put a char nick name[14] : " << std::endl;
			std::cin >> msgReqRoleRegister.nickname;
			std::cout << "please in put a sex : " << std::endl;
			std::cin >> msgReqRoleRegister.sex;
			memcpy(buf, &msgReqRoleRegister, sizeof(msgReqRoleRegister));
			break;
		case MSG_REQ_GAME_ENTER:
			msgReqGameEnter.msgid = msgid;
			std::cout << "please in put a roleid : " << std::endl;
			std::cin >> msgReqGameEnter.roleid;
			memcpy(buf, &msgReqGameEnter, sizeof(msgReqGameEnter));
			break;
		//default:
		//	break;
		}

		//if (msgid != MSG_REQ_REGISTER && msgid != MSG_REQ_LOGIN && msgid != MSG_REQ_ROLE_REGISTER)
		//{
		//	return -1;
		//}

		result = send(serverGate, buf, 64, sendFlags);
		if (result != 0)
		{
			std::cout << "server client send data to gate result : " << result << ", error : " << WSAGetLastError() /*<< ", gate : " << serverGate << ", sendFlags : " << sendFlags */<< std::endl;
		}
		//result = recv(serverGate, msg, 64, recvFlags);
		result = recv(serverGate, msg, sizeof(msg), MSG_PEEK);
		msgLength = result;
		result = recv(serverGate, msg, msgLength, recvFlags);
		strMsg = msg;
		if (strMsg.size() == 0)
		{
			result = recv(serverGate, msg, sizeof(msg), MSG_PEEK);
			msgLength = result;
			result = recv(serverGate, msg, msgLength, recvFlags);
			//result = recv(serverGate, msg, msgLength, recvFlags);
		}
		//result = recv(serverGate, msg, sizeof(msg), MSG_OOB);
		if (result != 0)
		{
			std::cout << "server client recv data from gate result : " << result << ", error : " << WSAGetLastError() << std::endl;
		}
		memcpy(&msgRspInfo, msg, sizeof(msgRspInfo));
		switch(msgRspInfo.msgid)
		{
		case MSG_RSP_REGISTER:
			memcpy(&msgRspRegister, msg, sizeof(msgRspRegister));
			std::cout << "recv msg from gate rsp register msgbody : " << msgRspRegister.msgid << "," << msgRspRegister.result << std::endl;
			break;
		case MSG_RSP_LOGIN:
			//memcpy(&msgRspLogin, msg, sizeof(msgRspLogin));
			memcpy(&msgRspLoginTemp, msg, sizeof(msgRspLoginTemp));
			msgRspLogin.client = msgRspLoginTemp.client;
			msgRspLogin.msgid = msgRspLoginTemp.msgid;
			msgRspLogin.needCloseClient = msgRspLoginTemp.needCloseClient;
			msgRspLogin.result = msgRspLoginTemp.result;
			msgRspLogin.roleCount = msgRspLoginTemp.roleCount;
			for (int i = 0; i < msgRspLoginTemp.roleCount; i++)
			{
				memcpy(&roleInfoLogin, msg + sizeof(msgRspLoginTemp), sizeof(RoleInfo));
				vecRoleInfosLogin.push_back(roleInfoLogin);
			}

			if (vecRoleInfosLogin.size() > 0)
			{
				msgRspLogin.vecRoleInfos = vecRoleInfosLogin;
			}

			std::cout << "recv msg from gate rsp login msgbody : " << msgRspLogin.msgid << "," << msgRspLogin.result << "," << msgRspLogin.needCloseClient << std::endl;
			break;
		case MSG_RSP_ROLE_REGISTER:
			//memcpy(&msgRspRoleRegister, msg, sizeof(msgRspRoleRegister));
			memcpy(&msgRspRoleRegisterTemp, msg, sizeof(msgRspRoleRegisterTemp));
			msgRspRoleRegister.client = msgRspRoleRegisterTemp.client;
			msgRspRoleRegister.msgid = msgRspRoleRegisterTemp.msgid;
			msgRspRoleRegister.result = msgRspRoleRegisterTemp.result;
			msgRspRoleRegister.roleCount = msgRspRoleRegisterTemp.roleCount;
			for (int i = 0; i < msgRspRoleRegisterTemp.roleCount; i++)
			{
				//memcpy(role, msg + sizeof(msgRspRoleRegisterTemp), 64);
				//memcpy(&roleInfoRegister, role + i * sizeof(RoleInfo), sizeof(RoleInfo));
				memcpy(&roleInfoRegister, msg + sizeof(msgRspRoleRegisterTemp) + i * sizeof(RoleInfo), sizeof(RoleInfo));
				vecRoleInfosRegister.push_back(roleInfoRegister);
			}

			if (vecRoleInfosRegister.size() > 0)
			{
				msgRspRoleRegister.vecRoleInfos = vecRoleInfosRegister;
			}

			std::cout << "recv msg from gate rsp role register msgbody : " << msgRspRoleRegister.msgid << "," << msgRspRoleRegister.result <<std::endl;
			if (msgRspRoleRegister.vecRoleInfos.size() > 0)
			{
				std::cout << "roleid : " << msgRspRoleRegister.vecRoleInfos[msgRspRoleRegister.vecRoleInfos.size() - 1].roleid << ", nickname : " << msgRspRoleRegister.vecRoleInfos[msgRspRoleRegister.vecRoleInfos.size() - 1].nickname << std::endl;
			}
			break;
		case MSG_RSP_GAME_ENTER:
			memcpy(&msgRspGameEnter, msg, sizeof(msgRspGameEnter));
			std::cout << "recv msg from gate rsp game enter msgbody : " << msgRspGameEnter.msgid << "," << msgRspGameEnter.result << std::endl;
			break;
		//default:
		//	break;
		}
		//std::cout << "recv msg from gate msgbody : " << msg << std::endl;	//case print rsp msgRspInfo.msgid
	}

	return 0;
}

