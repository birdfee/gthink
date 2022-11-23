#pragma once
#include <map>
#include <vector>
#include "../common/IRole.h"

class Users
{
public:
	Users(void){}
	~Users(void){}

	int loginRole(u_int64 userid, int roleid);	//登录某个角色
	int del(u_int64 userid);	//删除某个登录的角色
	//int addLoginRole(u_int64 userid);
	int add(u_int64 userid, /*int roleid,*/ std::string nickname, int sex);		//添加角色注册的信息
	std::vector<RoleInfo> findUserRoleInfos(u_int64 userid);

private:
	std::map<u_int64, std::vector<RoleInfo>> m_roleOnlineInfos;		//a account can login roles at the same time,已登录的角色们
	std::map<u_int64, std::vector<RoleInfo>> m_roleUserInfos; //注册的角色信息
};

extern Users users;	//extern / static

//class Roles
//{
//public:
//	Roles(void){}
//	~Roles(void){}
//
//	int add(u_int64 userid, /*int roleid,*/ std::string nickname, int sex);
//	std::map<u_int64, std::vector<RoleInfo>>::iterator find(u_int64 userid);
//
//private:
//	std::map<u_int64, std::vector<RoleInfo>> m_roleUserInfos; //注册的角色信息
//};
//
//static Roles roles;