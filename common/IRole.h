#pragma once

typedef unsigned __int64 u_int64;
typedef u_int64 uid;

struct RoleInfo
{
	uid roleid;
	//std::string nickname;	
	char nickname[14];
	int sex;
	int level;
	int hp;
	int attack;
	int defend;

	//RoleInfo()
	//{
	//	level = 0;
	//	hp = 0;
	//	attack = 0;
	//	defend = 0;
	//}
};