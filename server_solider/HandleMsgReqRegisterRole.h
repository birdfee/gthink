#include <map>
#include <vector>
#include "../common/IRole.h"
#include "../common/proto.h"
//typedef struct tagItemInfo
//{
//	int itemid;
//	int roleid;
//}ItemInfo;

struct ItemInfo
{
	int itemid;
	int roleid;
	bool operator < (const ItemInfo info) const
	{
		if ((this->itemid < info.itemid) || (this->roleid < info.roleid))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class HandleMsgReqRegisterRole
{
public:
	HandleMsgReqRegisterRole(void)
	{

	}

	~HandleMsgReqRegisterRole(void)
	{

	}

	void Init(u_int64 userid, MsgReqRoleRegister req);
	int Process();
	void Reply(int msgid, int result, MsgRspRoleRegister& rsp);
	MsgRspRoleRegister getRsp(){return m_rspRoleRegister;}
	int findItemRole(ItemInfo itemInfo);
	std::vector<RoleInfo> findUserRoleInfos(u_int64 userid);

protected:
	int RegisterRole(std::string nickname, int sex);

private:
	MsgReqRoleRegister m_reqRoleRegister;
	MsgRspRoleRegister m_rspRoleRegister;
	//std::map<ItemInfo, int> m_roleItemInfos;
	u_int64 m_userid;
	std::map<u_int64, std::vector<RoleInfo>> m_roleUserInfos; //注册的角色信息
};