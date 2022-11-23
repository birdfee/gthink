#include "HandleMsgReqGameEnter.h"
#include "../common/game_code.h"
#include "BaseUsers.h"

void HandleMsgReqGameEnter::Init(u_int64 userid, MsgReqGameEnter req)
{
	m_userid = userid;
	m_reqGameEnter = req;
	m_rspGameEnter.client = req.client;
}

int HandleMsgReqGameEnter::Process()
{
	int result = 0;

	result = GameEnter(m_reqGameEnter.roleid);
	Reply(MSG_RSP_GAME_ENTER, result, m_rspGameEnter);

	return result;
}

void HandleMsgReqGameEnter::Reply(int msgid, int result, MsgRspGameEnter& rsp)
{
	rsp.msgid = msgid;
	rsp.result = result;
}

int HandleMsgReqGameEnter::GameEnter(int roleid)
{
	int result = 0;

	if (roleid == 0)
	{
		return RESULT_FAILED_ROLEID_INVAILD;
	}

	result = users.loginRole(m_userid, roleid);

	//std::map<u_int64, >

	return result;
}