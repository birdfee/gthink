#pragma once
#include <vector>
#include "../common/IRole.h"
enum
{
	MSG_CLIENT_CLOSED = 2,	
	MSG_CLIENT_CLOSED_RSP = 3,

	MSG_REQ_REGISTER = 301,
	MSG_RSP_REGISTER = 302,
	MSG_REQ_LOGIN = 303,
	MSG_RSP_LOGIN = 304,
	MSG_REQ_ROLE_REGISTER = 305,
	MSG_RSP_ROLE_REGISTER = 306,
	MSG_REQ_GAME_ENTER = 307,
	MSG_RSP_GAME_ENTER = 308,
};

typedef struct tagMsgClientClosed
{
	int msgid;
}MsgClientClosed;

typedef struct tagMsgClientClosedRsp
{
	int msgid;
	int result;
}MsgClientClosedRsp;

typedef struct tagMsgReqRegister
{
	int client;
	int msgid;
	char account[20];
	char pwd[16];
}MsgReqRegister;

typedef struct tagMsgRspRegister
{
	int client;
	int msgid;
	int result;	//0为成功，非0为错误码
}MsgRspRegister;

typedef struct tagMsgReqLogin
{
	int client;
	int msgid;
	char account[20];
	char pwd[16];
}MsgReqLogin;

typedef struct tagMsgRspLogin
{
	int client;
	int msgid;
	int needCloseClient;
	int result;
	int roleCount;
	std::vector<RoleInfo> vecRoleInfos;
}MsgRspLogin;

typedef struct tagMsgRspLoginTemp
{
	int client;
	int msgid;
	int needCloseClient;
	int result;
	int roleCount;
	//std::vector<RoleInfo> vecRoleInfos;
}MsgRspLoginTemp;

typedef struct tagMsgReqRoleRegister
{
	int client;
	int msgid;
	char nickname[14];
	int sex;	
}MsgReqRoleRegister;

typedef struct tagMsgRspRoleRegister
{
	int client;
	int msgid;
	int result;
	int roleCount;
	std::vector<RoleInfo> vecRoleInfos;
}MsgRspRoleRegister;

typedef struct tagMsgRspRoleRegisterTemp
{
	int client;
	int msgid;
	int result;
	int roleCount;
	//std::vector<RoleInfo> vecRoleInfos;
}MsgRspRoleRegisterTemp;

typedef struct tagMsgReqGameEnter
{
	int client;
	int msgid;
}MsgReqGameEnter;

typedef struct tagMsgRspGameEnter
{
	int client;
	int msgid;
	int result;
}MsgRspGameEnter;