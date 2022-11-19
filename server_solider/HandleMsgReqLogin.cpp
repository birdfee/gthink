#include "HandleMsgReqLogin.h"
#include "../common/game_code.h"

void HandleMsgReqLogin::Init(MsgReqLogin req)
{
	m_reqLogin = req;
}

int HandleMsgReqLogin::Process()
{
	int result = 0;



	return result;
}

void HandleMsgReqLogin::Reply(int msgid, int result, MsgRspLogin& rsp)
{
	rsp.msgid = msgid;
	rsp.result = result;
}

int HandleMsgReqLogin::Login(std::string account, std::string pwd)
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



	return result;
}