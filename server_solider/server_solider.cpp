//------------------------------------------
//author : fly snack
//date : 2009/10/06
//email : 358657363@qq.com
//------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string>
#include "../common/proto.h"
#include "../common/IHandle.h"
#include <iostream>
#include "../common/IGlobal.h"
#include "BaseUsers.h"

#pragma comment(lib, "wsock32.lib")

typedef struct tagMsgReqInfo
{
	int msgBodyClient;	
	int msgid;
}MsgReqInfo;

//typedef unsigned __int64 u_int64;
//typedef u_int64 uid;
//
//uid guid = 0;
//
//int genGuid()
//{
//	guid++;
//}

void Bubble_sort(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			int temp = 0;
			if (a[j] > a[j+1])
			{
				temp = a[j+1];
				a[j+1] = a[j];
				a[j] = temp;
			}
		}
	}
}

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

	SOCKET client = (SOCKET)lpParam;
	char buf[64];
	char msg[1024];
	//std::string msgRsp = "";

	memset(buf, 0, 64);
	memset(msg, 0, 1024);

	int sendFlags = 0;
	int recvFlags = 0;

	std::string str = "";
	MsgReqInfo msgInfo;
	MsgClientClosed msgClientClosed;
	MsgReqRegister msgReqRegister;
	MsgReqLogin msgReqLogin;
	MsgRspLogin msgRspLogin;
	MsgRspLoginTemp msgRspLoginTemp;
	MsgReqRoleRegister msgReqRoleRegister;
	MsgRspRoleRegister msgRspRoleRegister;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp;
	MsgRspRoleRegisterTemp msgRspRoleRegisterTemp1;
	MsgReqGameEnter msgReqGameEnter;
	//MsgRspGameEnter msgRspGameEnter;
	HandleMsgReqRegister handleMsgReqRegister;
	HandleMsgReqRegisterRole handleMsgReqRegisterRole;
	HandleMsgReqGameEnter handleMsgReqGameEnter;

	RoleInfo roleInfoRegister;

	std::vector<std::string> vecStrs;
	std::vector<std::string> vecMsgBodyInfos;
	std::vector<std::string> vecMsgLastItem;
	char msgBody[64];
	//split(vecStrs, )

	//std::map<int, int> mapGates;
	//std::vector<int> vecGates;
	mapGates.insert(std::make_pair(client, client));
	std::map<int, int>::iterator iter = mapGates.begin();
	//iter++;

	//int n = client;
	
	while(true)
	{
		//int m = client;
		result = recv(client, buf, 64, recvFlags);
		//vecGates.push_back(client);
		//mapGates.insert(std::make_pair(client, client));
		//std::map<int, int>::iterator iter = mapGates.end();
		//iter--;
		std::cout << "server solider recv data from gate result : " << result << ", error : " << WSAGetLastError() << ", client : " << client << std::endl;
		if (result != 0)
		{
			if (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)
			{
				Sleep(1000);	//just close disconnect to gate thread sleep, others does not
				continue;
				//WSACleanup();
				//closesocket(client);
			}
			//std::cout << "server solider recv data from gate result : " << result << ", error : " << WSAGetLastError() << std::endl;
		}

		split_s(vecStrs, buf, ':');
		
		memset(msgBody, 0, 64);
		if (vecStrs.size() > 0)
		{
			memcpy(msgBody, vecStrs[vecStrs.size() - 1].c_str(), sizeof(vecStrs[vecStrs.size() - 1]));
		}
		
		split_s(vecMsgBodyInfos, msgBody, ',');		//save msg total infos;"305,flySnack,0", use string in the RoleInfo struct datum make vector size < 0 or capacity < 0, vector.size and capacity to abnormal reason for msg.sizeof() is not enough，msg.sizeof() < msg.data.length; with the same problem in server gate msg length : 1634561906
		split_s(vecMsgLastItem, msgBody, ',');

		if (vecMsgLastItem.size() > 1)
		{
			msgInfo.msgBodyClient = atoi(vecMsgLastItem[0].c_str());
			msgInfo.msgid = atoi(vecMsgLastItem[1].c_str());
		}

		//memcpy(&msgInfo, msgBody, sizeof(msgInfo));
		//msgInfo.msgid = atoi(vecMsgBodyInfos[0].c_str());	//msgInfo.msgid = msgBody.first.split(',');
		//memcpy(&msgInfo, msgBody, sizeof(msgInfo));	//use memcpy to binary data will be see the first selection
		u_int64 userid = 0;

		switch(msgInfo.msgid)
		{
		case MSG_CLIENT_CLOSED:
			msgClientClosed.msgid = msgInfo.msgid;

			handleMsgReqRegister.InitClientClosed(msgInfo.msgBodyClient, msgClientClosed);
			result = handleMsgReqRegister.ProcessClientClosed();
			memcpy(msg, &handleMsgReqRegister.getClientClosedRsp(), sizeof(MsgClientClosedRsp));
			send(iter->second, msg, sizeof(MsgClientClosedRsp), sendFlags);
		case MSG_REQ_REGISTER:
			//memcpy(&msgReqRegister, msgBody, sizeof(msgReqRegister));	//use memcpy to binary data will be see the first selection

			msgReqRegister.client = msgInfo.msgBodyClient;
			msgReqRegister.msgid = msgInfo.msgid;
			strcpy(msgReqRegister.account, vecMsgLastItem[2].c_str());
			strcpy(msgReqRegister.pwd, vecMsgLastItem[3].c_str());
			
			handleMsgReqRegister.Init(msgReqRegister);
			result = handleMsgReqRegister.Process();
			memcpy(msg, &handleMsgReqRegister.getRsp(), sizeof(MsgRspRegister));
			send(iter->second, msg, sizeof(MsgRspRegister), sendFlags);
			break;
		case MSG_REQ_LOGIN:
			//memcpy(&msgReqLogin, msgBody, sizeof(msgReqLogin));		//use memcpy to binary data will be see the first selection

			msgReqLogin.client = msgInfo.msgBodyClient;
			msgReqLogin.msgid = msgInfo.msgid;
			strcpy(msgReqLogin.account, vecMsgLastItem[2].c_str());
			strcpy(msgReqLogin.pwd, vecMsgLastItem[3].c_str());

			handleMsgReqRegister.InitLogin(msgInfo.msgBodyClient, msgReqLogin);
			result = handleMsgReqRegister.ProcessLogin();
			msgRspLogin = handleMsgReqRegister.getRspLogin();
			userid = handleMsgReqRegister.findOnlineUserID(msgInfo.msgBodyClient);
			if (userid > 0)
			{
				msgRspLogin.vecRoleInfos = users.findUserRoleInfos(userid);//handleMsgReqRegisterRole.findUserRoleInfos(userid);
				msgRspLogin.roleCount = msgRspLogin.vecRoleInfos.size();
			}
			msgRspLoginTemp.client = msgRspLogin.client;
			msgRspLoginTemp.msgid = msgRspLogin.msgid;
			msgRspLoginTemp.needCloseClient = msgRspLogin.needCloseClient;
			msgRspLoginTemp.result = msgRspLogin.result;
			msgRspLoginTemp.roleCount = msgRspLogin.roleCount;
			for (int i = 0; i < msgRspLogin.vecRoleInfos.size(); i++)
			{
				memcpy(msg + sizeof(MsgRspLoginTemp) + i * sizeof(RoleInfo), &msgRspLogin.vecRoleInfos[i], sizeof(RoleInfo));
			}
			memcpy(msg, &msgRspLoginTemp, sizeof(MsgRspLoginTemp));

			//
			//memcpy(msg, &msgRspLogin/*&handleMsgReqRegister.getRspLogin()*/, sizeof(MsgRspLogin));
			send(iter->second, msg, sizeof(MsgRspLogin), sendFlags);
			//result = handleMsgReqRegister.Login()
			break;
		case MSG_REQ_ROLE_REGISTER:

			msgReqRoleRegister.client = msgInfo.msgBodyClient;
			msgReqRoleRegister.msgid = msgInfo.msgid;
			strcpy(msgReqRoleRegister.nickname, vecMsgLastItem[2].c_str());
			msgReqRoleRegister.sex = atoi(vecMsgLastItem[3].c_str());

			userid = handleMsgReqRegister.findOnlineUserID(msgInfo.msgBodyClient);
			if (userid > 0)
			{
				handleMsgReqRegisterRole.Init(userid, msgReqRoleRegister);
				result = handleMsgReqRegisterRole.Process();
				msgRspRoleRegister = handleMsgReqRegisterRole.getRsp();
				msgRspRoleRegister.vecRoleInfos = users.findUserRoleInfos(userid);//handleMsgReqRegisterRole.findUserRoleInfos(userid);
				msgRspRoleRegister.roleCount = msgRspRoleRegister.vecRoleInfos.size();
				msgRspRoleRegisterTemp.client = msgRspRoleRegister.client;
				msgRspRoleRegisterTemp.msgid = msgRspRoleRegister.msgid;
				msgRspRoleRegisterTemp.result = msgRspRoleRegister.result;
				msgRspRoleRegisterTemp.roleCount = msgRspRoleRegister.roleCount;
				for (int i = 0; i < msgRspRoleRegister.vecRoleInfos.size(); i++)
				{
					memcpy(msg + sizeof(MsgRspRoleRegisterTemp) + i * sizeof(RoleInfo), &msgRspRoleRegister.vecRoleInfos[i], sizeof(RoleInfo));
				}
				memcpy(msg, &msgRspRoleRegisterTemp, sizeof(MsgRspRoleRegisterTemp));

				//memcpy(msg, &msgRspRoleRegister/*&handleMsgReqRegisterRole.getRsp()*/, sizeof(MsgRspRoleRegister));

				memcpy(&msgRspRoleRegisterTemp1, msg, sizeof(msgRspRoleRegisterTemp1));
				for (int i = 0; i < msgRspRoleRegisterTemp1.roleCount; i++)
				{
					memcpy(&roleInfoRegister, msg + sizeof(msgRspRoleRegisterTemp1) + i * sizeof(RoleInfo), sizeof(RoleInfo));
				}
				
				std::cout << "server solider send msg to gate size msgRspRoleRegisterTemp1 : " << sizeof(msgRspRoleRegisterTemp1) << ", 2 size RoleInfo : " << 2 * sizeof(RoleInfo) << std::endl;

				send(iter->second, msg, sizeof(msgRspRoleRegisterTemp) + msgRspRoleRegisterTemp.roleCount * sizeof(RoleInfo), sendFlags);	//need include msgbody.length in first location
			}
			break;
		case MSG_REQ_GAME_ENTER:
			msgReqGameEnter.client = msgInfo.msgBodyClient;
			msgReqGameEnter.msgid = msgInfo.msgid;
			msgReqGameEnter.roleid = atoi(vecMsgLastItem[2].c_str());

			userid = handleMsgReqRegister.findOnlineUserID(msgInfo.msgBodyClient);
			if (userid > 0)
			{
				handleMsgReqGameEnter.Init(userid, msgReqGameEnter);
				result = handleMsgReqGameEnter.Process();
				//msgRspGameEnter = handleMsgReqGameEnter.getRsp();
				memcpy(msg, &handleMsgReqGameEnter.getRsp(), sizeof(MsgRspGameEnter));
				send(iter->second, msg, sizeof(MsgRspGameEnter), sendFlags);
			}
		//default:
		//	break;
		}
		
		vecMsgLastItem.clear();

		//the five comment statement to send in case
		//memcpy(msg, buf, 64);
		//str = msg;
		//str = "solider : " + str;
		//strcpy(msg, str.c_str());
		//send(client, msg, 64, sendFlags);
	}
	return 0;
}

//typedef struct tagItemInfo
//{
//	int itemid;
//	//int roleid;
//}ItemInfo;

struct ItemInfo7
{
	int itemid;
	int roleid;
	bool operator < (const ItemInfo7 info) const	//not operator < will not to use, error C2676, can't map.insert(struct ItemInfo7,..)
	{
		if ((this->itemid < info.itemid) || (this->roleid < info.roleid))		//can't <=，will be make map.find(ItemInfo7) will be not find item,need itemid,roleid just only one different to order, then will be complete correct find
			return true;
		else
			return false;
	}

	bool operator == (const ItemInfo7 info) const
	{
		return (this->itemid == info.itemid) && (this->roleid == info.roleid);
	}
};

struct ItemInfo8
{
	int itemid;
	int roleid;
	bool operator < (const ItemInfo8 info) const
	{
		if (this->itemid < info.itemid)
			return true;
		else
			return false;
	}
};

struct ItemInfo7_1
{
	std::map<ItemInfo7, ItemInfo8> mapItemInfos;
	std::map<ItemInfo7, ItemInfo8>::iterator find(const ItemInfo7 info) const
	{
		//if ((this->itemid == info.itemid) && (this->roleid == info.roleid))
		//{
		//}
	}
};

typedef struct tagItemInfo2
{
	int itemid;
	//int roleid;
}ItemInfo2;

typedef struct tagItemInfo3
{
	int itemid;
	//int roleid;
}ItemInfo3;

class ItemInfo5
{
public:
	ItemInfo5(){itemid = 0;}
	int GetItemid(){return itemid;}
private:
	int itemid;
};

class ItemInfo6
{
public:
	ItemInfo6(){itemid = 0;}
	int GetItemid(){return itemid;}
private:
	int itemid;
};

//template<class ItemInfo2, class T2=int>
//std::pair<ItemInfo2, T2> make_pair1(ItemInfo2 itemInfo, T2 itemid)
//{
//	return (std::pair<ItemInfo2, T2>)(itemInfo, itemid);
//}

template <class T1, class T2>
struct pair
{
	typedef T1 first_type;
	typedef T2 second_type;
	T1 first;
	T2 second;
	pair() : first(T1()), second(T2())
	{}
	pair(const T1& a, const T2& b) : first(a), second(b)
	{}
};


template<class T1, class T2=int>
class Ai
{
public:
	pair<T1, T2> make_pair1(T1 x, T2 y)
	{
		return (pair<T1, T2>(x, y));
	}

	void insert(T1 x, T2 y)
	{
		mapItemInfos.insert(make_pair1(x, y));
	}

	int g(){return 0;}

private:
	std::map<T1, T2> mapItemInfos;
};

int _tmain(int argc, _TCHAR* argv[])
{
	int a[15] = {32,5,16,24,51,67,12,2,18,45,65,35,89,21,46};
	Bubble_sort(a, 15);

	int c[1000];
	//std::vector<int> randVector;
	for (int i = 0; i < 1000; i++)
	{
		//randVector.push_back(rand());
		c[i] = rand();
	}

	Bubble_sort(c, 1000);

	int b[15] = {};

	//std::string strQiu= "飘然的秋";
	//std::cout << "std string str Qiu size : " << strQiu.size() << ", strlen strQiu : " << strlen(strQiu.c_str()) << std::endl;
	//std::string strSnack = "flySnack";
	//std::cout << "std string str Snack size : " << strSnack.size() << ", strlen strSnack : " << strlen(strSnack.c_str()) << std::endl;
	//std::string strDemon = "blood of demon";
	//std::cout << "std string str Demon size : " << strDemon.size() << ", strlen strDemon : " << strlen(strDemon.c_str()) << std::endl;

	ItemInfo itemInfo;
	itemInfo.itemid = 1;
	//itemInfo.roleid = 10001;

	ItemInfo itemInfo2;
	itemInfo2.itemid = 2;
	//itemInfo2.roleid = 10001;

	ItemInfo2 itemInfo3;
	ItemInfo3 itemInfo4;

	ItemInfo7 itemInfo7;
	itemInfo7.itemid = 2;
	itemInfo7.roleid = 10001;
	ItemInfo8 itemInfo8;
	itemInfo8.itemid = 2;
	itemInfo8.roleid = 10001;
	ItemInfo7 itemInfo9;
	itemInfo9.itemid = 1;
	itemInfo9.roleid = 10001;
	ItemInfo8 itemInfo10;
	itemInfo10.itemid = 3;
	itemInfo10.roleid = 10001;

	ItemInfo7 itemInfo11;
	itemInfo11.itemid = 2;
	itemInfo11.roleid = 10002;

	ItemInfo7 itemInfo12;
	itemInfo12.itemid = 2;
	itemInfo12.roleid = 10002;

	ItemInfo8 itemInfo13;
	itemInfo13.itemid = 4;
	itemInfo13.roleid = 10003;

	ItemInfo5 itemInfo5;
	ItemInfo6 itemInfo6;

	//std::map<ItemInfo2, ItemInfo3> mapItemInfos;
	std::map<ItemInfo7, ItemInfo8> mapItemInfos;
	std::multimap<ItemInfo7, ItemInfo8> mmapItemInfos;
	std::vector<int> vecItemIDs;
	//vecItemIDs.
	//std::make_pair(itemInfo, itemInfo.itemid);
	Ai<ItemInfo5, ItemInfo6> ai;
	ai.g();

	//ai.make_pair1(itemInfo3, itemInfo4);
	//ai.insert(itemInfo3, itemInfo4);

	ai.make_pair1(itemInfo5, itemInfo6);
	//ai.insert(itemInfo5, itemInfo6);

	//mapItemInfos.insert(std::make_pair(itemInfo3, itemInfo4));
	mapItemInfos.insert(std::make_pair(itemInfo9, itemInfo10));

	mapItemInfos.insert(std::make_pair(itemInfo7, itemInfo8));

	mapItemInfos.insert(std::make_pair(itemInfo12, itemInfo13));

	mmapItemInfos.insert(std::make_pair(itemInfo9, itemInfo10));

	mmapItemInfos.insert(std::make_pair(itemInfo7, itemInfo8));
	
	std::map<ItemInfo7, ItemInfo8>::iterator iterItemInfo = mapItemInfos.find(itemInfo7);
	if (iterItemInfo == mapItemInfos.end())
	{
		std::cout << "server solider mapItemInfos find failed" << std::endl;
	}

	std::map<ItemInfo7, ItemInfo8>::iterator iterItemInfo2 = mapItemInfos.find(itemInfo11);
	if (iterItemInfo2 == mapItemInfos.end())
	{
		std::cout << "server solider mapItemInfos find failed" << std::endl;
	}
	else if (iterItemInfo2 != mapItemInfos.end())
	{
		std::cout << "server solider mapItemInfos find success : " << mapItemInfos[itemInfo11].itemid << "," << mapItemInfos[itemInfo11].roleid << std::endl;
	}

	std::multimap<ItemInfo7, ItemInfo8>::iterator miterItemInfo = mmapItemInfos.find(itemInfo7);
	if (miterItemInfo == mmapItemInfos.end())
	{
		std::cout << "server solider mmapItemInfos find failed" << std::endl;
	}

	std::multimap<ItemInfo7, ItemInfo8>::iterator miterItemInfo2 = mmapItemInfos.find(itemInfo11);
	if (miterItemInfo2 == mmapItemInfos.end())
	{
		std::cout << "server solider mmapItemInfos find failed" << std::endl;
	}
	else if (miterItemInfo2 != mmapItemInfos.end())
	{
		//std::cout << "server solider mmapItemInfos find success : " << miterItemInfo2.
	}
	//mapItemInfos.insert(std::make_pair(itemInfo3, itemInfo3.GetItemid()));
	//mapItemInfos.insert(std::make_pair(itemInfo2, itemInfo2.roleid));

	//std::map<ItemInfo, int>::iterator it = mapItemInfos.find(itemInfo);

	int result = 0;
	//加载套接字
	WORD wVersion = 1;
	wVersion = MAKEWORD(1,1);

	WSADATA wsaData;
	wsaData.wVersion = wVersion;

	int resultWSAStart = 0;
	resultWSAStart = WSAStartup(wVersion, &wsaData);
	if (resultWSAStart == WSAEWOULDBLOCK)
	{
		std::cout << "server solider wsa start up failed : " << std::endl;
		return -1;
	}

	SOCKET server, client;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		std::cout << "server solider create socket failed" << std::endl;
		return -1;
	}

	std::cout << "server solider create socket value : " << server << std::endl;	//同一个SOCKET 值导致一个recv的问题，gate, solider不能是同一个server SOCKET value; 156既是gate连接solider的值，又是gate send to solider的值

	//int mode = 1;
	//result = ioctlsocket(server, FIONBIO, (u_long*)&mode);	//often ioctl socket only can set be where connect port, for library，set socket ioctl mode need &, or failed
	//if (result == SOCKET_ERROR)
	//{
	//	std::cout << "server client set ioctlsocket failed" << std::endl;
	//	WSACleanup();
	//	return -1;
	//}


	SOCKADDR_IN serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddress.sin_port = 9901;
	result = bind(server, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (result == SOCKET_ERROR)
	{
		std::cout << "server solider bind socket failed" << std::endl;
		return -1;
	}

	int blacklog = 1;
	result = listen(server, blacklog);
	if (result == SOCKET_ERROR)
	{
		std::cout << "server solider listen socket failed" << std::endl;
		return -1;
	}
	else if (result == 0)
	{
		std::cout << "server solider listen socket success port : -------------------- 9901" << std::endl;
	}

	SOCKADDR_IN clientAddress;
	int len = sizeof(clientAddress);
	HANDLE handleRemote;
	SECURITY_ATTRIBUTES threadAttrs;
	threadAttrs.bInheritHandle = true;
	threadAttrs.lpSecurityDescriptor = NULL;
	threadAttrs.nLength = 0;
	DWORD dwCreationFlags = 0;
	DWORD threadId = 0;
	while (true)
	{
		client = accept(server, (sockaddr*)&clientAddress, &len);
		handleRemote = CreateThread(&threadAttrs, sizeof(threadAttrs), JoinThread, (LPVOID)client, dwCreationFlags, &threadId);	//all params need set correct can join to the thread func
	}

	return 0;
}

