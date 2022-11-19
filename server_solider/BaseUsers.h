#pragma once
#include <map>
#include <vector>
#include "../common/IRole.h"

class GameObject
{
public:
	GameObject(void){}
	~GameObject(void){}
private:
	std::map<u_int64, std::vector<RoleInfo>> m_roleOnlineInfos;		//a account can login roles at the same time
};

static GameObject object;