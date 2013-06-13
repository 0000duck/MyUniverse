#include "stdafx.h"
#include "udpThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma  comment (lib, "ws2_32.lib")

#define MSG_BUFFER_LENGTH (2048) //接受buffer长度
#define UDP_LISTENING_PORT (9160)

char recvbuf[MSG_BUFFER_LENGTH]; 

DWORD WINAPI UdpThread(LPVOID lpParam)
{
    WSADATA   wsd;
	SOCKET    recv_s;
	//char      *recvbuf;
	int       ret;
	int       port;
	SOCKADDR_IN localaddr;
	SOCKADDR_IN destaddr;
	int destlen = sizeof(destaddr);

	port = UDP_LISTENING_PORT;
    
	// ³õÊ¼»¯Winsock¿â
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup error!\n");
		exit(1);
	}
    // ´´½¨UDPÌ×½Ó×Ö
	recv_s = socket(AF_INET, SOCK_DGRAM, 0);
	if(recv_s == INVALID_SOCKET)
	{
		printf("socket() errror : %d\n",WSAGetLastError());
		exit(1);
	}

    // ¶Ô±¾µØµØÖ·½á¹¹½øÐÐÉèÖÃ
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons((u_short)port);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    // ¶ÔÌ×½Ó×Ö½øÐÐ°ó¶¨
	if(bind(recv_s, (SOCKADDR *)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("bind() error: %d\n", WSAGetLastError());
		exit(1);
	}

	printf("在端口%d监听...\n",port);

	// ·ÖÅä½ÓÊÕ»º´æÇø
	int recvlen = MSG_BUFFER_LENGTH;
    // ¿ªÊ¼½ÓÊÕÏûÏ¢
	while(1)
	{
		ret = recvfrom(recv_s, recvbuf, recvlen, 0,(SOCKADDR*)&destaddr, &destlen);//此函数为阻塞函数

		if(ret == SOCKET_ERROR)
		{
			printf("recvfrom() error: %d\n",WSAGetLastError());
			exit(1);
		}
		else
		{
			recvbuf[ret]='\0';
            printf("长度%d\n",ret);
			printf("收到主机 %s 的消息: %s\n",inet_ntoa(destaddr.sin_addr),recvbuf);
		}

	}
	closesocket(recv_s);
	//delete recvbuf;
	WSACleanup();
	return 0;
}
