#include <map>
#include "../common/proto.h"

typedef unsigned int u_int32;
typedef u_int32 USERID;

typedef unsigned __int64 u_int64;
typedef u_int64 uid;

typedef struct tagLoginAccountInfo
{
	uid id;		
	uid userid;
	int client;
	std::string account;
	std::string pwd;
	int loginTime;
	int lastLoginTime;
	//int lastOfflineTime;
}LoginAccountInfo;

class HandleMsgReqRegister
{
public:
	HandleMsgReqRegister(void)
	{

	}

	~HandleMsgReqRegister(void)
	{

	}

	void Init(MsgReqRegister req);
	void InitLogin(int client, MsgReqLogin req);
	void InitClientClosed(int client, MsgClientClosed close);
	int Process();
	int ProcessLogin();
	int ProcessClientClosed();
	void Reply(int msgid, int result, MsgRspRegister& rsp);
	void ReplyLogin(int msgid, int result, MsgRspLogin& rsp);
	void ReplyClientClosed(int msgid, int result, MsgClientClosedRsp& rsp);
	MsgRspRegister getRsp(){return m_rspRegister;}
	MsgRspLogin getRspLogin(){return m_rspLogin;}
	MsgClientClosedRsp getClientClosedRsp(){return m_clientClosedRsp;}
	//int getNeedCloseClient(){return m_needCloseClient;}
	u_int64 findOnlineUserID(int client);
protected:
	int Register(std::string account, std::string pwd);
	int Login(std::string account, std::string pwd);
	int ClientClosed();
private:
	MsgClientClosed m_clientClosed;
	MsgClientClosedRsp m_clientClosedRsp;
	MsgReqRegister m_reqRegister;
	MsgRspRegister m_rspRegister;
	MsgReqLogin m_reqLogin;
	MsgRspLogin m_rspLogin;
	int m_client;
	int m_closedClient;
	//int m_needCloseClient;
	std::map<std::string, std::string> m_accountInfos;		//account[20], pwd[16]
	std::map<std::string, LoginAccountInfo> m_loginOnlineInfos;	
	std::map<int, LoginAccountInfo> m_clientOnlineInfos;
	std::map<u_int64,LoginAccountInfo> m_userOnlineInfos;
};