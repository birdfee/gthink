#include "HandleMsgReqRegisterRole.h"
#include "../common/game_code.h"
#include "../common/IGlobal.h"
#include "BaseUsers.h"

void HandleMsgReqRegisterRole::Init(u_int64 userid, MsgReqRoleRegister req)
{
	m_userid = userid;
	m_reqRoleRegister = req;
	m_rspRoleRegister.client = req.client;
}

int HandleMsgReqRegisterRole::Process()
{
	int result = 0;

	result = RegisterRole(m_reqRoleRegister.nickname, m_reqRoleRegister.sex);
	Reply(MSG_RSP_ROLE_REGISTER, result, m_rspRoleRegister);

	return result;
}

void HandleMsgReqRegisterRole::Reply(int msgid, int result, MsgRspRoleRegister& rsp)
{
	rsp.msgid = msgid;
	rsp.result = result;
}

int HandleMsgReqRegisterRole::findItemRole(ItemInfo itemInfo)
{
	return 0;
}

std::vector<RoleInfo> HandleMsgReqRegisterRole::findUserRoleInfos(u_int64 userid)
{
	std::vector<RoleInfo> vecRoleInfos;

	if (userid == 0)
	{
		//return RESULT_FAILED_USERID_INVAILD;
		return vecRoleInfos;
	}

	if (m_roleUserInfos.size() == 0)
	{
		//return RESULT_FAILED_FIND_USERS_NOT_EXIST;
		return vecRoleInfos;
	}

	std::map<u_int64, std::vector<RoleInfo>>::iterator iterUserRoleInfos = m_roleUserInfos.find(userid);
	if (iterUserRoleInfos == m_roleUserInfos.end())
	{
		return vecRoleInfos;
	}
	else if (iterUserRoleInfos != m_roleUserInfos.end())
	{
		return iterUserRoleInfos->second;
	}

	return vecRoleInfos;
}

int HandleMsgReqRegisterRole::RegisterRole(std::string nickname, int sex)
{
	int result = 0;

	if (nickname[0] == 0)
	{
		return RESULT_FAILED_ROLE_NICKNAME_INVAILD;
	}

	if (nickname.size() > 14)
	{
		return RESULT_FAILED_ROLE_NICKNAME_LIMIT;
	}

	if (sex != 0 && sex != 1)
	{
		return RESULT_FAILED_ROLE_SEX_INVAILD;
	}

	users.add(m_userid, m_reqRoleRegister.nickname, m_reqRoleRegister.sex);

	//RoleInfo roleInfo;
	//std::map<u_int64, std::vector<RoleInfo>>::iterator iterRoleInfo = m_roleUserInfos.find(m_userid);
	//if (iterRoleInfo == m_roleUserInfos.end())
	//{
	//	roleInfo.roleid = genGuid();
	//	//roleInfo.nickname = m_reqRoleRegister.nickname;
	//	memcpy(roleInfo.nickname, m_reqRoleRegister.nickname, sizeof(roleInfo.nickname));
	//	roleInfo.sex = m_reqRoleRegister.sex;
	//	roleInfo.attack = 0;
	//	roleInfo.defend = 0;
	//	roleInfo.hp = 0;
	//	roleInfo.level = 0;
	//	std::vector<RoleInfo> vecRoleInfos;
	//	vecRoleInfos.push_back(roleInfo);
	//	m_roleUserInfos.insert(std::make_pair(m_userid, vecRoleInfos));
	//}
	//else if (iterRoleInfo != m_roleUserInfos.end())
	//{
	//	if (iterRoleInfo->second.size() > 12)
	//	{
	//		return RESULT_FAILED_ROLE_COUNT_FULL;
	//	}

	//	roleInfo.roleid = genGuid();
	//	//roleInfo.nickname = m_reqRoleRegister.nickname;
	//	memcpy(roleInfo.nickname, m_reqRoleRegister.nickname, sizeof(roleInfo.nickname));
	//	roleInfo.sex = m_reqRoleRegister.sex;
	//	roleInfo.attack = 0;
	//	roleInfo.defend = 0;
	//	roleInfo.hp = 0;
	//	roleInfo.level = 0;
	//	iterRoleInfo->second.push_back(roleInfo);
	//}

	return result;
}