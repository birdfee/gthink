enum game_code
{
	RESULT_SUCCESS = 0,

	RESULT_FAILED_INVAILD_ACCOUNT = 301,	//������ʺ���Ч
	RESULT_FAILED_INVAILD_PASSWORD = 302,	//�����������Ч
	RESULT_FAILED_ACCOUNT_LIMIT = 302,		//�ʺų������Ƴ���
	RESULT_FAILED_PASSWORD_LIMIT = 303,		//���볬�����Ƴ���
	RESULT_FAILED_ACCOUNT_ALREADY = 304,	//�ʺ��Ѵ���
	RESULT_FAILED_LOGIN_ACCOUNT_INVAILD = 305,	//��¼�ʺ���Ч
	RESULT_FAILED_LOGIN_PASSWORD_INVAILD = 306,	//��¼������Ч
	RESULT_FAILED_LOGIN_ACCOUNT_LIMIT = 307,		//��¼�ʺų������Ƴ���
	RESULT_FAILED_LOGIN_PASSWORD_LIMIT = 308,		//��¼���볬�����Ƴ���
	RESULT_FAILED_LOGIN_ACCOUNT_NOT_EXIST = 309,	//��¼���ʺŲ�����
	RESULT_FAILED_LOGIN_PASSWORD_ERROR = 310,		//��¼���������
	RESULT_FAILED_LOGIN_TIME_LIMIT = 311,			//��¼ʱ������Ϊ2����ֻ�ܵ�¼1��
	RESULT_FAILED_CLIENT_CLOSED_NOT_EXIST = 312,	//�ͻ��˹رյ���Ϣ������
	RESULT_FAILED_LOGIN_ONLINE_INFO_NOT_EXIST = 313,		//�ͻ��˹رյĵ�¼������Ϣ������
	RESULT_FAILED_ROLE_NICKNAME_INVAILD = 314,		//��ɫ�ǳ���Ч
	RESULT_FAILED_ROLE_NICKNAME_LIMIT = 315,		//��ɫ�ǳƹ���
	RESULT_FAILED_ROLE_COUNT_FULL = 316,		//��ɫ��������
	RESULT_FAILED_ROLE_SEX_INVAILD = 317,		//��ɫ�Ա���Ч
	RESULT_FAILED_USERID_INVAILD = 318,			//�û�ID��Ч
	RESULT_FAILED_FIND_USERS_NOT_EXIST = 319,		//��Ҫ���ҵ��û��ǲ�����
};