#include "BaseUsers.h"
#include "../common/game_code.h"
#include "../common/IGlobal.h"

Users users;

int Users::loginRole(u_int64 userid, int roleid)
{
	int result = 0;
	std::vector<RoleInfo> vecRoleInfos;

	if (userid == 0)
	{
		return RESULT_FAILED_USERID_INVAILD;
	}

	if (roleid == 0)
	{
		return RESULT_FAILED_ROLEID_INVAILD;
	}

	std::map<u_int64, std::vector<RoleInfo>>::iterator iterRoleInfos = m_roleUserInfos.find(userid);
	if (iterRoleInfos == m_roleUserInfos.end())
	{
		return RESULT_FAILED_USER_NOT_REGISTER;
	}
	else
	{
		for (std::vector<RoleInfo>::iterator iterRoleInfo = iterRoleInfos->second.begin(); iterRoleInfo != iterRoleInfos->second.end(); iterRoleInfo++)
		{
			if (iterRoleInfo->roleid == roleid)
			{
				std::map<u_int64, std::vector<RoleInfo>>::iterator iterUserInfo = m_roleOnlineInfos.find(userid);
				if (iterUserInfo == m_roleOnlineInfos.end())
				{
					vecRoleInfos.push_back(*iterRoleInfo);
					m_roleOnlineInfos.insert(std::make_pair(userid, vecRoleInfos));
				}
				else
				{
					vecRoleInfos = iterUserInfo->second;
					vecRoleInfos.clear();	//由于帐号的限定，一个帐号只能登录一个角色
					vecRoleInfos.push_back(*iterRoleInfo);
					iterUserInfo->second = vecRoleInfos;	//assignment unexpectedly correct!
				}
			}
		}
	}

	return result;
}

int Users::del(u_int64 userid)
{
	int result = 0;
	m_roleOnlineInfos.erase(userid);
	return result;
}

int Users::add(u_int64 userid, /*int roleid,*/ std::string nickname, int sex)
{
	int result = 0;

	if (userid == 0)
	{
		return RESULT_FAILED_USERID_INVAILD;
	}

	//if (roleid == 0)
	//{
	//	return RESULT_FAILED_ROLEID_INVAILD;
	//}

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

	RoleInfo roleInfo;
	std::map<u_int64, std::vector<RoleInfo>>::iterator iterRoleInfo = m_roleUserInfos.find(userid);
	if (iterRoleInfo == m_roleUserInfos.end())
	{
		roleInfo.roleid = genGuid();
		//roleInfo.nickname = m_reqRoleRegister.nickname;
		memcpy(roleInfo.nickname, nickname.c_str(), sizeof(roleInfo.nickname));
		roleInfo.sex = sex;
		roleInfo.attack = 0;
		roleInfo.defend = 0;
		roleInfo.hp = 0;
		roleInfo.level = 0;
		std::vector<RoleInfo> vecRoleInfos;
		vecRoleInfos.push_back(roleInfo);
		m_roleUserInfos.insert(std::make_pair(userid, vecRoleInfos));
	}
	else if (iterRoleInfo != m_roleUserInfos.end())
	{
		if (iterRoleInfo->second.size() > 12)
		{
			return RESULT_FAILED_ROLE_COUNT_FULL;
		}

		roleInfo.roleid = genGuid();
		//roleInfo.nickname = m_reqRoleRegister.nickname;
		memcpy(roleInfo.nickname, nickname.c_str(), sizeof(roleInfo.nickname));
		roleInfo.sex = sex;
		roleInfo.attack = 0;
		roleInfo.defend = 0;
		roleInfo.hp = 0;
		roleInfo.level = 0;
		iterRoleInfo->second.push_back(roleInfo);
	}


	return result;
}

std::vector<RoleInfo> Users::findUserRoleInfos(u_int64 userid)
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

//int Roles::add(u_int64 userid, /*int roleid,*/ std::string nickname, int sex)
//{
//	int result = 0;
//
//	if (userid == 0)
//	{
//		return RESULT_FAILED_USERID_INVAILD;
//	}
//
//	//if (roleid == 0)
//	//{
//	//	return RESULT_FAILED_ROLEID_INVAILD;
//	//}
//
//	if (nickname[0] == 0)
//	{
//		return RESULT_FAILED_ROLE_NICKNAME_INVAILD;
//	}
//
//	if (nickname.size() > 14)
//	{
//		return RESULT_FAILED_ROLE_NICKNAME_LIMIT;
//	}
//
//	if (sex != 0 && sex != 1)
//	{
//		return RESULT_FAILED_ROLE_SEX_INVAILD;
//	}
//
//	RoleInfo roleInfo;
//	std::map<u_int64, std::vector<RoleInfo>>::iterator iterRoleInfo = m_roleUserInfos.find(userid);
//	if (iterRoleInfo == m_roleUserInfos.end())
//	{
//		roleInfo.roleid = genGuid();
//		//roleInfo.nickname = m_reqRoleRegister.nickname;
//		memcpy(roleInfo.nickname, nickname.c_str(), sizeof(roleInfo.nickname));
//		roleInfo.sex = sex;
//		roleInfo.attack = 0;
//		roleInfo.defend = 0;
//		roleInfo.hp = 0;
//		roleInfo.level = 0;
//		std::vector<RoleInfo> vecRoleInfos;
//		vecRoleInfos.push_back(roleInfo);
//		m_roleUserInfos.insert(std::make_pair(userid, vecRoleInfos));
//	}
//	else if (iterRoleInfo != m_roleUserInfos.end())
//	{
//		if (iterRoleInfo->second.size() > 12)
//		{
//			return RESULT_FAILED_ROLE_COUNT_FULL;
//		}
//
//		roleInfo.roleid = genGuid();
//		//roleInfo.nickname = m_reqRoleRegister.nickname;
//		memcpy(roleInfo.nickname, nickname.c_str(), sizeof(roleInfo.nickname));
//		roleInfo.sex = sex;
//		roleInfo.attack = 0;
//		roleInfo.defend = 0;
//		roleInfo.hp = 0;
//		roleInfo.level = 0;
//		iterRoleInfo->second.push_back(roleInfo);
//	}
//
//
//	return result;
//}