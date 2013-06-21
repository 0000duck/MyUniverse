/* ==========================================================
*     ������Ա��ţ�ڽ�
*     ��ǰ�汾��1.0
*     ����ʱ�䣺2006-12-05
*     �޸�ʱ�䣺2006-12-05
*     ����˵����UDPͨ���еĽ��ն˳��򣬲�ȡWinsock��̷���
*     ʹ�÷�����UDPReceiver [port]

*/
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma  comment (lib, "ws2_32.lib")

#include "..\..\..\src\MyUniverse\udpType.h"

#define MSG_BUFFER_LENGTH 2048 // �������ݵ���󳤶�

int main(int argc, char **argv)
{
	WSADATA   wsd;
	SOCKET    recv_s;
	char      *recvbuf;
	int       ret;
	int       port;
	SOCKADDR_IN localaddr;
	SOCKADDR_IN destaddr;
	int destlen = sizeof(destaddr);

	if(argc != 2)
	{
		printf("usage: %s [port]",argv[0]);
		exit(1);
	}
	port=atoi(argv[1]); // ��ö˿ں�
    
	// ��ʼ��Winsock��
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup error!\n");
		exit(1);
	}
    // ����UDP�׽���
	recv_s = socket(AF_INET, SOCK_DGRAM, 0);
	if(recv_s == INVALID_SOCKET)
	{
		printf("socket() errror : %d\n",WSAGetLastError());
		exit(1);
	}

    // �Ա��ص�ַ�ṹ��������
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons((u_short)port);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    // ���׽��ֽ��а�
	if(bind(recv_s, (SOCKADDR *)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("bind() error: %d\n", WSAGetLastError());
		exit(1);
	}

	printf("�ڶ˿� %d ��ʼ������Ϣ...\n",port);

	recvbuf = new char[MSG_BUFFER_LENGTH]; // ������ջ�����
	int recvlen = MSG_BUFFER_LENGTH;
    // ��ʼ������Ϣ
	while(1)
	{
		ret = recvfrom(recv_s, recvbuf, recvlen, 0,
			(SOCKADDR*)&destaddr, &destlen);
		if(ret == SOCKET_ERROR)
		{
			printf("recvfrom() error: %d\n",WSAGetLastError());
			exit(1);
		}
		else if(ret == 0)
		{
			break;
		}
		else
		{
			recvbuf[ret]='\0';
            GlbCommand *pCMD;
			printf("�յ����� %s ��Ϣ�����ȣ�%d\n",inet_ntoa(destaddr.sin_addr),ret);
            pCMD = (GlbCommand*)recvbuf;
            printf("commamd:%d\n", pCMD->command);
            printf("ctrl1_ret0:%d\n",pCMD->ctrl1_ret0);
            printf("iParam1:%d\n",pCMD->iParam1);
            printf("iParam2:%d\n",pCMD->iParam2);
            printf("fParam1:%f\n",pCMD->fParam2);
            printf("fParam2:%f\n",pCMD->fParam2);
            printf("cParam1:%s\n",pCMD->cParam1);
            printf("cParam2:%s\n",pCMD->cParam2);
		}

	}
	closesocket(recv_s);
	delete recvbuf;
	WSACleanup();
	return 0;
}

