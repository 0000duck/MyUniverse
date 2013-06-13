#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma  comment (lib, "ws2_32.lib")

#define MSG_SEND_BUFFER 2048 // ���ͻ������󳤶�
int main(int argc, char **argv)
{
    WSADATA        wsd;
    SOCKET         send_s;
    char          *sendbuf = NULL;
    int            ret;
    SOCKADDR_IN    recv_addr;
	int port;
	int sendlen = MSG_SEND_BUFFER;

    if(argc != 3)
	{
		printf("usage : %s [destip] [port]\n",argv[0]);
		exit(1);
	}
	port=atoi(argv[2]);

    // ��ʼ��Winsock��
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup error!\n");
        exit(1);
    }

    // ����UDP�׽���
    send_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_s == INVALID_SOCKET)
    {
        printf("socket() error; %d\n", WSAGetLastError());
        exit(1);
    }
	// ���ý��ն˵�ַ�ṹ
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons((u_short)port);
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // ���䷢�ͻ���ռ�
    sendbuf = new char[MSG_SEND_BUFFER];
    memset(sendbuf, 0, MSG_SEND_BUFFER);
	// ��ʼ������Ϣ
	printf("������ %s �� %d �˿ڷ�����Ϣ...\n",argv[1],port);
	while(1)
	{
		printf("������Ҫ���͵���Ϣ:");
		gets(sendbuf);
		ret = sendto(send_s, sendbuf, sendlen, 0, 
			(SOCKADDR *)&recv_addr, sizeof(recv_addr));
		if (ret == SOCKET_ERROR)
		{
			printf("sendto() error; %d\n", WSAGetLastError());
			break;
		}
		else if (ret == 0)
			break;
	}                         
    closesocket(send_s);

    delete sendbuf;
    WSACleanup();
    return 0;
}