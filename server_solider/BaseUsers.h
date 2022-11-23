#pragma once
#include <map>
#include <vector>
#include "../common/IRole.h"

class Users
{
public:
	Users(void){}
	~Users(void){}

	int loginRole(u_int64 userid, int roleid);	//��¼ĳ����ɫ
	int del(u_int64 userid);	//ɾ��ĳ����¼�Ľ�ɫ
	//int addLoginRole(u_int64 userid);
	int add(u_int64 userid, /*int roleid,*/ std::string nickname, int sex);		//��ӽ�ɫע�����Ϣ
	std::vector<RoleInfo> findUserRoleInfos(u_int64 userid);

private:
	std::map<u_int64, std::vector<RoleInfo>> m_roleOnlineInfos;		//a account can login roles at the same time,�ѵ�¼�Ľ�ɫ��
	std::map<u_int64, std::vector<RoleInfo>> m_roleUserInfos; //ע��Ľ�ɫ��Ϣ
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
//	std::map<u_int64, std::vector<RoleInfo>> m_roleUserInfos; //ע��Ľ�ɫ��Ϣ
//};
//
//static Roles roles;