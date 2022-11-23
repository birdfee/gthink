#include "../common/proto.h"

class HandleMsgReqGameEnter
{
public:
	HandleMsgReqGameEnter(void){}
	~HandleMsgReqGameEnter(void){}

	void Init(u_int64 userid, MsgReqGameEnter req);
	int Process();
	void Reply(int msgid, int result, MsgRspGameEnter& rsp);
	MsgRspGameEnter getRsp(){return m_rspGameEnter;}

protected:
	int GameEnter(int roleid);
private:
	MsgReqGameEnter m_reqGameEnter;
	MsgRspGameEnter m_rspGameEnter;
	u_int64 m_userid;
};