#include "HandleMsgReqRegister.h"
#include "../common/game_code.h"
#include <string>	
#include <time.h>
#include "../common/IGlobal.h"
#include "BaseUsers.h"

void HandleMsgReqRegister::Init(MsgReqRegister req)
{
	//m_client = client;
	m_reqRegister = req;
	m_rspRegister.client = req.client;
}

void HandleMsgReqRegister::InitLogin(int client, MsgReqLogin req)
{
	m_client = client;
	m_reqLogin = req;
	m_rspLogin.needCloseClient = 0;
	m_rspLogin.client = req.client;
}

void HandleMsgReqRegister::InitClientClosed(int client, MsgClientClosed close)
{
	m_closedClient = client;
	m_clientClosed = close;
}

int HandleMsgReqRegister::Process()
{
	int result = 0;

	result = Register(m_reqRegister.account, m_reqRegister.pwd);
	Reply(MSG_RSP_REGISTER, result, m_rspRegister);

	return result;
}

int HandleMsgReqRegister::ProcessLogin()
{
	int result = 0;

	result = Login(m_reqLogin.account, m_reqLogin.pwd);
	ReplyLogin(MSG_RSP_LOGIN, result, m_rspLogin);

	return result;
}

int HandleMsgReqRegister::ProcessClientClosed()
{
	int result = 0;

	result = ClientClosed();
	ReplyClientClosed(MSG_CLIENT_CLOSED_RSP, result, m_clientClosedRsp);

	return result;
}

void HandleMsgReqRegister::Reply(int msgid, int result, MsgRspRegister& rsp)
{
	//int sendFlags = 0;

	rsp.msgid = msgid;
	rsp.result = result;
	//send(m_client, rsp, sizeof(rsp), sendFlags);
}

void HandleMsgReqRegister::ReplyLogin(int msgid, int result, MsgRspLogin& rsp)
{
	rsp.msgid = msgid;
	rsp.result = result;
}

void HandleMsgReqRegister::ReplyClientClosed(int msgid, int result, MsgClientClosedRsp& rsp)
{
	rsp.msgid= msgid;
	rsp.result = result;
}

u_int64 HandleMsgReqRegister::findOnlineUserID(int client)
{
	std::map<int, LoginAccountInfo>::iterator iter = m_clientOnlineInfos.find(client);
	if (iter == m_clientOnlineInfos.end())
	{
		return 0;
	}
	else
	{
		return iter->second.userid;
	}
}

int HandleMsgReqRegister::Register(std::string account, std::string pwd)
{
	int result = 0;

	if (account[0] == 0)
	{
		return RESULT_FAILED_INVAILD_ACCOUNT;
	}

	if (pwd[0] == 0)
	{
		return RESULT_FAILED_INVAILD_PASSWORD;
	}

	if (strlen(account.c_str()) >= 20)
	{
		return RESULT_FAILED_ACCOUNT_LIMIT;
	}

	if (strlen(pwd.c_str()) >= 16)
	{
		return RESULT_FAILED_PASSWORD_LIMIT;
	}

	std::map<std::string, std::string>::iterator iterItem = m_accountInfos.find(account);
	if (iterItem != m_accountInfos.end())
	{
		return RESULT_FAILED_ACCOUNT_ALREADY;
	}

	m_accountInfos.insert(make_pair(account, pwd));
	
	return result;
}

int HandleMsgReqRegister::Login(std::string account, std::string pwd)
{
	int result = 0;

	if (account[0] == 0)
	{
		return RESULT_FAILED_LOGIN_ACCOUNT_INVAILD;
	}

	if (pwd[0] == 0)
	{
		return RESULT_FAILED_LOGIN_PASSWORD_INVAILD;
	}

	if (strlen(account.c_str()) >= 20)
	{
		return RESULT_FAILED_LOGIN_ACCOUNT_LIMIT;
	}

	if (strlen(pwd.c_str()) >= 16)
	{
		return RESULT_FAILED_LOGIN_PASSWORD_LIMIT;
	}

	if (m_accountInfos.size() == 0)
	{
		return RESULT_FAILED_LOGIN_ACCOUNT_NOT_EXIST;
	}

	std::map<std::string, std::string>::iterator iterItem = m_accountInfos.find(account);
	if (iterItem == m_accountInfos.end())
	{
		return RESULT_FAILED_LOGIN_ACCOUNT_NOT_EXIST;
	}
	else if (iterItem != m_accountInfos.end())
	{
		if (iterItem->second != pwd)
		{
			return RESULT_FAILED_LOGIN_PASSWORD_ERROR;
		}
		else if (iterItem->second == pwd)
		{


			std::map<std::string, LoginAccountInfo>::iterator iterInfo = m_loginOnlineInfos.find(account);
			//std::map<int, LoginAccountInfo>::iterator iterClientOLInfo = m_clientOnlineInfos.find(m_closedClient);
			if (iterInfo == m_loginOnlineInfos.end())
			{
				LoginAccountInfo loginAccountInfo;
				loginAccountInfo.account = account;
				loginAccountInfo.userid = genGuid();
				loginAccountInfo.client = m_client;
				loginAccountInfo.loginTime = time(NULL);

				loginAccountInfo.id = genGuid();	//����2����ֻ�ܵ�¼1��
				loginAccountInfo.lastLoginTime = loginAccountInfo.loginTime - 2;		
				//loginAccountInfo.lastOfflineTime = 0;	
				loginAccountInfo.pwd = pwd;

				std::map<int, LoginAccountInfo>::iterator iterClientOLAlreadyInfo = m_clientOnlineInfos.find(loginAccountInfo.client);		//��ǰclient 164�Ѿ���¼��һ���ʺ�
				if (iterClientOLAlreadyInfo != m_clientOnlineInfos.end())
				{
					std::map<std::string, LoginAccountInfo>::iterator iterAlreadyInfo = m_loginOnlineInfos.find(iterClientOLAlreadyInfo->second.account);
					if (iterAlreadyInfo != m_loginOnlineInfos.end())
					{
						m_loginOnlineInfos.erase(iterAlreadyInfo);	//client 164�Ѵ��ڵ�¼��һ���ʺţ���ɾ������ʱ���ǲ���kick out����ȷ������Ӧ������Ҫkick out
					}

					m_clientOnlineInfos.erase(iterClientOLAlreadyInfo);
					users.del(iterAlreadyInfo->second.userid);
				}

				m_loginOnlineInfos.insert(make_pair(account, loginAccountInfo));
				m_clientOnlineInfos.insert(std::make_pair(loginAccountInfo.client, loginAccountInfo));
			}
			else
			{
				if (time(NULL) - iterInfo->second.lastLoginTime <= 2)
				{
					return RESULT_FAILED_LOGIN_TIME_LIMIT;
				}
				LoginAccountInfo loginAccountInfo;
				loginAccountInfo.account = iterInfo->second.account;
				loginAccountInfo.userid = iterInfo->second.userid;
				loginAccountInfo.client = m_client;
				loginAccountInfo.loginTime = time(NULL);

				loginAccountInfo.id = genGuid();	//���յ�msg������Ҫ���������Ա�rsp��ȷ��client;��������û�����ߣ���ֻ�ǿͻ�����Ӧ�����˳���¼�Ľ����ǲ��е�,��Ҫdisconnect,�Ͽ�����
				loginAccountInfo.lastLoginTime = iterInfo->second.loginTime;
				loginAccountInfo.pwd = pwd;
				//m_needCloseClient = iterInfo->second.client;
				m_rspLogin.needCloseClient = iterInfo->second.client;
				std::map<int, LoginAccountInfo>::iterator iterClientOLExpiredInfo = m_clientOnlineInfos.find(iterInfo->second.client);
				m_loginOnlineInfos.erase(iterInfo);		//����Ҫ������
				//m_clientOnlineInfos.erase(iterInfo->second.client,)

				if (iterClientOLExpiredInfo == m_clientOnlineInfos.end())
				{

				}
				else
				{
					m_clientOnlineInfos.erase(iterClientOLExpiredInfo);
				}
				
				users.del(iterInfo->second.userid);

				if (m_rspLogin.needCloseClient == loginAccountInfo.client)
				{

				}
				else
				{
					m_loginOnlineInfos.insert(make_pair(account, loginAccountInfo));
					m_clientOnlineInfos.insert(std::make_pair(loginAccountInfo.client, loginAccountInfo));
				}
			}

			return RESULT_SUCCESS;
		}
	}

	return result;
}

int HandleMsgReqRegister::ClientClosed()
{
	int result = 0;

	if (m_clientOnlineInfos.size() == 0)
	{
		return RESULT_FAILED_CLIENT_CLOSED_NOT_EXIST;
	}

	std::map<int, LoginAccountInfo>::iterator iterClientOLInfo = m_clientOnlineInfos.find(m_closedClient);
	if (iterClientOLInfo == m_clientOnlineInfos.end())
	{
		return RESULT_FAILED_CLIENT_CLOSED_NOT_EXIST;
	}
	//else
	//{
	//	m_clientOnlineInfos.erase(iterClientOLInfo);
	//}

	if (m_loginOnlineInfos.size() == 0)
	{
		return RESULT_FAILED_LOGIN_ONLINE_INFO_NOT_EXIST;
	}

	std::map<std::string, LoginAccountInfo>::iterator iterLoginOLInfo = m_loginOnlineInfos.find(iterClientOLInfo->second.account);
	if (iterLoginOLInfo == m_loginOnlineInfos.end())
	{
		return RESULT_FAILED_LOGIN_ONLINE_INFO_NOT_EXIST;
	}

	if (iterClientOLInfo != m_clientOnlineInfos.end())
	{
		m_clientOnlineInfos.erase(iterClientOLInfo);
	}

	if (iterLoginOLInfo != m_loginOnlineInfos.end())
	{
		m_loginOnlineInfos.erase(iterLoginOLInfo);
		users.del(iterLoginOLInfo->second.userid);
	}

	return result;
}