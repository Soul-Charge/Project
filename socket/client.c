/* 客户端程序 */
/* 运行系统：windows */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <WinSock2.h>
#include "Socket/Socket/myWinSocketAPI.h"
#define MSGLEN 200

int main(void)
{
    int connect_d;
	char recv_msg[MSGLEN] = { 0 };

    /* initialize */
    //system("chcp 65001");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    /* open, connect */
    connect_d = open_InetStream_socket();
    connect_ipv4(connect_d, "192.168.1.101", 2333);
    
    /* begin */
    read_in(connect_d, recv_msg, MSGLEN);
	if (strlen(recv_msg) != 0)
		printf("获得来自服务器的消息：%s\n", recv_msg);

	printf("向服务器发出消息：hello\n");
    say(connect_d, "hello");

    read_in(connect_d, recv_msg, MSGLEN);
	if (strlen(recv_msg) != 0)
		printf("获得来自服务器的消息：%s\n", recv_msg);

	closesocket(connect_d);
    system("pause");
    return 0;
}
