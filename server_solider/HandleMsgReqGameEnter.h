#include "../common/proto.h"

class HandleMsgReqGameEnter
{
public:
	HandleMsgReqGameEnter(void){}
	~HandleMsgReqGameEnter(void){}

	void Init(MsgReqGameEnter req);
	int Process();

protected:
	int GameEnter();
private:
	MsgReqGameEnter m_reqGameEnter;
	MsgRspGameEnter m_rspGameEnter;
};