#include "ClientNet.h"

void main()
{
	CClientNet client;
	
	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����
	printf("Connect\n");
	client.Connect(1235,"127.0.0.1");
	
	//������Ϣ
	printf("send msg\n");
	client.SendMsg("hello\0",sizeof("hello\0"));
	
	//�ر�socket
	printf("close\n");
	client.Close();

	system("pause");
}