#include <string>
#include "../common/proto.h"

class HandleMsgReqLogin
{
public:
	HandleMsgReqLogin(void)
	{

	}

	~HandleMsgReqLogin(void)
	{

	}

	void Init(MsgReqLogin req);
	int Process();
	void Reply(int msgid, int result, MsgRspLogin& rsp);
	MsgRspLogin getRsp(){return m_rspLogin;}

protected:
	int Login(std::string account, std::string pwd);

private:
	MsgReqLogin m_reqLogin;
	MsgRspLogin m_rspLogin;
};